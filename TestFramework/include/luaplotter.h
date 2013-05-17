/**@file 
@brief Bind LUA using its C-API
*/
#ifndef _LUA_PLOTTER_
#define _LUA_PLOTTER_

#include "testbase.h"
#include <stdarg.h>
#include <vector>
#include <string>

#ifdef __cplusplus
extern "C"{
#endif
//Additional headers to integrate LUA in a C++ application.
//Also need to link lua51.lib at $(LUA_DEV)/lib
#include "lua.h"
#include "lualib.h"
#include "lauxlib.h"
#ifdef __cplusplus
};
#endif

using namespace std;

class Plot;
class Dataset;
/**
@brief  A C++ wrapper for iup.pplot
TODO: I might need a base plotter with a virtual display() method, from which a lua plotter derives.*/
class LuaPlotter
{
public:
	LuaPlotter();
	LuaPlotter(string wndTitle);
	~LuaPlotter();
	
	Plot* createPlot(string xLabel, string yLabel, string title);

	void display();

	///Dump the current LUA stack to stdin
	void dumpStack();

	///Starts a LUA interpreter(just for fun)
	void startLuaShell();

protected:
	string mWndTitle;
	lua_State *mLuaState;

	vector<Plot*> mPlotContainer;
	/**
	@brief Invoke a LUA function
	Instead of using Type Deduction, it takes a simple format string to describe the input parameters.
	@param [in] luaFuncName Name of the LUA function to be called
	@param [in] format Describe the varied length parameters
	@param [in] ... varied length parameter list, requires stdarg.h
	@remarks Do not support LUA function with return values.
	Only support limited input types(i.e. int, double, char*)*/
	int invokeLuaFunc(const char* luaFuncName, const char* format, ...);

	///Open LUA's standard libs and load the script
	void initLuaEnv(string script);
	
	///Quit LUA environment
	void quitLuaEnv();

	///Deprecated
	void setLuaTable(const char* luaGTable, double* keys, double* values, int n);

	///Deprecated
	void setField(double, double val); 

	///Dump error msg from LUA stack to stdin
	void checkError(int e, string msg = "");
};

class Plot
{
public:
	Plot(string xLabel, string yLabel, string title);
	~Plot();
	Dataset* createDataset(string legend);

private:
	friend class LuaPlotter;
	vector<Dataset*> mDatasetContainer;
	string mTitle;
	string mXLabel;
	string mYLabel;
};

class Dataset
{
public:
	Dataset(string legend);
	~Dataset();
	void addData(double x, double y);

private:
	friend class LuaPlotter;
	friend class Plot;
	class Point
	{
	public:
		Point(double x, double y):mX(x),mY(y){}
		double mX; 
		double mY;
	};
	vector<Point*> mPoints;
	string mLegend;
};

///Deprecated.
class TestLuaBinding : public TestBase
{
public:
	TestLuaBinding();
	~TestLuaBinding();

	virtual void run(){}
};

//TODO: play with LuaPlus
#endif