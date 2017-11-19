#include <stdio.h>
#include <stdlib.h>

#include "red-blackBST.h"


ptree initTree(){
	
	pnode newNode = (pnode)malloc(sizeof(struct BSTnode));
	newNode->key = -1;
	newNode->parent = newNode;
	newNode->color = BLACK;
	newNode->count = 0;
	newNode->left = newNode;
	newNode->right = newNode;

	ptree newTree = (ptree)malloc(sizeof(struct BSTtree));
	newTree->count = 0;
	newTree->NIL = newNode;

	return newTree;
}


int leftRotate(pnode root){
	int ret = 1;
	
	if (root->right->color == BLACK){
		ret = 0;
		return ret;
	}

	pnode left = root;
	pnode right = root->right;
	pnode parent = root->parent;
	if (parent->left->key == root->key)parent->left = right;
	else if (parent->right->key == root->key)parent->right = right;
	else ret = 0;

	right->parent = parent;
	left->right = right->left;
	right->left->parent=left;
	right->left = left;
	left->parent = right;
	right->color = left->color;
	left->color = RED;


	return ret;
}

int rightRotate(pnode root){
	int ret = 1;

	if (root->left->color == BLACK){
		ret = 0;
		return ret;
	}

	pnode right = root;
	pnode left = root->left;
	pnode parent = root->parent;
	if (parent->left->key == root->key)parent->left = left;
	else if (parent->right->key == root->key)parent->right = left;
	else ret = 0;

	left->parent = parent;
	right->left = left->right;
	left->right->parent=right;
	left->right = right;
	right->parent = left;
	left->color = right->color;
	right->color = RED;

	return ret;
}

int flipColor(pnode root){
	if (root->left->color == RED&&root->right->color == RED){
		root->left->color = BLACK;
		root->right->color = BLACK;
		root->color = RED;
		return 1;
	}
	else return 0;
}

int Insert(ptree tree, int key){
	int ret = 1;

	pnode cur = tree->NIL;
	cur = cur->left;
	while (cur->count){
		if (cur->key == key){
			cur->count++;
			return ret;
		}
		else if (cur->key < key){
			if (!(cur->right)->count)break;
			cur = cur->right;
		}
		else{
			if (!(cur->left)->count)break;
			cur = cur->left;
		}
	}
	pnode newNode = (pnode)malloc(sizeof(struct BSTnode));
	newNode->count = 1;
	newNode->key = key;
	newNode->color = RED;
	newNode->parent = cur;
	newNode->left = tree->NIL;
	newNode->right = tree->NIL;

	if (!cur->count){//first node
		newNode->color = BLACK;
		tree->NIL->left = newNode;
		tree->NIL->right = newNode;
	}

	if (cur->color==BLACK&&cur->left->color==BLACK&&cur->right->color==BLACK){//2-node

		if (cur->key > key){
			cur->left = newNode;			
		}
		else{
			cur->right = newNode;
			leftRotate(cur);
		}
	}
	else{
		pnode lowbound = NULL;
		if ((cur->left)->color==RED&&cur->right->count==0){//3-node's 3nd
			if (cur->key < key){
				cur->right = newNode;
				flipColor(cur);
				lowbound = cur;
			}
			else ret = 0;
		}
		else if (cur->color == RED&&cur->left->count==0&&cur->right->count==0){//3-node's 1/2st
			if (cur->key < key){
				cur->right = newNode;
				leftRotate(cur);
				rightRotate(newNode->parent);
				flipColor(newNode);
				lowbound = newNode;
			}
			else{
				cur->left = newNode;
				rightRotate(cur->parent);
				flipColor(cur);
				lowbound = cur;
			}
		}
		else ret = 0;
		
		//reduce RED chain
		while (ret&&lowbound->parent->count){
			if (lowbound->color == RED&&lowbound->parent->right == lowbound){
				if (lowbound->parent->left->color == RED){
					flipColor(lowbound->parent);
					lowbound=lowbound->parent;
				}
				else{
					leftRotate(lowbound->parent);
					lowbound = lowbound->left;
				}

			}
			else if (lowbound->color == RED&&lowbound->parent->color == RED&&lowbound->parent->parent->left == lowbound->parent){
				rightRotate(lowbound->parent->parent);
				flipColor(lowbound->parent);
				lowbound = lowbound->parent;
			}
			else if (lowbound->color == RED&&lowbound->parent->color == RED){
				leftRotate(lowbound->parent->parent);
				flipColor(lowbound->parent);
				lowbound = lowbound->parent;
			}
			else break;
		}
		
	}
	
	tree->count++;
	tree->NIL->left->color = BLACK;

	return ret;

}

void preorderVisitNode(pnode cur, FILE *fp){

	if(cur->count==0)return;
	fprintf(fp, "%d",cur->key*((cur->color==RED)?(-1):1) );
	fprintf(fp," ");
	preorderVisitNode(cur->left,fp);
	preorderVisitNode(cur->right,fp);
	
	return;
}

void preorderVisit(ptree tree, FILE* fp){

	fprintf(fp,"%d ",tree->count);
	pnode cur = tree->NIL;
	cur = cur->left;
	preorderVisitNode(cur,fp);
	return;
}

void freeNode(pnode nd){
	if(nd->count==0)return;
	else{
		freeNode(nd->left);
		freeNode(nd->right);
		free(nd);
	}
}

void freeTree(ptree tree){
	freeNode(tree->NIL);
	free(tree);
}
