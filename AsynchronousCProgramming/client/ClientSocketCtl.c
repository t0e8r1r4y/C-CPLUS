#include "ClientSocketCtl.h"

/**
 * @brief socket create function
 * 
 * @param ip 
 * @param port 
 * @param modeflag 
 * @return int 
 */
int
tcpCreateClient(int ip, unsigned short port )
{
	int res = 0;			// function retrun;
	int socketFd  =  -1;
	int	buffSize;
	int lValue = 1;			// set NODELAY 옵션

	struct sockaddr_in	nSin;

	// socket
	socketFd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP );

	if( socketFd == INVALID_SOCKET )
	{
		printf("Socket Command error[%s-%d]\n", strerror(errno), errno );
		return (-1);
	}

	int temp = sizeof(int);
	// socket option get
	if(getsockopt(socketFd, SOL_SOCKET, SO_RCVBUF, &buffSize, (socklen_t *)&temp) < 0)
	{
		return -1;
	}

	// socket option set
	res = setsockopt(socketFd, IPPROTO_TCP, TCP_NODELAY, (char*)&lValue, sizeof(int));

	if(res == -1)
	{
		return -1;
	}


	nSin.sin_family 	= AF_INET;
	memcpy(&nSin.sin_addr, &ip, sizeof(ip));
	nSin.sin_port		= htons(port);


	// socket Connection

    res = connect(socketFd, (const struct sockaddr*)&nSin, sizeof(nSin));

    if( res < 0 )
	{
		printf("connection fail\r\n");
		return (-1);
	}

    
    // On Success to Connection.
    printf("Client Socket connect OK Server Socket[%d] IP[%s, %i] \n", socketFd, inet_ntoa(nSin.sin_addr), nSin.sin_port);

    // Non-Blocking setting
    res = fcntl( socketFd, F_SETFL, O_NONBLOCK);
    if( res < 0 )
    {
        printf("(HOST) : Error fcntl() %d - %s\n", errno, strerror(errno));
        tcpCloseSocket((int *)socketFd);
        return (-1);
    }
		

    return socketFd;
}

/**
 * @brief socket close function
 * 
 * @param socketFd 
 */
void
tcpCloseSocket(int *socketFd )
{
    close((*socketFd));
    (*socketFd) = -1;
    return;
}
