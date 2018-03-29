/*RBTree.cpp
Kaylee Gabus
EECS 2510 Spring 2017
Purpose: Inherits from BinarySearchTree.cpp to create a Red/Black Tree. Not fully implemented, only supports insert.*/

#include "stdafx.h"
#include "RBTree.h"


RBTree::RBTree()
{
	nodesRecoloured = 0;
	nodeCount = 0;
	comparisonCount = 0;
	pChangeCount = 0;
	totalWordsCount = 0;
	treeHeight = 0;

	nil = new RBNode;		//this is the single leaf node
	nil->colour = BLACK;
	nil->key = "";
	nil->LeftChild = nil;
	nil->RightChild = nil;
	nil->ParentNode = nil;

	treeRoot = nil;			//set the root to the existing nil node
}

void RBTree::insert(string key)
{
	insertKey(key);
}

int RBTree::getComparisons()
{
	return comparisonCount;
}

int RBTree::getTreeHeight()
{
	treeHeight = 0;
	currentHeight = 0;
	findTreeHeight(treeRoot, "");
	return treeHeight;
}

int RBTree::getNodeCount()
{
	return nodeCount;
}

int RBTree::getPointerChanges()
{
	return pChangeCount;
}

int RBTree::getTotalWordCount()
{	//totalWordsCount is set in traverseAndSum
	totalWordsCount = 0;
	traverseAndSum(treeRoot);
	return totalWordsCount;
}

int RBTree::getRecolourings()
{
	return nodesRecoloured;
}

void RBTree::insertKey(string key)
{	//inserts a string into the tree
	//if the string is already present in a node, the node's counter is incremented
	//otherwise a new node is created (red) and inserted according to the BST property
	//at the end of the insertion, rebalance is called to correct any of the Red/Black rules that were violated
	RBNode *x = treeRoot;         // x searches for insertion point
	RBNode *y = nil;          // y lags one step behind x (y is x’s parent)

	while (x != nil)
	{	//traverse the tree to find where to insert the node
		y = x;
		
		if (key == x->key)
		{	//if the key is already in the tree, increment it's counter and return
			x->count++;

			comparisonCount++;
			return;
		}

		if (key < x->key)
			x = x->LeftChild;
		else x = x->RightChild;

		comparisonCount += 2;
	}

	RBNode *newNode = new RBNode;			//the key was not found in the tree, make a new node and insert it
	newNode->key = key;
	newNode->ParentNode = y;
	nodeCount++;
	
	if (y == nil)	 	// If the Tree was empty, set as the root
		treeRoot = newNode;
	else
	{	//insert as a child of y
		if (newNode->key < y->key) 
			y->LeftChild = newNode; 
		else y->RightChild = newNode;

		comparisonCount++;
	}

	pChangeCount += 2;					//once for setting the parent pointer and once for adding on as a child
	
	newNode->LeftChild = nil;   		// Our new node is at the bottom …
	newNode->RightChild = nil;  		// .. of the tree, so its children are nil
	newNode->colour = RED;    			// Color this node red for now (may not be a safe decision)
	
	rebalance(newNode);					//      Fix anything we broke

}

void RBTree::rebalance(RBNode *node)
{	//called directly after insertion of a new node
	//corrects any of the Red/Black rules the insertion violated
	//works from the node inserted back up into the tree
	RBNode *y = new RBNode;

	while (node->ParentNode->colour == RED)
	{
		if (node->ParentNode == node->ParentNode->ParentNode->LeftChild)
		{	//the node was inserted as a left child
			y = node->ParentNode->ParentNode->RightChild;
			if (y->colour == RED)
			{	//case 1: a red node (node->parent) has a red child and a red sibling
				//recolour node->parent, it's sibling, and their parent and move up one level
				node->ParentNode->colour = BLACK;         	// Case 1 (re-colour only)
				y->colour = BLACK;		
				node->ParentNode->ParentNode->colour = RED; 		
				node = node->ParentNode->ParentNode;			

				nodesRecoloured += 3;
			}

			else
			{	
				if (node == node->ParentNode->RightChild)	//case 2 (always creates a case 3)
				{	//a red node has a red right child and a black sibling
					//perform a left rotate
					node = node->ParentNode;		
					leftRotate(node);		
				}
				
				//case 3: a red node has a red left child and a black sibling
				//perform a right rotation and recolour the node and it's parent
				node->ParentNode->colour = BLACK;			
				node->ParentNode->ParentNode->colour = RED;		
				rightRotate(node->ParentNode->ParentNode);	

				nodesRecoloured += 2;
			}
		}
			
		else  
		{	//the node was inserted as a right child
			y = node->ParentNode->ParentNode->LeftChild;
			if (y->colour == RED)
			{	//case 1: a red node (node->parent) has a red child and a red sibling
				//recolour node->parent, it's sibling, and their parent and move up one level
				node->ParentNode->colour = BLACK;         	// Case 1 (re-colour only)
				y->colour = BLACK;		// Case 1
				node->ParentNode->ParentNode->colour = RED; 		
				node = node->ParentNode->ParentNode;			

				nodesRecoloured += 3;
			}

			else
			{	
				if (node == node->ParentNode->LeftChild)	//case 2 (always creates a case 3)
				{// a red node has a red left child and a black sibling
				//perform a left rotate
					node = node->ParentNode;		
					rightRotate(node);		
				}

				//case 3: a red node has a red right child and a black sibling
				//perform a left rotation and recolour the node and it's parent
				node->ParentNode->colour = BLACK;			
				node->ParentNode->ParentNode->colour = RED;		
				leftRotate(node->ParentNode->ParentNode);	

				nodesRecoloured += 2;
			}
		}
	}
		
	treeRoot->colour = BLACK;
	nodesRecoloured++;
}

