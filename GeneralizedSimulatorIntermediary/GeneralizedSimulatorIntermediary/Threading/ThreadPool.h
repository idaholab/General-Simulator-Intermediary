//Copyright 2022, Battelle Energy Alliance, LLC
#pragma once

#include <thread>
#include <mutex>
#include <condition_variable>
#include <chrono>

using namespace std::chrono_literals;

#ifdef WINDOWS
#include <winsock2.h>
#include <ws2tcpip.h>
#pragma comment(lib,"Ws2_32.lib")
#endif

#include "../DataHandling/SharedResources/SharedResource.h"
#include "../DataHandling/InputScanning.h"

class ThreadPool
{
private:

	bool continueRunning;

	std::thread * mainPoolStorage;
	
	int numberOfThreads;
	std::mutex availabilityMutex;

	SOCKET* clientConnectionSockets;
	std::condition_variable* threadStops;
	std::mutex sharedMutex;

	SharedDataResource mainResource;
	void mainThreadLoop(int ThreadID);
public:

	ThreadPool(int numberOfThreads);
	~ThreadPool();

	void shutDown();

	bool assignConnectionToThread(SOCKET newClientSocket);

	SharedDataResource* getSharedDataResource();
};
