#include <arpa/inet.h>
#include <ctype.h>
#include <errno.h>
#include <netdb.h>
#include <netinet/in.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#define ISVALIDSOCKET(s) ((s) >= 0)
#define CLOSESOCKET(s) close(s)
#define SOCKET int
#define GETSOCKETERRNO() (errno)

int main() {
    printf("Configuring local address...\n");

    // 소켓 구성 설정을 위한 구조체 addrinfo를 선언하고 초기화
    struct addrinfo hints;
    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_INET;        // IPv4 주소 체계를 사용
    hints.ai_socktype = SOCK_STREAM;  // TCP 소켓을 사용
    hints.ai_flags = AI_PASSIVE;      // 수신 대기 모드

    // getaddrinfo 함수를 사용하여 소켓을 바인딩할 로컬 주소를 구성
    struct addrinfo *bind_address;
    getaddrinfo(0, "8080", &hints, &bind_address);

    printf("Creating socket...\n");

    // 로컬 주소를 사용하여 소켓을 생성
    SOCKET socket_listen;
    socket_listen = socket(bind_address->ai_family, bind_address->ai_socktype, bind_address->ai_protocol);

    // 소켓 생성이 실패한 경우 오류를 출력하고 프로그램을 종료
    if (!ISVALIDSOCKET(socket_listen)) {
        fprintf(stderr, "socket() failed. (%d)\n", GETSOCKETERRNO());
        return 1;
    }

    printf("Binding socket to local address...\n");

    // 소켓을 로컬 주소에 바인딩
    if (bind(socket_listen, bind_address->ai_addr, bind_address->ai_addrlen)) {
        fprintf(stderr, "bind() failed. (%d)\n", GETSOCKETERRNO());
        return 1;
    }

    // getaddrinfo 함수에서 할당한 메모리를 해제
    freeaddrinfo(bind_address);

    printf("Listening...\n");

    // 수신 대기 모드
    if (listen(socket_listen, 10) < 0) {
        fprintf(stderr, "listen() failed. (%d)\n", GETSOCKETERRNO());
        return 1;
    }

    // select 함수를 사용하여 여러 개의 소켓을 모니터링하기 위한 fd_set 구조체를 초기화
    fd_set master;
    FD_ZERO(&master);
    FD_SET(socket_listen, &master);
    SOCKET max_socket = socket_listen;

    printf("Waiting for connections...\n");

    // 클라이언트의 연결 대기
    while (1) {
        fd_set reads;
        reads = master;

        // select 함수를 사용하여 소켓 모니터링을 수행
        if (select(max_socket + 1, &reads, 0, 0, 0) < 0) {
            fprintf(stderr, "select() failed. (%d)\n", GETSOCKETERRNO());
            return 1;
        }
        SOCKET i;
        for (i = 1; i <= max_socket; ++i) {
            if (FD_ISSET(i, &reads)) {
                int client_port;
                if (i == socket_listen) {
                    struct sockaddr_storage client_address;
                    socklen_t client_len = sizeof(client_address);

                    // 클라이언트의 연결을 수락하고 새로운 클라이언트 소켓을 생성
                    SOCKET socket_client = accept(socket_listen, (struct sockaddr *)&client_address, &client_len);

                    // 클라이언트 소켓 생성이 실패한 경우 오류를 출력하고 프로그램을 종료
                    if (!ISVALIDSOCKET(socket_client)) {
                        fprintf(stderr, "accept() failed. (%d)\n", GETSOCKETERRNO());
                        return 1;
                    }

                    // 새로운 클라이언트 소켓을 모니터링 대상에 추가
                    FD_SET(socket_client, &master);

                    // 최대 소켓 번호를 업데이트
                    if (socket_client > max_socket) max_socket = socket_client;

                    // 클라이언트의 IP 주소와 포트를 문자열로 변환하여 출력
                    // NI_NUMERICSERV를 옵션으로 추가하여 클라이언트의 주소값도 저장한 후 출력
                    char address_buffer[100];
                    getnameinfo((struct sockaddr *)&client_address, client_len, address_buffer, sizeof(address_buffer), 0, 0, NI_NUMERICHOST | NI_NUMERICSERV);
                    client_port = ntohs(((struct sockaddr_in *)&client_address)->sin_port);
                    printf("New connection from Client %d (%s)\n", client_port, address_buffer);
                } else {
                    char read[1024];
                    int bytes_received = recv(i, read, 1024, 0);

                    // 수신된 데이터의 길이가 1보다 작은 경우, 클라이언트와의 연결을 종료
                    if (bytes_received < 1) {
                        FD_CLR(i, &master);
                        CLOSESOCKET(i);
                        continue;
                    }

                    SOCKET j;
                    for (j = 1; j <= max_socket; ++j) {
                        if (FD_ISSET(j, &master)) {
                            if (j == socket_listen || j == i)
                                continue;
                            else {
                                // Send the message along with the port number
                                char message[1024];
                                sprintf(message, "Client %d: %s", client_port, read);
                                send(j, message, strlen(message), 0);
                            }
                        }
                    }
                }
            }
        }
    }

    printf("Closing listening socket...\n");

    // 소켓을 닫습니다.
    CLOSESOCKET(socket_listen);

    printf("Finished.\n");

    return 0;
}