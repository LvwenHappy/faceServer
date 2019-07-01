#include "CEpoll.h"
#include <sys/epoll.h>
#include <stddef.h>
#include <errno.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

CEpoll::CEpoll()
	:	m_epollFd(0),
		m_pTcpServer(NULL),
		m_events(new epoll_event[MAXEVENTS]),
		m_pThreadPool(new CThreadPool())
{
	//Since Linux 2.6.8, the size argument is ignored, but must be greater than zero
	m_epollFd = epoll_create(1024);
	if (m_epollFd < 0)
    {
    	printf("epoll create error: %s (errno :%d)\n",strerror(errno),errno);
        exit(0);
    }


	m_pTcpServer = new CTCPServer();
	//default port 6666
	m_listenFd = m_pTcpServer->createListen(6666);

	//register events
	uint32_t events = EPOLLIN | EPOLLET;
	addEvent(m_listenFd, events);
}

CEpoll::~CEpoll()
{
	delete m_pTcpServer;
	delete [] m_events;
}

void CEpoll::mainLoop()
{
	while(true)
	{
		//Specifying a timeout of -1 causes epoll_wait() to block indefinitely
		//printf("waitting events...\n");
		//printf("%d\n", EFAULT);
		//printf("%d--%d--%d\n", EPOLLERR, EPOLLHUP, EPOLLIN);
		int eventNums = epoll_wait(m_epollFd, m_events, MAXEVENTS, -1);
		if (eventNums < 0)
	    {
	        perror("epoll wait error");
	        printf("epoll_wait errno: %d\n", errno);
	    }
		printf("eventNums: %d\n", eventNums);

		for(int i = 0; i < eventNums; ++i)
		{
			if(m_events[i].data.fd == m_listenFd)
			{

				int accept_fd = 0;
				while((accept_fd = m_pTcpServer->getAccept(m_events[i].data.fd)) > 0)
				{
					//set for nonblocking
					m_pTcpServer->setSocketNonBlock(accept_fd);

					uint32_t events = EPOLLIN | EPOLLET | EPOLLONESHOT;
        			addEvent(accept_fd, events);
				}
			}
			else
			{
				printf("m_events[i].events: %d\n", m_events[i].events);
				//EPOLLHUP epoll_wait(2) will always wait for this event; it is not necessary to set it in events.
				//EPOLLHUP: this event merely indicates that the peer closed its end of the channel.  Subsequent reads from the channel will return 0 (end of file) only after all outstanding data in the channel has been consumed.
				if ((m_events[i].events & EPOLLERR) || (m_events[i].events & EPOLLHUP))
				{
					printf("EPOLLERR || EPOLLHUP happend\n");
				}
				else
				{
					//create Task and transfer parameter include: accept_fd\m_TcpServer
					//use thread to process Task
					//printf("add jooooooooooooob...\n");
					CTask* pTask = new CTask(m_events[i].data.fd);
					m_pThreadPool->threadpool_add_job(pTask);
					//register acceptfd again
					//uint32_t events = EPOLLIN | EPOLLET | EPOLLONESHOT;
        			//modEvent(m_events[i].data.fd, events);

					//the following is the test program for epollTest.cc
					/*
					printf("accept_fd arrive!\n");
					char* buffer = new char[4096];
					int receiveNum = m_pTcpServer->receiveData(m_events[i].data.fd, buffer);
					printf("receive from client: %s\n", buffer+4);
					char sendBuf[] = "1,1:100,100";
					m_pTcpServer->sendData(m_events[i].data.fd, sendBuf, sizeof(sendBuf));
					//register acceptfd again
					uint32_t events = EPOLLIN | EPOLLET | EPOLLONESHOT;
        			modEvent(m_events[i].data.fd, events);
        			*/
				}
			}
		}
	}
}

void CEpoll::addEvent(int fd, uint32_t events)
{
	struct epoll_event event;
    event.data.fd = fd;
    event.events = events;
    //printf("add to epoll %d\n", fd);
    if(epoll_ctl(m_epollFd, EPOLL_CTL_ADD, fd, &event) < 0)
    {
        printf("epoll_add error");
        exit(0);
    }
}

void CEpoll::delEvent(int fd, uint32_t events)
{
    if(epoll_ctl(m_epollFd, EPOLL_CTL_DEL, fd, NULL) < 0)
    {
        printf("epoll_del error");
        exit(0);
    }
}

void CEpoll::modEvent(int fd, uint32_t events)
{
	struct epoll_event event;
    event.data.fd = fd;
    event.events = events;
    //printf("add to epoll %d\n", fd);
    if(epoll_ctl(m_epollFd, EPOLL_CTL_MOD, fd, &event) < 0)
    {
        printf("epoll_mod error");
        exit(0);
    }
}