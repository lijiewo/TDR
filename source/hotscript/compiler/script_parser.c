#include "script_parser.h"

#include "hotpot/hp_error.h"

hpint32 script_parser(SCRIPT_PARSER *self, const char* file_name, const HotObject *ho)
{
	hpint32 ret;

	hotoparr_init(&self->hotoparr);

	yyscriptlex_init_extra(&self->scanner, &self->scanner);

	self->stack_num = 0;
	script_open_file(&self->scanner, file_name);
	self->ho = ho;
	hotobject_get_const_iterator(&self->ho_const_iter, ho);

	ret = yyscriptparse(&self->scanner);
	if(ret == 0)
	{
		self->result = E_HP_NOERROR;
	}

	
	return self->result;
}

hpint32 script_open_file(yyscan_t *super, const char *file_name)
{
	SCRIPT_PARSER *self = HP_CONTAINER_OF(super, SCRIPT_PARSER, scanner);
	
	self->stack[self->stack_num].f = fopen(file_name, "r");
	if(self->stack[self->stack_num].f == NULL)
	{
		goto ERROR_RET;
	}
	self->stack[self->stack_num].bs = yyscript_create_buffer(self->stack[self->stack_num].f, YY_BUF_SIZE, self->scanner);
	if(self->stack[self->stack_num].bs == NULL)
	{
		goto ERROR_RET;
	}
	self->stack[self->stack_num].bs->yy_bs_column = 1;
	++self->stack_num;
	yyscript_switch_to_buffer(self->stack[self->stack_num - 1].bs, self->scanner);
	
	return E_HP_NOERROR;
ERROR_RET:
	return E_HP_ERROR;
}

hpint32 script_close_file(yyscan_t *super)
{
	SCRIPT_PARSER *self = HP_CONTAINER_OF(super, SCRIPT_PARSER, scanner);

	fclose(self->stack[self->stack_num - 1].f);
	yyscript_delete_buffer(self->stack[self->stack_num - 1].bs, self->scanner);

	--self->stack_num;
	if(self->stack_num <= 0)
	{
		goto ERROR_RET;
	}
	yyscript_switch_to_buffer(self->stack[self->stack_num - 1].bs, self->scanner);
	return E_HP_NOERROR;
ERROR_RET:
	return E_HP_ERROR;
}

hpint32 hotscript_do_text(SCRIPT_PARSER *self, const ST_STRING *text)
{
	HotOp *op = hotoparr_get_next_op(&self->hotoparr);
	op->op = HOT_ECHO;
	op->op0.str = malloc(text->str_len);
	memcpy(op->op0.str, text->str, text->str_len);
	op->op0.str_len = text->str_len;
	
	return E_HP_NOERROR;
}

hpint32 hotscript_do_push(SCRIPT_PARSER *self, const char prefix, const char*name)
{
	char *str = malloc(sizeof(char));
	HotOp *op = hotoparr_get_next_op(&self->hotoparr);
	op->op = HOT_PUSH;
	str[0] = prefix;
	op->op0.str = str;
	op->op0.str_len = 1;
	op->op1.str = name;
	op->op1.str_len = strlen(name);

	return E_HP_NOERROR;
}

hpint32 hotscript_do_push_index(SCRIPT_PARSER *self, hpuint32 ui32)
{
	if(ui32 != -1)
	{
		char *str = malloc(sizeof(char));
		HotOp *op = hotoparr_get_next_op(&self->hotoparr);
		op->op = HOT_PUSH_INDEX;
		op->op0.num = ui32;
	}

	return E_HP_NOERROR;
}

hpint32 hotscript_do_pop_index(SCRIPT_PARSER *self, hpuint32 ui32)
{
	if(ui32 != -1)
	{
		HotOp *op = hotoparr_get_next_op(&self->hotoparr);
		op->op = HOT_POP;
	}
	

	return E_HP_NOERROR;
}

hpint32 hotscript_do_pop(SCRIPT_PARSER *self)
{
	HotOp *op = hotoparr_get_next_op(&self->hotoparr);
	op->op = HOT_POP;
	return E_HP_NOERROR;
}


hpint32 hotscript_do_echo_trie(SCRIPT_PARSER *self)
{
	HotOp *op = hotoparr_get_next_op(&self->hotoparr);
	op->op = HOT_ECHO_TRIE;
	return E_HP_NOERROR;
}