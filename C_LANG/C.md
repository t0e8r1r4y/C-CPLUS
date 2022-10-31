# C언어 관련 핵심 내용 정리

## ✅ 포인터의 이해

#### *와 &
- `포인터 변수`란 `메모리의 주소값을 저장`하기 위한 변수
- 포인터는 `변수` 형태의 포인터, `상수` 형태의 포인터가 있다.
- 메모리 주소는 항상 `시작번지`만을 가지고 위치를 표시한다.
- 포인터 주소의 `크기`는 시스템이 32비트인지, 64비트인지에 따라 달라진다. ([관련링크](https://velog.io/@apt0/%EC%BB%B4%ED%93%A8%ED%84%B0%EC%9D%98-%EC%A3%BC%EC%86%8C%ED%91%9C%ED%98%84-%ED%8F%AC%EC%9D%B8%ED%84%B0%EC%9D%98-%ED%81%AC%EA%B8%B0-%ED%91%9C%ED%98%84%EA%B0%80%EB%8A%A5%ED%95%9C-%EB%A9%94%EB%AA%A8%EB%A6%AC-%EC%A3%BC%EC%86%8C%EA%B0%80-%ED%81%AC%EB%A9%B4-%EC%A2%8B%EC%9D%80-%EC%9D%B4%EC%9C%A0))
- 포인터 변수를 선언만하고 초기화 하지 않으면 포인터 변수는 쓰레기 값으로 초기와 된다.

```C
// example
#include <stdio.h>

int main( void ){

    int num = 7;
    int *pnum = &num;
    
    printf("%08X\r\n", *pnum); // 값을 출력함. -> 이 값은 num을 따라가서 num을 출력하는 것
    printf("%08X\r\n", pnum); // 주소값을 출력함 -> 주소값은 매번 실행마다 vfs가 배정하기 때문에 달라질 수 있음

    return 0;
}
```

<br/>


#### 포인터와 배열
- 배열은 `상수 형태`의 포인터이다.
- 포인터 변수와 배열을 비교
  | 비교대상과 조건 | 포인터 변수 | 배열의 이름 |
  | ----- | ----- | ----- |
  | 이름이 존재하는가? | 존재 | 존재 |
  | 무엇을 나타내거나 저장하는가? | 메모리의 주소 값 | 메모리의 주소 값 |
  | 주소 값의 변경이 가능한가? | 가능 | 불가능 |
- 배열의 이름을 피연산자로 하는 * 연산이 가능하다. ( 아래는 재미있는 테크닉이다. )
- `arr[i] == *(arr+i)` 같다는 것!

```C

#include <stdio.h>
#include <string.h>

int main( void ){

    unsigned char src[62];
    unsigned char dest[62];
    memset(src, 0, sizeof(src));
    unsigned char *pp = src;

    *pp++ = 0x02; // 가변인자 처럼 필요한 값들을 채워넣을 수 있다. -> 아마도 추측컨데 append 메서드들이 이런식으로 동작할 것 같다.
    *pp++ = 0x0A;
    *pp++ = 0x0B;
    *pp++ = 0x0C;
    *pp++ = 0x0D;
    *pp++ = 0x03;
    *pp++ = 0x00;

    
    memcpy(dest, src, strlen( (char *) src));

    for(int i = 0; i < strlen((char *)dest); i++){
        printf("%02X ", dest[i]);
    }
}

```


<br/>


#### 포인터와 함수에 대한 이해
- 아래와 같은 방식으로 포인터 변수를 함수의 인자로 넘겨 줄 수 있다.
```C
#include <stdio.h>
#include <string.h>

void valueCopy( unsigned char* src, unsigned char * dest) {
    unsigned char *pp = src;

    *pp++ = 0x02;
    *pp++ = 0x0A;
    *pp++ = 0x0B;
    *pp++ = 0x0C;
    *pp++ = 0x0D;
    *pp++ = 0x03;
    *pp++ = 0x00;

    memcpy(dest, src, strlen( (char *) src));

    return;
}

int main( void ){

    unsigned char src[62];
    unsigned char dest[62];

    valueCopy(src, dest);

    for(int i = 0; i < strlen((char *)dest); i++){
        printf("%02X ", dest[i]);
    }

}
```

<br/>


#### Call by Value & Call by Reference
- Call by Value : 단순히 값을 전달하는 형태의 함수 호출
- Call by Reference : 메모리의 접근에 사용되는 주소 값을 전달하는 형태의 함수 호출
- 아래 소켓 종료 예시에서 함수의 인자를 int socketFd로 넘겨준다면 해당 fd는 함수 내에서는 -1이 되겠지만, 결과적으로 입력한 socket에 대해서는 변경이 안된다.

```C
void
tcpCloseSocket(int *socketFd )
{
    close((*socketFd));
    (*socketFd) = -1;
    return;
}
```

<br/>

#### 포인터 대상의 const 선언
- 포인터 변수에 const를 선언함으로서 값을 변경하는 것을 허용하지 않을 수 있다.


#### 포인터의 포인터 (다중 포인터)
- 포인터의 포인터이다.
- 아래 예시에서 주소 값을 찍어보면, num은 자기 자신의 주소, pnum은 num의 주소값을 저장하고 있는 주소, ppnum은 pnum의 주소를 저장하는 주소를 출력한다.

```C
#include <stdio.h>

int main( void ){

    int num = 7;
    int *pnum = &num;
    int **ppnum = &pnum;

    printf("%d %d %d\r\n", num, *pnum, **ppnum); // 7 7 7
    printf("%04X %04X %04X\r\n", &num, &pnum, &ppnum); // BD01749C BD017490 BD017488

}
```

- 개념적으로 조금 헷갈릴 수 있는 부분인데 `포인터의 배열`과 `배열 포인터`는 헷갈리면 안된다.
- 보통 실무에서 3차원까지는 잘 안쓰기는하는데 2차원까지는 사용하므로 숙지가 필요하다.
```C
#include <stdio.h>

int main( void ){

    int arr[2][4] = {1,2,3,4,5,6,7,8};
    int (*dest)[4] = arr;

    for(int i = 0; i < 2; i++){
        for(int j = 0; j < 4; j++) {
            printf("%d ", dest[i][j]);
        }
        printf("\n");
    }

}
```


#### 포인터 함수
- 포인터 함수를 기깔나게 잘 쓰면 구조체와 함께 C에서도 마치 클래스를 쓰는 것 처럼 정의를해서 쓸 수 있다.
```C
#include <stdio.h>

void SimpleAdder(int n1, int n2) {
    printf("%d\r\n", n1 + n2);
}

void ShowString(char * str) {
    printf("%s\r\n", str);
}

int main( void ) {

    char * str = "Function Poiner";
    int num1 = 10;
    int num2 = 20;

    void (*fptr1)(int, int) = SimpleAdder;
    void (*fptr2)(char *) = ShowString;

    fptr1(num1, num2);
    fptr2(str);

    return 0;
}
```


## ✅ C언어에서 구조체


## ✅ C언어에서 메모리 관리


## ✅ 매크로와 선행처리기
