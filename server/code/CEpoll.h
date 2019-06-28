#ifndef __CEPOLL_H__
#define __CEPOLL_H__

#include "CTCPServer.h"
#include "CThreadPool.h"
#include "CTask.h"
#include <stddef.h>
#include <sys/epoll.h>

const int MAXEVENTS = 1024;

class CEpoll
{
private:
	

public:
	CEpoll();
	~CEpoll();

public:
	void mainLoop();

public:
	void addEvent(int fd, uint32_t events);
	void delEvent(int fd, uint32_t events);
	void modEvent(int fd, uint32_t events);

private:
	CThreadPool* m_pThreadPool;

private:
	int m_epollFd;
	struct epoll_event* m_events;

private:
	int m_listenFd;
	CTCPServer* m_pTcpServer;
};

#endif