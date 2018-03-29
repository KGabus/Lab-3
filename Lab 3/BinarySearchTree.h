/* Author:	Kaylee Gabus
   Date:	3-30-17
   Class:	EECS 2510 Non-Linear Spring 2017
   Purpose: Header file for BinarySearchTree.cpp
 */


#pragma once
#include <string>;

using namespace std;

struct BSTNode
{
	int count = 0;
	string key;
	BSTNode *LeftChild = nullptr;
	BSTNode *RightChild = nullptr;
	BSTNode *ParentNode = nullptr;
};

class BinarySearchTree
{
public:
	BSTNode *treeRoot;
	
	BinarySearchTree();
	int getComparisons();
	int getPointerChanges();
	int getTotalWordCount();
	int getTreeHeight();
	int getNodeCount();
	int insert(string key);
	int remove(string key);
	int searchKey(string key);
	int getTotalNodeCount();
	void listKeys();
	string next(string key);
	string previous(string key);
	string getMinumum();
	string getMaximum();

private:
	int nodeCount;
	int comparisonCount;
	int pChangeCount;
	int totalWordsCount;
	int treeHeight;
	int currentHeight;

	int insertKey(string key);
	int deleteKey(string key);
	void findTreeHeight(BSTNode *node, string path);
	void traverseAndSum(BSTNode *node);
	void traverseAndPrint(BSTNode *node);
	void transplantSubtree(BSTNode *nodeA, BSTNode *nodeB);
	BSTNode *treeMin(BSTNode *node);
	BSTNode *treeMax(BSTNode *node);
	BSTNode *findNodePointer(string key);
	BSTNode *nodePointerSucessor(BSTNode *currentNodePointer);
	BSTNode *nodePointerPredecessor(BSTNode *currentNodePointer);
};