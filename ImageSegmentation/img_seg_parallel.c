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
#define Initial(arr,val,size)		({int in_i_;for(in_i_=0;in_i_<size;in_i_++)arr[in_i_]=val;1;})
#define PutInts(arr,size)			({printf(#arr);printf(": ");int in_i_;for(in_i_=0;in_i_<size;in_i_++)printf("%d ",arr[in_i_]);printf("\n");})
#define Log()						({printf("%s",__func__);printf("\n");})
#define BP(note)                    ({printf(#note);printf(", In function %s",__func__);printf("\n");})
// #define For(i,a,b)                  for(i=(a);(((a)<=(b))?((b)-(i)):((i)-(b)))>0;i+=(((a)<=(b))?1:-1))
#define For(i,a,b)                  for(i=(a);i<(b);i++)
#define Min(a,b)					((a)>(b)?(b):(a))			
typedef enum{false,true}bool;

// bug:
// mergeUnion: not only one edge linked to delete
// to check : differ in three state :: other union / my union / deleted union (global mark)
// >>new data structrue for storing link out
// EDGE{
//	int length
//	int* target_addr
//}
// VERTICE{
//	 int* id_addr
// }
// merge == modify id 

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
	if(head_addr==NULL)Error(NIL addr for a lkd_int to insert);
	lkd_int new=newLinkedInt(a,b);
	if(*head_addr==NULL){
		// first node
		*head_addr=new;
		return 1;
	}
	lkd_int cur=*head_addr;
	if(cur->length>a){
		// minimal in length
		*head_addr=new;
		new->next=cur;
		return 1;
	}
	while(cur->next!=NULL){
		// iter find
		if(cur->length<=a&&cur->next->length>a){
			new->next=cur->next;
			cur->next=new;
			return 1;
		}
		cur=cur->next;
	}
	if(cur->length<=a){
		// last pivot
		cur->next=new;
		return 1;
	}
	else Error(Serching for pivot conflict!);
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
	if(a<0||a>=self->v||b<0||b>=self->v)Error(Illegal input to read!);
	insertLinkedInt(self->head+a,l,b);
	insertLinkedInt(self->head+b,l,a);
	return 1;
}
void putUnion(v_set self){
	int i,j;

	PutInts(self->unionSet,self->v);
	PutInts(self->deleted,self->v);
	PutInts(self->unionMinVertice,self->v);
	PutInts(self->unionSetHead,self->v);
	For(i,0,self->v){
		if(self->head[i]==NULL)printf(" NULL");
		else printf(" %d",self->head[i]->target);
	}
	printf("\n");

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
	printf("v:%d\n",vertice);
	// printf("v:%d,union:%d,deleted:%d\n",vertice,self->unionSet[vertice],self->deleted[vertice]);
	if(vertice>=0&&vertice<self->v&&self->unionSet[vertice]<0&&!self->deleted[vertice])return true;
	else return false;
}

// delete all members in a union 'root'
int deleteUnion(v_set self, int root){
	Log();
	// if not root or already deleted, exit with error
	printf("root:%d\n",root);
	if(root<0||root>=self->v||!isRoot(self,root))Error(NIL root to delete!);
	// iter from head
	int cur=self->unionSetHead[root];
	while(cur>=0){
		// delete in list
		self->deleted[cur]=1;
		// delete linked out
		self->head[cur]=NULL;
		// cur =next
		cur=self->unionSet[cur];
	}
	return 1;
}

// receive union
int mergeUnion(v_set self, int r1, int r2){// concurrent conflict?
	Log();
	if(!isRoot(self,r1)||!isRoot(self,r2))Error(Not root to merge!);
	if(r1==r2)Error(Same root to merge!);
	// look up link point
	int vertice_a=self->unionMinVertice[r1];
	int vertice_b=self->unionMinVertice[r2];
	printf("root:%d %d,vertice:%d %d.\n",r1,r2,vertice_a,vertice_b);
	// delete one min link out
	self->head[vertice_a]=self->head[vertice_a]->next;
	self->head[vertice_b]=self->head[vertice_b]->next;
	// select min root
	if(self->unionSet[r1]>self->unionSet[r2]){// a is smaller
		int temp=r2;
		r2=r1;
		r1=temp;
	}
	// now set a is larger one
	self->unionSet[r1]+=self->unionSet[r2];// size
	self->unionSet[r2]=self->unionSetHead[r1];// b's tail link to a's head
	self->unionSetHead[r1]=self->unionSetHead[r2];// union's head is b's head
	// restore min linked
	self->unionMinVertice[r1]=minUnionLinkedOut(self,r1);
	printf("root:%d\n",r1);
	return r1;
}

