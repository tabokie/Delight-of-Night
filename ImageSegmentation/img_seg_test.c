#include <stdio.h>
#include <time.h> /*  CLOCK_MONOTONIC, struct timespec, clock_gettime()  */
#include "img_seg.h"

/*  MACRO for timing  */
#define CLOCK_INIT                      struct timespec start,end;
#define CLOCK_START                     clock_gettime(CLOCK_MONOTONIC,&start);
#define CLOCK_END                       clock_gettime(CLOCK_MONOTONIC,&end);
#define CLOCK_SHOW                      printf("Elapsed time: %.6f ms\n",\
                                        1000*(end.tv_sec-start.tv_sec)+(double)(end.tv_nsec-start.tv_nsec)/1000000);


/*  initial timing struct  */
CLOCK_INIT

int main(void){

    /*  initial and read graph  */
    int v,e,C;
    scanf("%d%d%d", &v, &e, &C);
    // initial>>
    segmentor Instance = imgSeg_initial(v,e,C);
    // read edges>>
    int i;
    int a, b, l;
    for (i = 0; i<e; i++){
        scanf("%d%d%d", &a, &b, &l);
        // pass {a,b,l} to segmentor
        // i is the edge UUID by default
        imgSeg_read(Instance,i,a,b,l);
    }

    /*  START timing  */
    CLOCK_START
    if(!imgSeg_execute(Instance))printf("Execute Error!\n");
    CLOCK_END
    /*  END timing  */

    /*  print segemented graph  */
    imgSeg_putres(Instance);

    /*  show timing  */
    CLOCK_SHOW

    // terminate process
    imgSeg_terminate(Instance);

    system("pause");

    return 0;
}

