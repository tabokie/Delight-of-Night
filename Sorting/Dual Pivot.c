#include <stdio.h>

// with bug in exchange
void DualPivotSorting(int* raw, int num){
	
	if (num > 2){
		int temprandom1 = GetRandom(num), temprandom2 = GetRandom(num);
		int tempnumber[5] = { raw[temprandom1],raw[temprandom2],raw[0],raw[num - 1],raw[num / 2] };
		int tempidx[5] = { temprandom1,temprandom2,0,num - 1,num / 2 };
		int pivot1 = tempidx[GetLow(tempnumber, 5)], pivot2 = tempidx[GetHigh(tempnumber, 5)];
		if (pivot1 == pivot2){
			pivot1 = num / 2 - 1;
			pivot2 = num / 2 + 1;
			if (raw[pivot1] > raw[pivot2]){
				Exchange(raw, pivot1, pivot2);
			}
		}
		Exchange(raw, pivot1, 0);
		Exchange(raw, pivot2, num - 1);
		pivot1 = 0;
		pivot2 = num - 1;
						
		int value1 = raw[pivot1], value2 = raw[pivot2];
		int lowidx = pivot1 + 1, highidx = pivot2 - 1;
		while (lowidx <= highidx){
			if (raw[lowidx] < value1){
				Exchange(raw, lowidx, pivot1);
				lowidx++;
				pivot1++;
			}
			else if (raw[lowidx] >= value1&&raw[lowidx] <= value2){
				lowidx++;
			}
			else{
				Exchange(raw, lowidx, highidx);
				if (highidx < pivot2 - 1){
					Exchange(raw, pivot2, pivot2 - 1);
				}
				Exchange(raw, highidx, pivot2);
				pivot2--;
				highidx--;
			}
		}
		//bug
		DualPivotSorting(raw, pivot1);
		DualPivotSorting(raw + pivot1 + 1, pivot2 - pivot1 - 1);
		DualPivotSorting(raw + pivot2 + 1, num - pivot2 - 1);
	}
	else if (num == 2){
		if (raw[0] > raw[1]){
			Exchange(raw, 0, 1);
		}
	}
}

void DualPivotSorting_without_random_pivot(int* raw, int num){
	
	if (num > 2){
		int pivot1=0,pivot2=num-1;
		if(raw[pivot1]>raw[pivot2])Exchange(raw,pivot1,pivot2);
						
		int value1 = raw[pivot1], value2 = raw[pivot2];
		int lowidx = pivot1 + 1, highidx = pivot2 - 1;
		while (lowidx <= highidx){
			if (raw[lowidx] < value1){
				Exchange(raw, lowidx, pivot1+1);
				Exchange(raw,pivot1,pivot1+1);
				lowidx++;
				pivot1++;
			}
			else if (raw[lowidx] >= value1&&raw[lowidx] <= value2){
				lowidx++;
			}
			else{
				Exchange(raw, lowidx, highidx);
				if (highidx < pivot2 - 1){
					Exchange(raw, highidx, pivot2 - 1);
				}
				Exchange(raw, highidx, pivot2);
				pivot2--;
				highidx--;
			}
		}
		//bug
		DualPivotSorting(raw, pivot1);
		DualPivotSorting(raw + pivot1 + 1, pivot2 - pivot1 - 1);
		DualPivotSorting(raw + pivot2 + 1, num - pivot2 - 1);
	}
	else if (num == 2){
		if (raw[0] > raw[1]){
			Exchange(raw, 0, 1);
		}
	}
}