int minLinkedOut_length(v_set self,int vertice){
	Log();
	if(vertice<0||vertice>=self->v)Error(Illegal vertice to find min length!);
	// no link out
	if(self->head[vertice]==NULL)return Inf;
	// search for min link out
	lkd_int* head = self->head+vertice;
	while(*head!=NULL){
		if(!self->deleted[(*head)->target]){
			return (*head)->length;
		}
		*head=(*head)->next;
	}
	return Inf;
}
// return vertice
int minLinkedOut_target(v_set self,int vertice){
	Log();
	if(vertice<0||vertice>=self->v)Error(Illegal vertice to find min target!);
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
	if(!isRoot(self,root))Error(Not union to find min out!);
	int cur;
	int min_length=Inf;
	int min_vertice=-1;
	int temp;
	int v=self->v;

	cur=self->unionSetHead[root];
	while(cur>=0){
		//printf("<cur:%d>",cur);
		if(!self->deleted[cur]){
			temp=minLinkedOut_length(self,cur);
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
	putUnion(self);
	if(root<0||root>=self->v)Error(Illegal root to find min length!);
	// no link
	if(self->unionMinVertice[root]<0)return Inf;
	if(!self->deleted[minLinkedOut_target(self,self->unionMinVertice[root])])
		return minLinkedOut_length(self,self->unionMinVertice[root]);
	else{
		self->unionMinVertice[root]=minUnionLinkedOut(self,root);
		return minLinkedOut_length(self,self->unionMinVertice[root]);
	}
}

// return root
int minUnionLinkedOut_target(v_set self, int root){
	Log();
	if(root<0||root>=self->v)return -1;
	if(!self->deleted[minLinkedOut_target(self,self->unionMinVertice[root])])
		return ROOT(self->unionSet,minLinkedOut_target(self, self->unionMinVertice[root]));
	else{
		self->unionMinVertice[root]=minUnionLinkedOut(self,root);
		return ROOT(self->unionSet,minLinkedOut_target(self, self->unionMinVertice[root]));
	}
}



int main(void){

	srand(time(NULL));

	int THREAD_NUM=1;
	omp_set_num_threads(THREAD_NUM);
	
	// initial data set
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

	// conflict handler
	int* locked=New(int,v);
	Initial(locked,0,v);
	int active_union=v;
	// main parallel process
	#pragma omp parallel for 
	For(i,0,THREAD_NUM){
		// local param
		int cursor;
		int cursor_trail;
		int ifDelete;
		while(active_union>0){// if still component not deleted
			ifDelete=0;
			putUnion(verticeSetInstance);
			// main search block
			do{
				cursor_trail=-1;
				while(!isRoot(verticeSetInstance, cursor)&&active_union>0){
					cursor=GetRandom(v);
				}
				int temp;
				while(active_union>0){
					temp=minUnionLinkedOut_length(verticeSetInstance,cursor);
					if(temp==cursor_trail){
						// select the smaller union
						cursor_trail=minUnionLinkedOut_target(verticeSetInstance,cursor);
						if(cursor_trail<cursor){
							temp=cursor;
							cursor=cursor_trail;
							cursor_trail=temp;
						}
						break;
					}
					else if(temp<0){
						cursor=-1;
						break;
					}
					else{
						cursor_trail=temp;
						cursor=minUnionLinkedOut_target(verticeSetInstance,cursor);
					}
					printf("[cursor:%d,trial:%d]\n",cursor,cursor_trail);
				}
			}
			while(active_union>0&&(cursor<0||cursor>=0&&locked[cursor]));
			// assert(cursor<0||locked[cursor]==0);

			// conflict processing
			if(cursor<0||active_union<=0||cursor==cursor_trail)continue;
			// brutally queue the conflict cursors
			#pragma omp atomic
				locked[cursor]+=1;
			
			// filter for conflict cursor
			if(locked[cursor]>1)continue;
			// first in the 'atomic' queue can pass
			int cursor_history=cursor;

			// main processing clock
			int max_length=minUnionLinkedOut_length(verticeSetInstance, cursor);
			printf("merge %d %d with edge %d\n",cursor,cursor_trail,max_length);
			cursor = mergeUnion(verticeSetInstance, cursor,cursor_trail);// cursor_trail-->>cursor
			// int min_link=minUnionLinkedOut_length_restored(verticeSetInstance,cursor);
			int min_link=minUnionLinkedOut_length(verticeSetInstance,cursor);
			int size=-verticeSetInstance->unionSet[cursor];
			printf("cursor:%d,max:%d,min_link:%d,size:%d\n",cursor,max_length,min_link,size);
			if(max_length+C/size<min_link){
				BP(0);
				deleteUnion(verticeSetInstance,cursor);
				ifDelete=1;
			}
			#pragma omp atomic
				active_union-=ifDelete+1;

			// clear lock
			locked[cursor_history]&=0x0000;
		}
	}
	putUnion(verticeSetInstance);
	return 0;

}

