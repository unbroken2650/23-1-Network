#include <stdio.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <string.h>
#include <arpa/inet.h>
#define PORT 2650

int main()
{
  int srvSd, clntSd;
  struct sockaddr_in srvAddr, clntAddr;
  int clntAddrLen, readLen;
  char rBuff[BUFSIZ];
  char wBuff[] = "20192650";
  
  // Server에서 보내는 메시지 출력
  printf("Server: %s\n", wBuff);

  // 소켓 생성 (IPv4, TCP 프로토콜(전송방식), (기본 TCP 프로토콜))
  srvSd = socket(AF_INET, SOCK_STREAM, 0);
  // 생성 실패 시 종료
  if (srvSd == -1)
  {
    printf("Socket Error\n");
    return -1;
  }

  // 주소 지정
  memset(&srvAddr, 0, sizeof(srvAddr));
  srvAddr.sin_family = AF_INET;
  srvAddr.sin_addr.s_addr = htonl(INADDR_ANY);
  srvAddr.sin_port = htons(PORT);

  // bind()를 통해 소켓에 주소를 설정, 주소 지정 실패 시 종료
  if (bind(srvSd, (struct sockaddr *)&srvAddr, sizeof(srvAddr)) == -1)
  {
    printf("Bind Error");
    return -1;
  }

  // listen()을 통해 소켓에 연결요청 대기, 실패 시 종료
  if (listen(srvSd, 5) == -1)
  {
    printf("Listen Error");
    return -1;
  }

  // accept()을 통해 clntAddr의 접속 허용, 실패 시 종료
  clntAddrLen = sizeof(clntAddr);
  clntSd = accept(srvSd, (struct sockaddr *)&clntAddr, &clntAddrLen);
  if (clntSd == -1)
  {
    printf("Accept Error");
    return -1;
  }

  // read()를 통해 버퍼의 내용 읽기, 실패 시 종료
  readLen = read(clntSd, rBuff, sizeof(rBuff) - 1);
  if (readLen == -1)
  {
    printf("Read Error");
    return -1;
  }
  rBuff[readLen] = '\0';
  // 읽어들인 내용 출력
  printf("Client: %s \n", rBuff);

  // 서버의 메시지 전송
  write(clntSd, wBuff, sizeof(wBuff));
  // 수신 소켓 종료
  close(clntSd);
  // 송신 소켓 종료
  close(srvSd);

  return 0;
}
