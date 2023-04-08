#include <stdio.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>
#include <arpa/inet.h>

void errProc(const char *);
int main(int argc, char **argv)
{
	int mySock, readLen, nRecv, res;
	char buff[BUFSIZ];
	char *strAddr;
	struct sockaddr_in srcAddr, destAddr;
	socklen_t addrLen;

	if (argc != 2)
	{
		fprintf(stderr, "Usage: %s Port", argv[0]);
		return 0;
	}
	// 소켓 생성 (IPv4, UDP 프로토콜(전송방식), UDP 프로토콜(프로토콜))
	mySock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);

	// 소켓 생성 실패 시 종료
	if (mySock == -1)
		errProc("socket");

	// 주소 지정
	memset(&srcAddr, 0, sizeof(srcAddr));
	srcAddr.sin_addr.s_addr = htonl(INADDR_ANY);
	srcAddr.sin_family = AF_INET;
	srcAddr.sin_port = htons(atoi(argv[1]));

	// bind()를 통해 소켓에 주소를 설정
	res = bind(mySock, (struct sockaddr *)&srcAddr,
						 sizeof(srcAddr));

	// 주소 지정 실패 시 종료
	if (res == -1)
		errProc("bind");
	addrLen = sizeof(destAddr);

	while (1)
	{
		//메시지 수신 시작
		nRecv = recvfrom(mySock, buff, BUFSIZ - 1, 0,
										 (struct sockaddr *)&destAddr,
										 &addrLen);
		//수신 실패 시 종료
		if (nRecv == -1)
			errProc("recvfrom");
		// 수신 성공 시 nRecv에 메시지의 길이가 저장되어 마지막 자리를 공백문자로 저장
		if (nRecv > 0)
			buff[nRecv - 1] = '\0';
		else
			buff[nRecv] = '\0';

		strAddr = inet_ntoa(destAddr.sin_addr);

		// A 주소의 B 포트로 온 메시지 C를 출력
		printf("%s:%d>%s\n", strAddr, ntohs(destAddr.sin_port), buff);
		nRecv = strlen(buff);

		// 그대로 재전송
		sendto(mySock, buff, nRecv, 0,
					 (struct sockaddr *)&destAddr, addrLen);
	}
	// 통신 종료
	close(mySock);
	return 0;
}

void errProc(const char *str)
{
	fprintf(stderr, "%s: %s \n", str, strerror(errno));
	exit(1);
}