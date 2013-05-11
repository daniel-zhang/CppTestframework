#include "serialization.h"


TestSerialization::TestSerialization()
{
	mAlias = "TestBinSerialization";
}

void TestSerialization::run()
{
	string path = "./data";
	ofstream outFile;
	outFile.open(path.c_str(), ios::binary);
	if (outFile.fail())
	{
		cout<<"Failed to open:"<<path<<endl;
		system("pause");
		return;
	}
	TestSTL_string* testData = new TestSTL_string;
	testData->i = "good";
	testData->j = "morning";

	outFile.write((const char*)testData, sizeof(TestSTL_string));
	outFile.close();
	delete testData;

	ifstream inFile;
	inFile.open(path.c_str(), ios::binary);
	if (inFile.fail())
	{
		cout<<"Failed to open:"<<path<<endl;
		system("pause");
		return;
	}
	TestSTL_string* p = new TestSTL_string;
	inFile.read((char*)p, sizeof(TestSTL_string));
	inFile.close();

	__assert__((p->i == "good" && p->j == "morning"));
}
