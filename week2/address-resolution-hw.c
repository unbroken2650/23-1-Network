#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h> 

int printAddr(struct sockaddr_in *);
int main(char argc, char** argv) 
{
	char *sampleIP = "127.0.0.1";
	int port = 9002;

	struct sockaddr_in sockAddr1, sockAddr2, sockAddr3;

	sockAddr1.sin_family = AF_INET;
	sockAddr1.sin_addr.s_addr = inet_addr(sampleIP);
	sockAddr1.sin_port = htons(port);

	sockAddr2.sin_family = AF_INET;
	inet_aton(sampleIP,&(sockAddr2.sin_addr));
	sockAddr2.sin_port = htons(port);
	
	sockAddr3.sin_family = AF_INET;
	inet_pton(AF_INET, sampleIP, &(sockAddr3.sin_addr));
	sockAddr3.sin_port = htons(port);

	printAddr(&sockAddr1);
	printAddr(&sockAddr2);
	printAddr(&sockAddr3);
	
	printf("============ntoa==============\n");
	printf("IP:%s \n",inet_ntoa(sockAddr1.sin_addr));
	printf("IP:%s \n",inet_ntoa(sockAddr2.sin_addr));
	printf("IP:%s \n",inet_ntoa(sockAddr3.sin_addr));
	
	return 0;
}
int printAddr(struct sockaddr_in *myAddr)
{
	int port;
	port = ntohs(myAddr->sin_port);
	printf("IP:%s, Port:%d \n",inet_ntoa(myAddr->sin_addr), port);
	// IPv4 주소를 사용하므로 inet_ntop 함수를 inet_ntoa 함수로 변경
	return 0;
}
