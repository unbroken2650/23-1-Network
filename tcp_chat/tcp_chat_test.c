#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

#define PORT 9000
#define BACKLOG 5


int main() {
    int sockfd, newsockfd1, newsockfd2;
    socklen_t clilen;
    struct sockaddr_in serv_addr, cli_addr1, cli_addr2;

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        printf("ERROR opening socket");
    }

    memset((char *) &serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(PORT);

    if (bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0) {
        printf("ERROR on binding");
    }

    listen(sockfd, BACKLOG);

    clilen = sizeof(cli_addr1);
    newsockfd1 = accept(sockfd, (struct sockaddr *) &cli_addr1, &clilen);
    if (newsockfd1 < 0) {
        printf("ERROR on accept");
    }

    printf("Client 1 connected.\n");

    clilen = sizeof(cli_addr2);
    newsockfd2 = accept(sockfd, (struct sockaddr *) &cli_addr2, &clilen);
    if (newsockfd2 < 0) {
        printf("ERROR on accept");
    }

    printf("Client 2 connected.\n");

    pid_t pid = fork();
    if (pid < 0) {
        printf("ERROR on fork");
    }

    if (pid == 0) {
        
    } else {
        
    }

    close(newsockfd1);
    close(newsockfd2);
    close(sockfd);

    return 0;
}
