#ifndef MESSAGEQUEUE_H_
#define MESSAGEQUEUE_H_

#include "Incl.h"

#define MQSIZE 25
#define TRUE    2
#define USED    1
#define FALSE   0

typedef struct
{
    /* data */
    unsigned char used;
    unsigned char*         dataBuffer;
}__attribute__ ((__packed__))QUEUE_TYPE_T;


typedef struct
{
	int queueReadPtr;
    int queueWritePtr;

    QUEUE_TYPE_T q[MQSIZE];

}__attribute__ ((__packed__))MESSAGEQUEQUE_T;


void pushQueue(unsigned char* scrData, MESSAGEQUEQUE_T* queue );
void popQueue(unsigned char* dstData, MESSAGEQUEQUE_T* queue );
void printQueueStatus( MESSAGEQUEQUE_T* queue );
void printQueueStatusSpecificIndex(MESSAGEQUEQUE_T* queue, int index );
void allQueueClear(MESSAGEQUEQUE_T* queue);

#endif