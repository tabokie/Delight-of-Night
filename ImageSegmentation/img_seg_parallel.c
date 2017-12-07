#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <math.h>
#include <time.h>
#include <assert.h>

#define MAX_SIZE					(1000)
#define Inf 						(1e8)
#define Error(note)					do{printf(#note);printf("\n");system("pause");exit(1);}while(0)
#define New(typeName,size)			(typeName*)malloc(sizeof(typeName)*(size))
#define Initial(arr,val,size)		do{int in_i_;for(in_i_=0;in_i_<size;in_i_++)arr[in_i_]=val;1;}while(0)
#define PutInts(arr,size)			do{printf(#arr);printf(": ");int in_i_;for(in_i_=0;in_i_<size;in_i_++)printf("%d ",arr[in_i_]);printf("\n");}while(0)
#define Log							printf("%s",__func__);printf("\n");
#define BP(note)                    do{printf(#note);printf(", In function %s",__func__);printf("\n");}while(0)
// #define For(i,a,b)                  for(i=(a);(((a)<=(b))?((b)-(i)):((i)-(b)))>0;i+=(((a)<=(b))?1:-1))
#define For(i,a,b)                  for(i=(a);i<(b);i++)
#define Min(a,b)					((a)>(b)?(b):(a))			
typedef enum{ false, true }bool;

int ROOT(int* union_set, int i){
	int ROOT_i_ = i; while (ROOT_i_ >= 0 && union_set[ROOT_i_] >= 0)ROOT_i_ = union_set[ROOT_i_]; return ROOT_i_;
}

// sub-structure
typedef struct LINKED_INT_t{
	int length;// sorting pivot
	int* target_addr;
	struct LINKED_INT_t* next;
}*lkd_int;

typedef struct VERTICE_SET_t{
	int v;
	// for vertice v
	int** vertice_addr;
	lkd_int* head;// serve as a min-heap:: find min edge connected to certain vertice
	int* unionSet;//::store union info
	int* deleted;//:: store deletion info
				 // for union u
	int* unionMinVertice;//::the vertice in u with a min edge linked to outside
	int* unionSetHead;//::union set as a linked list
}*v_set;

int GetRandom(int ceiling);
lkd_int newLinkedInt(int length, int* target_addr);
int insertLinkedInt(lkd_int* head_addr, lkd_int node);
v_set verticeSetInitial(int v);
int minUnionLinkedOut_target(v_set self, int root);
int minUnionLinkedOut_length(v_set self, int root);
int minUnionLinkedOut_length_with_deleted(v_set self, int root);
int clearMinLinkedOut(v_set self, int root);
int mergeUnion(v_set self, int r1, int r2);
int deleteUnion(v_set self, int root);
bool isRoot(v_set self, int vertice);
void putUnion(v_set self);
int verticeSetRead(v_set self, int a, int b, int l);


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

lkd_int newLinkedInt(int length, int* target_addr){
	lkd_int new = New(struct LINKED_INT_t, 1);
	new->target_addr = target_addr;
	new->length = length;
	new->next = NULL;
	return new;
}
// return remains of inserted linked list
int insertLinkedInt(lkd_int* head_addr, lkd_int node){
	node->next = NULL;
	if (head_addr == NULL)Error(NIL addr for a lkd_int to insert);
	if (*head_addr == NULL){
		// first node
		*head_addr = node;
		return 1;
	}
	lkd_int cur = *head_addr;
	int length = node->length;
	if (cur->length>length){
		// minimal in length
		*head_addr = node;
		node->next = cur;
		return 1;
	}
	while (cur->next != NULL){
		// iter find
		if (cur->length <= length&&cur->next->length>length){
			node->next = cur->next;
			cur->next = node;
			return 1;
		}
		cur = cur->next;
	}
	if (cur->length <= length){
		// last pivot
		cur->next = node;
		return 1;
	}
	else Error(Serching for pivot conflict!);
}

