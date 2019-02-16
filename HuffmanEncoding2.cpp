#include "pch.h"
#include <iostream>
#include <vector>
#include <string>

using namespace std;

enum SpaceProcessingParameter { keep_spaces, throw_spaces };

struct HuffmanChar {
	char data;
	string path;

	HuffmanChar() {}
	HuffmanChar(char dataChar, string pathCode) {
		this->data = dataChar;
		this->path = pathCode;
	}
};

struct MinHeapNode {
	char data;		//One of the input characters
	unsigned freq;	//Frequency of the character

	//Children
	MinHeapNode *left_node, *right_node;

	//Constructors
	MinHeapNode() {
		this->left_node = NULL;
		this->right_node = NULL;
	}
	MinHeapNode(char datVal, unsigned freqVal) {
		this->data = datVal;
		this->freq = freqVal;
		this->left_node = NULL;
		this->right_node = NULL;
	}
	MinHeapNode(char datVal) : MinHeapNode(datVal, 1) {}
	MinHeapNode(char datVal, unsigned freqVal, MinHeapNode *left, MinHeapNode *right) :
		MinHeapNode(datVal, freqVal) {
		this->left_node = left;
		this->right_node = right;
	}
	MinHeapNode(unsigned freqVal, MinHeapNode *left, MinHeapNode *right) : MinHeapNode(char(), freqVal, left, right) {}
};

vector<MinHeapNode*> ScanStrForHuffmanInput(string inputStr, SpaceProcessingParameter spp) {
	vector<MinHeapNode*> outVect;
	outVect.clear();

	for (int i = 0; i < (int)inputStr.size(); i++) {
		bool foundChar = false;
		for (int j = 0; j < (int)outVect.size(); j++) {
			if (spp == throw_spaces && inputStr[i] == ' ') {
				foundChar = true;
			}
			else {
				if (outVect[j]->data == inputStr[i]) {
					foundChar = true;
					outVect[j]->freq++;
				}
			}
		}
		if (!foundChar) {
			outVect.push_back(new MinHeapNode(inputStr[i]));
		}
	}

	return outVect;
}

int CalculateTreeNodes(MinHeapNode *root, int count) {
	count++;
	if (root->left_node != NULL) {
		count = CalculateTreeNodes(root->left_node, count);
	}
	if (root->right_node != NULL) {
		count = CalculateTreeNodes(root->right_node, count);
	}
	return count;
}

vector<MinHeapNode*> SortHuffmanVector(vector<MinHeapNode*> inVect) {
	vector<MinHeapNode*> outVect;
	outVect.clear();

	for (int i = 0; i < inVect.size(); i++) {
		int maxFreq = 0;
		int num = 0;
		for (int j = 0; j < inVect.size(); j++) {
			if (inVect[j]->freq >= maxFreq) {
				maxFreq = inVect[j]->freq;
				num = j;
			}
		}
		outVect.push_back(new MinHeapNode(inVect[num]->data, inVect[num]->freq));
		inVect[num]->freq = 0;
	}

	cout << "Input vector:\n";
	for (MinHeapNode* node : outVect) {
		cout << "node = " << node->data << " " << node->freq << "\n";
	}

	return outVect;
}

MinHeapNode *BuildHuffmanTree(vector<MinHeapNode*> inVect) {
	inVect = SortHuffmanVector(inVect);
	do{
		MinHeapNode *left = inVect.back();
		inVect.pop_back();
		MinHeapNode *right = inVect.back();
		inVect.pop_back();
		int freq = right->freq + left->freq;
		inVect.push_back(new MinHeapNode(char(), freq, left, right));
	}while (inVect.size() > 1);

	return inVect.back();
}

void FindHuffmanLetters(MinHeapNode *root, vector<HuffmanChar*> *letters, string charPath) {
	if (root->left_node != NULL) {
		FindHuffmanLetters(root->left_node, letters, charPath + "0");
	}
	if (root->right_node != NULL) {
		FindHuffmanLetters(root->right_node, letters, charPath + "1");
	}
	if (root->left_node == NULL && root->right_node == NULL) {
		letters->push_back(new HuffmanChar(root->data, charPath));
	}
}

string GetHuffmanCode(char inChar, vector<HuffmanChar*> huffCharVect) {
	for (HuffmanChar *ch : huffCharVect) {
		if (inChar == ch->data) {
			return ch->path;
		}
	}
}

string EncodeMessage(string inStr, vector<HuffmanChar*> huffCharVect) {
	string outStr = "";
	for (char ch : inStr) {
		outStr += GetHuffmanCode(ch, huffCharVect);
	}

	return outStr;
}

MinHeapNode *NextNode(MinHeapNode *root, char way) {
	if (way == '0') {
		return root->left_node;
	}
	else if (way == '1') {
		return root->right_node;
	}
	else {
		return NULL;
	}
}

string DecodeMessage(string encodedMsg, MinHeapNode *root) {
	string outStr;
	MinHeapNode *node = root;

	for (int i = 0; i < encodedMsg.size(); i++)
	{
		if (NextNode(node, encodedMsg[i]) != NULL) {
			node = NextNode(node, encodedMsg[i]);
		}
		else {
			outStr += node->data;
			node = root;
			i -= 1;
		}
	}

	outStr += node->data;

	return outStr;
}

int main()
{
	cout << "*****Testing Huffman encoding*****\n";

	string inStr;

	cout << "Input your string: ";
	getline(cin, inStr);
	vector<MinHeapNode*> HuffmanInputVector = ScanStrForHuffmanInput(inStr, keep_spaces);

	MinHeapNode *htRoot = BuildHuffmanTree(HuffmanInputVector);
	
	vector<HuffmanChar*> HuffmanCharVect;
	HuffmanCharVect.clear();

	FindHuffmanLetters(htRoot, &HuffmanCharVect, "");
	cout << "Huffman Char Vect:\n";
	for (HuffmanChar *ch : HuffmanCharVect) {
		cout << " ch dat = " << ch->data << " ch path = " << ch->path << "\n";
	}

	string encodedMsg = EncodeMessage(inStr, HuffmanCharVect);
	cout << "encoded msg = " << encodedMsg << "\n";
	string decodedMsg = DecodeMessage(encodedMsg, htRoot);
	cout << "decoded msg = " << decodedMsg << "\n";
	cout << "initial msg = " << inStr << "\n";

	cout << "End.\nPress any key\n";
}