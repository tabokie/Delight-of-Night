#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

#include "sorting.h"
#include "basicfunc.h"

#define BOUND		5000

void main(){
	srand(time(NULL));

	FILE *fin, *fout;
	if ((fin = fopen("DATA.txt", "r")) == NULL){
		printf("File open ERROR!\n");
		exit(0);
	}
	if ((fout = fopen("sorted.txt", "w")) == NULL){
		printf("File create ERROR!\n");
	}

	int num;
	int* raw = NULL, *sorted = NULL;
	if (fscanf(fin, "%d", &num) == 1){
		int temp, i = 0;
		raw = (int*)malloc(sizeof(int)*num);
		sorted = (int*)malloc(sizeof(int)*num);
		while (fscanf(fin, "%d", &temp) == 1){
			raw[i++] = temp;
			sorted[i - 1] = temp;
		}
	}

	clock_t start;
	clock_t end;
	start = clock();
	//ThreeWaySorting(sorted, num); printf("THREE WAY\n");
	//220ms
	//DualPivotSorting(sorted, num); printf("DUAL PIVOT\n");
	//SelectionSorting(sorted, num); printf("SELECTION\n");
	//639000ms
	//ComplexSorting(sorted, num, BOUND);

	/*
	end = clock();
	printf("Time Used : %d ms\n", (int)((end - start) * 1000 / CLOCKS_PER_SEC));
	/**/

	/**/
	
	int minaverage = 1000;
	int boundaverage = 0;
	int countednum = 0;

	for (int bound = 5; bound <= 600; bound += 5){
		int average = 0;
		for (int i = 0; i < 30; i++){
			start = clock();
			ComplexSorting(sorted, num, bound);
			end = clock();
			average += (int)((end - start) * 1000 / CLOCKS_PER_SEC);
			//bug
			for (int j = 0; j < num; j++){
				sorted[j] = raw[j];
			}
		}
		average /= 30;

		if (average < minaverage){
			minaverage = average;
			boundaverage = bound;
			countednum = 1;
		}
		else if(average == minaverage){
			countednum++;
			boundaverage += bound;
		}

		printf("BOUND = %d --> Time Used : %d ms\n200", bound, average);
		for (int i = 0; i < average - 200; i++){
			printf("=");
		}
		printf("\n");
	}
	printf("The best BOUND value is : %d\n", boundaverage / countednum);
	/**/

	for (int i = 0; i < num; i++){
		fprintf(fout, "%d\n", sorted[i]);
	}

	if (fclose(fin)){
		printf("Cannot close the input file!\n");
	}
	if (fclose(fout)){
		printf("Cannot close the output file!\n");
	}
	
	system("pause");
	return;
}