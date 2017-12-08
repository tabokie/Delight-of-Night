#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <assert.h>

#define Inf 							(1e8)
#define New(typeName,size)				(typeName*)malloc(sizeof(typeName)*(size))
#define Initial(arr,val,size)     		do{int initial_i_;for(initial_i_=0;initial_i_<size;initial_i_++)arr[initial_i_]=val;}while(0)
#define Min(a,b)						((a)>(b)?(b):(a))		
#define Max(a,b)						((a)<(b)?(b):(a))

/*  MACRO for timing  */
#define CLOCK_INIT						struct timespec start,end;
#define CLOCK_START						clock_gettime(CLOCK_MONOTONIC,&start);
#define CLOCK_END						clock_gettime(CLOCK_MONOTONIC,&end);
#define CLOCK_SHOW						printf("Elapsed time: %.6f ms\n",\
										1000*(end.tv_sec-start.tv_sec)+(double)(end.tv_nsec-start.tv_nsec)/1000000);

// global variables
int v,e,C;
// check if legal vertice id in graph
#define LEGAL(vertice)					(vertice>=0&&vertice<v)					
// return union root of a vertice
#define ROOT(union_set,vertice)			({int ROOT_i_= vertice;\
	while(LEGAL(ROOT_i_)&&union_set[ROOT_i_]>=0)ROOT_i_=union_set[ROOT_i_];\
	if(!LEGAL(ROOT_i_))assert(LEGAL(ROOT_i_));\
	ROOT_i_;})

// \breif: merge two components
// \param:
// 	Graph info:
//		union set
//		max edge in spanning tree
//	Operand:
//		a,b: two union id
//		max_length: the minimum length connecting a and b
void mergeUnion(int* union_set,int* max_edge, int a, int b, int length);

// \brief: Dual Pivot Quick sort (without randomized pivot selection)
// \param: 
// 	2-dimension integer set (use first row as key)
// 		data: pointer
//		n: row number
//	Sorting info
//		init: start pivot
//		size: checking scope
void CombinedSort(int** data,int n,int init,int size);

// \brief Exchange on 2-dimension data set
// \param:
//	2-dimension integer set:
//		data: pointer
//		n: row number
//	a,b: exchange idx
void CombinedExchange(int** data,int n,int a,int b);


/*  initial timing struct  */
CLOCK_INIT

int main(void){

	/*  initial and read graph  */
	scanf("%d%d%d",&v,&e,&C);
	// apply for space>>
	// list for union set
	int* unionSet=New(int,v);
	Initial(unionSet,-1,v);// all vertices are single component with size 1
	int* maxEdge = New(int ,v);
	Initial(maxEdge,0,v);// all vertice has no edge in there spanning tree
	// list for edges
	int* edgeFrom=New(int,e);
	int* edgeTo=New(int,e);
	int* edgeLength=New(int,e);

	// read edges>>
	int i;
	int a,b,l;
	for(i=0;i<e;i++){
		scanf("%d%d%d",&a,&b,&l);
		edgeLength[i]=l;
		// select smaller id as FROM
		edgeFrom[i]=Min(a,b);
		// bigger id as TO
		edgeTo[i]=Max(a,b);
	}

	/*  START timing  */
	CLOCK_START	
	// set up combined data set
	int* sort_data[3]={edgeLength,edgeFrom,edgeTo};
	// sort by edge length
	CombinedSort(sort_data,3,0,e);

	int from,to,length;
	for(i=0;i<e;i++){// visit edges by length
		// load info about current edge
		from=edgeFrom[i];
		to=edgeTo[i];
		length=edgeLength[i];
		from = ROOT(unionSet,from);
		to = ROOT(unionSet,to);
		// IF either of u1, u2 satisfies condition for a component
		// pass
		if(maxEdge[from]+C/(-unionSet[from])<length\
			||maxEdge[to]+C/(-unionSet[to])<length)continue;
		// ELSE
		// merge u1 and u2
		mergeUnion(unionSet,maxEdge, from,to,length);

	}
	CLOCK_END
	/*  END timing  */

	/*  print segemented graph  */
	int j;
	int root;
	int size;
	for(i=0;i<v;i++){
		// fetch this element's union id
		root=ROOT(unionSet,i);
		// IF already printed
		// pass
		if(unionSet[root]==-Inf)continue;
		size=-unionSet[root];
		// print first element without space
		printf("%d",i);
		// scan the rest vertices looking for members
		for(j=i+1;j<v&&size>0;j++){
			if(ROOT(unionSet,j)==root){
				printf(" %d",j);
				size--;
			}
		}
		printf("\n");
		// mark the printed union
		unionSet[root]=-Inf;
	}	

	/*  show timing  */
	CLOCK_SHOW

	// release applied space
	free(unionSet);
	free(maxEdge);
	free(edgeLength);
	free(edgeTo);
	free(edgeFrom);

	return 0;
}

void mergeUnion(int* union_set, int* max_edge, int a, int b, int length){
	if(a<0||b<0)return;
	int root=ROOT(union_set,a),son=ROOT(union_set,b),size;

	// select the larger union as main union
	if(union_set[root]>union_set[son]){
		int temp = root;
		root=son;
		son=temp;
	}
	size=union_set[root]+union_set[son];
	// merge small union to main union
	// link union set
	union_set[son] = root;
	// store combined size
	union_set[root] = size;
	// store the max edge in spannin tree in new union
	max_edge[root] = length;
	// delete the previous max edge
	max_edge[son] = -1;
	return;
}

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
		for(i=0;i<3;i++)CombinedSort(data,n,initInfo[i],sizeInfo[i]);
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

void CombinedExchange(int** data,int n,int a,int b){
	assert(data!=NULL);
	int temp,i;
	// for each row:
	for(i=0;i<n;i++){
		assert(data[i]!=NULL);
		temp=data[i][a];
		data[i][a]=data[i][b];
		data[i][b]=temp;
	}
	return;
}
