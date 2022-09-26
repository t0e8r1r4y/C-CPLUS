#include "Incl.h"

// 타입 정의
typedef signed long	   BaseType_t;
typedef int			   SockType_t;
typedef int			   DescType_t;
typedef unsigned long  TickType_t;


#define INVALID_SOCKET (-1)


static SockType_t listenSocket( int portNum, struct sockaddr_in *servAddr );

// client, server socket
static SockType_t clientSockHandler;    
static SockType_t serverSockHandler;    

static struct sockaddr_in clientSockAddrIn;
static struct sockaddr_in servSockAddrIn;
static socklen_t clnt_addr_size = sizeof(clientSockAddrIn);

static BaseType_t clientSockPortNum = 8890; 

static int sendCnt = 0;
static int recvCnt = 0;

int main(void)
{
    printf("This is a server Proccesor\r\n");
    // server init
    while( ( serverSockHandler = listenSocket(clientSockPortNum, &servSockAddrIn) ) == INVALID_SOCKET )
    {
        usleep(100);
    }
    // accept function start.
    // 기본적으로 accept는 Blocking 함수이기 때문에 제어의 주도권이 넘어 갑니다. 정상적으로 연결 되는 client가 있어야 다음 코드가 실행됩니다.
    clientSockHandler = accept(serverSockHandler, (struct sockaddr*)&clientSockAddrIn, &clnt_addr_size );
    printf("client connection is success!\r\n");

    int recvlen = 0, sendLen = 0;
    unsigned char recvBuffer[1024] = {0x00, };
    char* sendMsg = "Im server\n";

    while (1)
    {
        /* code */
        recvlen = recv(clientSockHandler, recvBuffer, 1024, 0);
        
        if(recvlen > 0 )
        {
            recvCnt++;
			printf("[%d] recv data : %s\r\n", recvCnt, recvBuffer);

			sendCnt++;
            sendLen = send(clientSockHandler, sendMsg, strlen((const char*)sendMsg),0 );
			printf("[%d] send data : %s\r\n", sendCnt, sendMsg);

            recvlen = 0;
            memset(recvBuffer, 0x00, recvlen);
        }

        usleep(500);
        
    }

    close(clientSockHandler);
    close(serverSockHandler);

    return 0;
}

/**
 * @brief 서버 소켓 생성
 * 
 * @param portNum 
 * @param servAddr 
 * @return SockType_t 
 */
static SockType_t 
listenSocket( int portNum, struct sockaddr_in *servAddr )
{
	SockType_t serverSocketFd = INVALID_SOCKET; // fd 초기화
	BaseType_t socketOption;
	struct sigaction action;

	int errsv = 0;

	action.sa_handler = SIG_IGN;
	sigemptyset( &action.sa_mask );
	action.sa_flags = 0;

	if( sigaction( SIGPIPE, &action, NULL ) < 0 ) return INVALID_SOCKET; // SIGPIPE 시그널 무시


	if( ( serverSocketFd = socket( AF_INET, SOCK_STREAM, 0 ) ) < 0 ) // open TCP socket
	{
		printf("fail : can't open stream socket\n");
		return INVALID_SOCKET;
	}

    // 클라이언트 재접속 시 소켓 재사용 옵션 
	socketOption = 1; // Set Socket Option
	if( setsockopt( serverSocketFd, SOL_SOCKET, SO_REUSEADDR, ( char * )&socketOption, ( int )sizeof( socketOption ) ) < 0 )
	{
		printf("fail socket option apply. option name is SO_REUSEADDR.");
		close(serverSocketFd);
		return INVALID_SOCKET;
	}

    // Adreess init
	memset( servAddr, 0, sizeof(struct sockaddr_in) );
	servAddr->sin_family = AF_INET;
	servAddr->sin_addr.s_addr = htonl( INADDR_ANY );  // 어느 IP건 접속 허용
	servAddr->sin_port = htons( portNum );
	

    // Bind Socket
	if( bind( serverSocketFd, ( struct sockaddr * )servAddr, sizeof(struct sockaddr_in) ) == -1 )
	{
		errsv = errno;
	    printf( "fail to bind local address(%s)\n", strerror(errsv));
		close(serverSocketFd); 
		return INVALID_SOCKET;
	}

	if( listen( serverSocketFd, 15 ) < 0 )  // Listening, connection queue is 15
	{
        printf("fail to liten for connection\r\n");
		close(serverSocketFd); 
		return INVALID_SOCKET;
	}

	return serverSocketFd;
}

