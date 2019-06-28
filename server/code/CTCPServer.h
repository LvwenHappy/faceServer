#ifndef __CTCPSERVER_H__
#define __CTCPSERVER_H__

#include <arpa/inet.h>

class CTCPServer
{
public:
	CTCPServer();
	~CTCPServer();
	int createListen(uint16_t port = 6666);
	int getAccept(int fd);
	ssize_t receiveData(int acceptfd, char* buffer, size_t count);
	ssize_t receiveData(int acceptfd, char* buffer);
	ssize_t sendData(int fd, char* buffer, size_t count);

public:
	void setSocketNonBlock(int fd);

private:
	int m_listenfd;
};

#endif