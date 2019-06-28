#ifndef __CTASK_H__
#define __CTASK_H__

#include "CMemoryPool.h"
#include "CTCPServer.h"
#include "CLenet.h"

const int PAGESIZE = 4096;
const int BLOCKNUM = 1024;
const int BLOCKSIZE = PAGESIZE*5;

class CTask
{
public:
	CTask(int connectfd);
	~CTask();

public:
	void processTask();

private:
	char* getMemoryBlock();
	void releaseMemoryBlock(char* pBuffer);
	ssize_t receData(char* buffer);
	ssize_t sendData(char* buffer, size_t count);

private:
	static CMemoryPool* m_ptrMemoryPool;
	static CLenet* m_ptrLenet;
	static CTCPServer* m_ptrTcpServer;

private:
	

private:
	//CTCPServer* m_pTcpServer;
	int m_connectfd;
};

#endif