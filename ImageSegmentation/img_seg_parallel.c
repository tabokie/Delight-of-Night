#include <stdio.h> 
#include <stdlib.h> /*  malloc(), free(), srand(), rand()  */
#include <omp.h> /*  omp parallel for, omp atomic  */
#include <time.h> /*  time()  */
#include <assert.h> /*  assert()  */
#include <windows.h>

#define MAX_SIZE					(1000)
#define Inf 						(1e8)
#define Error(note)					do{printf(#note);printf("\n");system("pause");exit(1);}while(0)
#define New(typeName,size)			(typeName*)malloc(sizeof(typeName)*(size))
#define Initial(arr,val,size)		do{int in_i_;for(in_i_=0;in_i_<size;in_i_++)arr[in_i_]=val;1;}while(0)
#define PutInts(arr,size)			do{printf(#arr);printf(": ");int in_i_;for(in_i_=0;in_i_<size;in_i_++)printf("%d ",arr[in_i_]);printf("\n");}while(0)
#define Log							printf("%s",__func__);printf("\n");
#define BP(note)                    do{printf(#note);printf(", In function %s",__func__);printf("\n");}while(0)
// assert(a<=b)
#define For(i,a,b)                  for(i=(a);i<(b);i++)
#define Min(a,b)					((a)>(b)?(b):(a))			
typedef enum{ false, true }bool;


#define isRoot(self,vertice)				( vertice >= 0 && vertice<self->v&&self->unionSet[vertice]<0)

typedef struct LINKED_INT_t{
	int length;// sorting pivot
	int* target_addr;
	struct LINKED_INT_t* next;
}*lkd_int;

typedef struct VERTICE_SET_t{
	int v;
	// addr as unique id of VERTICE
	int** vertice_addr;
	// sorted linked list
	//:: find minimum edge linked to a UNION
	lkd_int* head;
	// linear list as union set
	// :: find UNION id of a VERTICE, 
	// :: find size of a UNION, 
	// :: visit all members of a UNION
	int* unionSet;
	// head for visit in union set
	int* unionSetHead;
	// max edge of spanning tree in a UNION
	int* maxEdge;
	int* components;
}*v_set;

// linked int function
lkd_int newLinkedInt(int length, int* target_addr);
int insertLinkedInt(lkd_int* head_addr, lkd_int node);

// vertice set function
// apply for space
v_set verticeSetInitial(int v);
// read and setup
int verticeSetRead(v_set self, int a, int b, int l);
// find minimum edge's target UNION of a UNION
int minUnionLinkedOut_target(v_set self, int root);
// find minimum edge's length of a UNION
int minUnionLinkedOut_length(v_set self, int root);
// maintenance of lkd_int* head
int clearMinLinkedOut(v_set self, int root);
// merge two UNIONs
int mergeUnion(v_set self, int r1, int r2, int length);
bool isComponent(v_set self, int root);
int updateComponent(v_set self, int root);
// print UNION info
void putUnion(v_set self);

// additional function
int GetRandom(int ceiling);
int C;
int* locked;

int main(void){

	srand(time(NULL));

	int THREAD_NUM = 4;
	omp_set_num_threads(THREAD_NUM);
	// initial data set
	int v, e;
	scanf("%d%d%d", &v, &e, &C);
	v_set verticeSetInstance = verticeSetInitial(v);
	int i;
	int a, b, l;
	For(i, 0, e){
		scanf("%d%d%d", &a, &b, &l);
		if (a < 0 || a >= v || b < 0 || b >= v)Error(No such vertice to link!);
		verticeSetRead(verticeSetInstance, a, b, l);
	}
		int active_union = v;
		// conflict handler
	locked = New(int, v);
	Initial(locked, 0, v);
		// main parallel process
	#pragma omp parallel for 
	For(i, 0, THREAD_NUM){
		// local param
		int next_cursor;
		int cursor;
		int edge;
		int num_cut;
		while (active_union > 1){// if still component not deleted
			num_cut = 0;
			// main search block
			do{
				edge = -1;
				cursor = *(verticeSetInstance->vertice_addr[GetRandom(v)]);
				//Sleep(GetRandom(1));
				int temp;
				while (active_union > 1){
					//BP(0);
					if (cursor >= 0 && cursor < verticeSetInstance->v && !locked[cursor] && isRoot(verticeSetInstance, cursor)){
						temp = minUnionLinkedOut_length(verticeSetInstance, cursor);
						if (temp == edge){
							// select the smaller union
							next_cursor = minUnionLinkedOut_target(verticeSetInstance, cursor);
							if (next_cursor < cursor){
								temp = cursor;
								cursor = next_cursor;
								next_cursor = temp;
							}
							break;
						}
						else if (temp == Inf){
							cursor = -1;
							break;
						}
						else{
							edge = temp;
							cursor = minUnionLinkedOut_target(verticeSetInstance, cursor);
						}
					}
					else{
						cursor = -1;
						break;
					}
				}
			} while (active_union > 1 && (cursor < 0 || cursor >= 0 && locked[cursor]));
			// conflict processing
			if (cursor < 0 || active_union <= 1 || cursor == next_cursor)continue;
			// brutally queue the conflict cursors
			#pragma omp atomic
			locked[cursor] += 1;
			// filter for conflict cursor
			if (locked[cursor] > 1){
				continue;
			}
			// first in the 'atomic' queue can pass
			int locked_cursor = cursor;
			//#pragma omp critical
			if (verticeSetInstance->maxEdge[cursor] + C / (-verticeSetInstance->unionSet[cursor]) >= edge\
				&&verticeSetInstance->maxEdge[next_cursor] + C / (-verticeSetInstance->unionSet[next_cursor]) >= edge){
				cursor = mergeUnion(verticeSetInstance, cursor, next_cursor, edge);
				num_cut = updateComponent(verticeSetInstance, cursor);
			}
			#pragma omp atomic
			active_union -= num_cut;
			//#pragma omp atomic
			// unlock
			locked[locked_cursor] &= 0x0000;
		}
	}
	putUnion(verticeSetInstance);


	//system("pause");
	return 0;

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

lkd_int newLinkedInt(int length, int* target_addr){
	lkd_int new = New(struct LINKED_INT_t, 1);
	new->target_addr = target_addr;
	new->length = length;
	new->next = NULL;
	return new;
}
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

	self->maxEdge = New(int, v);
	Initial(self->maxEdge, 0, v);
	int i;
	self->unionSetHead = New(int, v);
	For(i, 0, v)self->unionSetHead[i] = i;
	self->components = New(int, v);
	Initial(self->components, 0, v);
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
	int root;
	int v = self->v;
	int* have_visited = New(int, v);
	int top = -1;
	For(i, 0, v){
		root = *(self->vertice_addr[i]);
		For(j, 0, top + 1)if (root == have_visited[j])break;
		if (j <= top)continue;
		have_visited[++top] = root;
		printf("%d",root);
		For(j, i+1, v){
			if (*(self->vertice_addr[j]) == root)printf(" %d", j);
		}
		printf("\n");
	}
}

