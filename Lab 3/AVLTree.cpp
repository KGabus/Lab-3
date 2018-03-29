/*AVL.cpp
Kaylee Gabus
EECS 2510 Spring 2017
Purpose: Creates an AVL Tree. Not fully implemented, only supports insert.*/

#pragma once
#include "stdafx.h"
#include "AVLTree.h"
#include <iostream>

AVLTree::AVLTree()
{
	bFactorsAdjusted = 0;
	nodeCount = 0;
	comparisonCount = 0;
	pChangeCount = 0;
	totalWordsCount = 0;
	treeHeight = 0;
	treeRoot = nullptr;
}
int AVLTree::getBFsAdjustedCount()
{
	return bFactorsAdjusted;
}
void AVLTree::insert(string key)
{
	insertKey(key);
}
int AVLTree::getComparisons()
{
	return comparisonCount;
}
int AVLTree::getPointerChanges()
{
	return pChangeCount;
}
int AVLTree::getTotalWordCount()
{	//totalWordsCount is set in traverseAndSum
	totalWordsCount = 0;
	traverseAndSum(treeRoot);
	return totalWordsCount;
}
int AVLTree::getTreeHeight()
{
	treeHeight = 0;
	currentHeight = 0;
	findTreeHeight(treeRoot, "");
	return treeHeight;
}

int AVLTree::getNodeCount()
{
	return nodeCount;
}

void AVLTree::traverseAndSum(AVLNode *node)
{	//performs an in order traversal of the tree to sum all the nodes' counts
	//set totalWordsCount to zero before calling

	if (node == nullptr)
		return;

	if (node->LeftChild != nullptr)
		traverseAndSum(node->LeftChild);

	totalWordsCount += node->count;

	if (node->RightChild != nullptr)
		traverseAndSum(node->RightChild);
}


