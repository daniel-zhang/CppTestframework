#pragma once
#include "testbase.h"
#include "reflect.h"

class TestReflection : public TestBase
{
public:
	TestReflection()
	{
		mAlias = "TestReflection";
	}

	virtual void run(bool mute)
	{
		__assert__(1);
	}
};