// receive union
int mergeUnion(v_set self, int r1, int r2, int edge){

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
													// change the vertice id
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
	clearMinLinkedOut(self, r1);
	// update max edge
	self->maxEdge[r1] = edge;
	return r1;
}

int clearMinLinkedOut(v_set self, int root){

	if (!isRoot(self, root))return 0;
	if (self->head[root] == NULL)return 1;

	lkd_int* meta_cursor = self->head + root;
	// check first node
	while ((*meta_cursor) != NULL){
		if (*((*meta_cursor)->target_addr) == root){
			// same union: delete
			*meta_cursor = (*meta_cursor)->next;
		}
		else break;
	}
	lkd_int cur = self->head[root];
	if (cur == NULL)return 1;
	int* isLinked = New(int, self->v);
	Initial(isLinked, 0, self->v);
	isLinked[root] = 1;
	while (cur->next != NULL){
		if (isLinked[*(cur->next->target_addr)]){
			cur->next = cur->next->next;
			continue;
		}
		isLinked[*(cur->next->target_addr)] = 1;
		cur = cur->next;
	}
	free(isLinked);
	return 1;
}
bool isComponent(v_set self, int root){
	if (!isRoot(self, root))Error(NIL root!);
	if (self->components[root])return true;
	lkd_int cur = self->head[root];
	int neighbor;
	int length;
	while (cur != NULL){
		neighbor = *(cur->target_addr);
		length = cur->length;
		if (self->maxEdge[root] + C / (-self->unionSet[root]) >= length
			&&self->maxEdge[neighbor] + C / (-self->unionSet[neighbor]) >= length){
			return false;
		}
		cur = cur->next;
	}
	return true;
}
int updateComponent(v_set self, int root){
	if (!isRoot(self, root))Error(NIL root!);
	int ret = 0;
	// if already counted
	if (self->components[root])return 0;

	if (!isComponent(self, root)){
		return 0;
	}
	self->components[root] = 1;
	lkd_int cur = self->head[root];
	int neighbor;
	while (cur != NULL){
		neighbor = *(cur->target_addr);
		if (locked[neighbor]){
			cur = cur->next;
			continue;
		}
		if (!self->components[neighbor] && isComponent(self, neighbor)){
			self->components[neighbor] = 1;
			ret += -self->unionSet[neighbor];
		}
		cur = cur->next;
	}
	// is component
	ret += -self->unionSet[root];
	return ret;
}
int minUnionLinkedOut_length(v_set self, int root){
	if (locked[root])return Inf;
	lkd_int cur = self->head[root];
	if (cur == NULL)return Inf;
	while (cur != NULL&&self->components[*(cur->target_addr)])cur = cur->next;
	if (cur == NULL)return Inf;
	return cur->length;
}
int minUnionLinkedOut_target(v_set self, int root){
	if (locked[root])return -1;
	lkd_int cur = self->head[root];
	if (cur == NULL)return -1;
	while (cur != NULL&&self->components[*(cur->target_addr)])cur = cur->next;
	if (cur == NULL)return Inf;
	return *(cur->target_addr);
}
