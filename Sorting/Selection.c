#include <stdio.h>

void SelectionSorting(int* raw, int num){
	int idx = 0;
	for (idx = 0; idx < num; idx++){
		//printf("Sorted : %d\n", idx);
		int minidx = idx;
		int minnum = raw[idx];
		for (int i = idx + 1; i < num; i++){
			if (raw[i] < minnum){
				minidx = i;
				minnum = raw[i];
			}
			//ultimaze
			if (idx > 0 && minnum == raw[idx - 1]){
				break;
			}
		}
		Exchange(raw, minidx, idx);
	}

}