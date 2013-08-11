#include "hotprotocol/hp_lua_writer.h"
#include "hotpot/hp_error_code.h"
#include "hotpot/hp_number.h"
#include "hotprotocol/hp_abstract_writer.h"

#include "lua.h"
#include "lauxlib.h"
#include "lualib.h"

#include <string.h>
#include <assert.h>
#include <stdio.h>


hpint32 lua_writer_init(HP_LUA_WRITER *self, lua_State *lua_state)
{
	memset(&self->super, 0, sizeof(HPAbstractWriter));

	self->ls = lua_state;
	

	self->super.write_struct_begin = lua_write_struct_begin;
	self->super.write_struct_end = lua_write_struct_end;
	self->super.write_vector_begin = lua_write_vector_begin;
	self->super.write_vector_end = lua_write_vector_end;
	self->super.write_field_begin = lua_write_field_begin;
	self->super.write_field_end = lua_write_field_end;
	self->super.write_vector_item_begin = lua_write_vector_item_begin;
	self->super.write_vector_item_end = lua_write_vector_item_end;

	self->super.write_hpint8 = lua_write_hpint8;
	self->super.write_hpint16 = lua_write_hpint16;
	self->super.write_hpint32 = lua_write_hpint32;
	self->super.write_hpint64 = lua_write_hpint64;
	self->super.write_hpuint8 = lua_write_hpuint8;
	self->super.write_hpuint16 = lua_write_hpuint16;
	self->super.write_hpuint32 = lua_write_hpuint32;
	self->super.write_hpuint64 = lua_write_hpuint64;

	self->super.write_enum_number = lua_write_enum_number;
	self->super.write_hpchar = lua_write_hpchar;
	self->super.write_hpdouble = lua_write_hpdouble;
	self->super.write_bytes = lua_write_bytes;
	self->super.write_string = lua_write_string;
	self->super.write_hpbool = lua_write_hpbool;
	self->super.write_null = lua_write_null;
	self->super.write_counter = lua_write_counter;
	
	

	return E_HP_NOERROR;
}

hpint32 lua_writer_fini(HP_LUA_WRITER *self)
{
	memset(&self->super, 0, HP_OFFSET_OF(HPAbstractWriter, stack));

	return E_HP_NOERROR;
}

hpint32 lua_write_struct_begin(HPAbstractWriter *super, const char *struct_name)
{
	HP_LUA_WRITER *self = HP_CONTAINER_OF(super, HP_LUA_WRITER, super);

	lua_newtable(self->ls);
	
	return E_HP_NOERROR;
}

hpint32 lua_write_struct_end(HPAbstractWriter *super, const char *struct_name)
{
	HP_LUA_WRITER *self = HP_CONTAINER_OF(super, HP_LUA_WRITER, super);
	
	return E_HP_NOERROR;
}

hpint32 lua_write_vector_begin(HPAbstractWriter *super)
{
	HP_LUA_WRITER *self = HP_CONTAINER_OF(super, HP_LUA_WRITER, super);	
	
	lua_newtable(self->ls);
	return E_HP_NOERROR;
}

hpint32 lua_write_vector_end(HPAbstractWriter *super)
{
	HP_LUA_WRITER *self = HP_CONTAINER_OF(super, HP_LUA_WRITER, super);
	
	
	return E_HP_NOERROR;
}

hpint32 lua_write_field_begin(HPAbstractWriter *super, const char *var_name)
{
	HP_LUA_WRITER *self = HP_CONTAINER_OF(super, HP_LUA_WRITER, super);
	
	lua_pushstring( self->ls, var_name);
	return E_HP_NOERROR;
}

hpint32 lua_write_field_end(HPAbstractWriter *super, const char *var_name)
{
	HP_LUA_WRITER *self = HP_CONTAINER_OF(super, HP_LUA_WRITER, super);	
	lua_rawset(self->ls, -3);
	return E_HP_NOERROR;
}


HP_API hpint32 lua_write_vector_item_begin(HPAbstractWriter *super, hpuint32 index)
{
	HP_LUA_WRITER *self = HP_CONTAINER_OF(super, HP_LUA_WRITER, super);
	lua_pushinteger( self->ls, index + 1);
	return E_HP_NOERROR;
}

HP_API hpint32 lua_write_vector_item_end(HPAbstractWriter *super, hpuint32 index)
{
	HP_LUA_WRITER *self = HP_CONTAINER_OF(super, HP_LUA_WRITER, super);
	lua_rawset(self->ls, -3);
	return E_HP_NOERROR;
}


HP_API hpint32 lua_write_enum_number(HPAbstractWriter *super, const int val)
{
	HP_LUA_WRITER *self = HP_CONTAINER_OF(super, HP_LUA_WRITER, super);
	lua_pushinteger(self->ls, val);
	return E_HP_NOERROR;
}

