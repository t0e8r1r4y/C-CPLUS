#include "MessageQueue.h"

void pushQueue(unsigned char* scrData, MESSAGEQUEQUE_T* queue )
{
    int queueIndex = 0;
    size_t i = 0;

    for (i = 0; i < MQSIZE; i++)
    {
        /* code */
        queueIndex = (( queue->queueWritePtr + i ) % MQSIZE );

        if( queue->q[queueIndex].used == FALSE)
        {
            queue->q[queueIndex].used       = USED;
            queue->q[queueIndex].dataBuffer = (unsigned char*)malloc(strlen((const char *)scrData) +1 );

            memcpy(queue->q[queueIndex].dataBuffer, (unsigned char*)scrData, strlen((const char *)scrData) +1);

            queue->queueWritePtr            = queueIndex + 1;
            queue->q[queueIndex].used       = TRUE;
            break;
        }
    }

    if( i >= (MQSIZE-1) ) printf("Queue Overflow\r\n");
    
    return;
}


void 
popQueue(unsigned char* dstData, MESSAGEQUEQUE_T* queue )
{
    int queueIndex = 0;

    size_t i = 0;
    for ( i = 0; i < MQSIZE; i++)
    {
        /* code */
        queueIndex = (queue->queueReadPtr + i) % MQSIZE;

        if(queue->q[queueIndex].used == TRUE)
        {
            memcpy((unsigned char*)dstData, queue->q[queueIndex].dataBuffer, strlen( (const char *)queue->q[queueIndex].dataBuffer));
            free(queue->q[queueIndex].dataBuffer);
            
            queue->q[queueIndex].used   = FALSE;
            queue->queueReadPtr         = queueIndex + 1;
            break;
        }
    }

    return;
}

void 
printQueueStatus( MESSAGEQUEQUE_T* queue )
{
    printf("queueReadPtr : %d\r\n", queue->queueReadPtr);
    printf("queueWritePtr : %d\r\n", queue->queueWritePtr);

    for (size_t i = 0; i < MQSIZE; i++)
    {
        /* code */
        printf("[ %zu index ] %02X\r\n", i ,queue->q[i].used);
    }
    
}

void 
printQueueStatusSpecificIndex( MESSAGEQUEQUE_T* queue, int index )
{
    if( index >= MQSIZE ) return;

    if(queue->q[index].used == TRUE)
    {
        for (size_t i = 0; i < strlen((char *)queue->q[index].dataBuffer); i++)
        {
            /* code */
            printf("%02X ", queue->q[index].dataBuffer[i]);
        }
        printf("\r\n");
    }
    
    return;
}

void 
allQueueClear( MESSAGEQUEQUE_T* queue )
{
    for (size_t i = 0; i < MQSIZE; i++)
    {
        /* code */
        queue->q[i].used = TRUE;
        free(queue->q[i].dataBuffer);
    }

    queue->queueReadPtr = 0;
    queue->queueWritePtr = 0;
    
    return;
}