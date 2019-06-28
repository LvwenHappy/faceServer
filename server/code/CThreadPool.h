#ifndef __CTHREADPOOL_H__
#define __CTHREADPOOL_H__

#include "CTask.h"
#include <assert.h>
#include <pthread.h>

const int MAXTASK = 1024;
const int THREADNUM = MAXTASK;

class CThreadPool
{
public:
	CThreadPool();
	~CThreadPool();

private:
	static bool queueEmpty()
	{
		return m_taskQueueHead == m_taskQueueTail;
	}

	static bool queueFull()
	{
		return (m_taskQueueTail+1)%MAXTASK == m_taskQueueHead;
	}

	static CTask* getTask()
	{
		assert(!queueEmpty());

		CTask* res = m_taskQueue[m_taskQueueHead];
		m_taskQueueHead = (m_taskQueueHead+1)%MAXTASK;
		return res;
	}

	static void addTask(CTask* pTask)
	{
		assert(!queueFull());

		m_taskQueue[m_taskQueueTail] = pTask;
		m_taskQueueTail = (m_taskQueueTail+1)%MAXTASK;
	}

	static void delTask(CTask* pTask)
	{
		delete pTask;
	}

public:
	static void threadpool_add_job(CTask* pTask);

private:
	static void* threadpool_function(void* arg);


private:
	static CTask* m_taskQueue[THREADNUM];
	static int m_taskQueueHead;
	static int m_taskQueueTail;

private:
	static pthread_t* m_pThreads;
	static pthread_mutex_t m_mutex;
	static pthread_cond_t m_condQueueEmpty;
	static pthread_cond_t m_condQueueFull;

};

#endif