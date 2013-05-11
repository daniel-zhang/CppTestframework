#pragma once

#include <vector>
#include <string>
#include <iostream>
#include <exception>

#include "testbase.h"

using namespace std;

class TestFramework
{
public:
	TestFramework();
	~TestFramework();

	template<typename T>
	void addTest()
	{
		//Check T
		TestBase* p = new T;

		mTestRegistry.push_back(p);
	}

	void startAll();
	void setMute();
	
private:
	void touch(bool& isTouched);
	void evaluate();

	bool mMute;
	
	vector<TestBase*> mTestRegistry;
};