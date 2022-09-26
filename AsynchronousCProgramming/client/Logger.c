#include "Logger.h"
#include "MessageQueue.h"

void* loggerRunnerable(void *Param);

T_LOGGER_TASK           loggerTask;
extern MESSAGEQUEQUE_T  sendQueue;

pthread_t 
loggerTaskRegister( void )
{
    T_LOGGER_TASK *spLoggerTask = (T_LOGGER_TASK*) &loggerTask;
    int nstat = 0;

    nstat = pthread_create(&spLoggerTask->loggerTask, NULL, loggerRunnerable, NULL);

    if( nstat < 0 ) exit(EXIT_FAILURE);
    return spLoggerTask->loggerTask;
}

void* 
loggerRunnerable(void *Param)
{
    printf("Thread Start - loggerRunnerable\r\n");
    unsigned char mesage[12] = {'h', 'e', 'l', 'l', 'o', ' ' , 'W', 'o','r','l','d','\0'};
    MESSAGEQUEQUE_T  *spSendQueue = &sendQueue;

    while (1)
    {
        /* code */
        pushQueue(mesage, spSendQueue);
        sleep(5);
    }

    return NULL;
    
}
