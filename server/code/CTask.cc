#include "CTask.h"

CMemoryPool* CTask::m_ptrMemoryPool = new CMemoryPool(BLOCKNUM, BLOCKSIZE);
CLenet* CTask::m_ptrLenet = new CLenet();
CTCPServer* CTask::m_ptrTcpServer = new CTCPServer();

CTask::CTask(int connectfd)
{
	m_connectfd = connectfd;
}

CTask::~CTask()
{
	//delete m_ptrMemoryPool;
}

void CTask::processTask()
{
	printf("CTask::processTask!\n");
	//get memory block
	char* pDataBuffer = getMemoryBlock();

	//receive data
	receData(pDataBuffer);

	//process data
	printf("beginning process data...\n");
	float fRes = 0.999876;

	//send result
	printf("beginning send data...\n");
	sendData((char*)&fRes, sizeof(fRes));
	printf("end send data...\n");


	releaseMemoryBlock(pDataBuffer);
}

char* CTask::getMemoryBlock()
{
	return m_ptrMemoryPool->getMemoryBlock();
}

void CTask::releaseMemoryBlock(char* pBuffer)
{
	return m_ptrMemoryPool->releaseMemoryBlock(pBuffer);
}

ssize_t CTask::receData(char* buffer)
{
	return m_ptrTcpServer->receiveData(m_connectfd, buffer);
}

ssize_t CTask::sendData(char* buffer, size_t count)
{
	return m_ptrTcpServer->sendData(m_connectfd, buffer, count);
}