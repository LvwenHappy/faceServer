#include "CTCPServer.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <fcntl.h>

CTCPServer::CTCPServer() : m_listenfd(0)
{

}

CTCPServer::~CTCPServer()
{

}

int CTCPServer::createListen(uint16_t port)
{
    struct sockaddr_in servaddr;

    //create TCP socket
    if( (m_listenfd = socket(AF_INET,SOCK_STREAM,0)) == -1) {
        printf(" create socket error: %s (errno :%d)\n",strerror(errno),errno);
        exit(0);
    }

    // clean bind "Address already in use" error
    int optval = 1;
    if(setsockopt(m_listenfd, SOL_SOCKET,  SO_REUSEADDR, &optval, sizeof(optval)) == -1)
    {
    	printf(" set socket error: %s (errno :%d)\n",strerror(errno),errno);
        exit(0);
    }
	
    //set servaddr to zero，detect any IP
    memset(&servaddr,0,sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons(port);

	
    //addr bind to listenfd
    if ( bind(m_listenfd, (struct sockaddr*)&servaddr, sizeof(servaddr)) == -1) {
        printf(" bind socket error: %s (errno :%d)\n",strerror(errno),errno);
        exit(0);
    }

    //listenfd
    if( listen(m_listenfd,1024) == -1) {
        printf(" listen socket error: %s (errno :%d)\n",strerror(errno),errno);
        exit(0);
    }

    //set listenfd to nonblocking
    setSocketNonBlock(m_listenfd);

    return m_listenfd;
}

int CTCPServer::getAccept(int fd)
{
    return accept(fd, NULL, NULL);
}

ssize_t CTCPServer::receiveData(int fd, char* buffer)
{
	int resNum = 0;
    int readNum = 0;
    printf("beginning read...\n");

    /*
    char cImgSize[5] = {0};
    readNum = read(fd, cImgSize, 5);
    if(readNum < -1)
        return -1;

    uint32_t count = *((uint32_t*)cImgSize);
    //printf("count: %d\n", count);
    */
    uint32_t count = 4;
    bool bFlag = true;
	while(readNum = read(fd, buffer, count))
	{
		printf("readNum: %d\n", readNum);
		
        if(bFlag)
        {
        	printf("count:\t%d--%d\n", count, readNum);
        	count = *((uint32_t*)buffer);
        	printf("count:\t%d--%d\n", count, readNum);
        	//count += 4;
        	//printf("count:\t%d--%d\n", count, readNum);
        	//if(readNum >= count)
        	//{
        	//	buffer[count] = '\0';
        	//	return count;
        	//}

        	resNum += readNum;
			//buffer += readNum;
        	//count -= readNum;
        	//printf("count:\t%d\n", count);
        	bFlag = false;
        	//break;
        	continue;
        }
        

		if(readNum < 0)
		{
            printf("readNum %d--%d--%s--%d\n", readNum, errno, strerror(errno), count);
            break;
			if(errno == EINTR || errno == EAGAIN)
				continue;
			else
				return -1;
		}
		else if(readNum == 0)
        {
            printf("readNum...: %d\n", readNum);
            break;
        }
        else
		{
            printf("readNum: %d--%s\n", readNum, buffer);
			resNum += readNum;
			buffer += readNum;
			count -= readNum;
			printf("after read count: %d\n", count);
			if(count <= 0)
				break;
            //break;        //tcpServerTest need;
		}
	}

	return resNum;
}

ssize_t CTCPServer::receiveData(int fd, char* buffer, size_t count)
{
    int resNum = 0;
    int readNum = 0;

    /*
    char cImgSize[5] = {0};
    readNum = read(fd, cImgSize, 5);
    if(readNum < -1)
        return -1;

    uint32_t count = *((uint32_t*)cImgSize);
    //printf("count: %d\n", count);
    */

	while(readNum = read(fd, buffer, count))
	{
        //printf("%s--%d--%d\n", buffer, readNum, errno);
		if(readNum < 0)
		{
            //printf("readNum %d--%d--%s\n", readNum, errno, strerror(errno));
			if(errno == EINTR || errno == EAGAIN)
				continue;
			else
				return -1;
		}
		else if(readNum == 0)
        {
            printf("readNum: %d\n", readNum);
            break;
        }
        else
		{
            printf("readNum: %d\n", readNum);
			resNum += readNum;
			buffer += readNum;
			count -= readNum;
            break;        //tcpServerTest need;
		}
	}

	return resNum;
}


ssize_t CTCPServer::sendData(int fd, char* buffer, size_t count)
{
	int resNum = 0;
	while(int writeNum = write(fd, buffer, count))
	{
		if(writeNum < 0)
		{
			if(errno == EINTR || errno == EAGAIN)
				continue;
			else
				return -1;
		}
		else
		{
			resNum += writeNum;
			buffer += writeNum;
			count -= writeNum;
            //break;        //tcpServerTest need;
		}
	}

	return resNum;
}

void CTCPServer::setSocketNonBlock(int fd)		//阻塞与非阻塞读的区别:  //阻塞和非阻塞的区别在于没有数据到达的时候是否立刻返回．
{
	//set for nonblock
    int flag = fcntl(fd, F_GETFL, 0);
    if(flag == -1)
    {
    	printf("fcntl get faild\n");
        exit(0);
    }

    flag |= O_NONBLOCK;
    if(fcntl(fd, F_SETFL, flag) == -1)
    {
    	printf("fcntl set faild\n");
        exit(0);
    }
}