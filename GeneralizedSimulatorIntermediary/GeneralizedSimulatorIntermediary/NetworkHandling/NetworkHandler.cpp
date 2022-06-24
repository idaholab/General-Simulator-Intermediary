#include "NetworkHandler.h"


NetworkHandler::NetworkHandler(std::string Port, std::string IpAddress)
{
	this->Port = Port;
	this->IpAddress = IpAddress;
	ListenSocket = INVALID_SOCKET;

	continueRunning = true;

	iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (iResult != 0)
	{
		printf("WSAStartup failed: %d\n", iResult);
		return;
	}

	ZeroMemory(&hints, sizeof(hints));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;
	hints.ai_flags = AI_PASSIVE;

	iResult = getaddrinfo(IpAddress.c_str(), Port.c_str(), &hints, &result);
	if (iResult != 0)
	{
		printf("getaddrinfo failed: %d\n", iResult);
		WSACleanup();
		return;
	}

	ListenSocket = socket(result->ai_family, result->ai_socktype, result->ai_protocol);

	if (ListenSocket == INVALID_SOCKET)
	{
		printf("ERROR at socket(): %ld\n", WSAGetLastError());
		freeaddrinfo(result);
		WSACleanup();
		return;
	}

	iResult = bind(ListenSocket, result->ai_addr, (int)result->ai_addrlen);
	if (iResult == SOCKET_ERROR)
	{
		printf("bind failed with error: %d\n", WSAGetLastError());
		freeaddrinfo(result);
		closesocket(ListenSocket);
		WSACleanup();
		return;
	}

	freeaddrinfo(result);
}

NetworkHandler::~NetworkHandler()
{
	

}

void NetworkHandler::sendNoAvailableHandlerThreadMessage(SOCKET clientSocket)
{
	std::string returnValue = "No Client Thread To Accept Connection";
	send(clientSocket, returnValue.c_str(), returnValue.size(), 0);
	closesocket(clientSocket);
}

void NetworkHandler::Shutdown()
{
	closesocket(ListenSocket);
	continueRunning = false;
}

void NetworkHandler::handleNetworkCommunication()
{

	printf("Listening\n");
	while (continueRunning)
	{
		if (listen(ListenSocket, SOMAXCONN) == SOCKET_ERROR)
		{
			printf("Listen failed with error: %ld\n", WSAGetLastError());
			break;
		}

		SOCKET ClientSocket = INVALID_SOCKET;

		ClientSocket = accept(ListenSocket, NULL, NULL);
		
		if (continueRunning)
		{
			printf("Accepting Client\n");
			if (ClientSocket == INVALID_SOCKET)
			{
				printf("accept failed: %d\n", WSAGetLastError());
			}
			else
			{
				if (!threadPool->assignConnectionToThread(ClientSocket))
				{
					sendNoAvailableHandlerThreadMessage(ClientSocket);
				}
			}
		}
		else
		{
			break;
		}
	}

	WSACleanup();

	//this code is just stored here temporarily
	// this is the client code
	//char recvbuf[DEFAULT_BUFLEN]{};
	//int iSendResult;
	//int recvbuflen = DEFAULT_BUFLEN;

	//do
	//{
	//	iResult = recv(ClientSocket, recvbuf, recvbuflen, 0);
	//	if (iResult > 0)
	//	{
	//		printf("Bytes received: %d\n", iResult);

	//		printf(recvbuf);
	//		printf("\nEchoing back to Client\n");
	//		iSendResult = send(ClientSocket, recvbuf, iResult, 0);
	//		if (iSendResult == SOCKET_ERROR)
	//		{
	//			printf("send failed: %d\n", WSAGetLastError());
	//			closesocket(ClientSocket);
	//			WSACleanup();
	//			return ;
	//		}
	//		printf("Bytes sent: %d\n", iSendResult);
	//	}
	//	else if (iResult == 0)
	//	{
	//		printf("Connection closing...\n");
	//	}
	//	else
	//	{
	//		printf("recv failed: %d\n", WSAGetLastError());
	//		closesocket(ClientSocket);
	//		WSACleanup();
	//		return ;
	//	}
	//} while (iResult > 0);

	//printf("Shutting Down\n");

	//iResult = shutdown(ClientSocket, SD_SEND);
	//if (iResult == SOCKET_ERROR)
	//{
	//	printf("shutdown failed: %d\n", WSAGetLastError());
	//	closesocket(ClientSocket);
	//	WSACleanup();
	//	return ;
	//}

	//closesocket(ClientSocket);
	//WSACleanup();

}

void NetworkHandler::setThreadPool(ThreadPool* threadPoolIn)
{
	this->threadPool = threadPoolIn;
}