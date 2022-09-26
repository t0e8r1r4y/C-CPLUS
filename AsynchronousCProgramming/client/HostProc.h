#ifndef HOSTPROC_H_
#define HOSTPROC_H_

#include "Incl.h"

typedef struct HostProc
{
    /* data */
    pthread_t hostRxTxTask;
	
}__attribute__ ((__packed__)) T_HOST_PROC_TASK;

pthread_t hostTaskRegister( void );


#endif