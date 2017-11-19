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

	int flag=1;

	while(flag){
		printf(">>>Enter new test case:(One case at a time, as formula{ N [item_i] }) \n");

		int iter=1e8;
		int i;
		Tree tree=readTree(&iter);
		int res;
		//////////////////////////////////////////
		start = clock();
		for(i=0;i<iter;i++)res=isRBT(tree);
		stop = clock();
		/////////////////////////////////////////
		if(res)printf("Yes\n");
		else printf("No\n");
		freeTree(tree);

		clocks = stop - start;
		total_time = ((double)clocks) / CLK_TCK;
		printf("Ticks:%d\n", clocks);
		printf("Duration(sec):%.12f\n", ((double)total_time)/iter);
		printf("=================================================================\n");
		printf(">>>continue?(1/0)\n");
		scanf("%d",&flag);
	}
	printf("Quitting...");
	return 0;
}
