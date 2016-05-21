#include <iostream>
#include <fstream>
#include <cctype>

#include "Tree.h"
#include "Fano.h"

using namespace std;

bool test1()
{
	Tree<int> tr;

	bool passed = tr.isEmpty() ? true : false;

	if (passed)
	{
		tr.add(10);
		tr.add(5);
		tr.add(12);
		tr.add(2);
		tr.add(7);
		tr.add(6);
		tr.add(11);

		passed = (tr.getLeaves() == vector<int>{ 2, 6, 11 } ? true : false);

		if (passed)
		{

			tr.deleteElem(6);

			passed = (tr.getLeaves() == vector<int>{2, 7, 11} ? true : false);

			if (passed)
			{
				tr.deleteElem(5);

				passed = (tr.getLeaves() == vector<int>{7, 11} ? true : false);
			}
		}
	}

	return passed;
}

bool test2()
{
	Fano data("Data.txt");

	data.encryptBinary("BinaryEncrypt.txt", "Codes.txt");
	data.encryptInSymbols("LetterEncrypt.txt");
	data.decrypt("BinaryDecrypt.txt", "Decrypt.txt");

	bool passed = true;

	ifstream in1("Data.txt");
	ifstream in2("Decrypt.txt");

	char ch1;
	char ch2;

	while (in1 >> ch1)
	{
		in2 >> ch2;

		if (int(ch1) >= 0 && isgraph(ch1))
		{
			if (ch1 != ch2)
			{
				passed = false;
				break;
			}
		}
	}

	return passed;
}

bool test3()
{
	_CrtMemState c1, c2, c3;

	_CrtMemCheckpoint(&c1);

	{
		Tree<int> test;
		test.add(3);
		test.add(2);
		test.add(1);
	}

	_CrtMemCheckpoint(&c2);

	return !_CrtMemDifference(&c3, &c1, &c2);
}


int main()
{
	cout << "Waiting for tests' results...\n";

	if (!test1())
	{
		cout << "Test 1 failed" << endl;
	}
   
	if (!test2())
	{
		cout << "Test 2 failed" << endl;
	}
	
	if (!test3())
	{
		cout << "Test 3 failed" << endl;
	}

	system("pause");
	return 0;
}