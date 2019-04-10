#ifndef OneLayeredBucket_h
#define OneLayeredBucket_h

#include <stdbool.h>
#include "DualyLinkedList.h"

//////////////////////////////////////////////////
typedef enum OLBOption {
    OLB_OPTION_NONE,
    OLB_OPTION_WITH_ELEMENT
} OLB_OPTION_e;

typedef struct BucketLayer {
    int beginValue;
    int range;                  // end value - begin value + 1
    
    int minPos;                 // index of cell including min. value.
    int num;                    // number of elements including lower layer.
    
    DLL_t **array;              // array of buckets.
} OLB_t;

//////////////////////////////////////////////////
OLB_t *createOLBucket(int beginValue, int endValue);
bool destroyOLBucket(OLB_t *B, OLB_OPTION_e option);
bool insertIntoOLBucket(OLB_t *B, int value, void *element);
void *pullMinOnOLBucket(OLB_t *B);
int findNextOnTheLayer(OLB_t *B);
bool isEmptyOLBucket(OLB_t *B);

#endif
