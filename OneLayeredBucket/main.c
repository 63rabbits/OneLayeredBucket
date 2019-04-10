#include <unistd.h>
//#define checkMemoryLeak

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <limits.h>
#include <math.h>
#include "OneLayeredBucket.h"

//////////////////////////////////////////////////
typedef struct Element {
    int value;
} Element_t;

//////////////////////////////////////////////////
void test(void);
Element_t *createElement(int value);
bool destroyElement(Element_t *element);
int getValue(Element_t *element);

//////////////////////////////////////////////////
int main(void) {
    
#ifdef checkMemoryLeak
    while (true) {
        test();
        sleep(1);
    }
#else
    test();
#endif
    
    return EXIT_SUCCESS;
}

void test() {
    printf("=== start [%s] ===\n", __func__);
    
    int retry = 1;
    int minValue = 0;
    int maxValue = 100;
    int count = 0;
    
    OLB_t *B = createOLBucket(minValue, maxValue);
    if (B == NULL) {
        printf("error [%s] : could not create a 1-Layered-Bucket.\n", __func__);
        return;
    }
    
    for (int i=0; i<=retry; i++) {
        printf("*** insert [%s] ***\n", __func__);
        count = 0;
        for (int j=0; j<2; j++) {
            for (int k=minValue; k<=maxValue; k++) {
                count++;
                Element_t *element = createElement(k);
                printf("insert #%4d : %10d\n", count, getValue(element));
                bool result = insertIntoOLBucket(B, getValue(element), element);
                if (!result) {
                    printf("error [%s] : could not insert value %d.\n", __func__, element->value);
                    destroyElement(element);
                }
            }
        }
        
        printf("*** pull [%s] ***\n", __func__);
        count = 0;
        while (!isEmptyOLBucket(B)) {
            count++;
            Element_t *element = pullMinOnOLBucket(B);
            printf("pull #%4d : %10d\n", count, element->value);
            destroyElement(element);
        }
    }
    destroyOLBucket(B, OLB_OPTION_WITH_ELEMENT);
    
    printf("=== end [%s] ===\n", __func__);
}

Element_t *createElement(int value) {
    Element_t *element = malloc(sizeof(Element_t));
    if (element == NULL) return NULL;
    element->value = value;
    return element;
}

bool destroyElement(Element_t *element) {
    // Block illegal parameters.
    if (element == NULL) return false;
    free(element);
    return true;
}

int getValue(Element_t *element) {
    return element->value;
}

