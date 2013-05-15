/**@file 
@brief Bind LUA using its C-API
*/
#ifndef _LUA_BINDING_H
#define _LUA_BINDING_H

#include "testbase.h"

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

/**
@brief  A C++ wrapper for iup.pplot

@remark 
How to pass data to LUA?

A naive way:
\arg Set data in to a global LUA table 
\arg Call the LUA function to process the table

Or alternatively:
\arg Write LUA function that takes its parameters as input data
\arg Call the function and pass data to it
*/
class LuaPlotter
{
public:
	LuaPlotter();
	~LuaPlotter();
	
	/**
	@brief Open LUA's standard libs and load the script
	@param[in] script The file name of the script. e.g. "../plotter.lua" or "c:/plotter.lua"*/
	void initLuaEnv(string script);

	void quitLuaEnv();
	void feedData(double* qsortStep, double* qsortTime, double* nsortStep, double* nsortTime, double* xValues, int n);
	void go(const char* luaFuncName);
	
	///Optional method that starts a LUA interpreter
	void startLuaShell();

	/**
	Construct a LUA table on the stack and set it to a global LUA table 
	@todo Support more types, e.g. char*
	@param[in] luaGTable The table's name in LUA script
	@param[in] keys Served as the key in a LUA table
	@param[in] values Served as the value in a LUA table
	@param[in] n The number of items in keys
	*/
	void setLuaTable(const char* luaGTable, double* keys, double* values, int n);

	///@brief Dump the current LUA stack to stdin
	void dumpStack();
protected:
	void setLegend(const char* luaGTable, const char* legend);
	
	void setField(int key, double val); 
	void setField(const char* key, const char* val);

	void checkError(int e, string msg = "");
	

	lua_State *mLuaState;
};

class TestLuaBinding : public TestBase
{
public:
	TestLuaBinding();
	~TestLuaBinding();

	void getTestData();
	void clearTestData();
	virtual void run();

protected:
	//Test Data
	int xNum;
	double* xValues;
	double* qsortStep; 
	double* qsortTime;
	double* nsortStep;
	double* nsortTime;

	LuaPlotter *mpLuaPlotter;
};

//TODO: play with LuaPlus
#endif