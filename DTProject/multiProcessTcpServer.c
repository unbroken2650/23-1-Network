#include <arpa/inet.h>
#include <errno.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>

void errProc();
void errPrint();
int main(int argc, char **argv) {
    int srvSd, clntSd;
    struct sockaddr_in srvAddr, clntAddr;
    int clntAddrLen, readLen, strLen;
    char rBuff[BUFSIZ];
    pid_t pid;

    // 서버 시작
    if (argc != 2) {
        printf("Usage: %s [port] \n", argv[0]);
        exit(1);
    }
    printf("Server start...\n");

    // 소켓 생성 및 주소 지정
    srvSd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (srvSd == -1) {
        errProc();
    }
    memset(&srvAddr, 0, sizeof(srvAddr));
    srvAddr.sin_addr.s_addr = htonl(INADDR_ANY);
    srvAddr.sin_family = AF_INET;
    srvAddr.sin_port = htons(atoi(argv[1]));

    // Binding + Listening
    if (bind(srvSd, (struct sockaddr *)&srvAddr, sizeof(srvAddr)) == -1) {
        errProc();
    }
    if (listen(srvSd, 5) < 0) {
        errProc();
    }

    // 자식 프로세스와 통신
    clntAddrLen = sizeof(clntAddr);
    while (1) {
        clntSd = accept(srvSd, (struct sockaddr *)&clntAddr, &clntAddrLen);
        if (clntSd == -1) {
            errPrint();
            continue;
        }
        printf("client %s:%d is connected...\n", inet_ntoa(clntAddr.sin_addr), ntohs(clntAddr.sin_port));
        pid = fork();
        if (pid == 0) { /* 자식 프로세스에 수행할 작업 */
            close(srvSd);
            while (1) {
                readLen = read(clntSd, rBuff, sizeof(rBuff) - 1);
                if (readLen == 0) break;
                rBuff[readLen] = '\0';
                printf("Client(%d): %s\n", ntohs(clntAddr.sin_port), rBuff);
                write(clntSd, rBuff, strlen(rBuff));
            }
            printf("Client(%d): is disconnected\n", ntohs(clntAddr.sin_port));
            close(clntSd);
            return 0;
        } else if (pid == -1) /*자식 프로세스 생성 실패*/
            errProc("fork");
        else { /* 부모 프로세스에서 수행할 작업*/
            close(clntSd);
        }
    }
    close(srvSd);
    return 0;
}

void errProc(const char *str) {
    fprintf(stderr, "%s: %s \n", str, strerror(errno));
    exit(1);
}

void errPrint(const char *str) { fprintf(stderr, "%s: %s \n", str, strerror(errno)); }
