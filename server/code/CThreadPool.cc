#include "CThreadPool.h"
#include <stdio.h>
#include <stdlib.h>

CTask* CThreadPool::m_taskQueue[THREADNUM] = {NULL};
int CThreadPool::m_taskQueueHead = 0;
int CThreadPool::m_taskQueueTail = 0;

pthread_t* CThreadPool::m_pThreads = new pthread_t[THREADNUM];
pthread_mutex_t CThreadPool::m_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t CThreadPool::m_condQueueEmpty = PTHREAD_COND_INITIALIZER;
pthread_cond_t CThreadPool::m_condQueueFull = PTHREAD_COND_INITIALIZER;

CThreadPool::CThreadPool()
{
	/*
	for(int i = 0; i < THREADNUM; ++i)
	{
		m_taskQueue[i] = new CTask(0);
	}
	*/

	/*
	if (pthread_mutex_init(&m_mutex, NULL))
    {
        printf("failed to init m_mutex!\n");
        exit(0);
    }

    if (pthread_cond_init(&m_condQueueEmpty, NULL))
    {
        printf("failed to init m_condQueueEmpty!\n");
        exit(0);
    }

    if (pthread_cond_init(&m_condQueueFull, NULL))
    {
        printf("failed to init m_condQueueFull!\n");
        exit(0);
    }
    */

    for(int i = 0; i < THREADNUM; ++i)
    {
    	pthread_create(&m_pThreads[i], NULL, threadpool_function, NULL);
    }
}

CThreadPool::~CThreadPool()
{
	//delete[] m_taskQueue;
}

void CThreadPool::threadpool_add_job(CTask* pTask)
{
	//printf("add job...\n");
	pthread_mutex_lock(&m_mutex);
	while(queueFull())
	{
	//	printf("queue full...\n");
		pthread_cond_wait(&m_condQueueFull, &m_mutex);
	//	printf("queue not full...\n");
	}
	//printf("queue nooooooooooot full...\n");

	if(queueEmpty())
	{
	//	printf("begin pthread_cond_broadcast...\n");
		pthread_cond_broadcast(&m_condQueueEmpty);
	//	printf("end pthread_cond_broadcast...\n");
	}
	addTask(pTask);
	//printf("ennnnnnnnd pthread_cond_broadcast...\n");
	pthread_mutex_unlock(&m_mutex);
}

void* CThreadPool::threadpool_function(void* arg)
{
	//printf("thread function...\n");
	while(true)
	{
	//	printf("thread function wait...\n");
		pthread_mutex_lock(&m_mutex);
		while(queueEmpty())
		{
	//		printf("queueEmpty...\n");
			pthread_cond_wait(&m_condQueueEmpty, &m_mutex);
	//		printf("queue has job...\n");
		}
	//	printf("queue hsa joooooob...\n");

		if(queueFull())
			pthread_cond_broadcast(&m_condQueueFull);

		CTask* pTask = getTask();
		pthread_mutex_unlock(&m_mutex);

		pTask->processTask();

		delTask(pTask);

		//register acceptfd again
		//uint32_t events = EPOLLIN | EPOLLET | EPOLLONESHOT;
		//modEvent(m_events[i].data.fd, events);
	}
}