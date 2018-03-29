#pragma once
#include "BinarySearchTree.h"

using namespace std;

struct AVLNode
{
	int BF = 0;
	int count = 1;
	string key;
	AVLNode *LeftChild = nullptr;
	AVLNode *RightChild = nullptr;
	AVLNode *ParentNode = nullptr;
};

class AVLTree : public BinarySearchTree
{
public:
	AVLNode *treeRoot;

	AVLTree();
	int getBFsAdjustedCount();
	void insert(string key);
	int getComparisons();
	int getPointerChanges();
	int getTotalWordCount();
	int getTreeHeight();
	int getNodeCount();


private:
	int bFactorsAdjusted;
	int nodeCount;
	int comparisonCount;
	int pChangeCount;
	int totalWordsCount;
	int treeHeight;
	int currentHeight;

	void traverseAndSum(AVLNode *node);
	void insertKey(string key);
	void findTreeHeight(AVLNode *node, string path);
};
