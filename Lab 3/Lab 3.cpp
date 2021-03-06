/*Lab 3.cpp
Kaylee Gabus
EECS 2510 Spring 2017
Purpose: Defines the entry point for the console application. 
			Program loops through file specified three times to create a binary search, an AVL, and a red/black tree.
			Outputs time taken and select measurements to estimate work done for each tree.*/
#include "stdafx.h"
#include <iostream>
#include <string>
#include <fstream>
#include <ctime>
#include "BinarySearchTree.h"
#include "AVLTree.h"
#include "RBTree.h"

string filePath = "C:\\Users\\Kaylee\\Desktop\\Text Files\\shakespeare.txt";		//<-------------------------SET FILE PATH HERE

double elapsedTime;
double overheadTime;
double RBTTime;
double AVLTime;
double BSTTime;
clock_t startTime;


int main()
{
	char c; 
	RBTree RBT;
	AVLTree AVL;
	BinarySearchTree BST;

	char chari[100]; // assumes no word is longer than 99 characters (there's a word in Middlemarch that is longer than 49 chars)
	int iPtr; 
	bool IsDelimiter = false, WasDelimiter = false; 
	
	for (int i = 0; i<sizeof(chari); i++) chari[i] = '\0';

	ifstream inFile(filePath, ios::binary); 
	if (inFile.fail())
	{	//if the file is unable to be read, notify user and close the program
		cout << "Unable to open input file\n\n"
			<< "Program Exiting\n\nPress ENTER to exit\n";     
		cin.get(c);     
		exit(1);
	}  
	
	cout << "Collecting overhead time: ";

	for (int i = 0; i < 4; i++)
	{	//for each of the three trees and a run to get overhead time

		inFile.clear();				//reset the stream to the begininging of the file
		inFile.seekg(0, ios::beg);

		iPtr = 0;

		startTime = clock();						//Start timer
		
		inFile.get(c);

		while (!inFile.eof())
		{
			IsDelimiter = (c == 32 || c == 10 || c == 13 || c == 9 || c == '.' || c == ',' || c == '!' || c == ';' || c == ':' || c == '(' || c == ')');
			if (IsDelimiter && !WasDelimiter)   // if THIS character is a delimiter and the last character WASN�T 
			{
				WasDelimiter = true;
				if (i == 0);		//do nothing, used to collect overhead time info
				if (i == 1) BST.insert(chari);  	// insert it in the BST 
				if (i == 2) AVL.insert(chari);  	// insert it in the AVL Tree 
				if (i == 3) RBT.insert(chari);  	// insert this word in the RBT 

				for (int i = 0; i < sizeof(chari); i++) chari[i] = '\0'; // zero the word         
				iPtr = 0;
			}
			else if (!IsDelimiter)  // if this isn�t a delimiter, keep going 
			{
				chari[iPtr] = c;
				iPtr++;
			}
			else if (IsDelimiter && WasDelimiter)
			{
				// Do nothing -- two consecutive delimiters. 
			}
			WasDelimiter = IsDelimiter;   // for the NEXT iteration     
			inFile.get(c);
		}

		elapsedTime = (double)(clock() - startTime) / CLOCKS_PER_SEC;
		switch (i)
		{	//save and output the times for each run
		case 0: overheadTime = elapsedTime; cout << overheadTime << "\nStarting BST." << endl;  break;
		case 1: BSTTime = elapsedTime - overheadTime; cout << "Starting AVL." << endl; break;
		case 2: AVLTime = elapsedTime - overheadTime; cout << "Starting RBT." << endl; break;
		case 3: RBTTime = elapsedTime - overheadTime; cout << endl; break;
		}

	}
	inFile.close();
	
	cout << "     Time:     Nodes:     Words:     Height:     Comparisons:     Pointer Changes:     Other:" << endl;
	printf("BST: %2.3f%*d%*d%*d%*d%*d\n", BSTTime, 10, BST.getNodeCount(), 12, BST.getTotalWordCount(), 10, BST.getTreeHeight(), 15, BST.getComparisons(), 15, BST.getPointerChanges());
	printf("AVL: %2.3f%*d%*d%*d%*d%*d%*d\n", AVLTime, 10, AVL.getNodeCount(), 12, AVL.getTotalWordCount(), 10, AVL.getTreeHeight(), 15, AVL.getComparisons(), 15, AVL.getPointerChanges(), 20, AVL.getBFsAdjustedCount());
	printf("RBT: %2.3f%*d%*d%*d%*d%*d%*d\n", RBTTime, 10, RBT.getNodeCount(), 12, RBT.getTotalWordCount(), 10, RBT.getTreeHeight(), 15, RBT.getComparisons(), 15, RBT.getPointerChanges(), 20, RBT.getRecolourings());
	
    return 0;
}

