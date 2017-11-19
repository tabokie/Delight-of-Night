#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int TYPE = 0;
int NUM = 0;

void DefineGenerator(int temp,int num);

int yield();

void main(){
	srand(time(NULL));

	printf("Type : 1)inverted sequence 2)common random 3)concentrated sequence 4)general formula\n");
	int type;
	scanf("%d", &type);
	printf("Ceiling : \n");
	int ceiling;
	scanf("%d", &ceiling);
	printf("Number : \n");
	int num;
	scanf("%d", &num);
	DefineGenerator(type, ceiling);

	FILE *fout;
	if ((fout = fopen("DATA.txt", "w")) == NULL){
		printf("File create ERROR!\n");
	}
	
	fprintf(fout,"%d\n", num);
	for (int i = 0; i < num; i++){
		int temp = (int)yield();
		fprintf(fout, "%d\n", temp);
	}

	if (fclose(fout)){
		printf("File close ERROR!\n");
	}
	system("pause");
	return;
}

void DefineGenerator(int temp,int num){
	TYPE = temp;
	NUM = num;
}

int yield(){
	static int value = 0;
	switch (TYPE){
	case 1:
	{
		static int is_init = 0;
		if (!is_init){
			value = NUM;
			is_init = 1;
		}
		value--;
		if (value < 0){
			value = NUM;
		}
		break;
	}
	case 2:
	{
		value = GetRandom(NUM + 1);
		break;
	}
	case 3:
	{
		value = GetRandom(NUM / 4 + 1);
		break;
	}
	//case 4:
	//{
	//	printf("Formula : \n");
	//	break;
	//}
	default:
	{
		break;
	}
	}

	return value;
}

//generate random number in [0,ceiling)
int GetRandom(int ceiling){

	int random = 0;
	if (ceiling - 1 < RAND_MAX){
		random = (int)rand() % ceiling;
	}
	else{
		random = (int)rand()*(ceiling - 1) / RAND_MAX;
	}
	return random;
}