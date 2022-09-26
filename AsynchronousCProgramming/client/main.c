#include "Incl.h"
#include "Logger.h"
#include "MessageQueue.h"
#include "HostProc.h"

#define TEST_THREAD_CNT 2
static void printPthreadJoinResult( int result );


/**
 * @brief 메인함수입니다. thread 2개를 실행하고 thread의 종료를 기다립니다.
 * 
 * @return int 
 */
int main(void)
{
    printf("Async Non Blocking Socket Proccess\r\n");

    pthread_t parr[TEST_THREAD_CNT];
    int index = 0, result = 0;
    void *retval;

    parr[index++] = hostTaskRegister();
    parr[index++] = loggerTaskRegister();

    for (size_t i = 0; i < TEST_THREAD_CNT; i++)
    {
        /* code */
        result = pthread_join(parr[i], &retval);
        printPthreadJoinResult(result);
        pthread_exit(NULL);
    }

    return 0;
}

static void 
printPthreadJoinResult( int result )
{
    switch (result)
    {
        case 0:
            printf("The thread joined successfully\n");
            break;
        case EDEADLK:
            printf("Deadlock detected\n");
            break;
        case EINVAL:
            printf("The thread is not joinable\n");
            break;
        case ESRCH:
            printf("No thread with given ID is found\n");
            break;
        default:
            printf("Error occurred when joining the thread\n");
    }

    return;
}