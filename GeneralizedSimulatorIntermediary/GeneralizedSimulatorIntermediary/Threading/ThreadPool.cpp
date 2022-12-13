//Copyright 2022, Battelle Energy Alliance, LLC
#include "ThreadPool.h"

ThreadPool::ThreadPool(int numberOfThreads)
{
	this->numberOfThreads = numberOfThreads;
	continueRunning = true;
	mainPoolStorage = new std::thread[numberOfThreads];
	clientConnectionSockets = new SOCKET[numberOfThreads];
	threadStops = new std::condition_variable[numberOfThreads];

	for (int i = 0; i < numberOfThreads; i++)
	{
		clientConnectionSockets[i] = INVALID_SOCKET;
		mainPoolStorage[i] = std::thread([this, i] {this->mainThreadLoop(i); });
	}
}

ThreadPool::~ThreadPool()
{
	shutDown();

	for (int i = 0; i < numberOfThreads; i++)
	{
		mainPoolStorage[i].join();
	}

	delete[] mainPoolStorage;
	delete[] clientConnectionSockets;
	delete[] threadStops;
}

void ThreadPool::shutDown()
{
	continueRunning = false;

	for (int i = 0; i < numberOfThreads; i++)
	{
		threadStops[i].notify_all();
	}
}

#define DEFAULT_BUFLEN 512

void ThreadPool::mainThreadLoop(int ThreadID)
{
	char recvbuf[DEFAULT_BUFLEN]{};
	int iResult;
	int iSendResult;
	int recvbuflen = DEFAULT_BUFLEN;

	while (continueRunning)
	{

		std::unique_lock<std::mutex> lock(sharedMutex);
		while (continueRunning && clientConnectionSockets[ThreadID] == INVALID_SOCKET) threadStops[ThreadID].wait_for(lock, 250ms);

		
		if (!continueRunning) break;

		do
		{
			iResult = recv(clientConnectionSockets[ThreadID], recvbuf, recvbuflen, 0);

			recvbuf[iResult>0  &&iResult < DEFAULT_BUFLEN ? iResult:DEFAULT_BUFLEN-1] = 0;
			if (iResult > 0)
			{
				ConnectionDatum datum = InputScanning::parseConnectionDatumFromSentPacket(std::string((char *)recvbuf));
				
				std::string returnValue = "Invalid Format";

				if (datum.command != ConnectionCommand::Invalid_Parse)
				{
					returnValue = mainResource.processConnectionDatum(datum);
				}

				iSendResult = send(clientConnectionSockets[ThreadID], returnValue.c_str(), returnValue.size(), 0);
				
				if (iSendResult == SOCKET_ERROR)
				{
					break;
				}
			}
			else if (iResult == 0)
			{
			}
			else
			{
				break;
			}
		} while (iResult > 0);

		closesocket(clientConnectionSockets[ThreadID]);
		availabilityMutex.lock();
		clientConnectionSockets[ThreadID] = INVALID_SOCKET;
		availabilityMutex.unlock();

	}
}

bool ThreadPool::assignConnectionToThread(SOCKET newClientSocket)
{
	bool keepChecking = true;

	for (int i = 0; i < numberOfThreads; i++)
	{
		availabilityMutex.lock();

		if (clientConnectionSockets[i] == INVALID_SOCKET)
		{
			keepChecking = false;
			clientConnectionSockets[i] = newClientSocket;
			threadStops[i].notify_all();
		}

		availabilityMutex.unlock();

		if (!keepChecking) break;
	}

	return !keepChecking;
}

SharedDataResource* ThreadPool::getSharedDataResource()
{
	return &mainResource;
}
