#include <stdio.h>

void ThreeWaySorting(int* raw, int num){
	if (num > 0){
		int temprandom1 = GetRandom(num), temprandom2 = GetRandom(num);
		int temparraynum[5] = { raw[0],raw[num - 1],raw[num / 2],raw[temprandom1],raw[temprandom2] };
		int temparrayidx[5] = { 0,num - 1,num / 2,temprandom1,temprandom2 };
		int pivot = temparrayidx[GetCenter(temparraynum, 5)];
		
		int value = raw[pivot];
		Exchange(raw, pivot, 0);
		pivot = 0;//bug
		int lowidx = 1,highidx = num - 1;
		while (lowidx <= highidx){
			if (raw[lowidx] < value){
				Exchange(raw, pivot, lowidx);
				pivot++;
				lowidx++;
			}
			else if (raw[lowidx] > value){
				Exchange(raw, lowidx, highidx);
				highidx--;
			}
			else{
				lowidx++;
			}
		}
		
		ThreeWaySorting(raw, pivot);
		ThreeWaySorting(raw + lowidx, num - lowidx);
	}
}
