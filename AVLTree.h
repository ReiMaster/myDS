/****************************************************************
>> File Name: AVLTree.h
>> Author: yy
>> Mail: RNHisnothuman@gmail.com
****************************************************************/

/****************************************************************
>> Function: an AVL tree
>> Directions: support basic operations.
>> Version: v0
****************************************************************/

#include <iostream>
#include <queue>
#include <stdio.h>
using namespace std;



template<class T>
class Tree;

template<class T>
class TreeNode{
	public:
		T data;
		int height;//the height of node
		TreeNode<T>* leftPtr;
		TreeNode<T>* rightPtr;

		TreeNode(T value){
			data = value;
			leftPtr = rightPtr = 0;
			height = 0;
		}

        T getData(){
			return data;
		}

};


template<class T>
class Tree
{
private:
	TreeNode<T>* rootTreeNode;

public:
	Tree()
	{
		rootTreeNode = 0;
	}

	~Tree()
	{
		destroyHelper(rootTreeNode);
	}

	TreeNode<T>* treeSearch(T value);//search value, if the value is exsit, return the ptr. if not, return null.
    
    void outputTree();//print the tree out

    void insertTreeNode(T);//insert a value

	void deleteTreeNode(T);//delete a value

	void inorderTraversal();//inorder traversal the tree

private:
	//rotate the tree node
	void rotateLeft(TreeNode<T>* &);
	void rotateRight(TreeNode<T>* &);
	void doubleRotateLeft(TreeNode<T>* &);
	void doubleRotateRight(TreeNode<T>* &);

	//support function of delete
	void deleteHelper1(TreeNode<T>* &, T);
	T deleteHelper2(TreeNode<T>* &, T);

	//culculate the height
    int high(TreeNode<T> *);

	//return the height of the higher node
	int max(int s1, int s2)
	{
		return s1>s2?s1:s2;
	}

	//some recursion tools
	void outputTreeHelper(TreeNode<T>*, int);
	void destroyHelper(TreeNode<T>*);
	TreeNode<T>* searchHelper(TreeNode<T>*, T);
	void insertHelper(TreeNode<T>* &, T);
    void inHelper(TreeNode<T>*);

};



template<class T>
void Tree<T>::rotateLeft(TreeNode<T>* &nodePtr)
{
	TreeNode<T>* temp = nodePtr -> leftPtr;
	nodePtr -> leftPtr = temp -> rightPtr;
	nodePtr -> height = max(high(nodePtr -> leftPtr),high(nodePtr -> rightPtr)) + 1;
	temp -> rightPtr = nodePtr;
	nodePtr = temp;
	nodePtr -> height = max(high(nodePtr -> leftPtr),high(nodePtr -> rightPtr)) + 1;
}

template<class T>
void Tree<T>::rotateRight(TreeNode<T>* &nodePtr)
{
	TreeNode<T>* temp = nodePtr -> rightPtr;
	nodePtr -> rightPtr = temp -> leftPtr;
	nodePtr -> height = max(high(nodePtr -> leftPtr),high(nodePtr -> rightPtr)) + 1;
	temp -> leftPtr = nodePtr;
	nodePtr = temp;
	nodePtr -> height = max(high(nodePtr -> leftPtr),high(nodePtr -> rightPtr)) + 1;
}

template<class T>
void Tree<T>::doubleRotateLeft(TreeNode<T>* &nodePtr)
{
	rotateRight(nodePtr -> leftPtr);
	rotateLeft(nodePtr);
}

template<class T>
void Tree<T>::doubleRotateRight(TreeNode<T>* &nodePtr)
{
	rotateLeft(nodePtr -> rightPtr);
	rotateRight(nodePtr);
}

template<class T>
int Tree<T>::high(TreeNode<T>* nodePtr)
{
	return nodePtr == 0 ? -1 : nodePtr->height;
}

template<class T>
TreeNode<T>* Tree<T>::treeSearch(T value)
{
	return searchHelper(rootTreeNode, value);
}

template<class T>
TreeNode<T>* Tree<T>::searchHelper(TreeNode<T>* nodePtr, T value)
{
	TreeNode<T>* retPtr = 0;
	if(0 == nodePtr)
		return retPtr;
	if(value < nodePtr -> data)
		return searchHelper(nodePtr -> left, value);
	if(value > nodePtr -> data)
		return searchHelper(nodePtr -> right, value);
	return nodePtr;//the least possibility is last
}

template<class T>
void Tree<T>::insertTreeNode(T value)
{
	insertHelper(rootTreeNode, value);
}

template<class T>
void Tree<T>::insertHelper(TreeNode<T>* &nodePtr, T value)
{
	if(0 == nodePtr)
		nodePtr = new TreeNode<T>(value);
	else
	{
		if(value < nodePtr -> data)
		{
			insertHelper(nodePtr -> leftPtr, value);
			if(high(nodePtr -> leftPtr)-high(nodePtr -> rightPtr) == 2)
			{
				if(value < nodePtr -> leftPtr -> data)
					rotateLeft(nodePtr);
				else
					doubleRotateLeft(nodePtr);
			}
		}
		else if(value > nodePtr -> data)
		{
			insertHelper(nodePtr -> rightPtr, value);
			if(high(nodePtr -> rightPtr)-high(nodePtr -> leftPtr) == 2)
			{
				if(value < nodePtr -> rightPtr -> data)
					doubleRotateRight(nodePtr);
				else
					rotateRight(nodePtr);
			}
		}
		else ;	
	}

	nodePtr -> height = max(high(nodePtr -> leftPtr),high(nodePtr -> rightPtr)) + 1;
}

