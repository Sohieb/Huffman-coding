#include <vector>
#include <string>
#include <map>

using namespace std;

class Node{
private:
	char data;
	int frequency;
	Node * left;
	Node * right;
public:
	Node(){}
	Node(int, int);
	inline char getData(){ return this->data; }
	inline int getFrequency() {return this->frequency; }
	inline Node * getLeft(){ return this->left; }
	inline Node * getRight(){ return this->right; }
	void merge(Node *, Node *);
};

class Huffman{
private:
	Node * root;
	map<char, string> lookUpTable;
public:
	Huffman(string);
	Node * constructTree(vector<Node *>);
	void constructTable(Node * , string);
	void compress(string);
	void decompress(string);
	void printTable(); // just for debug purposes
};