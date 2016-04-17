#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include <cmath>

#include "Tree.h"

using namespace std;

typedef vector<long double>::iterator Iterator;

void sortFreq(vector<long double>& freq, vector<char>& sym)
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

void decryptCode(int start, int finish, const vector<long double>& freq, Node<string>* & node , char symbol)
{

	node->data += symbol;

	if (finish - start > 0)
	{
		long double min = 1;
		int min_start;

		for (size_t i = start; i < finish; ++i)
		{
			if (abs(freq[i] - freq[i + 1]) < min)
			{
				min = abs(freq[i] - freq[i + 1]);
				min_start = i;
			}
		}

		node->setLeftSon(node->data);
		node->setRightSon(node->data);

		decryptCode(start, min_start, freq, node->getLeft(), '0');
		decryptCode(min_start + 1, finish, freq, node->getRight(), '1');

	}
}

vector<string> getFanoCodes(const vector<long double>& freq)
{
	Tree<string> tr;

	tr.add("");

	decryptCode(0, freq.size() - 1, freq,  tr.getRoot(), ' ');

	vector<string> toReturn = tr.getLeaves();

	for (size_t i = 0; i < toReturn.size(); ++i)
	{
		toReturn[i].erase(0, 1);
	}

	/*
	for (size_t i = 0; i < toReturn.size(); ++i)
	{
		Node<string>* node = tr.find(toReturn[i]);

		if (node->left != nullptr || node->right != nullptr)
		{
			toReturn.erase(i, i);
		}
	}
	*/

	tr.print();

	return toReturn;
}

template <typename T>
string toString(const T& t)
{
	ostringstream oStream;
	oStream << t;

	return oStream.str();
}

int toDecimal(const string& st)
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


string toBinary(int dec)
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


int main()
{
	
	vector<long double> frequency(128);
	vector<int> numOfSymbols(128);
	vector<char> text;

	/*
	for (size_t i = 0; i < 128; ++i)
	{
		cout << frequency[i] << endl;
	}
	*/
	

	ifstream in("Data.txt");
	char ch;
	int textSize = 0;

	while (in.get(ch))
	{
		numOfSymbols[int(ch)]++;
		text.push_back(ch);
		++textSize;
	}

	in.clear();
	in.seekg(0);
	in.close();

	for (size_t i = 0; i < 128; ++i)
	{
		frequency[i] = double(numOfSymbols[i]) / textSize;
	}

	/*
	long double sumOfFrequencies = 0;

	
	for (size_t i = 0; i < 128; ++i)
	{
		cout << frequency[i] << endl;
		sumOfFrequencies += frequency[i];
	}

	cout << sumOfFrequencies << endl;
	*/
	

	vector<long double> nonZeroFrequencies;
	vector<char> nonZeroSymbols;

	long double sumOfFrequencies = 0;
	 
	// don't forget to decomment!!! - OK
	
	for (size_t i = 0; i < 128; ++i)
	{
		if (frequency[i] != 0)
		{
			nonZeroFrequencies.push_back(frequency[i]);
			nonZeroSymbols.push_back(char(i));
			sumOfFrequencies += frequency[i];
		}
	}

	sortFreq(nonZeroFrequencies, nonZeroSymbols);

	
	for (size_t i = 0; i < nonZeroFrequencies.size(); ++i)
	{
		cout << nonZeroSymbols[i] << " -- " << nonZeroFrequencies[i] << endl;
	}
	

	// THAT'S WHERE FUN BEGINS :))

	vector<string> codes = getFanoCodes(nonZeroFrequencies);

	ofstream out("BinaryEncrypt.txt");

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

	ifstream binaryInput("BinaryEncrypt.txt");


	vector<char> decryptedText;

	string buffer = "";
	char read;

	while (binaryInput.get(read))
	{
		buffer += read;

		if (buffer.size() == 8)
		{
			char write = char(toDecimal(buffer));
			decryptedText.push_back(write);
			buffer = "";
		}
	}

	if (buffer.size() != 0)
	{
		buffer += string(8 - buffer.size(), ' ');
		char write = char(toDecimal(buffer));
		decryptedText.push_back(write);
		buffer = "";
	}

	binaryInput.close();


	// Still OK



	ofstream binaryOutput("BinaryDecrypt.txt");

	for (size_t i = 0; i < decryptedText.size(); ++i)
	{
		if (int(decryptedText[i]) < 0)
		{
			binaryOutput << toBinary(decryptedText[i] + 256);
		}
		else
		{
			binaryOutput << toBinary(decryptedText[i]);
		}
	}


	binaryOutput.close();

	ifstream toDecrypt("BinaryDecrypt.txt");
	ofstream decrypted("Decrypt.txt");

	string buffer1 = "";

	while (!toDecrypt.eof())
	{
		toDecrypt.get(read);
		buffer1 += read;

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

	

	system("pause");
	return 0;
}