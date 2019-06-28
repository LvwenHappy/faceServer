#include "CMemoryPool.h"
#include <stdio.h>
#include <unistd.h>
#include <pthread.h>

CMemoryPool* ptr = new CMemoryPool(2);

void* threadFun1(void* arg)
{
	char* ptrTmp = ptr->getMemoryBlock();
	sleep(1);
	ptrTmp = ptr->getMemoryBlock();
}

void* threadFun2(void* arg)
{
	char* ptrTmp = ptr->getMemoryBlock();
	sleep(2);
	ptr->releaseMemoryBlock(ptrTmp);
}

int main()
{
	pthread_t t1, t2;
	pthread_create(&t1, NULL, threadFun1, NULL);
	pthread_create(&t2, NULL, threadFun2, NULL);

	pthread_join(t1, NULL);
	pthread_join(t2, NULL);
	

	return 0;
}