v_set verticeSetInitial(int v){
	v_set self = New(struct VERTICE_SET_t, 1);

	self->v = v;

	self->vertice_addr = New(int*, v);
	Initial(self->vertice_addr, NULL, v);
	self->head = New(lkd_int, v);
	Initial(self->head, NULL, v);

	self->unionSet = New(int, v);
	Initial(self->unionSet, -1, v);

	self->deleted = New(int, v);
	Initial(self->deleted, 0, v);

	self->unionMinVertice = New(int, v);
	int i;
	For(i, 0, v)self->unionMinVertice[i] = i;

	self->unionSetHead = New(int, v);
	For(i, 0, v)self->unionSetHead[i] = i;
	return self;
}
int verticeSetRead(v_set self, int a, int b, int l){
	
	if (a<0 || a >= self->v || b<0 || b >= self->v)Error(Illegal input to read!);
	int* a_addr; int* b_addr;
	if (self->vertice_addr[a] == NULL){
		a_addr = New(int, 1);
		self->vertice_addr[a] = a_addr;
		*a_addr = a;
	}
	else a_addr = self->vertice_addr[a];
	if (self->vertice_addr[b] == NULL){
		b_addr = New(int, 1);
		self->vertice_addr[b] = b_addr;
		*b_addr = b;
	}
	else b_addr = self->vertice_addr[b];

	lkd_int a_node = newLinkedInt(l, a_addr);
	lkd_int b_node = newLinkedInt(l, b_addr);

	insertLinkedInt(self->head + a, b_node);
	insertLinkedInt(self->head + b, a_node);

	return 1;
}
void putUnion(v_set self){
	int i, j;

	PutInts(self->unionSet, self->v);
	PutInts(self->deleted, self->v);
	PutInts(self->unionMinVertice, self->v);
	PutInts(self->unionSetHead, self->v);
	For(i, 0, self->v)printf("%d ", *(self->vertice_addr[i]));
	For(i, 0, self->v){
		if (self->head[i] == NULL)printf(" NULL");
		else printf(" %d", *(self->head[i]->target_addr));
	}
	printf("\n");

	int root;
	int v = self->v;
	int* have_visited = New(int, v);
	int top = -1;
	For(i, 0, v){
		root = ROOT(self->unionSet, i);
		For(j, 0, top + 1)if (root == have_visited[j])break;
		if (j <= top)continue;
		printf("root:%d;", root);
		//printf("[%d]",minUnionLinkedOut_length(self,root));
		have_visited[++top] = root;
		For(j, i, v){
			if (ROOT(self->unionSet, j) == root)printf("(%d)", j);
		}
		printf("\n");
	}
}


bool isRoot(v_set self, int vertice){
	
	int v = vertice;
	//printf("v:%d;union:%d;deleted:%d\n",v,self->unionSet[v],self->deleted[v]);
	// printf("v:%d,union:%d,deleted:%d\n",vertice,self->unionSet[vertice],self->deleted[vertice]);
	if (v >= 0 && v<self->v&&self->unionSet[v]<0 && !self->deleted[v])return true;
	else return false;
}

// delete all members in a union 'root'
int deleteUnion(v_set self, int root){
	
	// if not root or already deleted, exit with error
	if (root<0 || root >= self->v || !isRoot(self, root))Error(NIL root to delete!);

	// iter from head
	int cur = self->unionSetHead[root];
	while (cur >= 0){
		// delete in list
		self->deleted[cur] = 1;
		// delete linked out
		self->head[cur] = NULL;
		// cur =next
		cur = self->unionSet[cur];
	}

	return 1;
}

// receive union
int mergeUnion(v_set self, int r1, int r2){// concurrent conflict?
	
	if (!isRoot(self, r1) || !isRoot(self, r2))Error(Not root to merge!);
	if (r1 == r2)Error(Same root to merge!);
	// select min root
	if (self->unionSet[r1]>self->unionSet[r2]){// a is smaller
		int temp = r2;
		r2 = r1;
		r1 = temp;
	}
	// now set a is larger one
	self->unionSet[r1] += self->unionSet[r2];// size
	self->unionSet[r2] = self->unionSetHead[r1];// b's tail link to a's head
	self->unionSetHead[r1] = self->unionSetHead[r2];// union's head is b's head
													// change the vertice addr
	int cur = self->unionSetHead[r1];
	while (cur >= 0){
		*(self->vertice_addr[cur]) = r1;
		cur = self->unionSet[cur];
	}
	// merge link out
	lkd_int* head = self->head + r1;
	lkd_int cursor = self->head[r2];
	lkd_int next;
	while (cursor != NULL){
		next = cursor->next;
		insertLinkedInt(head, cursor);
		cursor = next;
	}

	return r1;
}

