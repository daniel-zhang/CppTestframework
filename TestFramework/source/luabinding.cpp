#include "luabinding.h"
#include <iostream>

using namespace std;

//
//LuaPlotter
//
LuaPlotter::LuaPlotter()
{

}

LuaPlotter::~LuaPlotter()
{

}

void LuaPlotter::initLuaEnv( string script )
{
	mLuaState = lua_open();
	luaL_openlibs(mLuaState);
	//Script should not contain MAIN chunk
	int error = luaL_dofile(mLuaState, script.c_str());
	checkError(error, string("Load LUA script:\"") + script + "\"...OK");
}

void LuaPlotter::quitLuaEnv()
{
	lua_close(mLuaState);
}

//Assume initLuaEnv() is called and succeeded
void LuaPlotter::feedData( double* qsortStep, double* qsortTime, double* nsortStep, double* nsortTime, double* xValues, int n )
{
// 	//dump ori data
// 	for (int i = 0; i < n; ++i)
// 	{
// 		cout<<i<<": "<<qsortStep[i]<<endl;
// 	}
//	setLegend("qsortLegend", "Quick Sort");
//	setLegend("nsortLegend", "Naive Sort");

	setLuaTable("qsortSteps", xValues, qsortStep, n);
	setLuaTable("qsortTime", xValues, qsortTime, n);
	setLuaTable("nsortSteps", xValues, nsortStep, n);
	setLuaTable("nsortTime", xValues, nsortTime, n);

	//dump lua data
/*	luaL_dostring(mLuaState, "for i,v in pairs(qsortSteps) do print(i,v) end");*/
}


void LuaPlotter::go(const char* luaFuncName)
{
	lua_getglobal(mLuaState, luaFuncName);
	int error = lua_pcall(mLuaState, 0, 0, 0);
	checkError(error);
}

//Internal stack operations
void LuaPlotter::checkError(int e, string msg)
{
	if (e)
	{
		cout<<lua_tostring(mLuaState, -1)<<endl;
		lua_pop(mLuaState, 1);

#ifdef _DEBUG
		dumpStack();
#endif

	}
#ifdef _DEBUG
	else if (!msg.empty())
	{
		cout<<msg<<endl;
	}
#endif
}

void LuaPlotter::dumpStack()
{
	cout<<"Stack Dump[base..top]:"<<endl;
	for (int i = 1; i <= lua_gettop(mLuaState); i++)
	{
		int t = lua_type(mLuaState, i);
		switch(t)
		{
		case LUA_TSTRING:
			std::cout<<i<<":(string)"<<lua_tostring(mLuaState, i);
			break;

		case LUA_TBOOLEAN:
			std::cout<<i<<":(boolean)"<<(lua_toboolean(mLuaState, i)?"true":"false");
			break;

		case LUA_TNUMBER:
			std::cout<<i<<":(number)"<<(int)lua_tonumber(mLuaState, i);
			break;

		default:
			std::cout<<i<<":"<<lua_typename(mLuaState, t);
			break;
		}
		std::cout<<" -> ";
	}
	std::cout<<endl<<endl;
}


void LuaPlotter::setLegend( const char* luaGTable, const char* legend )
{
	lua_newtable(mLuaState);
	setField(luaGTable, legend);
	lua_setglobal(mLuaState, luaGTable);
}

void LuaPlotter::setLuaTable( const char* luaGTable, double* keys, double* values , int n)
{
	lua_newtable(mLuaState);
	for (int i = 0; i < n; ++i)
	{
		setField(keys[i], values[i]);
	}
	lua_setglobal(mLuaState, luaGTable);
}

void LuaPlotter::setField( int key, double val )
{
	lua_pushnumber(mLuaState, key);
	lua_pushnumber(mLuaState, val);
	lua_settable(mLuaState, -3);
}

void LuaPlotter::setField( const char* key, const char* val )
{
	lua_pushstring(mLuaState, key);
	lua_pushstring(mLuaState, val);
	lua_settable(mLuaState, -3);
}

void LuaPlotter::startLuaShell()
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

//
//Test Lua Binding
//

TestLuaBinding::TestLuaBinding()
{
	mAlias = "TestLuaBinding";
}

TestLuaBinding::~TestLuaBinding()
{

}

void TestLuaBinding::getTestData()
{
	xNum = 10;
	qsortStep = new double[xNum];
	qsortTime = new double[xNum];
	nsortStep = new double[xNum];
	nsortTime = new double[xNum];
	xValues = new double[xNum];

	for (int i = 0; i < xNum; ++i)
	{
		xValues[i] = (i)*10000;
		qsortStep[i] = i*i + 3*i + 12;
		qsortTime[i] = i*i + 2*i + 3;

		nsortTime[i] = i*i + 4*i;
		nsortStep[i] = i*i - i;
	}
}

void TestLuaBinding::clearTestData()
{
	delete[] qsortTime;
	delete[] qsortStep;
	delete[] nsortTime;
	delete[] nsortStep;
	delete[] xValues;
}


void TestLuaBinding::run()
{
	getTestData();

	mpLuaPlotter = new LuaPlotter();
	mpLuaPlotter->initLuaEnv("../lua/test.lua");

	mpLuaPlotter->feedData(qsortStep, qsortTime, nsortStep, nsortTime, xValues, xNum);
	mpLuaPlotter->go("plot");

	mpLuaPlotter->quitLuaEnv();

	delete mpLuaPlotter;

	clearTestData();
}
