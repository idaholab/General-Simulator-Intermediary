#pragma once

#include <iostream>
#include <stdio.h>

#ifdef WINDOWS
#include <winsock2.h>
#include <ws2tcpip.h>
#pragma comment(lib,"Ws2_32.lib")
#endif
#include "../Threading/ThreadPool.h"

#define DEFAULT_BUFLEN 512


class NetworkHandler
{
private:
	std::string Port;
	std::string IpAddress;
	bool continueRunning;

	int connectionNumber;

	struct addrinfo* result = NULL;
	struct addrinfo* ptr = NULL;
	struct addrinfo hints;
	WSADATA wsaData;
	int iResult;

	SOCKET ListenSocket;
	ThreadPool* threadPool;

	void sendNoAvailableHandlerThreadMessage(SOCKET clientSocket);

public:

	NetworkHandler(std::string Port, std::string IpAddress);
	~NetworkHandler();
	void Shutdown();
	void handleNetworkCommunication();

	void setThreadPool(ThreadPool * threadPoolIn);
}; 

