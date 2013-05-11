#pragma once

#include <iostream>
#include <fstream>
#include <string>

#include "testbase.h"

using namespace std;


class TestSTL_string
{
public:
	string i;
	string j;
	TestSTL_string()
	{
		i = "default"; 
		j ="default";
	}
};

class TestSerialization : public TestBase
{
public:
	TestSerialization();

	//二进制序列化，测试std::string深拷贝(copy on write)
	virtual void run(bool mute);
};


