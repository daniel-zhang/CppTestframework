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

	execLuaScript();
	callLuaFunction();

	shutLua();
}

void TestLuaBinding::initLua()
{
	mLuaState = lua_open();
	luaL_openlibs(mLuaState);
	cout<<"LUA Shell"<<endl;
	string inputBuff;
	int error;
	while(getline(cin, inputBuff))
	{
		error = luaL_loadbuffer(mLuaState, inputBuff.c_str(), inputBuff.size(), "line") || lua_pcall(mLuaState, 0, 0, 0);
		if (error)
		{
			cout<<lua_tostring(mLuaState, -1)<<endl;
			lua_pop(mLuaState, 1);
		}
	}
}

void TestLuaBinding::shutLua()
{

}

void TestLuaBinding::execLuaScript()
{

}

void TestLuaBinding::callLuaFunction()
{
	lua_close(mLuaState);
}


