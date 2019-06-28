#include "CTCPServer.h"
#include <stdio.h>

int main()
{
	CTCPServer* pTcpServer = new CTCPServer();
	int listenfd = pTcpServer->createListen(6666);
	char* buffer = new char[4096];
	int connectfd = pTcpServer->getAccept(listenfd);
	while(true)
	{
		ssize_t dataBytes = pTcpServer->receiveData(connectfd, buffer, 4096);
		printf("hhhhhhhhhh, %ld\n", dataBytes);
		buffer += '\0';
		printf("recv msg from client: %s\n", buffer);
		char sendBuf[] = "0,0;100,100";
		ssize_t sendBytes = pTcpServer->sendData(connectfd, sendBuf, sizeof(sendBuf));
		if(dataBytes != -1)
			break;
	}

	return 0;
}