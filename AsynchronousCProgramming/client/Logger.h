#include "Incl.h"


#define _D_LOG_BUFFER_LEN (256l)

typedef	struct
{
	int fileSize;

	pthread_t			loggerTask;		
}__attribute__ ((__packed__))T_LOGGER_TASK;

pthread_t loggerTaskRegister( void );