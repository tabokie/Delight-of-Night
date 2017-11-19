#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "project2.h"
//included API:
//type Tree
//Tree readTree(void);
//int isRBT(Tree);
//int freeTree(Tree);

clock_t start, stop;
int clocks;
double total_time;
double duration;

int main(void){

	FILE* fin;
	FILE* fout;
	if((fin=fopen("treeSample.txt","r"))==NULL||(fout=fopen("sampleResult.txt","w"))==NULL){
		printf("File open error\n");
		system("pause");
		exit(1);
	}

	int sampleSize;
	fscanf(fin,"%d",&sampleSize);
	printf(">>>Testing with file... \n");
	while(sampleSize--){
		
		int i;
		int iter=1e8;
		Tree tree=readTree_with_file(fin,fout,&iter);
		int res;
		
		//////////////////////////////////////////
		start = clock();
		for(i=0;i<iter;i++)res=isRBT(tree);
		stop = clock();
		//////////////////////////////////////////ss
		if(res){
			printf("Yes\n");
			fprintf(fout,"Yes\n");
		}
		else{
			printf("No\n");
			fprintf(fout,"No\n");
		}
		freeTree(tree);

		clocks = stop - start;
		total_time = ((double)clocks) / CLK_TCK;
		printf("Ticks:%d\n", clocks);
		printf("Duration(sec):%.12f\n", ((double)total_time)/iter);
		printf("=================================================================\n");

		fprintf(fout,"Ticks:%d\n", clocks);
		fprintf(fout,"Duration(sec):%.12f\n", ((double)total_time)/iter);
		fprintf(fout,"=================================================================\n");

	}
	if((fclose(fin))||(fclose(fout))){
		printf("File close error\n");
		system("pause");
		exit(1);
	}

	printf("Quitting...");

	return 0;
}