template<class T>
void Tree<T>::deleteTreeNode(T value)
{
	if(0 != rootTreeNode)
		deleteHelper1(rootTreeNode, value);
}

template<class T>
void Tree<T>::deleteHelper1(TreeNode<T>* &nodePtr, T value)
{
	if(0 == nodePtr)
	{
		cout<<endl<<"there is no the value in the tree!!";
		return ;
	}
	if(nodePtr -> data < value)
	{
		deleteHelper1(nodePtr -> rightPtr, value);
		if(high(nodePtr -> leftPtr)-high(nodePtr -> rightPtr) == 2)
		//delete a node in right tree and cause unbalance
		{
			if(high(nodePtr -> leftPtr -> rightPtr) - high(nodePtr -> leftPtr -> leftPtr))
				doubleRotateLeft(nodePtr);
			else
				rotateLeft(nodePtr);//equal or bigger
		}
	}
	else if(nodePtr -> data > value)
	{	
		deleteHelper1(nodePtr -> leftPtr, value);
		if(high(nodePtr -> rightPtr)-high(nodePtr -> leftPtr) == 2)
		{
			if(high(nodePtr -> rightPtr -> leftPtr) - high(nodePtr -> rightPtr -> rightPtr))
				doubleRotateRight(nodePtr);
			else
				rotateRight(nodePtr);
		}
			
	}
	else
	{
		if(0 == nodePtr -> rightPtr)
		{
			TreeNode<T>* temp = nodePtr;
			nodePtr = nodePtr -> leftPtr;
			delete temp;
		}
		else if(0 == nodePtr -> leftPtr)
		{
			TreeNode<T>* temp = nodePtr;
			nodePtr = nodePtr -> rightPtr;
			delete temp;
		}
		else
		{
			nodePtr -> data = deleteHelper2(nodePtr -> rightPtr, value);
			if(high(nodePtr -> leftPtr) - high(nodePtr -> rightPtr) == 2)
			{
				if(high(nodePtr -> leftPtr -> rightPtr) > high(nodePtr -> leftPtr -> leftPtr))
					doubleRotateLeft(nodePtr);
				else
					rotateLeft(nodePtr);
			}
		}
	}

	if(0 != nodePtr)
		nodePtr -> height = max(high(nodePtr -> leftPtr), high(nodePtr -> rightPtr)) + 1;
}

template<class T>
T Tree<T>::deleteHelper2(TreeNode<T>* &nodePtr, T value)
{
	T retVal;
	if(0 != nodePtr -> leftPtr)
	{
		retVal = deleteHelper2(nodePtr -> leftPtr, value);
		if(high(nodePtr ->rightPtr) - high(nodePtr -> leftPtr) == 2)
		{
			if(high(nodePtr -> rightPtr -> leftPtr) > high(nodePtr -> rightPtr -> rightPtr))
				doubleRotateRight(nodePtr);
			else
				rotateRight(nodePtr);
		}
		nodePtr -> height = max(high(nodePtr -> leftPtr),high(nodePtr -> rightPtr)) + 1;
	}
	else
	{
		retVal = nodePtr -> data;
		TreeNode<T>* temp = nodePtr;
		nodePtr = nodePtr -> rightPtr == 0 ? 0 : nodePtr -> rightPtr;
		delete temp;
	}
	return retVal;
}


template<class T>
void Tree<T>::inorderTraversal()
{
	inHelper(rootTreeNode);
}

template<class T>
void Tree<T>::inHelper(TreeNode<T>* nodePtr)
{
	if(0 == nodePtr)
		return;
	inHelper(nodePtr -> leftPtr);
	cout<<"  "<<nodePtr -> data;
	inHelper(nodePtr -> rightPtr);
}

template<class T>
void Tree<T>::destroyHelper(TreeNode<T>* nodePtr)
{
	if(0 == nodePtr)
		return;
	destroyHelper(nodePtr -> leftPtr);
	destroyHelper(nodePtr -> rightPtr);
	delete nodePtr;
}

template<class T>
void Tree<T>::outputTree()
{
	cout<<endl;
	if(0 != rootTreeNode)
	{
		outputTreeHelper(rootTreeNode,0);
		cout<<endl;
	}
	else
		cout<<"the tree is empty!!";
}


template<class T>
void Tree<T>::outputTreeHelper(TreeNode<T>* nodePtr, int space)
{
	if(0 != nodePtr)
	{
		outputTreeHelper(nodePtr -> rightPtr, space+5);
		for(int i = 0; i < space; i++)
			cout<<" ";
		cout<<nodePtr -> getData() <<endl;
		outputTreeHelper(nodePtr -> leftPtr, space+5);
	}
}

