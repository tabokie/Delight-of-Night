#include <stdio.h>
#include <stdlib.h>

#ifndef RED_BLACKBST_H
#define RED_BLACKBST_H

#define RED		1
#define BLACK	0

typedef struct BSTnode{
	int color;
	int key;
	int count;
	struct BSTnode* parent;
	struct BSTnode* left;
	struct BSTnode* right;
}* pnode;

typedef struct BSTtree{
	int count;
	pnode NIL;
}* ptree;


ptree initTree();

void freeNode(pnode nd);
void freeTree(ptree);

int Search(ptree, int);

int leftRotate(pnode);

int rightRotate(pnode);

int flipColor(pnode);

int Insert(ptree, int);

void preorderVisitNode(pnode tree, FILE* fp);
void preorderVisit(ptree tree, FILE* fp);

#endif