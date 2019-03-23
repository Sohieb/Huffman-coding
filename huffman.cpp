#include "huffman.h"
#include <algorithm>
#include <iostream>
#include <fstream>
#include <stdio.h>
#include <string>
#include <vector>
#include <queue>
#include <bitset>

using namespace std;

Node::Node(int _data, int _frq) {
	this->data = _data;
	this->frequency = _frq;
	this->left = NULL;
	this->right = NULL;
}

void Node::merge(Node * leftNode, Node * rightNode) {
	this->left = leftNode;
	this->right = rightNode;
	this->frequency = leftNode->frequency + rightNode->frequency;
}

int binaryToDecimal(string bin) {
	while ((int)bin.size() < 8) bin += "0";
	int dec = 0;
	reverse(bin.begin(), bin.end());
	for (int i = 0; i < (int)bin.size(); ++i)
		dec += (bin[i] == '1') * (1 << i);
	return dec;
}

string decimalToBinary(int dec, int fixLen = -1) {
	string bin;
	while (dec > 0) {
		bin = char('0' + dec % 2) + bin;
		dec /= 2;
	}
	if (fixLen == -1)
		return bin;
	while ((int)bin.size() < fixLen)
		bin = "0" + bin;
	return bin;
}

Huffman::Huffman(string fileName, string option) {
	if (option == "-c") {
		// COMPRESS
		vector<int> frq(127, 0); // [32 - 126] all printable standard ASCII
		char dummy;
		freopen(fileName.c_str(), "r", stdin);
		while(scanf("%c", &dummy) != EOF)
			++frq[dummy];
		fclose(stdin);
		vector<Node *> initNodes;
		for(int i = 32; i < 127; ++i){	// 126
			Node * newNode = new Node(char(i), frq[i]);
			initNodes.push_back(newNode);
		}
		this->root = constructTree(initNodes);
		this->constructTable(this->root, "");
		this->compress(fileName);
	} else {
		// DECOMPRESS
		this->decompress(fileName);
	}
}

bool cmp(Node* &LHS, Node* &RHS) {
	if(LHS->getFrequency() > RHS->getFrequency())
		return true;
	return false;
}

Node * Huffman:: constructTree(vector<Node *> initNodes){
	priority_queue<Node *, vector<Node *>, decltype(&cmp)> huffmanHeap(&cmp);
	for(int i = 0; i < (int)initNodes.size(); ++i)
		huffmanHeap.push(initNodes[i]);
	while((int)huffmanHeap.size() > 1){
		Node *leftNode = huffmanHeap.top();
		huffmanHeap.pop();
		Node *rightNode = huffmanHeap.top();
		huffmanHeap.pop();
		Node * parNode = new Node();
		parNode->merge(leftNode, rightNode);
		huffmanHeap.push(parNode);
	}
	return huffmanHeap.top();
}

void Huffman::constructTable(Node * curNode, string curCode) {
	if(curNode->getLeft() == NULL && curNode->getRight() == NULL) {
		this->lookUpTable[curNode->getData()] = curCode;
		return;
	}
	constructTable(curNode->getLeft(), curCode + '0');
	constructTable(curNode->getRight(), curCode + '1');
	return;
}

void Huffman::compress(string originalFileName) {
	// TODO
	string inBuf;
	string outBuf;
	string header;
	string outStr;

	for (int i = 32; i < 127; ++i) {
		string codeWord = this->lookUpTable[i];
		int codeWordLength = (int)codeWord.size();
		int ignoreLen = 40 - codeWordLength;
		while ((int)codeWord.size() < 40)
			codeWord += "0";
		header += decimalToBinary(ignoreLen, 8);
		header += codeWord;
	}
	
	char dummy;
	freopen(originalFileName.c_str(), "r", stdin);
	while(scanf("%c", &dummy) != EOF) {
		inBuf += this->lookUpTable[dummy];
	}

	outStr = header + inBuf;
	
	for (int i = 0; i < (int)outStr.size(); i += 8) {
		outBuf += char(binaryToDecimal(outStr.substr(i, 8)));
	}

	fstream outFile;
	outFile.open("compressed.huf", ios::out | ios::binary);
	outFile.write(outBuf.c_str(), outBuf.size());
	fclose(stdin);
}

void Huffman::decompress(string compressedFile) {
	// TODO
	fstream inFile;
	inFile.open(compressedFile, ios::in | ios::binary);
	string inFileStream;
	char dummy;
	while (inFile.read(&dummy, 1)) {
		bitset<8> bt = dummy;
		inFileStream += bt.to_string();
	}
	for (int i = 32; i < 127; ++i) {
		string ignoreLenCode = inFileStream.substr(0, 8);
		int ignoreLen = binaryToDecimal(ignoreLenCode);
		inFileStream = inFileStream.substr(8);

		string codeWord = inFileStream.substr(0, 40);
		inFileStream = inFileStream.substr(40);
		codeWord = codeWord.substr(0, 40 - ignoreLen);

		this->lookUpTable[i] = codeWord;
	}
	
	freopen("decompressed1.txt", "w", stdout);
	string curWord;
	for (int i = 0; i < (int)inFileStream.size(); ++i) {
		curWord += inFileStream[i];
		bool found = false;
		for (auto it : this->lookUpTable) {
			if (it.second == curWord) {
				cout << it.first;
				found = true;
				break;
			}
		}
		if (found == true) {
			curWord = "";
		}
	}
	fclose(stdout);
}

void Huffman::printTable() {
	for(auto it : this->lookUpTable)
		cout << it.first << " " << it.second << endl;
}

int main(int argc, char* argv[]) {
	if (argc != 3 || (argc == 3 && string(argv[1]) != "-c" && string(argv[1]) != "-d")) {
		cerr << "[-] Usage:" << endl;
		cerr << "To COMPRESS:" << "\t" << argv[0] << " -c " << "<filename.txt>" << endl;
		cerr << "To DECOMPRESS:" << "\t" << argv[0] << " -d " << "<filename.huf>" << endl;
		return 1;
	} else {
		Huffman huffTree(argv[2], argv[1]);
		// huffTree.printTable();
	}
}