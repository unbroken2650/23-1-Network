#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <netinet/in.h>
#include <sys/socket.h>

#define PORT 8080
#define BUFSIZE 1024

int main()
{
  int sockfd, newsockfd, portno, clilen, n;
  char buffer[BUFSIZE];
  struct sockaddr_in serv_addr, cli_addr;
  FILE *fp;
  struct stat st;
  char *file_buffer;
  size_t file_size;
  char http_header[BUFSIZE];

  sockfd = socket(AF_INET, SOCK_STREAM, 0);
  if (sockfd < 0)
    printf("socket error");

  memset((char *)&serv_addr, 0, sizeof(serv_addr));
  portno = PORT;

  // 서버 주소 및 포트 할당
  serv_addr.sin_family = AF_INET;
  serv_addr.sin_addr.s_addr = INADDR_ANY;
  serv_addr.sin_port = htons(portno);

  // 서버 소켓 바인드
  if (bind(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
    printf("bind error");

  // 서버 listen
  listen(sockfd, 5);

  // 클라이언트 요청 accept
  clilen = sizeof(cli_addr);
  newsockfd = accept(sockfd, (struct sockaddr *)&cli_addr, (socklen_t *)&clilen);
  if (newsockfd < 0) // accept error
    printf("accept error");

  // 클라이언트 패킷 수신
  memset(buffer, 0, BUFSIZE);
  n = read(newsockfd, buffer, BUFSIZE);
  if (n < 0) // reading error
    printf("reading error");

  // 파일 오픈
  fp = fopen("image.jpg", "rb");
  if (fp == NULL)
  { // file open error
    printf("file open error: %s\n", strerror(errno));
    return -1;
  }

  // 파일 크기 확인 (stat 함수 사용 하여 st 구조체에 저장, 파일 크기는 file_size 변수에 저장)
  if (stat("image.jpg", &st) == -1)
  { // state() error
    printf("stat() error: %s\n", strerror(errno));
    fclose(fp);
    return -1;
  }
  file_size = st.st_size;

  // 파일 버퍼 동적 할당 (malloc)
  file_buffer = (char *)malloc(file_size);
  if (file_buffer == NULL)
  { // malloc error
    printf("malloc error\n");
    fclose(fp);
    return -1;
  }

  // 파일 읽기
  if (fread(file_buffer, file_size, 1, fp) != 1)
  { // reading error
    printf("file reading error\n");
    fclose(fp);
    free(file_buffer);
    return -1;
  }

  // http header 만들기
  sprintf(http_header, "HTTP/1.1 200 OK\r\n"
                       "Content-Type: image/jpeg\r\n"
                       "Content-Length: %ld\r\n"
                       "Connection: close\r\n"
                       "\r\n",
          file_size);

  // http header 전송
  if (write(newsockfd, http_header, strlen(http_header)) < 0)
  { // write() error
    printf("http header write() error\n");
    fclose(fp);
    free(file_buffer);
    return -1;
  }

  // http body 전송
  if (write(newsockfd, file_buffer, file_size) < 0)
  { // write() error
    printf("http body write() error\n");
    fclose(fp);
    free(file_buffer);
    return -1;
  }
  // so much time crycry
  fclose(fp);
  free(file_buffer);

  close(newsockfd);
  close(sockfd);

  return 0;
}