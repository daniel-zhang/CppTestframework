#include "luabinding.h"
#include <iostream>

using namespace std;

TestLuaBinding::TestLuaBinding()
{
	mAlias = "TestLuaBinding";
}

TestLuaBinding::~TestLuaBinding()
{

}

void TestLuaBinding::run()
{
	initLua();



	testLuaStack();

	shutLua();
}

void TestLuaBinding::initLua()
{
	mLuaState = lua_open();

}

void TestLuaBinding::shutLua()
{
	lua_close(mLuaState);
}

void TestLuaBinding::stackDump( lua_State* L )
{
	int top = lua_gettop(L);

	for (int i = 1; i <= top; i++)
	{
		int t = lua_type(L, i);
		switch(t)
		{
		case LUA_TSTRING:
			std::cout<<lua_tostring(L, i);
			break;
			
		case LUA_TBOOLEAN:
			std::cout<<(lua_toboolean(L, i)?"true":"false");
			break;

		case LUA_TNUMBER:
			std::cout<<(int)lua_tonumber(L, i);
			break;

		default:
			std::cout<<lua_typename(L, t);
			break;
		}
		std::cout<<" | ";
	}
	std::cout<<endl;
}

void TestLuaBinding::startLuaShell()
{
	luaL_openlibs(mLuaState);
	std::cout<<"LUA Shell"<<endl;
	string inputBuff;
	int error;
	while(getline(cin, inputBuff))
	{
		error = luaL_loadbuffer(mLuaState, inputBuff.c_str(), inputBuff.size(), "line") || lua_pcall(mLuaState, 0, 0, 0);
		if (error)
		{
			std::cout<<lua_tostring(mLuaState, -1)<<endl;
			lua_pop(mLuaState, 1);
		}
	}
}

void TestLuaBinding::testLuaStack()
{
	lua_pushboolean(mLuaState, true);
	lua_pushnumber(mLuaState, 10);
	lua_pushnil(mLuaState);
	lua_pushstring(mLuaState, "Hello");
	stackDump(mLuaState);

	lua_pushvalue(mLuaState, -4); stackDump(mLuaState);
	/* true  10  nil  `hello'  true  */

	lua_replace(mLuaState, 3); stackDump(mLuaState);
	/* true  10  true  `hello'  */

	lua_settop(mLuaState, 6); stackDump(mLuaState);
	/* true  10  true  `hello'  nil  nil  */

	lua_remove(mLuaState, -3); stackDump(mLuaState);
	/* true  10  true  nil  nil  */

	lua_settop(mLuaState, -5); stackDump(mLuaState);
	/* true  */
	lua_pushvalue(mLuaState, 1); stackDump(mLuaState);
	lua_insert(mLuaState, 1); stackDump(mLuaState);
}


