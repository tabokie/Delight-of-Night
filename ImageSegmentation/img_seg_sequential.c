#include <stdio.h>
#include <stdlib.h> /*  malloc(), free()  */
#include <assert.h> /*  assert()  */
#include "img_seg.h"

/**  Top-level Function Class  **/

segmentor imgSeg_initial(int v, int e, int C){
    // call for memory
    segmentor new_instance = New(struct SEGMENTOR_t, 1);

    // basic info
    new_instance->v = v;
    new_instance->e = e;
    new_instance->C = C;

    // segmentor's storing components
    new_instance->unionSet = New(int, v);
    Initial(new_instance->unionSet, -1, v);// all vertices are single component with size 1
    new_instance->maxEdge = New(int, v);
    Initial(new_instance->maxEdge, 0, v);// all vertice has no edge in there spanning tree
    // list for edges
    new_instance->edgeFrom = New(int, e);
    new_instance->edgeTo = New(int, e);
    new_instance->edgeLength = New(int, e);

    return new_instance;
}

int imgSeg_read(segmentor instance, int id, int a, int b, int l){
    instance->edgeLength[id] = l;
    // store the smaller vertice in From[]
    instance->edgeFrom[id] = Min(a,b);
    // and the larger in To[]
    instance->edgeTo[id] = Max(a,b);

    return 1;
}

int imgSeg_execute(segmentor instance){
    // set up combined data set
    int* sort_data[3] = { instance->edgeLength, instance->edgeFrom, instance->edgeTo };
    // sort by edge length
    CombinedSort(sort_data, 3, 0,instance->e);

    int i;
    int from, to, length;
    for (i = 0; i<instance->e; i++){// visit edges by length
        // load info about current edge
        from = instance->edgeFrom[i];
        to = instance->edgeTo[i];
        length = instance->edgeLength[i];
        // get two vertice's union
        from = ROOT(instance, from);
        to = ROOT(instance, to);
        // IF either of u1, u2 satisfies condition for a component
        // pass
        if (instance->maxEdge[from] + instance->C / (-(instance->unionSet[from]))<length\
            || instance->maxEdge[to] + instance->C / (-instance->unionSet[to])<length)continue;
        // ELSE
        // merge u1 and u2
        mergeUnion(instance, from, to, length);

    }
    return 1;
}

int imgSeg_putres(segmentor instance){
    int i,j;
    int root;
    int size;
    for (i = 0; i<instance->v; i++){
        // fetch this element's union id
        root = ROOT(instance, i);
        // IF already printed
        // pass
        if (instance->unionSet[root] == -Inf)continue;
        size = -instance->unionSet[root];
        // print first element without space
        printf("%d", i);
        // scan the rest vertices looking for members
        for (j = i + 1; j < instance->v && size > 0; j++){
            if (ROOT(instance, j) == root){
                printf(" %d", j);
                // search until every member in this union is visited
                size--;
            }
        }
        // end mark
        printf("\n");
        // mark the printed union
        instance->unionSet[root] = -Inf;
    }
    return 1;
}

int imgSeg_terminate(segmentor instance){
    // release all the applied spacez
    free(instance->unionSet);
    free(instance->maxEdge);
    free(instance->edgeLength);
    free(instance->edgeTo);
    free(instance->edgeFrom);
    free(instance);
    return 1;
}

/**  Second-level Function Class  **/

int ROOT(segmentor instance, int vertice){
    // set up needed instance members
    int cur = vertice;
    int* union_set = instance->unionSet;
    int v = instance->v;
    // while link holds
    while(LEGAL(v,cur)&&union_set[cur]>=0)
        cur=union_set[cur];
    assert(LEGAL(v,cur));
    return cur;
}

void mergeUnion(segmentor instance, int a, int b, int length){
    // set up needed instance members
    int root = ROOT(instance, a), son = ROOT(instance, b), size;
    int* union_set = instance->unionSet;
    int* max_edge = instance->maxEdge;

    // select the larger union as main union
    if (union_set[root]>union_set[son]){
        int temp = root;
        root = son;
        son = temp;
    }
    size = union_set[root] + union_set[son];
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

/**  Basic-level Function Class  **/

void CombinedSort(int** data, int n, int init, int size){
    // select key row
    int* key = data[0];
    // if more than 2 elements
    if (size > 2){
        // select pivot
        int pivot1 = 0, pivot2 = size - 1;
        // normalize pivot
        if (key[init + pivot1] > key[init + pivot2]){
            CombinedExchange(data, n, init + pivot1, init + pivot2);
        }
        // store pivot value
        int value1 = key[init + pivot1], value2 = key[init + pivot2];
        // iterative boundary
        int lowidx = pivot1 + 1, highidx = pivot2 - 1;
        // main loop
        while (lowidx <= highidx){
            // if left bound is smaller than pivot 1
            // exchange to left area
            if (key[init + lowidx] < value1){
                CombinedExchange(data, n, init + lowidx, init + pivot1 + 1);
                CombinedExchange(data, n, init + pivot1, init + pivot1 + 1);
                lowidx++;
                pivot1++;
            }
            // if left bound is between pivot 1 and pivot 2
            // pass
            else if (key[init + lowidx] >= value1&&key[init + lowidx] <= value2){
                lowidx++;
            }
            // if left bound is bigger than pivot 2
            // exchange to right area
            else{
                CombinedExchange(data, n, init + lowidx, init + highidx);
                if (highidx < pivot2 - 1){
                    CombinedExchange(data, n, init + highidx, init + pivot2 - 1);
                }
                CombinedExchange(data, n, init + highidx, init + pivot2);
                pivot2--;
                highidx--;
            }
        }
        // load recursion info
        int i;
        int initInfo[3] = { init + 0,init + pivot1 + 1,init + pivot2 + 1 };
        int sizeInfo[3] = { pivot1,pivot2 - pivot1 - 1,size - pivot2 - 1 };
        // recursion begin
        for (i = 0; i<3; i++)CombinedSort(data, n, initInfo[i], sizeInfo[i]);
    }
    // if only two element
    else if (size == 2){
        // if not sorted
        if (key[init + 0] > key[init + 1]){
            CombinedExchange(data, n, init + 0, init + 1);
        }
    }
    // if only one element
    // pass
}

void CombinedExchange(int** data, int n, int a, int b){
    assert(data != NULL);
    int temp, i;
    // for each row:
    for (i = 0; i<n; i++){
        assert(data[i] != NULL);
        // exchange a and b in i-th row
        temp = data[i][a];
        data[i][a] = data[i][b];
        data[i][b] = temp;
    }
    return;
}
