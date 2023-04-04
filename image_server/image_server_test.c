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

int main() {
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

    memset((char *) &serv_addr, 0, sizeof(serv_addr));
    portno = PORT;

    // 서버 주소 및 포트 할당

    // 서버 소켓 바인드

   
    // 서버 listen
    listen(sockfd, 5);

    // 클라이언트 요청 accept
   

    // 클라이언트 패킷 수신
    memset(buffer, 0, BUFSIZE);
    n = read(newsockfd, buffer, BUFSIZE);
    if (n < 0) 
        printf("ERROR reading from socket");

    // 파일 오픈
    
    // 파일 크기 확인 (stat 함수 사용 하여 st 구조체에 저장, 파일 크기는 file_size 변수에 저장)

    // 파일 버퍼 동적 할당 (malloc)

    // 파일 읽기


    // http header 만들기
    sprintf(http_header, "HTTP/1.1 200 OK\r\n"
            "Content-Type: image/jpeg\r\n"
            "Content-Length: %ld\r\n"
            "Connection: close\r\n"
            "\r\n", file_size);

    // http header 전송
   

    // http body 전송
   

    fclose(fp);
    free(file_buffer);

    close(newsockfd);
    close(sockfd);

    return 0;
}
