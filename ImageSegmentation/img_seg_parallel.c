#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <math.h>
#include <time.h>
#include <assert.h>

#define MAX_SIZE					(1000)
#define Inf 						(1e8)
#define Error(note)					({printf(#note);printf("\n");exit(1);})
#define New(typeName,size)			(typeName*)malloc(sizeof(typeName)*(size))
#define Initial(array,val,size)     ({int in_i_;for(in_i_=0;in_i_<size;in_i_++)array[in_i_]=val;}) 
#define PutInts(array,size)         ({printf(#array);printf(": ");int in_i_;for(in_i_=0;in_i_<size;in_i_++)printf("%d ",array[in_i_]);printf("\n");}) 
#define Log()                       ({printf("%s",__func__);printf("\n");})
#define BP(note)                    ({printf(#note);printf(", In function %s",__func__);printf("\n");})
// #define For(i,a,b)                  for(i=(a);(((a)<=(b))?((b)-(i)):((i)-(b)))>0;i+=(((a)<=(b))?1:-1))
#define For(i,a,b)                  for(i=(a);i<(b);i++)

#define Min(a,b)					((a)>(b)?(b):(a))			
typedef enum{false,true}bool;

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

// Class: link_out
// data structure for storing linked edges
typedef struct LINKED_INT_t{
	int length;// sorting pivot
	int target;
	// struct LINKED_INT_t* prior;
	struct LINKED_INT_t* next;
}* lkd_int;
lkd_int newLinkedInt(int a, int b){
	lkd_int new=New(struct LINKED_INT_t,1);
	new->target=b;
	new->length=a;
	// new->prior=NULL;
	new->next=NULL;
	return new;
}
int InsertLinkedInt(lkd_int* head_addr, int a, int b){
	Log();
	if(head_addr==NULL)return 0;
	lkd_int new=newLinkedInt(a,b);
	if(*head_addr==NULL){
		*head_addr=new;
		return 1;
	}
	lkd_int cur=*head_addr;
	if(cur->length>a){
		*head_addr=new;
		new->next=cur;
		// cur->prior=new;
		return 1;
	}
	while(cur->next!=NULL){
		if(cur->length<=a&&cur->next->length>a){
			// cur->next->prior=new;
			new->next=cur->next;
			cur->next=new;
			// new->prior=cur;
			return 1;
		}
	}
	if(cur->length<=a){
		cur->next=new;
		// new->prior=cur;
		return 1;
	}
	else return 0;
}

typedef struct findLinkedOutClass{
	int v;
	int e;
	lkd_int* head;
	lkd_int* end;
}*link_out;


link_out findLinkedOut_Initial(int v,int e){
	Log();
	link_out self=New(struct findLinkedOutClass, 1);

	self->v=v;
	self->e=e;

	self->head=New(lkd_int,v);
	Initial(self->head,NULL,v);
	self->end=New(lkd_int,v);
	Initial(self->end,NULL,v);
	return self;
}
int findLinkedOut_Read(link_out self,int a, int b, int l){
	Log();
	if(a<0||a>=self->v||b<0||b>=self->v)return 0;
	InsertLinkedInt(self->head+a,l,b);
	if(self->end[a]==NULL)self->end[a]=self->head[a];
	else if(self->end[a]->next!=NULL)self->end[a]=self->end[a]->next;
	InsertLinkedInt(self->head+b,l,a);
	if(self->end[b]==NULL)self->end[b]=self->head[b];
	else if(self->end[b]->next!=NULL)self->end[b]=self->end[b]->next;
	return 1;
}

int minLinkedOut_length(link_out self,int vertice){
	Log();
	if(vertice<0||vertice>=self->v)return 0;
	if(self->head[vertice]==NULL)return Inf;
	return self->head[vertice]->length;
}
int minLinkedOut_target(link_out self,int vertice){
	Log();
	if(vertice<0||vertice>=self->v)return -1;// illegal vertice input
	if(self->head[vertice]==NULL)return -1;// no link out
	return self->head[vertice]->target;
}
void deleteVertice(link_out self, int vertice){
	Log();
	if(vertice<0||vertice>=self->v)return;
	self->head[vertice]=NULL;
	self->end[vertice]=NULL;
	return;
}
void delMinOut(link_out self, int vertice){
	Log();
	if(vertice<0||vertice>=self->v)return;
	self->head[vertice]=self->head[vertice]->next;
	return;
}


#define SET_SIZE	(10)
// Class: Vertice Set
typedef struct VERTICE_SET_t{

	link_out linkOut;
	int* unionSet;
	int* unionMinLinkOut;

}* v_set;

#define ROOT(union_set, i)		({int ROOT_i_=i;while(union_set[ROOT_i_]>=0)ROOT_i_=union_set[ROOT_i_];ROOT_i_;})

