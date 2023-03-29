#include <stdio.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <string.h>
#define PORT 9001

int main()
{
	int clntSd;
	struct sockaddr_in clntAddr;
	int clntAddrLen, readLen;
	char wBuff[] = "How old are you?";
	char rBuff[BUFSIZ];

	clntSd = socket(PF_INET, SOCK_STREAM, 0);
	if (clntSd == -1)
	{
		printf("Socket Creation Error");
		return -1;
	}
	clntAddr.sin_family = AF_INET;
	clntAddr.sin_addr.s_addr = inet_addr("127.0.0.1");
	clntAddr.sin_port = htons(PORT);

	if (connect(clntSd, (struct sockaddr *)&clntAddr, sizeof(clntAddr)) == -1)

	{
		printf("Connect Error");
		close(clntSd);
		return -1;
	}

	send(clntSd, wBuff, sizeof(wBuff), 0);
	// write 함수를 send 함수로 변경, flags(0) 추가
	printf("Client: %s\n", wBuff);

	readLen = recv(clntSd, rBuff, sizeof(rBuff) - 1, 0);
	// read 함수를 recv 함수로 변경, flags(0) 추가 
	if (readLen == -1)
	{
		printf("Read Error");
		return -1;
	}
	rBuff[readLen] = '\0';
	printf("Server: %s\n", rBuff);
	close(clntSd);
	return 0;
}
