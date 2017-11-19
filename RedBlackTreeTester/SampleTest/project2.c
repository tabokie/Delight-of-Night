#include <stdio.h>
#include <stdlib.h>
#include "project2.h"

Tree readTree(int* iter){
	Tree newTree=(Tree)malloc(sizeof(struct TNode));
	int size;
	scanf("%d",&size);
	*iter/=size;
	int firstNode;
	scanf("%d",&firstNode);
	newTree->key=abs(firstNode);
	newTree->color=(firstNode>0)?(BLACK):(RED);
	newTree->r=NULL;
	newTree->l=NULL;
	newTree->father=NULL;
	size--;
	while(size--){
		if(!readNode(newTree)){
			printf("readind node error\n");
			system("pause");
			exit(1);
		}
	}
	return newTree;

}

int readNode(Tree tree){
	pnode cur;
	pnode next=tree;
	int key;
	scanf("%d",&key);
	int flag=BLACK;
	if(key<0)flag=RED;
	else if(key==0){
		printf("ZeroNode\n");
		system("pause");
		exit(1);
	}
	key=abs(key);

	do{
		cur=next;
		if(key<cur->key)next=cur->l;
		else if(key>cur->key)next=cur->r;
		else {
			printf("SameNode: %d\n",key);
			system("pause");
			exit(1);
		}

	}
	while(next!=NULL);

	pnode newNode=(pnode)malloc(sizeof(struct TNode));
	newNode->key=key;
	newNode->color=flag;
	newNode->r=NULL;
	newNode->l=NULL;
	newNode->father=cur;
	if(key<cur->key)cur->l=newNode;
	else cur->r=newNode;
	//printf("%d->%d\n",cur->key,newNode->key);
	return 1;

}

Tree readTree_with_file(FILE* fp,FILE* fout,int* iter){
	Tree newTree=(Tree)malloc(sizeof(struct TNode));
	int size;
	fscanf(fp,"%d",&size);
	*iter/=size;
	fprintf(fout,"Sample Size: %d\n",size);
	int firstNode;
	fscanf(fp,"%d",&firstNode);
	newTree->key=abs(firstNode);
	newTree->color=(firstNode>0)?(BLACK):(RED);
	newTree->r=NULL;
	newTree->l=NULL;
	newTree->father=NULL;
	size--;
	while(size--){
		if(!readNode_with_file(newTree,fp)){
			printf("readind node error\n");
			system("pause");
			exit(1);
		}
	}
	return newTree;

}

int readNode_with_file(Tree tree,FILE* fp){
	pnode cur;
	pnode next=tree;
	int key;
	fscanf(fp,"%d",&key);
	int flag=BLACK;
	if(key<0)flag=RED;
	else if(key==0){
		printf("ZeroNode\n");
		system("pause");
		exit(1);
	}
	key=abs(key);

	do{
		cur=next;
		if(key<cur->key)next=cur->l;
		else if(key>cur->key)next=cur->r;
		else {
			printf("SameNode: %d\n",key);
			system("pause");
			exit(1);
		}

	}
	while(next!=NULL);

	pnode newNode=(pnode)malloc(sizeof(struct TNode));
	newNode->key=key;
	newNode->color=flag;
	newNode->r=NULL;
	newNode->l=NULL;
	newNode->father=cur;
	if(key<cur->key)cur->l=newNode;
	else cur->r=newNode;
	//printf("%d->%d\n",cur->key,newNode->key);
	return 1;

}

int checkRedNode(Tree tree){
	if(tree==NULL)return 1;
	if(tree->color==RED){
		if(tree->r!=NULL&&tree->r->color==RED)return 0;
		if(tree->l!=NULL&&tree->l->color==RED)return 0;
	}
	return checkRedNode(tree->r)&&checkRedNode(tree->l);
}

int BlackNodeSize(Tree tree){
	pnode cur=tree;
	int size=0;
	while(cur!=NULL){
		if(cur->color==BLACK)size++;
		cur=cur->l;
	}
	return size;
}

int checkBlackNodeRucursive(Tree tree, int expectedSize){
	if(tree==NULL){
		if(expectedSize==0)return 1;
		else return 0;
	}
	else{
		if(tree->color==RED)return checkBlackNodeRucursive(tree->r,expectedSize)&&checkBlackNodeRucursive(tree->l,expectedSize);
		else return checkBlackNodeRucursive(tree->r,expectedSize-1)&&checkBlackNodeRucursive(tree->l,expectedSize-1);
	}
}

int checkBlackNode(Tree tree){
	int size=BlackNodeSize(tree);
	return checkBlackNodeRucursive(tree, size);
}

int isRBT(Tree tree){
	if(tree->color==RED)return 0;//condition 1
	return checkBlackNode(tree)&&checkRedNode(tree);
}

int freeTree(Tree tree){
	if(tree==NULL)return 1;
	else{
		freeTree(tree->l);
		freeTree(tree->r);
		free(tree);
		return 1;
	}
}