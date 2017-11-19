#include <stdio.h>
#include <stdlib.h>

#ifndef PROJECT2_H
#define PROJECT2_H
#define RED 		0
#define BLACK		1
typedef int ElementType ;

typedef struct TNode{
	ElementType key;
	struct TNode* l;
	struct TNode* r;
	struct TNode* father;
	int color;
}* pnode;

typedef pnode Tree;

Tree readTree(int* iter);
int readNode(Tree tree);
Tree readTree_with_file(FILE* fp,FILE* fout,int* iter);
int readNode_with_file(Tree tree,FILE* fp);
int checkRedNode(Tree tree);
int BlackNodeSize(Tree tree);
int checkBlackNodeRucursive(Tree tree, int expectedSize);
int checkBlackNode(Tree tree);
int isRBT(Tree tree);
int freeTree(Tree tree);

#endif