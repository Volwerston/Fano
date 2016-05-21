#pragma once

#include <iostream>
#include <fstream>
#include <string>
#include <cstring>
#include <vector>

#include "Fano.h"
#include "Tree.h"

using namespace std;

void Fano::sortFreq(vector<long double>& freq, vector<char>& sym)
{
	for (size_t i = 0; i < freq.size(); ++i)
	{
		for (size_t j = 0; j < freq.size() - (i + 1); ++j)
		{
			if (freq[j] < freq[j + 1])
			{
				long double buf1 = freq[j];
				freq[j] = freq[j + 1];
				freq[j + 1] = buf1;

				char buf2 = sym[j];
				sym[j] = sym[j + 1];
				sym[j + 1] = buf2;
			}
		}
	}
}

string Fano::toBinary(int dec)
{
	string base = string(8, ' ');

	int currentNum = dec;

	int power = 7;

	while (currentNum > 0)
	{
		if (currentNum >= pow(2, power))
		{
			base[base.size() - (power + 1)] = '1';
			currentNum -= pow(2, power);
		}

		--power;
	}

	for (size_t i = 0; i < base.size(); ++i)
	{
		if (base[i] == ' ')
		{
			base[i] = '0';
		}
	}

	return base;
}

int Fano::toDecimal(const string& st)
{
	int toReturn = 0;

	for (int i = st.size() - 1; i >= 0; --i)
	{
		if (st[i] == '1')
		{
			toReturn += pow(2, st.size() - (i + 1));
		}
	}

	return toReturn;
}

void Fano::decryptCode(int start, int finish, const vector<long double>& freq, Node<string>* & node, char symbol)
{
	node->data += symbol;

	if (finish - start > 0)
	{
		long double min = 1;
		int min_start;
		long double down_freq;
		long double top_freq;

		
		for (size_t i = start; i < finish; ++i)
		{

			down_freq = 0;
			top_freq = 0;

			for (size_t j = start; j <= i; ++j)
			{
				top_freq += freq[j];
			}

			for (size_t j = start + 1; j <= finish; ++j)
			{
				down_freq += freq[j];
			}

			if (abs(top_freq - down_freq) < min)
			{
				min = abs(top_freq - down_freq);
				min_start = i;
			}
		}
		

		node->setLeftSon(node->data);
		node->setRightSon(node->data);

		decryptCode(start, min_start, freq, node->getLeft(), '0');
		decryptCode(min_start + 1, finish, freq, node->getRight(), '1');

	}
}

vector<string> Fano::getFanoCodes(const vector<long double>& freq)
{
	Tree<string> tr;

	tr.add("");

	decryptCode(0, freq.size() - 1, freq, tr.getRoot(), ' ');

	vector<string> toReturn = tr.getLeaves();

	for (size_t i = 0; i < toReturn.size(); ++i)
	{
		toReturn[i].erase(0, 1);
	}

	return toReturn;
}

Fano::Fano(string _dir)
	: sourceFile(_dir),
	binaryEncryptFile(""),
	symbolEncryptFile(""),
	binaryDecryptFile(""),
	decryptedData(""),
	codesLocation("")
{
	for (size_t i = 0; i < 256; ++i)
	{
		frequency.push_back(0.0);
		numOfSymbols.push_back(0);
	}

	ifstream in(sourceFile);

	char ch;
	int textSize = 0;

	while (in.get(ch))
	{
		numOfSymbols[int(ch) + 128]++;
		text.push_back(ch);
		++textSize;
	}

	in.clear();
	in.seekg(0);
	in.close();

	for (size_t i = 0; i < 256; ++i)
	{
		frequency[i] = double(numOfSymbols[i]) / textSize;
	}

	for (size_t i = 0; i < 256; ++i)
	{
		if (frequency[i] != 0)
		{
			nonZeroFrequencies.push_back(frequency[i]);
			nonZeroSymbols.push_back(char(i - 128));
		}
	}

	sortFreq(nonZeroFrequencies, nonZeroSymbols);
}

