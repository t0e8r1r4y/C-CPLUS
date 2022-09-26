#include "HostProc.h"
#include "MessageQueue.h"
#include "ClientSocketCtl.h"
#include "RecvSend.h"

static void procRecvQueue(MESSAGEQUEQUE_T *rq);
static void procSendQueue(MESSAGEQUEQUE_T *sq);
void* hostRxTxQueProcRunnerable(void *Param);

T_HOST_PROC_TASK        hostProcTask;
T_HOST_CONNECTION_INFO  hostInfo;
MESSAGEQUEQUE_T         sendQueue;
MESSAGEQUEQUE_T         recvQueue;

static int recvCnt = 0;
static int sendCnt = 0;

pthread_t
hostTaskRegister( void )
{
    T_HOST_PROC_TASK        *spHostProcTask = &hostProcTask;
    T_HOST_CONNECTION_INFO  *spHostInfo = &hostInfo;

    memset( spHostInfo, 0, sizeof(T_HOST_CONNECTION_INFO) );
    memset( spHostProcTask, 0, sizeof(T_HOST_PROC_TASK) );

    spHostInfo->iAddress    = inet_addr(TEST_IP);
    spHostInfo->unPort      = TEST_PORT;
    spHostInfo->socketFd    = tcpCreateClient(spHostInfo->iAddress, spHostInfo->unPort);
    
    printf("socket create %d\r\n", spHostInfo->socketFd);
    if( spHostInfo->socketFd < 0 ) exit(EXIT_FAILURE);

    int nstat = 0;
    nstat = pthread_create(&spHostProcTask->hostRxTxTask, NULL, hostRxTxQueProcRunnerable ,NULL);

    if( nstat < 0 ) exit(EXIT_FAILURE);

    return spHostProcTask->hostRxTxTask;
}


void* 
hostRxTxQueProcRunnerable(void *Param)
{
    printf("Thread Start - hostRxTxQueProcRunnerable\r\n");

    T_HOST_CONNECTION_INFO  *spHostInfo = &hostInfo;

    int recvLen = 0;
    unsigned char recvBuffer[1024] = {0x00, };

    MESSAGEQUEQUE_T *spSendQueue = &sendQueue;
    MESSAGEQUEQUE_T *spRecvQueue = &recvQueue;

    while( 1 )
    {
        recvLen = hostRecv(spHostInfo->socketFd, recvBuffer, 1024);
        if(recvLen > 0)
        {
            pushQueue(recvBuffer, spRecvQueue);
            memset(recvBuffer, 0x00, recvLen);
        }

        procRecvQueue(spRecvQueue);
        procSendQueue(spSendQueue);
        usleep(1);
    }

    return NULL;
}

static void
procRecvQueue(MESSAGEQUEQUE_T *rq)
{
    unsigned char dest[1024] = {0x00, };
    popQueue(dest, rq);

    if(strlen(dest) > 0)
    {
        printf("[%d] %s\r\n", recvCnt,dest);
        memset(dest, 0x00, strlen(dest));
        recvCnt++;
    }
    return;
}

static void
procSendQueue(MESSAGEQUEQUE_T *sq)
{
    T_HOST_CONNECTION_INFO  *spHostInfo = &hostInfo;

    unsigned char dest[64] = {0x00, };
    popQueue( dest, sq );
    if(strlen(dest) > 0)
    {
        hostSend(spHostInfo->socketFd,dest,strlen(dest));
        printf("[%d] %s\r\n",sendCnt, dest);
        memset(dest, 0x00, sizeof(dest));
        sendCnt++;
    }
    return;
}