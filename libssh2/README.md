# libssh2

link : https://libssh2.org/

ver : libssh2 1.10.0 from August 29th 2021  
cmake : 3.23.0-rc2-windows-x86_64  
release compile : Visual studio 2019  

사용법  
lib에 저장된 .h 파일과 dll을 프로젝트에 추가하여 사용하면 됩니다.  
  
    
      
      
---------------------
컴파일 방법  
1. cmake 설치
2. cmake-gui.exe 실행
3. libssh2 1.10.0 압축 풀기
4. libssh 압축 푼 경로에 build 디렉토리 만들기
5. cmake-gui.exe 창에서 where is  the source code : 에 압축 푼 libssh2 폴더를 지정
6. cmake-gui.exe 창에 where to build the binaries : 에 압축 푼 libssh2 폴더의 build 폴더 설정(아까 만든 것)
7. 하단 Configue 버튼 실행
8. 설치 된 Visual Studio Version 선택후 Use default native compilers 선택
9. finish 누른다.
10. 진행이 완료되면 구성요소가 불완전한 목록이 빨간색으로 뜸
11. BUILD_SHARED_LIBS 체크하고 Generate
12. 완료가 되면 아까 만든 build 폴더에 VC로 컴파일 가능한 파일 생성
13. libssh2.sln 실행하여 비쥬얼 스튜디오에서 Release 빌드 진행
14. build/Src/Release 경로에서 dll과 lib 생성 확인 가능
15. libssh2/include에 포함된 .h파일 3개와 위 dll, lib파일 3개, /build/scr의 libssh2_config.h 1개 총 7개 파일을 프로젝트 적용



---------------------------

요즘엔 자바나 파이썬에 구현된 라이브러리들이 좋기도 하고, 온프레미스 환경이 아니고서 크게 잘 사용하지는 않는듯함
