#include "Incl.h"

#define NON_BLOCKING		1
#define BLOCKING			2
#define	INVALID_SOCKET		(-1)
#define	SOCKET_ERROR		(-1)

#define TEST_IP             "127.0.0.1"
#define TEST_PORT           8890

typedef struct
{
	int socketFd;					// socket fd
	int iAddress;					// ip
	unsigned short unPort;			// 포트
}__attribute__ ((__packed__))T_HOST_CONNECTION_INFO;

int tcpCreateClient( int ip, unsigned short port );
void tcpCloseSocket( int *socketFd );