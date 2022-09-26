# Qt window Application에 라이브러리를 포함하여 만든 샘플 예제  

해당 소스코드는 https://m.blog.naver.com/PostView.naver?isHttpsRedirect=true&blogId=browniz1004&logNo=221316244185 출처이며  
해당 소스에서 동작하지 않는 버튼 이벤트 함수를 수정하여 테스트함  


개발환경 : Qt Creator 6.0.2  
컴파일 환경 : Qt 6.2.3 msvc 2019 64bits  
언어 : C++ ( 내가 만든 프로그램은 C/C++ )


![캡처](https://user-images.githubusercontent.com/91730236/157051057-53e364bd-b37f-4019-b868-11f711158dea.PNG)


--------------------------------------------
회사에서 개발한 내용을 올릴 수는 없지만...  

#내가 개선한 부분  
      -해당 프로그램은 sftp 처리과정에서 예외처리 (IP 형식, remote 주소 등)   
      -별도 파일로 모듈화하여 적용  
      -sftp로 write하는 파일에 대해서 전송의 무결성이 보장되는지 체크  
      -sftp로 read하는 부분과 shell 명령 전송 후 결과 return 기능  
      -write하는 file size malloc으로 동적 메모리 사용 ( 파일크기가 매우 큰 경우도 있음 )  
