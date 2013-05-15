#include "luaplotter.h"
#include <iostream>

using namespace std;

//
//LuaPlotter
//
LuaPlotter::LuaPlotter()
{
}

LuaPlotter::LuaPlotter( string wndTitle ):mWndTitle(wndTitle)
{
}

LuaPlotter::~LuaPlotter()
{
	for (unsigned int i = 0; i < mPlotContainer.size(); ++i)
	{
		delete mPlotContainer[i];
		mPlotContainer[i] = NULL;
	}
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


void LuaPlotter::setLuaTable( const char* luaGTable, double* keys, double* values , int n)
{
	lua_newtable(mLuaState);
	for (int i = 0; i < n; ++i)
	{
		setField(keys[i], values[i]);
	}
	lua_setglobal(mLuaState, luaGTable);
}

void LuaPlotter::setField( double key, double val )
{
	lua_pushnumber(mLuaState, key);
	lua_pushnumber(mLuaState, val);
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

Plot* LuaPlotter::createPlot( string xLabel, string yLabel, string title )
{
	Plot* p = new Plot(xLabel, yLabel, title);
	mPlotContainer.push_back(p);
	return p;
}

void LuaPlotter::display()
{
	initLuaEnv("../lua/plot.lua");

	vector<Plot*>::const_iterator plot;
	vector<Dataset*>::const_iterator dataset;
	vector<Dataset::Point*>::const_iterator point;
	for (plot = mPlotContainer.begin(); plot != mPlotContainer.end(); plot++)
	{
		invokeLuaFunc("createPlot","sss", (*plot)->mTitle.c_str(), (*plot)->mXLabel.c_str(), (*plot)->mYLabel.c_str());
		for (dataset = (*plot)->mDatasetContainer.begin(); dataset != (*plot)->mDatasetContainer.end(); dataset++)
		{
			invokeLuaFunc("startAdd", "");
			for (point = (*dataset)->mPoints.begin(); point != (*dataset)->mPoints.end(); point++)
				invokeLuaFunc("addData", "dd", (*point)->mX, (*point)->mY);
			invokeLuaFunc("finishAdd","s",(*dataset)->mLegend.c_str());
		}
		invokeLuaFunc("finishPlot","");
	}
	invokeLuaFunc("show","s", mWndTitle.c_str());
	quitLuaEnv();
}

/**Plot*/
Plot::Plot( string xLabel, string yLabel, string title ):mTitle(title), mXLabel(xLabel), mYLabel(yLabel){}
Plot::~Plot()
{
	for (unsigned int i = 0; i < mDatasetContainer.size(); ++i)
	{
		delete mDatasetContainer[i];
		mDatasetContainer[i] = NULL;
	}
}

Dataset* Plot::createDataset( string legend )
{
	Dataset* ds = new Dataset(legend);
	mDatasetContainer.push_back(ds);
	return ds;
}

/**Dataset */
Dataset::Dataset( string legend ):mLegend(legend){}
Dataset::~Dataset()
{
	for (unsigned int i = 0; i < mPoints.size(); ++i)
	{
		delete mPoints[i];
		mPoints[i] = NULL;
	}
}

void Dataset::addData( double x, double y )
{
	mPoints.push_back(new Point(x,y));
}