void Fano::encryptBinary(string _dir, string _codesLocate)
{
	vector<string> codes = getFanoCodes(nonZeroFrequencies);

	codesLocation = _codesLocate;
	binaryEncryptFile = _dir;

	ofstream outCodes(codesLocation);

	for (size_t i = 0; i < codes.size(); ++i)
	{
		outCodes << nonZeroSymbols[i] << " " << codes[i] << endl;
	}


	outCodes.close();

	ofstream out(binaryEncryptFile);

	for (size_t i = 0; i < text.size(); ++i)
	{
		int position = 0;

		for (size_t j = 0; j < nonZeroSymbols.size(); ++j)
		{
			if (text[i] == nonZeroSymbols[j])
			{
				position = j;
				break;
			}
		}

		out << codes[position];
	}

	out.close();
}

void Fano::encryptInSymbols(string _dirSymbols)
{

	symbolEncryptFile = _dirSymbols;

	ifstream binaryInput(binaryEncryptFile);
	ofstream outLetters(symbolEncryptFile, ios::binary);

	int counter = 0;

	string buffer = "";
	char read;

	while (binaryInput.get(read))
	{
		buffer += read;
		++counter;

		if (buffer.size() == 8)
		{
			char write = char(toDecimal(buffer));

			char bufferChar[1];
			bufferChar[0] = write;

			outLetters.write(bufferChar , 1);
			buffer = "";
		}
	}

	if (buffer.size() != 0)
	{
		buffer += string(8 - buffer.size(), ' ');
		char write = char(toDecimal(buffer));

		char bufferChar[1];
		bufferChar[0] = write;

		outLetters.write(bufferChar, 1);
		buffer = "";
	}

	outLetters.close();
	binaryInput.close();
}

vector<char> Fano::getSymbols(string _source)
{
	vector<char> toReturn;
	int ignore = 0;
	bool canRead = true;
	char ch[1];
	
	ifstream in(symbolEncryptFile, ios::binary);

	while (canRead)
	{
		in.clear();
		in.seekg(ignore, ios::beg);

		int counter = 0;

		in.clear();

		int prev = ignore;

		string buf;

		while (in.read(ch, 1))
		{
			buf = ch;
			toReturn.push_back(buf[0]);
			++ignore;
		}

		toReturn.push_back('\x1a');

		++ignore;
		in.clear();

		if (ignore - prev == 1)
		{
			break;
		}
	}

	return toReturn;
}

void Fano::decrypt(string _dirBinary, string _dirText)
{
	binaryDecryptFile = _dirBinary;
	decryptedData = _dirText;

	vector<string> codes(nonZeroSymbols.size());

	ifstream inCodes(codesLocation);

	string code;
	char symbol;

	while (!inCodes.eof())
	{
		inCodes.get(symbol);
		inCodes >> code;
		inCodes.ignore(10000, '\n');

		for (size_t i = 0; i < nonZeroSymbols.size(); ++i)
		{
			if (symbol == nonZeroSymbols[i])
			{
				codes[i] = code;
				break;
			}
		}
	}

	inCodes.close();

	ofstream binaryOutput(_dirBinary);
	int counter = 0;

	vector<char> ourChars = getSymbols(symbolEncryptFile);

	for (size_t i = 0; i < ourChars.size(); ++i)
	{
		if (int(ourChars[i]) < 0)
		{
			binaryOutput << toBinary(int(ourChars[i]) + 256);
		}
		else
		{
			binaryOutput << toBinary(int(ourChars[i]));
		}
	}

	binaryOutput.close();

	ifstream toDecrypt(binaryDecryptFile);
	ofstream decrypted(decryptedData);
	char ch1;
	string buffer1 = "";

	while (!toDecrypt.eof())
	{
		toDecrypt.get(ch1);
		buffer1 += ch1;

		for (size_t i = 0; i < codes.size(); ++i)
		{
			if (buffer1 == codes[i])
			{
				decrypted << nonZeroSymbols[i];
				buffer1 = "";
				break;
			}
		}
	}

	toDecrypt.close();
	decrypted.close();
	
}
