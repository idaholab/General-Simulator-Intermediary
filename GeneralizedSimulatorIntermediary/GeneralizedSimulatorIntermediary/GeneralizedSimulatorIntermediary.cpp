// GeneralizedSimulatorIntermediary.cpp : Defines the entry point for the application.
//

#include "GeneralizedSimulatorIntermediary.h"
#include "Threading/ThreadPool.h"
#include "DataHandling/SharedResources/SharedResource.h"

std::string Port;
std::string ipAddress;
NetworkHandler * netHandler;
ThreadPool* threadPool;

#define NUMBER_OF_THREADS 8

void ShutdownNethandler(int value)
{
	netHandler->Shutdown();
}

void constructResources()
{
	threadPool = new ThreadPool(NUMBER_OF_THREADS);
	netHandler = new NetworkHandler(Port, ipAddress);
	signal(SIGINT, ShutdownNethandler);
	netHandler->setThreadPool(threadPool);
}

void disposeResources()
{
	delete netHandler;
	delete threadPool;
}

int main(int argc, char ** argv)
{

	//processConfig
	Port = "33417";
	ipAddress = "127.0.0.1";

	constructResources();

	netHandler->handleNetworkCommunication();
	
	disposeResources();
	printf("System Exiting");
	return 0;
}