int clearMinLinkedOut(v_set self, int root){
	
	if (!isRoot(self, root))return 0;
	if (self->head[root] == NULL)return 1;

	lkd_int* meta_cursor = self->head + root;
	// check first node
	while ((*meta_cursor) != NULL){
		if (*((*meta_cursor)->target_addr) == root || self->deleted[*((*meta_cursor)->target_addr)]){
			// same union: delete
			*meta_cursor = (*meta_cursor)->next;
		}
		else return 1;
	}
	return 0;
}
int minUnionLinkedOut_length(v_set self, int root){
	
	if (!clearMinLinkedOut(self, root))return Inf;

	//assert(isRoot(self,root));
	lkd_int cur = self->head[root];
	if (cur == NULL)return Inf;
	return cur->length;
}
int minUnionLinkedOut_target(v_set self, int root){
	
	if (!clearMinLinkedOut(self, root))return -1;

	//assert(isRoot(self, root));
	lkd_int cur = self->head[root];
	if (cur == NULL)return -1;
	return *(cur->target_addr);
}

int main(void){

	srand(time(NULL));

	int THREAD_NUM = 4;
	omp_set_num_threads(THREAD_NUM);

	// initial data set
	int v, e, C;
	scanf("%d%d%d", &v, &e, &C);
	v_set verticeSetInstance = verticeSetInitial(v);
	int i;
	int a, b, l;
	For(i, 0, e){
		scanf("%d%d%d", &a, &b, &l);
		if (a<0 || a >= v || b<0 || b >= v)Error(No such vertice to link!);
		verticeSetRead(verticeSetInstance, a, b, l);
	}

	int active_union = v;
	// check special cases: single vertice
	For(i, 0, v){
		int min_link = minUnionLinkedOut_length(verticeSetInstance, i);
		int size = 1;
		if (C / size<min_link){
			deleteUnion(verticeSetInstance, i);
			active_union--;
		}
	}

	// conflict handler
	int* locked = New(int, v);
	Initial(locked, 0, v);

	// main parallel process
	#pragma omp parallel for 
	For(i, 0, THREAD_NUM){
		// local param
		int cursor;
		int cursor_trail;
		int ifDelete;
		while (active_union>1){// if still component not deleted
			// printf("active:%d\n", active_union);
			ifDelete = 0;
			putUnion(verticeSetInstance);
			// main search block
			do{
				cursor_trail = -1;
				do{
					cursor = GetRandom(v);
				} while (!isRoot(verticeSetInstance, cursor) && active_union > 1);
				int temp;
				while (active_union>1){
					temp = minUnionLinkedOut_length(verticeSetInstance, cursor);
					if (temp == cursor_trail){
						// select the smaller union
						cursor_trail = minUnionLinkedOut_target(verticeSetInstance, cursor);
						if (cursor_trail<cursor){
							temp = cursor;
							cursor = cursor_trail;
							cursor_trail = temp;
						}
						break;
					}
					else if (temp==Inf){
						cursor = -1;
						break;
					}
					else{
						cursor_trail = temp;
						cursor = minUnionLinkedOut_target(verticeSetInstance, cursor);
					}
				}
			} while (active_union>1 && (cursor<0 || cursor >= 0 && locked[cursor]));
			// assert(cursor<0||locked[cursor]==0);

			// conflict processing
			if (cursor<0 || active_union <= 1 || cursor == cursor_trail)continue;
			// brutally queue the conflict cursors
			#pragma omp atomic
			locked[cursor] += 1;

			// filter for conflict cursor
			if (locked[cursor]>1){
				continue;
			}
			// first in the 'atomic' queue can pass
			int cursor_history = cursor;

			// main processing clock
			int max_length = minUnionLinkedOut_length(verticeSetInstance, cursor);
			// printf("merge %d %d with edge %d\n",cursor,cursor_trail,max_length);
			cursor = mergeUnion(verticeSetInstance, cursor, cursor_trail);// cursor_trail-->>cursor
																		  //bug			
			int min_link = minUnionLinkedOut_length(verticeSetInstance, cursor);
			int size = -verticeSetInstance->unionSet[cursor];
			// printf("cursor:%d,max:%d,min_link:%d,size:%d\n",cursor,max_length,min_link,size);
			if (max_length + C / size <= min_link){
				deleteUnion(verticeSetInstance, cursor);
				ifDelete = 1;
			}
			#pragma omp atomic
			active_union -= ifDelete + 1;

			// clear lock
			locked[cursor_history] &= 0x0000;
		}
		// printf("Thread %d quit!<<<<<<<<<<<<<<\n", i);
	}
	putUnion(verticeSetInstance);
	// system("pause");
	return 0;

}

