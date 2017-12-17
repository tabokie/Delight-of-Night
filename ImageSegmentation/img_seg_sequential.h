#include <stdio.h>
#include <stdlib.h> /*  malloc(), free()  */
#include <assert.h> /*  assert()  */

#ifndef IMG_SEG_H_
#define IMG_SEG_H_

/**  Basic Macro  **/

// Infinity
#ifndef Inf
#define Inf                             (1e8)
#endif
// Call for memory block
#ifndef New
#define New(typeName,size)              (typeName*)malloc(sizeof(typeName)*(size))
#endif
// Initial memory block
#ifndef Initial
#define Initial(arr,val,size)           do{int initial_i_;for(initial_i_=0;initial_i_<size;initial_i_++)arr[initial_i_]=val;}while(0)
#endif
// Return minimum elememt of two
#ifndef Min
#define Min(a,b)                        ((a)>(b)?(b):(a))       
#endif
// Return maximum elememt of two
#ifndef Max
#define Max(a,b)                        ((a)<(b)?(b):(a))
#endif


/**  Main Data Structure  **/
typedef struct SEGMENTOR_t{
    int v,e,C;

    int* unionSet;
    int* maxEdge;
    
    int* edgeFrom;
    int* edgeTo;
    int* edgeLength;
}* segmentor;

/**  Top-level Function Class  **/

// \brief: Initial a segmentor with {v,e,C}
segmentor imgSeg_initial(int v, int e, int C);

// \brief: Pass edge information to a existing segmentor
// \param: The edge's UUID
// \param: instance Segmentor
// \param: a,b Vertices
// \param: l Length
int imgSeg_read(segmentor instance, int id, int a, int b, int l);

// \brief: Execute image segment routine
int imgSeg_execute(segmentor instance);

// \brief: Print the components segmented by instance
int imgSeg_putres(segmentor instance);

// \brief: terminate the segmentor and release applied space
int imgSeg_terminate(segmentor instance);


/**  Second-level Function Class  **/

// Check if legal vertice id in graph
#ifndef LEGAL
#define LEGAL(v,vertice)                  (vertice>=0&&vertice<v)                 
#endif

// Return root of a given vertice in a graph
int ROOT(segmentor instance, int vertice);

// \brief: merge two components
// \param: instance Segmentor
// \param: a,b Union ids
// \param: max_length Minimum edge length connecting a and b
void mergeUnion(segmentor instance, int a, int b, int length);


/**  Basic-level Function Class  **/

// \brief: Dual Pivot Quick sort (without randomized pivot selection)
// \param: data Pointer to 2-dimension array
// \param: n Row number
// \param: init Start pivot
// \param: size Checking scope
void CombinedSort(int** data, int n, int init, int size);

// \brief Exchange on 2-dimension data set
// \param: data Pointer to 2-dimension array
// \param: n Row number
// \param: a,b Index to exchange
void CombinedExchange(int** data, int n, int a, int b);

#endif /*  IMG_SEG_H_  */