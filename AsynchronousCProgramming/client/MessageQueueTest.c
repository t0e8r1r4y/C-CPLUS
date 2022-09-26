#include "MessageQueueTest.h"

// 메시지 큐에 데이터를 추가하는 테스트
void test1()
{
    MESSAGEQUEQUE_T mq;
    memset(&mq, 0x00, sizeof(MESSAGEQUEQUE_T));
    unsigned char test[5] = {0x12, 0x13, 0x44, 0x55, 0x66};

    for (size_t i = 0; i < 4; i++)
    {
        pushQueue(test, &mq);   
    }

    printQueueStatusSpecificIndex(&mq, 2);
    printQueueStatus(&mq);
    allQueueClear(&mq);
    return;
}

// 저장 된 데이터를 POP하는 테스트
void test2()
{
    MESSAGEQUEQUE_T mq;
    memset(&mq, 0x00, sizeof(MESSAGEQUEQUE_T));
    unsigned char test[5] = {0x12, 0x13, 0x44, 0x55, 0x66};
    unsigned char dest[5];

    for (size_t i = 0; i < 4; i++)
    {
        /* code */
        pushQueue(test, &mq);
    }
    
    printQueueStatusSpecificIndex(&mq, 2);
    printQueueStatus(&mq);

    for (size_t i = 0; i < 100; i++)
    {
        /* code */
        popQueue(dest, &mq);
        memset(dest, 0x00, sizeof(dest));
    }

    printQueueStatus(&mq);
    allQueueClear(&mq);
    return;
}

void test3()
{
    MESSAGEQUEQUE_T mq;
    memset(&mq, 0x00, sizeof(MESSAGEQUEQUE_T));
    unsigned char test[5] = {0x12, 0x13, 0x44, 0x55, 0x66};

    for (size_t i = 0; i < 100; i++)
    {
        /* code */
        pushQueue(test, &mq);
    }

    allQueueClear(&mq);
    return;
}