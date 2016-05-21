#pragma once

#include <string>
#include <vector>

#include "Tree.h"

using namespace std;

class Fano
{

	vector<long double> frequency;
	vector<int> numOfSymbols;
	vector<char> text;
	vector<long double> nonZeroFrequencies;
	vector<char> nonZeroSymbols;

	string sourceFile;
	string binaryEncryptFile;
	string symbolEncryptFile;
	string binaryDecryptFile;
	string decryptedData;
	string codesLocation;

	int sizeOfLetterFile;

	void sortFreq(vector<long double>&, vector<char>&);

	string toBinary(int);

	int toDecimal(const string&);

	void decryptCode(int, int, const vector<long double>&, Node<string>* &, char);

	vector<string> getFanoCodes(const vector<long double>&);

	vector<char> getSymbols(string);

public:
	Fano(string = "");

	void encryptBinary(string, string);

	void encryptInSymbols(string);

	void decrypt(string, string);

};