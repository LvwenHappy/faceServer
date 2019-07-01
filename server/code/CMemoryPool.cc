#include "CMemoryPool.h"
#include <stdio.h>
#include <stdlib.h>


CMemoryPool::CMemoryPool(size_t blockNum, size_t blockSize)
	: m_ptrHead(NULL)
{
	if (pthread_mutex_init(&m_mutex, NULL))
	{
		printf("failed to init m_mutex!\n");
		exit(0);
	}

	if (pthread_cond_init(&m_condEmpty, NULL))
	{
		printf("failed to init m_condEmpty!\n");
		exit(0);
	}


	char* ptr = NULL;
	for(int i = 0; i < blockNum; ++i)
	{
		//printf("i: %d\n", i);
		ptr = new char[blockSize];
		//printf("addr: %ld\n", (long)ptr);
		*((char**)&(*ptr)) = m_ptrHead;
		m_ptrHead = ptr;
	}
	/*
	ptr = m_ptrHead;
	for(int i = 0; i < blockNum; ++i)
	{
		printf("i: %d, addr: %ld\n", i, (long)ptr);
		ptr = *((char**)&(*ptr));
	}
	printf("addr: %ld\n", (long)ptr);
	*/
}

CMemoryPool::~CMemoryPool()
{
	
}

char* CMemoryPool::getMemoryBlock()
{
	pthread_mutex_lock(&m_mutex);
    while (m_ptrHead == NULL)				//Spurious wakeups: https://blog.csdn.net/chrdww/article/details/78031060
    {
    	printf("wait for block...\n");
        pthread_cond_wait(&m_condEmpty, &m_mutex);		//has not memory block
        printf("get block\n");
    }

    char* ptr = m_ptrHead;
    m_ptrHead = *((char**)&(*m_ptrHead));

    pthread_mutex_unlock(&m_mutex);

    return ptr;
}

void CMemoryPool::releaseMemoryBlock(char* ptr)
{
	pthread_mutex_lock(&m_mutex);

	*((char**)&(*ptr)) = m_ptrHead;
	m_ptrHead = ptr;

	pthread_mutex_unlock(&m_mutex);

	if(*((char**)&(*m_ptrHead)) == NULL)
	{
		printf("release one block\n");
		pthread_cond_broadcast(&m_condEmpty);
	}
}