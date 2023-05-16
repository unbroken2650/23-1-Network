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

    // ���� ���� ������ ���� ����ü addrinfo�� �����ϰ� �ʱ�ȭ
    struct addrinfo hints;
    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_INET;        // IPv4 �ּ� ü�踦 ���
    hints.ai_socktype = SOCK_STREAM;  // TCP ������ ���
    hints.ai_flags = AI_PASSIVE;      // ���� ��� ���

    // getaddrinfo �Լ��� ����Ͽ� ������ ���ε��� ���� �ּҸ� ����
    struct addrinfo *bind_address;
    getaddrinfo(0, "8080", &hints, &bind_address);

    printf("Creating socket...\n");

    // ���� �ּҸ� ����Ͽ� ������ ����
    SOCKET socket_listen;
    socket_listen = socket(bind_address->ai_family, bind_address->ai_socktype, bind_address->ai_protocol);

    // ���� ������ ������ ��� ������ ����ϰ� ���α׷��� ����
    if (!ISVALIDSOCKET(socket_listen)) {
        fprintf(stderr, "socket() failed. (%d)\n", GETSOCKETERRNO());
        return 1;
    }

    printf("Binding socket to local address...\n");

    // ������ ���� �ּҿ� ���ε�
    if (bind(socket_listen, bind_address->ai_addr, bind_address->ai_addrlen)) {
        fprintf(stderr, "bind() failed. (%d)\n", GETSOCKETERRNO());
        return 1;
    }

    // getaddrinfo �Լ����� �Ҵ��� �޸𸮸� ����
    freeaddrinfo(bind_address);

    printf("Listening...\n");

    // ���� ��� ���
    if (listen(socket_listen, 10) < 0) {
        fprintf(stderr, "listen() failed. (%d)\n", GETSOCKETERRNO());
        return 1;
    }

    // select �Լ��� ����Ͽ� ���� ���� ������ ����͸��ϱ� ���� fd_set ����ü�� �ʱ�ȭ
    fd_set master;
    FD_ZERO(&master);
    FD_SET(socket_listen, &master);
    SOCKET max_socket = socket_listen;

    printf("Waiting for connections...\n");

    // Ŭ���̾�Ʈ�� ���� ���
    while (1) {
        fd_set reads;
        reads = master;

        // select �Լ��� ����Ͽ� ���� ����͸��� ����
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

                    // Ŭ���̾�Ʈ�� ������ �����ϰ� ���ο� Ŭ���̾�Ʈ ������ ����
                    SOCKET socket_client = accept(socket_listen, (struct sockaddr *)&client_address, &client_len);

                    // Ŭ���̾�Ʈ ���� ������ ������ ��� ������ ����ϰ� ���α׷��� ����
                    if (!ISVALIDSOCKET(socket_client)) {
                        fprintf(stderr, "accept() failed. (%d)\n", GETSOCKETERRNO());
                        return 1;
                    }

                    // ���ο� Ŭ���̾�Ʈ ������ ����͸� ��� �߰�
                    FD_SET(socket_client, &master);

                    // �ִ� ���� ��ȣ�� ������Ʈ
                    if (socket_client > max_socket) max_socket = socket_client;

                    // Ŭ���̾�Ʈ�� IP �ּҿ� ��Ʈ�� ���ڿ��� ��ȯ�Ͽ� ���
                    // NI_NUMERICSERV�� �ɼ����� �߰��Ͽ� Ŭ���̾�Ʈ�� �ּҰ��� ������ �� ���
                    char address_buffer[100];
                    getnameinfo((struct sockaddr *)&client_address, client_len, address_buffer, sizeof(address_buffer), 0, 0, NI_NUMERICHOST | NI_NUMERICSERV);
                    client_port = ntohs(((struct sockaddr_in *)&client_address)->sin_port);
                    printf("New connection from Client %d (%s)\n", client_port, address_buffer);
                } else {
                    char read[1024];
                    int bytes_received = recv(i, read, 1024, 0);

                    // ���ŵ� �������� ���̰� 1���� ���� ���, Ŭ���̾�Ʈ���� ������ ����
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

    // ������ �ݽ��ϴ�.
    CLOSESOCKET(socket_listen);

    printf("Finished.\n");

    return 0;
}