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
	//As luaL_dofile is used here, script should not contain MAIN chunk
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

int LuaPlotter::invokeLuaFunc( const char* luaFuncName, const char* format, ... )
{
	lua_getglobal(mLuaState, luaFuncName);

	va_list argv;
	va_start(argv, format);
	int argc = 0;
	while(*format)
	{
		switch(*format++)
		{
		case 'd':
			lua_pushnumber(mLuaState, va_arg(argv, double));
			break;

		case 'i':
			lua_pushnumber(mLuaState, va_arg(argv, int));
			break;

		case 's':
			lua_pushstring(mLuaState, va_arg(argv, char*));
			break;

		case '>':
			goto endwhile;

		default:
			cout<<"invokeLuaFunc: invalid option "<<*(format-1)<<endl;
			return -1;
		}
		argc++;
		//Check if stack overflows, since a varied length arg list might cause that.
		luaL_checkstack(mLuaState, 1, "too many arguments");
	}endwhile:

	int error = lua_pcall(mLuaState, argc, 0, 0);
	checkError(error);
	va_end(argv);

	return error;
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

void TestLuaBinding::run()
{	
	LuaPlotter* p = new LuaPlotter();
	p->initLuaEnv("../lua/functest.lua");
	
	int error = 0;

	error = p->invokeLuaFunc("t0", "" );
	__assert__(error==0);

	error = p->invokeLuaFunc("t1", "s", "first" );
	__assert__(error==0);

	error = p->invokeLuaFunc("t2", "sd", "first", 2.0001);
	__assert__(error==0);

	error = p->invokeLuaFunc("t3", "sdi", "first", 2.0001, 3 );
	__assert__(error==0);

	p->quitLuaEnv();

	delete p;
}
