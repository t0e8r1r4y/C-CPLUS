#include "RecvSend.h"

/**
 * recv와 send 함수는 return value가 항상 보내려는 길이대로 return 되지 않음
 * 따라서 일반적으로 해당 함수는 길이를 계산해서 반복문 처리로 다 보낼때 까지 계속 보내는 것임.
 * ( 커널 버퍼로 보내는 시스템 콜을 계속 요청하는 행위 )
 *
 * 따라서 해당 함수들은 별도로 분리를 해두고 차후에 문제가 생기면 수정하는 것으로 하려고 함.
 * nginx 등 오픈소스 분석하니까 다 그렇게 해두었던데...
 * 발생하는 에러에 따른 예외처리도 이렇게 따로 빼두는게 좋아보임.
 */

int hostRecv( int socketfd, unsigned char* data, int iMaxLen )
{
	int recvLen = 0;

	recvLen = recv(socketfd, (char *)data, iMaxLen, 0);

#if 0

	if( recvLen > 0 )
	{
		printf("hostrecv called %d - (%d - %d)\r\n", socketfd, iMaxLen, recvLen);
		int i = 0;
		for( i = 0; i < recvLen; i++)
		{
			printf("%02X ", (unsigned char)data[i]);
		}
		printf("\r\n");
	}

#endif

	return recvLen;
}





int hostSend( int socketfd, unsigned char *data, int len )
{
	int sendLen = 0;

	sendLen = send(socketfd, data, len, MSG_NOSIGNAL);

#if 0

	if(sendLen != -1 )
	{
		printf("hostSend called %d - (%d - %d)\r\n", socketfd, len, sendLen);
		int i = 0;
		for( i = 0; i < sendLen; i++)
		{
			printf("%02X ", (unsigned char)data[i]);
		}
		printf("\r\n");
	}

#endif

	if(sendLen < 0)
	{
		// 여기서 thread를 재기동 시키는게 맞는건지?
		// 소켓에 대한 예외처리를 여기서 가져가는 로직으로 되어 있음.
		printf("[ERR] send function return error on hostSend function. error is %s(%d)\r\n", strerror(errno), errno);
		return -1;
	}

	return sendLen;
}