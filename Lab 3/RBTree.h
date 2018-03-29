/*RBTree.H
Kaylee Gabus
EECS 2510 Spring 2017
Purpose: Header file for RBTree.cpp*/


#pragma once
#include "BinarySearchTree.h"

#define RED 0
#define BLACK 1

using namespace std;

struct RBNode
{
	int colour;
	int count = 1;
	string key;
	RBNode *LeftChild = nullptr;
	RBNode *RightChild = nullptr;
	RBNode *ParentNode = nullptr;
};

class RBTree : public BinarySearchTree
{
public:
	RBNode *treeRoot;
	RBNode *nil;

	RBTree();
	void insert(string key);
	int getComparisons();
	int getPointerChanges();
	int getTotalWordCount();
	int getTreeHeight();
	int getNodeCount();
	int getRecolourings();


private:
	int nodesRecoloured;
	int nodeCount;
	int comparisonCount;
	int pChangeCount;
	int totalWordsCount;
	int treeHeight;
	int currentHeight;

	void insertKey(string key);
	void rebalance(RBNode *node);
	void findTreeHeight(RBNode *node, string path);
	void traverseAndSum(RBNode *node);
	void leftRotate(RBNode *node);
	void rightRotate(RBNode *node);
};