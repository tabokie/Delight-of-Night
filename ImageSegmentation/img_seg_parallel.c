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

// merge link_out and vertice_set
// optimize get_cursor(get random)
// optimize deletion
// api
// putUnion
// isRoot
// minUnionLinkedOut_length
// minUnionLinkedOut_target
// mergeSet
// deleteSet


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

// vertice set class:

// sub-structure
typedef struct LINKED_INT_t{
	int length;// sorting pivot
	int target;
	struct LINKED_INT_t* next;
}* lkd_int;
lkd_int newLinkedInt(int length, int target){
	lkd_int new=New(struct LINKED_INT_t,1);
	new->target=target;
	new->length=length;
	new->next=NULL;
	return new;
}
int insertLinkedInt(lkd_int* head_addr, int a, int b){
	// Log();
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
		return 1;
	}
	while(cur->next!=NULL){
		if(cur->length<=a&&cur->next->length>a){
			new->next=cur->next;
			cur->next=new;
			return 1;
		}
	}
	if(cur->length<=a){
		cur->next=new;
		return 1;
	}
	else return 0;
}

typedef struct VERTICE_SET_t{
	int v;
	// for vertice v
	lkd_int* head;// serve as a min-heap:: find min edge connected to certain vertice
	int* unionSet;//::store union info
	int* deleted;//:: store deletion info
	// for union u
	int* unionMinVertice;//::the vertice in u with a min edge linked to outside
	int* unionSetHead;//::union set as a linked list
}* v_set;
#define ROOT(union_set, i)		({int ROOT_i_=i;while(ROOT_i_>=0&&union_set[ROOT_i_]>=0)ROOT_i_=union_set[ROOT_i_];ROOT_i_;})
v_set verticeSetInitial(int v){
	// Log();
	v_set self=New(struct VERTICE_SET_t, 1);

	self->v=v;

	self->head=New(lkd_int,v);
	Initial(self->head,NULL,v);

	self->unionSet=New(int,v);
	Initial(self->unionSet,-1,v);

	self->deleted=New(int,v);
	Initial(self->deleted,0,v);

	self->unionMinVertice=New(int,v);
	int i;
	For(i,0,v)self->unionMinVertice[i]=i;

	self->unionSetHead=New(int,v);
	For(i,0,v)self->unionSetHead[i]=i;
	return self;
}
int verticeSetRead(v_set self,int a, int b, int l){
	// Log();
	if(a<0||a>=self->v||b<0||b>=self->v)return 0;
	insertLinkedInt(self->head+a,l,b);
	insertLinkedInt(self->head+b,l,a);
	return 1;
}
int deleteUnion(v_set self, int root){
	if(root<0||root>=self->v)return 1;
	int cur=self->unionSetHead[root];
	while(cur>=0){
		self->deleted[cur]=1;
		self->head[cur]=NULL;
		cur=self->unionSet[cur];
	}
	return 1;
}

int mergeUnion(v_set self, int v1, int v2){
	Log();
	// already root
	int root_a=ROOT(self->unionSet,v1);
	int root_b=ROOT(self->unionSet,v2);
	int vertice_a=minUnionLinkedOut_target(self,v2);
	int vertice_b=minUnionLinkedOut_target(self, v1);
	self->head[vertice_a]=self->head[vertice_a]->next;
	self->head[vertice_b]=self->head[vertice_b]->next;
	if(self->unionSet[root_a]>self->unionSet[root_b]){// a is smaller
		int temp=root_b;
		root_b=root_a;
		root_a=temp;
	}
	// now set a is larger one
	self->unionSet[root_a]+=self->unionSet[root_b];// size
	self->unionSet[root_b]=self->unionSetHead[root_a];// b's tail link to a's head
	self->unionSetHead[root_a]=self->unionSetHead[root_b];// union's head is b's head
	//printf("unionset a(%d):%d,b(%d):%d\n",root_a,self->unionSet[root_a],root_b,self->unionSet[root_b]);
	self->unionMinVertice[root_a]=minUnionLinkedOut(self,root_a);
	return root_a;
}

int minLinkedOut_length(v_set self,int vertice){
	// Log();
	if(vertice<0||vertice>=self->v)return Inf;
	if(self->head[vertice]==NULL)return Inf;
	lkd_int* head =self->head+vertice;
	while(*head!=NULL){
		if(!self->deleted[(*head)->target]){
			return (*head)->length;
		}
		*head=(*head)->next;
	}
	return Inf;
}
int minLinkedOut_target(v_set self,int vertice){
	// Log();
	if(vertice<0||vertice>=self->v)return -1;// illegal vertice input
	if(self->head[vertice]==NULL)return -1;// no link out
	lkd_int* head =self->head+vertice;
	while(*head!=NULL){
		if(!self->deleted[(*head)->target]){
			break;
		}
		*head=(*head)->next;
	}
	if(*head==NULL)return -1;
	return (*head)->target;
}