v_set verticeSetInitial(link_out linkoutInstance, int v){// numbered from 0 to v-1
	Log();
	v_set new=New(struct VERTICE_SET_t,1);
	new->linkOut=linkoutInstance;
	new->unionSet=New(int,v);
	Initial(new->unionSet,-1,v);
	new->unionMinLinkOut=New(int,v);
	int i;
	For(i,0,v)new->unionMinLinkOut[i]=minLinkedOut_length(linkoutInstance,i);
	return new;
}

int mergeSet(v_set self, int a, int b){
	Log();
	delMinOut(self->linkOut,a);
	delMinOut(self->linkOut,b);
	int root_a = ROOT(self->unionSet,a);
	int root_b = ROOT(self->unionSet,b);
	if(self->unionSet[root_a]>self->unionSet[root_b]){// a is smaller
		int temp=root_b;
		root_b=root_a;
		root_a=temp;
	}
	self->unionSet[root_a]+=self->unionSet[root_b];
	self->unionSet[root_b]=root_a;
	self->unionMinLinkOut[root_a]=minUnionLinkedOut_length(self,root_a);
	return root_a;
}

int minUnionLinkedOut_length(v_set self, int root){
	Log();
	int size=-self->unionSet[root];
	int cur=0;
	int min_length=Inf;
	int v=self->linkOut->v;
	while(size--){
		while(cur<v&&ROOT(self->unionSet,cur)!=root){
			cur++;
		}
		if(cur>=v)break;
		min_length=Min(min_length,minLinkedOut_length(self->linkOut,cur));
	}
	return min_length;
}

int minUnionLinkedOut_target(v_set self, int root){
	Log();
	int size=-self->unionSet[root];
	int cur=0;
	int min_length=Inf;
	int temp;
	int min_target;
	int v=self->linkOut->v;
	while(size--){
		while(cur<v&&ROOT(self->unionSet,cur)!=root){
			cur++;
		}
		if(cur>=v)break;
		temp=minLinkedOut_length(self->linkOut,cur);
		if(temp<min_length){
			min_length=temp;
			min_target=minLinkedOut_target(self->linkOut,cur);
		}
	}
	return min_target;
}


int deleteSet(v_set self, int root){
	Log();
	// delete in linkOut
	int size=-self->unionSet[root];
	int cur=0;
	while(size--){
		while(ROOT(self->unionSet,cur)!=root){
			cur++;
		}
		deleteVertice(self->linkOut, cur);
	}
}


int main(void){

	srand(time(NULL));

	int THREAD_NUM = omp_get_num_procs();
	printf("Reset thread num?\n");
	int reply;
	scanf("%d",&reply);
	if(reply){
		printf("Enter thread num:\n");
		scanf("%d",&THREAD_NUM);
	}
	omp_set_num_threads(THREAD_NUM);

	int v,e,C;
	scanf("%d%d%d",&v,&e,&C);
	link_out linkoutInstance = findLinkedOut_Initial(v,e);
	v_set verticeSetInstance = verticeSetInitial(linkoutInstance, v);
	int i;
	int a,b,l;
	For(i,0,e){
		scanf("%d%d%d",&a,&b,&l);
		if(a<0||a>=v||b<0||b>=v)Error(No such vertice to link!);
		findLinkedOut_Read(linkoutInstance,a,b,l);
	}

	int* locked=New(int,v);
	Initial(locked,0,v);
	int active_union=v;
	// main parallel process
	#pragma omp parallel for 
	For(i,0,THREAD_NUM){
		int cursor;
		int cursor_trail;
		int ifDelete=0;
		while(active_union>0){// if still component not deleted
			// main search block
			// printf("Thread %d, new run\n",i);
			do{
				cursor=GetRandom(v);
				cursor_trail=-1;
				while(true){
					cursor=minUnionLinkedOut_target(verticeSetInstance,cursor);
					if(cursor==cursor_trail)break;
					else{
						cursor_trail=cursor;
						cursor=minUnionLinkedOut_target(verticeSetInstance,cursor);
					}
				}
			}
			while(cursor>=0&&locked[cursor]);
			printf("Active union:%d,Find cursor:%d\n",active_union, cursor);
			// assert(cursor<0||locked[cursor]==0);

			// conflict processing
			if(cursor<0)continue;
			// brutally queue the conflict cursors
			#pragma omp atomic
				locked[cursor]+=1;
			
			// filter for conflict cursor
			if(locked[cursor]>1)continue;
			// first in the 'atomic' queue can pass

			// main processing clock
			cursor_trail=minLinkedOut_target(linkoutInstance, cursor);
			int max_length=minLinkedOut_length(linkoutInstance, cursor);
			cursor = mergeSet(verticeSetInstance, cursor,cursor_trail);// cursor_trail-->>cursor
			int min_link=verticeSetInstance->unionMinLinkOut[cursor];
			int size=-verticeSetInstance->unionSet[cursor];
			if(max_length+C/size<min_link){
				printf("delete component:%d\n",cursor);
				deleteSet(verticeSetInstance,cursor);
				ifDelete=1;
			}
			#pragma omp atomic
				active_union-=ifDelete+1;

			// clear lock
			locked[cursor]&=0x0000;
		}
	}
	return 0;

}

