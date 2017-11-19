#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

//generate random number in [0,ceiling)
int GetRandom(int ceiling){
	
	int random = 0;
	if (ceiling - 1 < RAND_MAX && ceiling > 0){
		random = (int)(rand() % ceiling);
	}
	else{
		double temp = ceiling / RAND_MAX;//WHY?
		random = (int)(temp*rand());
	}
	return random;
}

int GetAverage(int* number, int num){
	int average = 0;
	for (int i = 0; i < num; i++){
		average += number[i];
	}
	average /= num;
	return average;
}

//average method

int GetCenter(int* number, int num){
	int average = GetAverage(number, num);
	int center = 0;
	int mindiff = abs(number[0] - average);
	for (int i = 1; i < num; i++){
		if (abs(number[i] - average) < mindiff){
			center = i;
		}
	}
	return center;
}

//median method

int GetLow(int *number, int num){
	int low = 0;

	/*
	if (num > 4){
		int minnum = number[0];
		for (int i = 1; i < num; i++){
			if (number[i] < minnum){
				minnum = number[i];
			}
		}
		int lowaverage = (GetAverage(number, num) + minnum) / 2;

		int mindiff = abs(number[0] - lowaverage);
		for (int i = 1; i < num; i++){
			if (abs(number[i] - lowaverage) < mindiff){
				low = i;
			}
		}
	}
	else if (num == 3){
		int minnum = min(min(number[0], number[1]), number[2]);
		for (int i = 0; i < num; i++){
			if (number[i] == minnum){
				low = i;
				break;
			}
		}
	}
	else if(num == 2){
		if (number[0] < number[1]){
			low = 0;
		}
		else low = 1;
	}
	*/

	int unit = 3 * num / 10 + 1;
	int minnum = 100000, minbefore = number[0], minidx = 0;
	for (int i = 0; i < num; i++){
		if (number[i]<minbefore){
			minbefore = number[i];
			minidx = i;
		}
	}
	for (int i = 2; i <= unit; i++){
		for (int j = 0; j < num; j++){
			if (number[j]<minnum&&number[j]>minbefore){
				minnum = number[j];
				minidx = j;
			}
		}
		minbefore = minnum;
	}
	
	low = minidx;

	return low;
}
int GetHigh(int *number, int num){
	int high = 0;

	/*
	if (num > 4){
		int maxnum = number[0];
		for (int i = 1; i < num; i++){
			if (number[i] > maxnum){
				maxnum = number[i];
			}
		}
		int highaverage = (GetAverage(number, num) + maxnum) / 2;

		int mindiff = abs(number[0] - highaverage);
		for (int i = 1; i < num; i++){
			if (abs(number[i] - highaverage) < mindiff){
				high = i;
			}
		}
	}
	else if (num == 3){
		int maxnum = max(max(number[0], number[1]), number[2]);
		for (int i = 0; i < num; i++){
			if (number[i] == maxnum){
				high = i;
				break;
			}
		}
	}
	else if (num == 2){
		if (number[0] < number[1]){
			high = 1;
		}
	}
	*/

	int unit = 3 * num / 10 + 1;
	int maxnum = -1, maxbefore = number[0], maxidx = 0;
	for (int i = 0; i < num; i++){
		if (number[i]>maxbefore){
			maxbefore = number[i];
			maxidx = i;
		}
	}
	for (int i = 2; i <= unit; i++){
		for (int j = 0; j < num; j++){
			if (number[j]>maxnum&&number[j]<maxbefore){
				maxnum = number[j];
				maxidx = j;
			}
		}
		maxbefore = maxnum;
	}
	high = maxidx;

	return high;
}

void Exchange(int* array, int idx1, int idx2){
	int temp = array[idx1];
	array[idx1] = array[idx2];
	array[idx2] = temp;
}