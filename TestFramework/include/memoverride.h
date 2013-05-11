#pragma once
#include "testbase.h"

class StaticClass
{
public:
	
};

class TestMemOverride : public TestBase
{
public:
	TestMemOverride(){mAlias = "TestMemOverride";}
	virtual void run(bool mute);
};