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
	void addTest(bool mute = true)
	{
		TestBase* p = new T;
		p->setMute(mute);
		mTestRegistry.push_back(p);
	}

	void startAll();
	void setMute();
	
private:
	void touch(TestBase* testCase);
	void evaluate();
	vector<TestBase*> mTestRegistry;
};