# AsynchronousCProgramming


### 설명
- C언어 프로그래밍 비동기 패턴의 일반적인 구현입니다.
- 현업에서 사용했던 소스는 비즈니스 로직이 많아서 간략하게 필요한 부분들만 재구성하여 구현하였습니다. ( private repo 비공개입니다. )
- HeartBeat이나 timeout을 통한 처리에 대한 관리도 반드시 들어가야 되는 부분이지만 구현하지 않았습니다.
- Blocking/NonBlocking Sync/Async에 대한 개념 정리는 블로그에 함께 정리하였습니다.


---
### 구현 내용 설명

    요약
    각각의 Task Thread가 Event Loop 기반으로 동작하며 데이터를 처리하는 구조입니다.
    Queue에 데이터 유무를 확인하여 데이터가 있을 경우 해당 데이터를 처리하고 다음 처리를 위해 Queue에 넣는 구조입니다.
    각 Task의 관심사 분리 효과가 있으며 구현 및 테스트가 용이합니다.



![비동기 프로그래밍 drawio](https://user-images.githubusercontent.com/91730236/182301874-279ec2fb-7480-4958-907b-ac209db0aa68.png)



- 비동기 I/O 중에서 Socket I/O에 대한 예시입니다.
- 소켓을 NonBlocking으로 설정하고 메시지큐를 사용하여 멀티스레딩 구현을 통해 데이터 주고받는 처리로직입니다.
- Logger.c에서 생성한 Thread가 Hello world라는 로그가 계속 발생하여 이를 서버로 송신하는 시나리오입니다.
- Logger.c는 생성된 로그를 서버로 보내는 작업이 완료되기 전까지 대기를 하지 않고 다음 Task를 처리하기 위해서 Log 생성 후 Queue에 저장하고 다음 일을 합니다.
- Queue에 저장된 로그는 HostProc.c 파일에서 Queue에 저장된 데이터를 가져가고 해당 데이터를 서버로 send합니다.
- HostProc.c에서 생성한 Thread도 send 처리 후 바로 다음 Task를 수행합니다.
- HostProc.c은 서버에서 수신 받은 데이터 또한 동일한 흐름으로 처리 할 수 있습니다. ( 미구현 )




---
### 코드 동작 방법 ( 아래 내용은 코드 수정이 있을 때마다 변경 )
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


---
### 장점
- 멀티스레딩 방식으로 비동기 프로그래밍을 하게되면 각각의 Task에서 정의한 처리 흐름을 지연 없이 처리가 가능합니다.
- 각각의 Task가 가지는 관심사를 분리하여 구현 및 테스트가 가능합니다.


---
### 단점
- (간단한 예시라 미구현 내용이지만) 여기서 Thread의 갯수가 더 증가하게 되어 동일한 Queue를 서로 점유하게 되어 Thread Race Condition에 쉽게 빠질 수 있습니다.
- Thread Race Condition에 대해서 semaphore 혹은 mutex를 사용하여 이슈를 해결 할 수 있지만, 코드 복잡도가 증가하게 되면 관리의 어려움은 여전히 남아있습니다.
- 특히 경험상 thread간에 처리하는 데이터의 sync를 맞춰야 하는 경우는 정교한 설계가 필요합니다.
- 데이터 처리 결과에 대해서 관리가 필요하게 됩니다. 그래서 보통은 timeout을 설정하여 데이터를 처리합니다.