void RBTree::findTreeHeight(RBNode * node, string path)
{	//traverses the tree to find the paths to each leaf node
	//saves the longest path's length to treeHeight
	//clear treeHeight before calling
	string nodePath = path;
	if (node->LeftChild != nil)
	{	//if following a left path, concat a 0
		findTreeHeight(node->LeftChild, nodePath + "0");
	}

	if (node->LeftChild == nil && node->RightChild == nil)
	{	//if the node is a leaf, check to see if the path to it is longer than the previously saved longest path length
		if (nodePath.length() > treeHeight)
			treeHeight = nodePath.length();
	}

	if (node->RightChild != nil)
	{	//if following a right path, concat a 1
		findTreeHeight(node->RightChild, nodePath + "1");
	}
}

void RBTree::traverseAndSum(RBNode * node)
{	//performs an in order traversal of the tree to sum all the nodes' counts
	//set totalWordsCount to zero before calling

	if (node->LeftChild != nil)
		traverseAndSum(node->LeftChild);

	totalWordsCount += node->count;

	if (node->RightChild != nil)
		traverseAndSum(node->RightChild);
}

void RBTree::leftRotate(RBNode * node)
{	//adjusts the pointers of the tree to create a more balanced tree
	//preserves BST property
	//sets the left subtree of the passed in node's right child to the passed in node's right child
	//sets the passed in node to the left child of it's right child
	//adjusts parent pointers for both nodes
	if (treeRoot->ParentNode == nil && node->RightChild != nil)
	{
		RBNode *y = node->RightChild;              // y is the passed in node’s right child
		node->RightChild = y->LeftChild; 		//Turn y’s left subtree into the passed in node's right subtree
		pChangeCount++;
		
		if (y->LeftChild != nil)
		{
			y->LeftChild->ParentNode = node;
			pChangeCount++;
		}
			
		y->ParentNode = node->ParentNode;               	// Link the passed in node’s parent to y
		pChangeCount++;
		
		if (node->ParentNode == nil)		//the passed in node was the root, so set y as the root
		{
			treeRoot = y;
			pChangeCount++;
		}
		else
		{
			if (node == node->ParentNode->LeftChild)	// Otherwise swap the nodes
				node->ParentNode->LeftChild = y;	
			else node->ParentNode->RightChild = y;

			pChangeCount++;
		}
		
		y->LeftChild = node;			// set y as the passed in node's parent
		node->ParentNode = y;	

		pChangeCount += 2;
	}

	
}

void RBTree::rightRotate(RBNode * node)
{	//adjusts the pointers of the tree to create a more balanced tree
	//preserves BST property
	//sets the right subtree of the passed in node's left child to the passed in node's left child
	//sets the passed in node to the right child of it's left child
	//adjusts parent pointers for both nodes
	if (treeRoot->ParentNode == nil && node->LeftChild != nil)
	{
		RBNode *y = node->LeftChild;              // y is the passed in node’s left child
		node->LeftChild = y->RightChild; 		//Turn y’s Right subtree into the passed in node’s Left subtree
		pChangeCount++;

		if (y->RightChild != nil)
		{
			y->RightChild->ParentNode = node;
			pChangeCount++;
		}
			
		y->ParentNode = node->ParentNode;               	// Link the passed in node’s parent to y
		pChangeCount++;

		if (node->ParentNode == nil)		// the passed in node was the root, so y becomes the new root
			treeRoot = y;			
		else
		{
			if (node == node->ParentNode->RightChild)	// Otherwise swap the two nodes
				node->ParentNode->RightChild = y;	
			else node->ParentNode->LeftChild = y;	
		}
		pChangeCount++;

		y->RightChild = node;			// set y as the passed in node's parent
		node->ParentNode = y;	
		pChangeCount += 2;
	}
}
