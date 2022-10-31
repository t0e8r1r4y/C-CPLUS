# 일반적인 C언어 프로그래밍 인터뷰 질문 모음

## ❓프로그래밍 언어의 주요 기능?
- 휴대성 : 플랫폼 독립적인 언어
- 모듈성 : 큰 프로그램을 작은 모듈로 분해 가능
- 적응성 : 프로그래머가 언어를 제어 할 수 잇는 가능성
- 성능(속도) : 다른 고급언어와 비교할 때 고속으로 컴파일하고 실행함
- 확장성 : 프로그래머가 새로운 기능 추가 가능

<br/>


## ❓C와 관련된 기본 데이터 유형
- Int, Float, double, char, void

<br/>



## ❓구문 오류란 ( C언어에서 발생 할 수 있는 오류 더 찾아볼 것 )
- 프로그램을 만드는 동안 발생하는 실수/오류를 구문 오류라고함
- 철자가 틀린 명령 혹은 잘못된 대/소문자 구분, 메서드/함수 호출 시 잘못된 매개변수 등

<br/>


## ❓예약어란
- 표준 C언어 라이브러리의 일부인 단어를 예약어라고 함
- return, int, void 등등 이런 예약어는 프로그래머가 마음대로 쓸 수 없음

<br/>


## ❓[댕글링 포인터 문제( 매달려 있는 포인터 문제)](https://dojang.io/mod/forum/discuss.php?d=365)
- 어떤 변수의 메모리 주소를 가리키는 포인터가 있지ㅏㄴ 일정 시간이 지난 후 포인터가 해당 위치를 가리키는 포인터를 유지하면서 변수가 메모리 위치에서 삭제 된 경우 매달린 포인터라고 함
- free 함수는 포인터로 할당한 메모리를 반환하며, 할당한 메모리에 재접근이 가능함.
- 이 때 예기치 않은 오류가 발생할 수 있는데, 해제된 공간을 "0"으로 초기화 시키지 않을 경우 여기에 있는 값을 가져가 쓰면서 발생하는 문제
- 방어적인 프로그래밍에서는 free()함수를 실행 한 뒤 해당 값에 대해서 NULL로 반드시 초기화 시켜두는게 중요함.

<br/>


## ❓정적 함수의 사용법
- static 키워드가 접두사로 붙은 함수가 정적 함수로 정의. static function은 동일한 소스 코드 내에서만 호출 됩니다.

<br/>


## ❓abs()와 fabs()의 차이
- abs는 정수값이고 fabs는 부동 유형 숫자에서 사용하는 절대값 방식입니다.

<br/>


## ❓[와일드 포인터란?](https://ko.wikipedia.org/wiki/%ED%97%88%EC%83%81_%ED%8F%AC%EC%9D%B8%ED%84%B0)
- C 코드에서 초기화되지 않은 포인터.
- 임의의 메모리 위치를 가리키며 잘 못된 프로그램 동작이나 충돌을 일으킬 수 있음

<br/>


## ❓C 프로그래밍에서 = 연산과 == 기호의 차이
- =연산은 오른쪽의 값을 왼쪽의 변수에 할당하는데 사용되는 할당 연산자
- ==는 왼쪽 값 또는 표현식을 오른쪽 값 또는 표현식과 비교하는 데 사용되는 비교 연산자

<br/>


## ❓프로토 타입 기능
- 정의 된 함수 이외 사용자가 정의하는 함수

<br/>


## ❓C 프로그래밍에서 헤더 파일과 그 사용법
- 프로그램에서 사용되는 함수의 정의와 프로토 타입이 포함 된 파일을 헤더 파일이라고 함.

<br/>


## ❓디버깅 할 때 삭제하는 것보다 주석 기호에 일부 코드 블록을 유지하는 코딩 방식이 있음. 디버깅 시 미치는 영향
- 주석 처리를 통해 오류의 가능한 원인을 스캔하는 코드를 분리할 수 있다,
- 또 주석 처리한 코드가 문제가 있을 경우에 복구를 하면되고
- 문제가 되는 코드라면 단순히 주석에서 제거하면 되므로 디버깅이 편리하다.

<br/>


## ❓C에서 함수를 정의하는 주요 섹션
- 반환 유형 : 함수 반환 값의 데이터 유형
- 기능 이름 : 기능의 이름과 기능의 활동을 설명하는 의미있는 이름을 갖는 것이 중요함
- 매개 변수 : 필요한 작업을 수행하는 데 사용되는 함수의 입력 값
- 기능 본체 : 필요한 작업을 수행하는 코드 모음

<br/>


## ❓포인터에 대한 포인터는?
- 

<br/>


## ❓break 문을 사용할 수 있는 유효한 위치는?
- loop 또는 switch에서만 사용 가능.

<br/>



## ❓헤더 파일이 ""와 <>에 포함되어 있을 때 동작상의 차이는?
- ""에 포함되면 컴파일러는 먼저 작업 디레고리에서 특정 헤더 파일을 검색함. 찾을 수 없는 경우 포함 경로에서 파일을 검색함
- 그러나 <> 안에 포함되면 컴파일러는 작업 디렉토리에서 특정 헤더 파일만 검색함

<br/>



## ❓C의 Modifier 변수에 대해서
- short, long, signed, unsigned

<br/>


## ❓서로에게 포인터를 추가 할 수 있는가?
- 포인터를 함께 추가 할 수 없습니다. 포인터에 주소 세부 정보가 포함되어 있으므로 이 작업에서 값을 검색 할 방법이 없음.

<br/>



## ❓모듈식 프로그래밍
- 메인 프로그램을 실행 가능한 하위 섹션으로 나누는 과정을 모듈 프로그래밍이라고 함.
- 이 개념은 재사용성을 촉진함.

<br/>
