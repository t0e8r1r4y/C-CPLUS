# Event Driven Pattern을 적용한 IoT 실시간 스트림 데이터 처리

> 회사에서 개바한 내용을 공개할 수 없어, EDP 패턴으로 구현하는 일반적인 패턴에서 정리하였습니다.
> IoT 센서가 목적하는 이벤트가 발생했을 경우에 해당 이벤를 Event Driven Parttern으로 처리한다고 합니다.

### :small_orange_diamond: 기술키워드 : C, Arm-linux-cross-compiler, cmake, EDP


<br/>

## :white_check_mark: 개요
- IOT 센서에서 시리얼 통신을 사용하기에 해당 데이터를 수신받아 처리 후, 결과를 서버로 TCP/IP 통신으로 전송하는 어플리케이션입니다.
- 시리얼 통신의 특성으로 정의 된 형태의 프레임으로 데이터를 수신받기까지 TCP/IP보다 속도가 느립니다. 따라서 통신 속도가 비대칭이 되기에 양방향 통신과정에서 처리지연일 발생합니다. 이를 개선하기 위한 EDP 패턴을 적용하였습니다.

<br/>


## :white_check_mark: 코드 구현 내용 설명

> 회사 코드를 변형하여 구현하였습니다.

![비동기 프로그래밍 drawio](https://user-images.githubusercontent.com/91730236/182301874-279ec2fb-7480-4958-907b-ac209db0aa68.png)


- Logger.c에서 생성한 Thread가 Hello world라는 로그가 계속 발생하여 이를 서버로 송신하는 시나리오입니다.
- Logger.c는 생성된 로그를 서버로 보내는 작업이 완료되기 전까지 대기를 하지 않고 다음 Task를 처리하기 위해서 Log 생성 후 Queue에 저장하고 다음 일을 합니다.
- Queue에 저장된 로그는 HostProc.c 파일에서 Queue에 저장된 데이터를 가져가고 해당 데이터를 서버로 send합니다.
- HostProc.c에서 생성한 Thread도 send 처리 후 바로 다음 Task를 수행합니다.
- HostProc.c은 서버에서 수신 받은 데이터 또한 동일한 흐름으로 처리 할 수 있습니다. ( 미구현 )



<br/>


## :white_check_mark: Technical Issue ( 프로젝트를 진행하며 고민한 사항과 주안점 )

- Non-Blocking 소켓을 사용하여 Blocking 현상방지
- Multi-thread를 사용하여 Task 단위로 처리흐름을 구분하여 병렬처리
- Thread 간 처리 데이터를 공유하기 위해서 EventQueue를 사용하여 처리함
- 실제 업무에서는 epoll을 사용하여 이벤트 수신을 체크했지만, Mac에서는 구현 방식이 달라서 epoll은 사용하지 않음
- EventQueue는 원형 배열로 선언하였고, Queue가 Overflow 되더라도 다음 index를 해제하고 사용할 수 있도록 구현함
- 위 소스에서 malloc을 통해 동적 배열로 관리하였지만, 실제 업무에서는 데이터 프레임이 정해져있어 넉넉한 크기의 배열을 선언하고 동적으로 할당하지 않음



<br/>

---
## :white_check_mark: 코드 동작 방법 ( 아래 내용은 코드 수정이 있을 때마다 변경 )
- 코드를 내려받아서 gcc로 컴파일하면 됩니다. 해당 코드는 mac에서 작성되 아래 내용을 따릅니다.
- gcc version은 4.2.1입니다.
- 실행만 시켜보고 싶다면 client는 test, server는 ServerTest를 실행시키면 됩니다.
- 실행 순서는 서버를 먼저 실행시키고 클라이언트를 실행해야 됩니다. ( 예외처리 등은 제외한 로직임 )
- 로그는 아래 이미지 처럼 출력됩니다.

```
Apple clang version 13.0.0 (clang-1300.0.27.3)
Target: x86_64-apple-darwin21.4.0
Thread model: posix
```

<img width="1040" alt="스크린샷 2022-08-02 오후 3 37 09" src="https://user-images.githubusercontent.com/91730236/182308440-ee684c2b-d894-48d0-a53f-9e132b78affe.png">


<br/>


## :white_check_mark: 어플리케이션 구현 도식
- EDP를 적용한 처리흐름을 시퀀스 다이어그램으로 정리하였습니다.

![최신 drawio](https://user-images.githubusercontent.com/91730236/195737367-8579c841-6b9d-4c22-bc3b-6b17ed275ecb.png)






