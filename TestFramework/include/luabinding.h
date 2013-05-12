#ifndef _LUA_BINDING_H
#define _LUA_BINDING_H
#include "testbase.h"

/*
Additional includes to integrate LUA in a C++ application
Also need to link lua51.lib at $(LUA_DEV)/lib
*/
#ifdef __cplusplus
extern "C"{
#endif

#include "lua.h"
#include "lualib.h"
#include "lauxlib.h"

#ifdef __cplusplus
};
#endif

//Play with LUA C-API
class TestLuaBinding : public TestBase
{
public:
	TestLuaBinding();
	~TestLuaBinding();
	virtual void run();

protected:
	void initLua();
	void shutLua();

	void stackDump(lua_State* L);
	
	void startLuaShell();

	void testLuaStack();

	lua_State* mLuaState;
};

//TODO: play with LuaPlus
#endif