void AVLTree::insertKey(string key)
{	//inserts a value into the tree
	//if value isn't already present, create new node and adjust balance factors as needed 
	//if it is, increment it's count
	
	AVLNode *nodeToInsert;
	AVLNode *lastBalancedPointer, *B, *lastBalancedParent;    
	AVLNode *currentNodePointer, *currentNodeParent;        
	AVLNode *C, *CL, *CR;   
		int  d;             // displacement; Used to adjust BFs

	if (treeRoot == NULL)   
	{	//if the tree is empty, create a root node and return
		nodeToInsert = new AVLNode;    // make and fill a node
		nodeToInsert->key = key;
		nodeToInsert->LeftChild = nodeToInsert->RightChild = NULL;  // leaf --> no children
		nodeToInsert->BF = 0;     // it is, by definition, balanced
		treeRoot = nodeToInsert;        // root was NULL, so set this as the root

		pChangeCount++;
		nodeCount++;
		return;          // This was the trivial case
	}

	// Locate insertion point for the passed in key.
	// currentNodePointer scans through the tree until it falls off bottom
	// currentNodeParent is currentNodePointer’s parent
	// nodeToInsert will be added as either the Left or Right child of lastBalancedParent
	// lastBalancedPointer is the last parent above nodeToInsert with a BF of ±1 (before the insert)
	// lastBalancedParent is lastBalancedPointer’s parent
		
	lastBalancedParent = currentNodeParent = NULL; // the parent pointers lag, so they start NULL
	lastBalancedPointer = currentNodePointer = treeRoot;

	while (currentNodePointer != NULL) 
	{// search tree for insertion point
		if (key == currentNodePointer->key)
		{	//if the node if found, increment it's count
			currentNodePointer->count++;
			comparisonCount++;
			return;
		}
		
		if (currentNodePointer->BF != 0)
		{	//finds the last node to have a non-zero balance factor
			lastBalancedPointer = currentNodePointer; 
			lastBalancedParent = currentNodeParent;
		}  
		
		if (key < currentNodePointer->key) 
		{
			currentNodeParent = currentNodePointer; 
			currentNodePointer = currentNodePointer->LeftChild;
			comparisonCount += 2;
		} 
		else 
		{
			currentNodeParent = currentNodePointer; 
			currentNodePointer = currentNodePointer->RightChild; 
			comparisonCount += 2;
		} 
	}

	//currentNodePointer is now NULL, but currentNodeParent points at the last node where the key should be inserted as a leaf

	nodeToInsert = new AVLNode;   // Make a new node
	nodeToInsert->key = key;
	nodeToInsert->LeftChild = NULL; 
	nodeToInsert->RightChild = NULL; 
	nodeToInsert->BF = 0;    // Leaves are always balanced by definition

	nodeCount++;
			  
	if (key < currentNodeParent->key)	//determine whether to insert as a left or right child and insert
		currentNodeParent->LeftChild = nodeToInsert;
	else currentNodeParent->RightChild = nodeToInsert;

	comparisonCount++;
	pChangeCount++;

	// lastBalancedPointer was the LAST ancestor with a BF of ± 1, (or is still the root,
	// because we never FOUND a BF of ± 1), so ALL nodes BETWEEN A and Q 
	// must have a BF of 0, and will, therefore, BECOME ± 1.
	//
	// If X is inserted in the LEFT subtree of lastBalancedPointer, then d = +1
	if (key > lastBalancedPointer->key)
	{ // Which way is the displacement (d)
		B = currentNodePointer = lastBalancedPointer->RightChild; 
		d = -1; 
	} 
	else 
	{ 
		B = currentNodePointer = lastBalancedPointer->LeftChild;
		d = +1;
	} // B is identified as the last balanced node’s child
	comparisonCount++;

	while (currentNodePointer != nodeToInsert)  // currentNodePointer is now one node below lastBalancedPointer. 
	{               //  Adjust from here to the insertion point.  Don’t do anything to new node 
		if (key > currentNodePointer->key) 
		{ // adjust BF and move forward
			currentNodePointer->BF = -1; 
			currentNodePointer = currentNodePointer->RightChild; 
		} 
		else 
		{
			currentNodePointer->BF = +1;
			currentNodePointer = currentNodePointer->LeftChild;
		}

		comparisonCount++;
		bFactorsAdjusted++;
	}

	// Now we check the BF at A and see if we just pushed the tree INTO 
	// BALANCE, into an “unacceptable IMBALANCE”, or if it is still
	// “BALANCED ENOUGH”.
	if (lastBalancedPointer->BF == 0) // Tree WAS completely balanced before the insert.
	{             // The insert pushed it to slight (acceptable) imbalance
		lastBalancedPointer->BF = d;    // Set the BF to +/- 1 (displacement tells direction)
		bFactorsAdjusted++;
		return;     // This is close enough to live with, so exit now
	}

	if (lastBalancedPointer->BF == -d) // If the tree had a slight imbalance the OTHER way, 
	{               // then did the insertion throw the tree INTO complete
		lastBalancedPointer->BF = 0;   // balance? If so, set the BF to zero and return
		bFactorsAdjusted++;
		return;      //
	}
	//if neither return was reached, the tree is imbalanced enough to need correcting

	if (d == +1) // this is a left imbalance.
	{
		if (B->BF == +1) // LL rotation
		{	//lastBalancedPointer's left child become's its grandchild's right child
			//its grandchild moves up to become it's left child
			lastBalancedPointer->LeftChild = B->RightChild;	// Change the child pointers to reflect the rotation
			B->RightChild = lastBalancedPointer;
			B->BF = lastBalancedPointer->BF = 0;			// Adjust the BFs
			
			pChangeCount += 2;
			bFactorsAdjusted += 2;

		}
		else  // LR Rotation: three cases (structurally the same; BFs vary)
		{
			C = B->RightChild; // C is B's right child
			CL = C->LeftChild; // CL and CR are C's left and right children
			CR = C->RightChild; 


			lastBalancedPointer->LeftChild = CR;
			B->RightChild = CL;
			C->LeftChild = B;
			C->RightChild = lastBalancedPointer;
			
			pChangeCount += 4;
			

			switch (C->BF)
			{	// Set the new BFs based on the BF at C.
				case 0: lastBalancedPointer->BF = B->BF = 0; break;
				case 1: lastBalancedPointer->BF = -1; B->BF = 0; break;
				case -1: B->BF = 1; lastBalancedPointer->BF = 0; break;
			}

			C->BF = 0; // Regardless, C is now balanced
			bFactorsAdjusted += 3;
			B = C;     // B is the root of the now-rebalanced subtree (recycle)
		} // end of else (LR Rotation)
	} // end of “if (d = +1)”
	else // d=-1.  This is a right imbalance
	{
		if (B->BF == -1) // RR rotation
		{
			lastBalancedPointer->RightChild = B->LeftChild;
			B->LeftChild = lastBalancedPointer;
			B->BF = lastBalancedPointer->BF = 0;			// Adjust the BFs at A & B

			pChangeCount += 2;
			bFactorsAdjusted += 2;
		}
		else		// RL rotation
		{	//lastBalancedPointer's right child become's its grandchild's left child
			//its grandchild moves up to become it's right child
			C = B->LeftChild; // C is B's right child
			CL = C->LeftChild; // CL and CR are C's left and right children
			CR = C->RightChild;

			lastBalancedPointer->RightChild = CL;
			B->LeftChild = CR;
			C->RightChild = B;
			C->LeftChild = lastBalancedPointer;
			
			pChangeCount += 4;

			switch (C->BF)
			{	// Set the new BFs based on the BF at C.
			case 0: lastBalancedPointer->BF = B->BF = 0; break;
			case -1: lastBalancedPointer->BF = 1; B->BF = 0; break;
			case 1: B->BF = -1; lastBalancedPointer->BF = 0; break;
			}

			C->BF = 0; // Regardless, C is now balanced
			bFactorsAdjusted += 3;
			B = C;     // B is the root of the now-rebalanced subtree (recycle)
		}
	}

	// Finish up:
	//
	// Regardless, the subtree rooted at B has been rebalanced, and needs to
	// be the new child of lastBalancedParent.

	if (lastBalancedParent == NULL) { treeRoot = B; pChangeCount++; return; } // B is the tree’s new root - done
										
	//connect the balanced subtree to the rest of the tree
	if (lastBalancedPointer == lastBalancedParent->LeftChild) { lastBalancedParent->LeftChild = B; pChangeCount++; return; }
	if (lastBalancedPointer == lastBalancedParent->RightChild) { lastBalancedParent->RightChild = B; pChangeCount++; return; }

 // End of AVL Insert  
}

void AVLTree::findTreeHeight(AVLNode * node, string path)
{	//traverses the tree to find the paths to each leaf node
	//saves the longest path's length to treeHeight
	//clear treeHeight before calling
	string nodePath = path;
	if (node->LeftChild != nullptr)
	{	//if following a left path, concat a 0
		findTreeHeight(node->LeftChild, nodePath + "0");
	}

	if (node->LeftChild == nullptr && node->RightChild == nullptr)
	{	//if the node is a leaf, check to see if the path to it is longer than the previously saved longest path length
		if (nodePath.length() > treeHeight)
			treeHeight = nodePath.length();
	}

	if (node->RightChild != nullptr)
	{	//if following a right path, concat a 1
		findTreeHeight(node->RightChild, nodePath + "1");
	}
}

