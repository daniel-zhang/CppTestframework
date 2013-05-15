/**@file 
@brief Bind LUA using its C-API
*/
#ifndef _LUA_BINDING_H
#define _LUA_BINDING_H

#include "testbase.h"
#include <stdarg.h>
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

Some methods are deprecated since I found them coupled TOO closely to LUA code.*/
class LuaPlotter
{
public:
	LuaPlotter();
	~LuaPlotter();
	
	/**
	@brief Open LUA's standard libs and load the script
	@param[in] script The file name of the script. e.g. "../plotter.lua" or "c:/plotter.lua"*/
	void initLuaEnv(string script);
	
	///Quit LUA environment
	void quitLuaEnv();
	
	/**
	@brief Invoke a LUA function
	Instead of using Type Deduction, the method takes a simple format string to describe the input parameters.
	@param [in] luaFuncName Name of the LUA function to be called
	@param [in] format Describe the varied length parameters
	@param [in] ... varied length parameter list, requires stdarg.h
	@remarks Do not support LUA function with return values.
	Only support limited input types(i.e. int, double, char*)*/
	int invokeLuaFunc(const char* luaFuncName, const char* format, ...);

	///Dump the current LUA stack to stdin
	void dumpStack();
	///Starts a LUA interpreter(just for fun)
	void startLuaShell();

	///Deprecated, use invokeLuaFunc() instead.
	void feedData(double* qsortStep, double* qsortTime, double* nsortStep, double* nsortTime, double* xValues, int n);
	///Deprecated, use invokeLuaFunc() instead.
	void go(const char* luaFuncName);
	///Deprecated since set data directly is not preferable.
	void setLuaTable(const char* luaGTable, double* keys, double* values, int n);
protected:
	///Deprecated
	void setLegend(const char* luaGTable, const char* legend);
	///Deprecated
	void setField(int key, double val); 
	///Deprecated
	void setField(const char* key, const char* val);

	/**@brief If error, dump the error msg in the LUA stack into stdin
	@param e The error code returned from previous LUA stack operation.
	A none-zero indicates an error
	@param msg User defined msg, used only in DEBUG mode.
	*/
	void checkError(int e, string msg = "");
	
	lua_State *mLuaState;
};

class TestLuaBinding : public TestBase
{
public:
	TestLuaBinding();
	~TestLuaBinding();

	virtual void run();

};

//TODO: play with LuaPlus
#endif