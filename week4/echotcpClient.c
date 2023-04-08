#include <stdio.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <string.h>
#define PORT 2650

int main()
{
  int clntSd;
  struct sockaddr_in clntAddr;
  int clntAddrLen, readLen;
  char wBuff[] = "한수호";
  char rBuff[BUFSIZ];

  // 소켓 생성 (IPv4, TCP 프로토콜(전송방식), (기본 TCP 프로토콜))
  clntSd = socket(PF_INET, SOCK_STREAM, 0);
  // 생성 실패 시 종료
  if (clntSd == -1)
  {
    printf("Socket Creation Error");
    return -1;
  }

  // 주소 지정
  clntAddr.sin_family = AF_INET;
  clntAddr.sin_addr.s_addr = inet_addr("127.0.0.1");
  clntAddr.sin_port = htons(PORT);

  // connect()를 통해 소켓에 연결 요청, 실패 시 종료 
  if (connect(clntSd, (struct sockaddr *)&clntAddr, sizeof(clntAddr)) == -1)
  {
    printf("Connect Error");
    close(clntSd);
    return -1;
  }

  // write()를 통해 버퍼에 내용 쓰기
  write(clntSd, wBuff, sizeof(wBuff));
  // 버퍼의 내용 출력
  printf("Client: %s\n", wBuff);

  // read()를 통해 버퍼의 내용 읽기, 실패 시 종료
  readLen = read(clntSd, rBuff, sizeof(rBuff) - 1);
  if (readLen == -1)
  {
    printf("Read Error");
    return -1;
  }
  rBuff[readLen] = '\0';

  // 읽어들인 내용 출력
  printf("Server: %s\n", rBuff);

  // 수신 소켓 종료
  close(clntSd);
  return 0;
}
