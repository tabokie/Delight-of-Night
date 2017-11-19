#include <stdio.h>
#include <stdlib.h>
#include "red-blackBST.h"

int main(){


	FILE *fp;
	if((fp=fopen("numberSample.txt","r"))==NULL){
		printf("File open error!\n");
		exit(0);
	}

	FILE *output;
	if ((output = fopen("treeSample.txt", "w")) == NULL){
		printf("File create error!\n");
	}

	int i,sampleSize;
	fscanf(fp,"%d",&sampleSize);
	fprintf(output, "%d\n",sampleSize );
	for(i=0;i<sampleSize;i++){
		ptree tree = initTree();
		int n = 0;
		fscanf(fp,"%d", &n);
		while (n--){
			int temp;
			fscanf(fp,"%d", &temp);
			Insert(tree, temp);
		}
		preorderVisit(tree,output);
		fprintf(output, "\n");
		freeTree(tree);
	}
	


	if (fclose(fp) || fclose(output)){
		printf("File close error!\n");
	}
	//system("pause");
	return 0;
}