int minUnionLinkedOut(v_set self, int root){
	Log();
	// if(!isRoot(root));
	int cur;
	int min_length=Inf;
	int min_vertice=-1;
	int temp;
	int v=self->v;

	cur=self->unionSetHead[root];
	while(cur>=0){
		//printf("<cur:%d>",cur);
		if(!self->deleted[cur]){
			temp=minUnionLinkedOut_length(self,cur);
			if(temp<min_length){
				min_vertice=cur;
				min_length=temp;
			}
		}
		cur=self->unionSet[cur];
	}
	return min_vertice;
}

int minUnionLinkedOut_length(v_set self, int root){
	Log();
	if(root<0||root>=self->v)return Inf;
	return minLinkedOut_length(self,self->unionMinVertice[root]);
}

int minUnionLinkedOut_target(v_set self, int root){
	Log();
	if(root<0||root>=self->v)return -1;
	return ROOT(self->unionSet,minLinkedOut_target(self, self->unionMinVertice[root]));
}

void putUnion(v_set self){
	int i,j;
	int root;
	int v=self->v;
	int* have_visited=New(int,v);
	int top=-1;
	For(i,0,v){
		root=ROOT(self->unionSet, i);
		For(j,0,top+1)if(root==have_visited[j])break;
		if(j<=top)continue;
		printf("root:%d;",root);
		have_visited[++top]=root;
		For(j,i,v){
			if(ROOT(self->unionSet,j)==root)printf("(%d)",j);
		}
		printf("\n");
	}
}

int isRoot(v_set self, int vertice){
	if(ROOT(self->unionSet,vertice)==vertice)return true;
	else return false;
}

int main(void){

	srand(time(NULL));

	/*
	int THREAD_NUM = omp_get_num_procs();
	printf("Reset thread num?\n");
	int reply;
	scanf("%d",&reply);
	if(reply){
		printf("Enter thread num:\n");
		scanf("%d",&THREAD_NUM);
	}
	*/
	int THREAD_NUM=1;
	omp_set_num_threads(THREAD_NUM);
	
	int v,e,C;
	scanf("%d%d%d",&v,&e,&C);
	v_set verticeSetInstance = verticeSetInitial(v);
	int i;
	int a,b,l;
	For(i,0,e){
		scanf("%d%d%d",&a,&b,&l);
		if(a<0||a>=v||b<0||b>=v)Error(No such vertice to link!);
		verticeSetRead(verticeSetInstance,a,b,l);
	}

	int* locked=New(int,v);
	Initial(locked,0,v);
	int active_union=v;
	// main parallel process
	#pragma omp parallel for 
	For(i,0,THREAD_NUM){
		int cursor;
		int cursor_trail=-1;
		int ifDelete=0;
		while(active_union>0){// if still component not deleted
			putUnion(verticeSetInstance);
			// main search block
			// printf("Thread %d, new run\n",i);

			do{
				cursor_trail=-1;
				cursor=GetRandom(v);
				if(!isRoot(verticeSetInstance, cursor)){
					cursor=-1;
					continue;
				}
				printf("cursor:%d",cursor);
				int temp;
				while(true){
					temp=minUnionLinkedOut_length(verticeSetInstance,cursor);
					printf(" prior[%d],now[%d]",cursor_trail,temp);
					if(temp==cursor_trail)break;
					else if(temp<0){
						cursor=-1;
						break;
					}
					else{
						cursor_trail=temp;
						cursor=minUnionLinkedOut_target(verticeSetInstance,cursor);
					}
					printf(" cursor:%d",cursor);
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
			int cursor_history=cursor;

			// main processing clock
			cursor_trail=minUnionLinkedOut_target(verticeSetInstance, cursor);
			int max_length=minUnionLinkedOut_length(verticeSetInstance, cursor);
			cursor = mergeUnion(verticeSetInstance, cursor,cursor_trail);// cursor_trail-->>cursor
			int min_link=minUnionLinkedOut_length(verticeSetInstance,cursor);
			int size=-verticeSetInstance->unionSet[cursor];
			printf("min_link:%d,max_length:%d,size:%d\n",min_link,max_length,size);
			if(max_length+C/size<min_link){
				printf("delete component:%d\n",cursor);
				deleteUnion(verticeSetInstance,cursor);
				ifDelete=1;
			}
			#pragma omp atomic
				active_union-=ifDelete+1;

			// clear lock
			locked[cursor_history]&=0x0000;
		}
	}
	return 0;

}

