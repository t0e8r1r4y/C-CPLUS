/**
 * @file main.c
 * @author TERRY
 * @brief  알아두면 유용한 함수들에 대한 정리
 * @version 0.1
 * @date 2022-03-03
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int ascii2IntHexAndDecimal(char * strAscii, unsigned char hexflag);
int ascii2IntOnlyHex(char* strAsc, char len);
int hexToAscii( char* strArr, unsigned short hexval );


/**
 * @brief 테스트를 위한 메인함수입니다. 실제 사용 시 필요한 로직에서 위 함수를 호출하면 됩니다.
 * 
 * @return int 
 */
int main( void )
{
    unsigned short test[10] = { 500, 600, 700, 800, 900, 1000, 1100, 1200, 1300, 1400 };
    int i = 0;
    int len = 0;
    unsigned char tempBuffer[1024] = { 0, };

    int res = ascii2IntHexAndDecimal("5DC", 1);
    printf("%d\r\n", res);

    res = ascii2IntOnlyHex("ABCDEF",6);
    printf("%d\r\n", res);

    res = ascii2IntOnlyHex(NULL,6);
    printf("%d\r\n", res);

    res = 0;

    for( i = 0; i < 10; i++)
    {
        res = hexToAscii((char*)&tempBuffer[len], test[i] );
        len += res;
        tempBuffer[len] = ' ';
        len += 1;
    }

    tempBuffer[len] = '\0';

    printf("%s\r\n", tempBuffer);

    return 0;
}


/**
 * @brief 아스키로 표현된 (헥사, 데시말 값)을 int형으로 변환하는 함수입니다.
 * 
 * @param strAscii 
 * @param hexflag 
 * @return int 
 */
int 
ascii2IntHexAndDecimal( char * strAscii, unsigned char hexflag )
{
    int res = 0;                            // return value.
	unsigned int len = strlen(strAscii);    // 문자열 길이

	unsigned int i = 0, k = 0;
	
	int multifactor = 1;                    // 각 자리수

	int weightnum           = 10;           // 10진수 가중치
	if(hexflag) weightnum   = 16;           // 16진수 가중치


    for(i=0; i<len; i++)
    {
        multifactor = 1;
            
        for(k=0; k<(len-i-1); k++)
            multifactor = multifactor * weightnum; // 자리수에 따라 반복횟수 증감.

        if( strAscii[i] & 0x30 )
            res += ((int)(strAscii[i] - 0x30) * multifactor); // '0' ~ '9' 사이
        else                                 
            res += ((int)(strAscii[i] - 0x37) * multifactor); // 'A' ~ 'F' 사이 

    }


	return res;
}


/**
 * @brief 아스키로 된 헥사 값을 int값으로 변환하는 함수입니다. 여기서 지금 버그가 발견되었습니다.
 * 
 * @param strAsc 
 * @param len 
 * @return int 
 */
int 
ascii2IntOnlyHex(char* strAsc, char len) 
{                                         
	int i = 0;                            
	int res = 0;

    if(strAsc == NULL) return res; // 데이터가 NULL이 들어왔을 때 예외처리가 다 필요하네.
	
	for(i=0; i< len; i++)
	{
		if( strAsc[i] > 0x40 )
			res = ( res * 16 ) + ( strAsc[i] - 0x37 ); // 'A'(0x41) ~ 'F'(0x46) 
		else                                 
			res = ( res * 16 ) + ( strAsc[i] - 0x30 ); // '0'(0x30) ~ '9'(0x39) 
	}

	return res;
}


/**
 * @brief Hex 값인 unsigned short 범위의 값을 Ascii 값으로 변환하는 함수입니다. (sprintf 성능 개선 효과)
 * 
 * @param strArr 
 * @param hexval 
 * @return int 
 */
int 
hexToAscii( char* strArr, unsigned short hexval )
{
	int i;
	int n=0;
	unsigned short tmpval;

	for( i=0; i<4; i++ )
	{
        tmpval = ( hexval >> (4*(4-i-1)) ) & 0x000F;

        if( tmpval > 9 )
        {
            strArr[n++] = tmpval - 10 + 'A';
        }
        else if( tmpval == 0 )
		{
			if( n != 0 )
			{
				strArr[n++] = '0';
			}
		}
		else
        {
            strArr[n++] = tmpval + '0';
        }
	}

	if( n==0 ) strArr[n++] = '0';

	return n;
}

