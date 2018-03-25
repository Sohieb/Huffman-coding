#include "huffman.h"
#include <iostream>
#include <stdio.h>
#include <string>
#include <vector>
#include <queue>

using namespace std;

Node:: Node(int _data, int _frq){
	this->data = _data;
	this->frequency = _frq;
	this->left = NULL;
	this->right = NULL;
}

void Node:: merge(Node * leftNode, Node * rightNode){
	this->left = leftNode;
	this->right = rightNode;
	this->frequency = leftNode->frequency + rightNode->frequency;
}

Huffman:: Huffman(string fileName){
	if(fileName.find(".txt") != std::string::npos){
		// COMPRESS

		vector<int> frq(256, 0); // [32 - 255] all printable ASCII
		char dummy;
		freopen(fileName.c_str(), "r", stdin);
		while(scanf("%c", &dummy) != EOF)
			++frq[dummy];
		fclose(stdin);
		vector<Node *> initNodes;
		for(int i = 32; i < 256; ++i){
			if(frq[i] == 0) continue;
			//cout << (char(i)) << " " << frq[i] << endl;
			Node * newNode = new Node(char(i), frq[i]);
			initNodes.push_back(newNode);
		}
		this->root = constructTree(initNodes);
		constructTable(this->root, "");
		compress(fileName);
	} else if(fileName.find(".huf") != std::string::npos){
		// DECOMPRESS
	} else {
		// Not a valid file name
	}
}

bool cmp(Node* &LHS, Node* &RHS){
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

void Huffman:: constructTable(Node * curNode, string curCode){
	if(curNode->getLeft() == NULL && curNode->getRight() == NULL){
		this->lookUpTable[curNode->getData()] = curCode;
		return;
	}
	constructTable(curNode->getLeft(), curCode + '0');
	constructTable(curNode->getRight(), curCode + '1');
	return;
}

void Huffman:: compress(stirng originalFile){
	// TODO
}

void Huffman:: decompress(stirng originalFile){
	// TODO
}

void Huffman:: printTable(){
	for(auto it : this->lookUpTable)
		cout << it.first << " " << it.second << endl;
}

int main(){
	Huffman huffTree("testdata.txt");
	huffTree.printTable();
}