// GeneralizedSimulatorIntermediary.cpp : Defines the entry point for the application.
//

#include "GeneralizedSimulatorIntermediary.h"
#include "Threading/ThreadPool.h"
#include "DataHandling/SharedResources/SharedResource.h"

std::string Port;
std::string ipAddress;
NetworkHandler * netHandler;
ThreadPool* threadPool;


void ShutdownNethandler(int value)
{
	netHandler->Shutdown();
}

void dropInMenu(int value)
{
	printf("Type a command to continue:\n");
	printf("count -> checks the number of key value pairs\n");
	printf("check -> prints all keys and values stored\n");
	printf("clear -> empties all keys and values and frees memory\n");
	printf("shutdown -> shuts down system\n");
	printf("push [tag] [value] -> adds a value to the system\n");
	printf("poll [tag] -> retrieves a value from the system\n");
	printf("delete [tag] -> deletes the tag from the system\n");
	printf("anything else closes this menu\n");

	char command[9]{};

	scanf("%8s", command);

	std::string commandStr = std::string(command);

	if (commandStr == "count")
	{
		threadPool->getSharedDataResource()->count();
	}
	else if (commandStr == "check")
	{
		threadPool->getSharedDataResource()->check();
	}
	else if (commandStr == "clear")
	{
		threadPool->getSharedDataResource()->clear();
	}
	else if (commandStr == "shutdown")
	{
		ShutdownNethandler(value);
	}
	else if (commandStr == "push")
	{
		char tag[1001]{};
		scanf("%1000s", tag);
		char value[1001]{};
		scanf("%1000s", value);

		std::string tagStr = std::string(tag);
		std::string valueStr = std::string(value);

		ConnectionDatum datum;
		datum.command = ConnectionCommand::Push;
		datum.tag = tagStr;
		datum.value = value;

		printf("%s\n",(threadPool->getSharedDataResource()->processConnectionDatum(datum)).c_str());

	}
	else if (commandStr == "poll")
	{
		char tag[1001]{};
		scanf("%1000s", tag);

		std::string tagStr = std::string(tag);

		ConnectionDatum datum;
		datum.command = ConnectionCommand::Poll;
		datum.tag = tagStr;

		printf("%s\n", (threadPool->getSharedDataResource()->processConnectionDatum(datum)).c_str());

	}
	else if (commandStr == "delete")
	{
		char tag[1001]{};
		scanf("%1000s", tag);

		std::string tagStr = std::string(tag);

		ConnectionDatum datum;
		datum.command = ConnectionCommand::Delete;
		datum.tag = tagStr;

		printf("%s\n", (threadPool->getSharedDataResource()->processConnectionDatum(datum)).c_str());
	}

	signal(SIGINT, dropInMenu);
}

void constructResources(int numberOfThreads)
{
	threadPool = new ThreadPool(numberOfThreads);
	netHandler = new NetworkHandler(Port, ipAddress);
	signal(SIGINT, dropInMenu);
	netHandler->setThreadPool(threadPool);
}

void disposeResources()
{
	delete netHandler;
	delete threadPool;
}

int main(int argc, char ** argv)
{
	if (argc < 4)
	{
		printf("The first argument must be the IP address, the second must be the port, the thrid must be the number of threads");
		return 1;
	}

	ipAddress = std::string(argv[1]);
	Port = std::string(argv[2]);
	int numberOfThreads = atoi(argv[3]);

	constructResources(numberOfThreads);

	netHandler->handleNetworkCommunication();
	
	disposeResources();
	printf("System Exiting");
	return 0;
}
