server : server.cc CDaemon.h CDaemon.cc
	g++ -o server server.cc CDaemon.h CDaemon.cc

memoryPoolTest : memoryPoolTest.cc CMemoryPool.h CMemoryPool.cc
	g++ -o memoryPoolTest memoryPoolTest.cc CMemoryPool.h CMemoryPool.cc -lpthread

tcpServerTest : tcpServerTest.cc CTCPServer.h CTCPServer.cc
	g++ -o tcpServerTest tcpServerTest.cc CTCPServer.h CTCPServer.cc

epollTest : epollTest.cc CEpoll.h CEpoll.cc CTCPServer.h CTCPServer.cc CThreadPool.h CThreadPool.cc CTask.h CTask.cc CLenet.h CLenet.cc CMemoryPool.h CMemoryPool.cc
	g++ -o epollTest epollTest.cc CEpoll.h CEpoll.cc CTCPServer.h CTCPServer.cc CThreadPool.h CThreadPool.cc CTask.h CTask.cc CLenet.h CLenet.cc CMemoryPool.h CMemoryPool.cc -lpthread

clean : 
	rm -rf server