#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <limits.h>
#include <math.h>
#include "OneLayeredBucket.h"

//////////////////////////////////////////////////
OLB_t *createOLBucket(int beginValue, int endValue) {
    OLB_t *B = malloc(sizeof(OLB_t));
    if (B == NULL) return NULL;
    
    B->beginValue = beginValue;
    B->range = endValue - beginValue + 1;
    B->minPos = 0;
    B->num = 0;
    
    B->array = malloc(B->range * sizeof(void *));
    if (B->array == NULL) {
        free(B);
        return NULL;
    }
    for (int i=0; i<B->range; i++) {
        B->array[i] = createDLList();
    }
    
    return B;
}

bool destroyOLBucket(OLB_t *B, OLB_OPTION_e option) {
    for (int i= 0; i < B->range; i++) {
        if (option == OLB_OPTION_WITH_ELEMENT) {
            destroyDLList(B->array[i], DLL_OPTION_WITH_ELEMENT);
        }
        else {
            destroyDLList(B->array[i], DLL_OPTION_NONE);
        }
    }
    free(B->array);
    free(B);
    
    return true;
}

bool insertIntoOLBucket(OLB_t *B, int value, void *element) {
    // Block illegal parameters.
    if (B == NULL) return false;
    if (element == NULL) return false;
    
    if ((value < B->beginValue) ||
        (value >= (B->beginValue + B->range))) {
        return false;
    }

    bool result = false;
    int i = value - B->beginValue;
    result = insertAtTailOnDLList(B->array[i], element);
    if (result) {
        B->num++;
        if (value < (B->beginValue + B->minPos - 1)) {
            B->minPos = i;
        }
    }
    return result;
}

void *pullMinOnOLBucket(OLB_t *B) {
    // Block illegal parameters.
    if (B == NULL) return NULL;
    
    while (B->num > 0) {
        DLL_t *list = B->array[B->minPos];
        if (!isEmptyDLList(list)) {
            void *element = pullHeadOnDLList(list);
            B->num--;
            if (isEmptyDLList(list)) {
                B->minPos = findNextOnTheLayer(B);
            }
            return element;
        }
        B->minPos = findNextOnTheLayer(B);
    }
    return NULL;
}

int findNextOnTheLayer(OLB_t *B) {
    for (int i=1; i<B->range; i++) {
        int j = (B->minPos + i) % B->range;
        if (!isEmptyDLList(B->array[j])) {
            return j;
        }
    }
    return B->minPos;
}

bool isEmptyOLBucket(OLB_t *B) {
    if (B->num <= 0) return true;
    return false;
}
