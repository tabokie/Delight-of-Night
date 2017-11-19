#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int GetRandom(int ceiling);
void exchange(int* list, int i,int j){
	if(i==j)return;
	else{
		int temp=list[i];
		list[i]=list[j];
		list[j]=temp;
		return;
	}
}

void main(){
	srand(time(NULL));

	printf("Number of samples : \n");
	int size;
	scanf("%d", &size);
	printf("Size for one sample : \n");
	int num;
	scanf("%d",&num);
	int ceiling=10*num;

	FILE *fout;
	if ((fout = fopen("numberSample.txt", "w")) == NULL){
		printf("File create ERROR!\n");
	}
	
	fprintf(fout,"%d\n", size);
	int i,j,k;
	for (i = 0; i < size; i++){
		fprintf(fout, "%d ", num);
		int* list=(int*)malloc(sizeof(int)*num);
		for(j=0;j<num;j++){
			list[j]=j+1;
		}
		for(j=0;j<num;j++){
			exchange(list,j,GetRandom(num));
		}
		for(j=0;j<num;j++)fprintf(fout,"%d ",list[j]);
		free(list);
		fprintf(fout, "\n");
	}

	if (fclose(fout)){
		printf("File close ERROR!\n");
	}
	//system("pause");
	return;
}


//generate random number in [0,ceiling)
int GetRandom(int ceiling){

	int random = 0;
	if (ceiling - 1 < RAND_MAX){
		random = (int)rand() % ceiling;
	}
	else{
		random = (int)(((double)rand()/ RAND_MAX)*(ceiling - 1)) ;
	}
	return random;
}