HP_API hpint32 lua_write_enum_name(HPAbstractWriter *super, const hpchar *enum_name)
{
	HP_LUA_WRITER *self = HP_CONTAINER_OF(super, HP_LUA_WRITER, super);	
	return E_HP_NOERROR;
}

hpint32 lua_write_hpchar(HPAbstractWriter *super, const char val)
{
	HP_LUA_WRITER *self = HP_CONTAINER_OF(super, HP_LUA_WRITER, super);
	char str[2];
	str[0] = val;
	str[1] = 0;
	
	lua_pushstring(self->ls, str);
	return E_HP_NOERROR;
}

hpint32 lua_write_hpdouble(HPAbstractWriter *super, const double val)
{
	HP_LUA_WRITER *self = HP_CONTAINER_OF(super, HP_LUA_WRITER, super);
	lua_pushnumber(self->ls, val);
	
	return E_HP_NOERROR;
}

hpint32 lua_write_hpint8(HPAbstractWriter *super, const hpint8 val)
{
	HP_LUA_WRITER *self = HP_CONTAINER_OF(super, HP_LUA_WRITER, super);	
	lua_pushinteger(self->ls, val);
	return E_HP_NOERROR;
}

hpint32 lua_write_hpint16(HPAbstractWriter *super, const hpint16 val)
{
	HP_LUA_WRITER *self = HP_CONTAINER_OF(super, HP_LUA_WRITER, super);
	lua_pushinteger(self->ls, val);
	return E_HP_NOERROR;
}

hpint32 lua_write_hpint32(HPAbstractWriter *super, const hpint32 val)
{
	HP_LUA_WRITER *self = HP_CONTAINER_OF(super, HP_LUA_WRITER, super);
	lua_pushinteger(self->ls, val);
	return E_HP_NOERROR;
}

hpint32 lua_write_hpint64(HPAbstractWriter *super, const hpint64 val)
{
	HP_LUA_WRITER *self = HP_CONTAINER_OF(super, HP_LUA_WRITER, super);
	//32位机上面使用这个可能会有问题
	lua_pushinteger(self->ls, val);
	return E_HP_NOERROR;
}


hpint32 lua_write_hpuint8(HPAbstractWriter *super, const hpuint8 val)
{
	HP_LUA_WRITER *self = HP_CONTAINER_OF(super, HP_LUA_WRITER, super);
	lua_pushunsigned(self->ls, val);
	return E_HP_NOERROR;
}

hpint32 lua_write_hpuint16(HPAbstractWriter *super, const hpuint16 val)
{
	HP_LUA_WRITER *self = HP_CONTAINER_OF(super, HP_LUA_WRITER, super);
	lua_pushunsigned(self->ls, val);
	return E_HP_NOERROR;
}


hpint32 lua_write_hpuint32(HPAbstractWriter *super, const hpuint32 val)
{
	HP_LUA_WRITER *self = HP_CONTAINER_OF(super, HP_LUA_WRITER, super);
	lua_pushunsigned(self->ls, val);
	return E_HP_NOERROR;
}

hpint32 lua_write_hpuint64(HPAbstractWriter *super, const hpuint64 val)
{
	HP_LUA_WRITER *self = HP_CONTAINER_OF(super, HP_LUA_WRITER, super);
	lua_pushunsigned(self->ls, val);
	return E_HP_NOERROR;
}

hpint32 lua_write_bytes(HPAbstractWriter *super, const hpbytes bytes)
{
	HP_LUA_WRITER *self = HP_CONTAINER_OF(super, HP_LUA_WRITER, super);
	lua_pushlstring(self->ls, bytes.ptr, bytes.len);
	
	return E_HP_NOERROR;
}

hpint32 lua_write_string(HPAbstractWriter *super, const hpchar* str)
{
	HP_LUA_WRITER *self = HP_CONTAINER_OF(super, HP_LUA_WRITER, super);
	lua_pushstring(self->ls, str);

	return E_HP_NOERROR;
}

hpint32 lua_write_hpbool(HPAbstractWriter *super, const hpbool val)
{
	HP_LUA_WRITER *self = HP_CONTAINER_OF(super, HP_LUA_WRITER, super);
	
	lua_pushboolean(self->ls, val);
	return E_HP_NOERROR;
}

hpint32 lua_write_null(HPAbstractWriter *super)
{
	HP_LUA_WRITER *self = HP_CONTAINER_OF(super, HP_LUA_WRITER, super);
	
	lua_pushnil(self->ls);

	return E_HP_NOERROR;
}

hpint32 lua_write_counter(HPAbstractWriter *super, const hpchar *name, const hpuint32 val)
{
	lua_write_field_begin(super, name);
	lua_write_hpuint32(super, val);
	lua_write_field_end(super, name);

	return E_HP_NOERROR;
}
