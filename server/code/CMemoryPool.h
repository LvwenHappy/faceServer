#ifndef __CMEMORYPOOL_H__
#define __CMEMORYPOOL_H__

#include <pthread.h>

class CMemoryPool
{
public:
	CMemoryPool(size_t blockNum = 1024, size_t blockSize = 4096);
	~CMemoryPool();

public:
	char* getMemoryBlock();
	void releaseMemoryBlock(char* ptr);

private:
	char* m_ptrHead;
	pthread_mutex_t m_mutex;
	pthread_cond_t m_condEmpty;
};

#endif