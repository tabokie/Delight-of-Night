#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <math.h>
#include <time.h>
#include <assert.h>

#define MAX_SIZE						(1000)
#define Inf 							(1e8)
#define Error(note)						({printf(#note);printf("\n");exit(1);})
#define New(typeName,size)				(typeName*)malloc(sizeof(typeName)*(size))
// use MACRONAME_i_ to interfere
#define Initial(array,val,size)     	({int initial_i_;for(initial_i_=0;initial_i_<size;initial_i_++)array[initial_i_]=val;}) 
#define For(i,a,b)                  	for(i=(a);(((a)<=(b))?((b)-(i)):((i)-(b)))>0;i+=(((a)<=(b))?1:-1))
#define Min(a,b)						((a)>(b)?(b):(a))		
#define Max(a,b)						((a)<(b)?(b):(a))	


#define CLOCK_INIT					struct timespec start,end;
#define CLOCK_START					clock_gettime(CLOCK_MONOTONIC,&start);
#define CLOCK_END					clock_gettime(CLOCK_MONOTONIC,&end);
#define CLOCK_SHOW					printf("Elapsed time: %.6f ms\n",\
									1000*(end.tv_sec-start.tv_sec)+(double)(end.tv_nsec-start.tv_nsec)/1000000);


#define ROOT(union_set,i)				({int ROOT_i_=i;while(union_set[ROOT_i_]>=0)ROOT_i_=union_set[ROOT_i_];ROOT_i_;})	
#define FATHER(union_set,i)				({int FATHER_i_=i;while(FATHER_i_>=0)FATHER_i_=union_set[FATHER_i_];-1*FATHER_i_;})	
#define COMPONENT_MAX_EDGE(union_set,i)	(FATHER(union_set,i)/BASE)
#define COMPONENT_SIZE(union_set,i)		(FATHER(union_set,i)%BASE)
// v<=1000
#define BASE							(10000)

void CombinedExchange(int** data,int n,int a,int b);
void CombinedSort(int** data,int n,int init,int size);
void mergeUnionSet(int* union_set, int a, int b, int max_length);


CLOCK_INIT
int main(void){

	int v,e,C;
	scanf("%d%d%d",&v,&e,&C);

	int* unionSet=New(int,v);
	Initial(unionSet,-1,v);// all vertice is single component
	int* edgeFrom=New(int,e);
	int* edgeTo=New(int,e);
	int* edgeLength=New(int,e);


	int i;
	int a,b,l;
	For(i,0,e){
		scanf("%d%d%d",&a,&b,&l);
		edgeLength[i]=l;
		edgeFrom[i]=Min(a,b);
		edgeTo[i]=Max(a,b);
	}
	CLOCK_START	
	int* sort_data[3]={edgeLength,edgeFrom,edgeTo};
	CombinedSort(sort_data,3,0,e);

	int from,to,length;
	For(i,0,e){// visit by edge
		from=edgeFrom[i];
		to=edgeTo[i];
		length=edgeLength[i];
		if(COMPONENT_MAX_EDGE(unionSet,from)+C/COMPONENT_SIZE(unionSet,from)<length\
			||COMPONENT_MAX_EDGE(unionSet,to)+C/COMPONENT_SIZE(unionSet,to)<length)continue;
		mergeUnionSet(unionSet,from,to,length);

	}
	CLOCK_END
	int j;
	int root;
	For(i,0,v){
		root=ROOT(unionSet, i);
		if(unionSet[root]==-Inf)continue;
		printf("%d",i);
		For(j,i+1,v){
			if(ROOT(unionSet,j)==root)printf(" %d",j);
		}
		printf("\n");
		unionSet[root]=-Inf;
	}
	CLOCK_SHOW
	
	return 0;
}

void CombinedExchange(int** data,int n,int a,int b){
	if(data==NULL)Error(NIL column pointer doing exchange!);
	int temp,i;
	For(i,0,n){
		if(data[i]==NULL)Error(NIL row pointer doing exchange!);
		temp=data[i][a];
		data[i][a]=data[i][b];
		data[i][b]=temp;
	}

	return;
}

// Dual Pivot Quick sort (without randomized pivot selection)
// Param: 
// 	2-dimension integer set (use first row as key)
// 		data: pointer
//		n: row number
//	Sorting info
//		init: start
//		size: checking scope
void CombinedSort(int** data,int n,int init,int size){
	// select key row
	int* key=data[0];
	// if more than 2 elements
	if (size > 2){
		// select pivot
		int pivot1 = 0, pivot2 = size-1;
		// normalize pivot
		if (key[init+pivot1] > key[init+pivot2]){
			CombinedExchange(data,n, init+pivot1, init+pivot2);
		}
		// store pivot value
		int value1 = key[init+pivot1], value2 = key[init+pivot2];
		// iterative boundary
		int lowidx = pivot1 + 1, highidx = pivot2 - 1;
		// main loop
		while (lowidx <= highidx){
			// if left bound is smaller than pivot 1
			// exchange to left area
			if (key[init+lowidx] < value1){
				CombinedExchange(data,n, init+lowidx, init+pivot1+1);
				CombinedExchange(data,n, init+pivot1, init+pivot1+1);
				lowidx++;
				pivot1++;
			}
			// if left bound is between pivot 1 and pivot 2
			// pass
			else if (key[init+lowidx] >= value1&&key[init+lowidx] <= value2){
				lowidx++;
			}
			// if left bound is bigger than pivot 2
			// exchange to right area
			else{
				CombinedExchange(data,n, init+lowidx, init+highidx);
				if (highidx < pivot2 - 1){
					CombinedExchange(data,n, init+highidx, init+pivot2 - 1);
				}
				CombinedExchange(data,n, init+highidx, init+pivot2);
				pivot2--;
				highidx--;
			}
		}
		// load recursion info
		int i;
		int initInfo[3]={init+0,init+pivot1+1,init+pivot2+1};
		int sizeInfo[3]={pivot1,pivot2 - pivot1 - 1,size - pivot2 - 1};
		// recursion begin
		For(i,0,3)CombinedSort(data,n,initInfo[i],sizeInfo[i]);
	}
	// if only two element
	else if (size == 2){
		// if not sorted
		if (key[init+0] > key[init+1]){
			CombinedExchange(data,n, init+0, init+1);
		}
	}
	// if only one element
	// pass
}

// 
void mergeUnionSet(int* union_set, int a, int b, int max_length){
	if(a<0||b<0)return;
	int root,son,size;
	if(COMPONENT_SIZE(union_set,a)<COMPONENT_SIZE(union_set,b)){
		root=ROOT(union_set,b);
		son=ROOT(union_set,a);
	}
	else{
		root=ROOT(union_set,a);
		son=ROOT(union_set,b);
	}
	size=COMPONENT_SIZE(union_set,root)+COMPONENT_SIZE(union_set,son);
	union_set[son]=root;
	union_set[root]=-1*(max_length*BASE+size);
	return;
}