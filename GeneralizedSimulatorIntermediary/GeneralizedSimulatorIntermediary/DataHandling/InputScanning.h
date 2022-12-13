//Copyright 2022, Battelle Energy Alliance, LLC
#pragma once
#include <regex>
#include <string>
#include <iostream>

enum ConnectionCommand
{
	Push, Poll, Delete, Invalid_Parse
};

struct ConnectionDatum
{
	ConnectionCommand command;
	std::string tag;
	std::string value;

	ConnectionDatum()
	{
		command = ConnectionCommand::Invalid_Parse;
	}

	void printDump()
	{
		printf("Command: %s\n",command == 0 ? "push" : command == 1 ? "poll" : command == 2 ? "delete": "invalid parse");
		if (command == ConnectionCommand::Invalid_Parse) return;
		printf("Tag: %s\n", tag.c_str());
		if (command == ConnectionCommand::Push)
		{
			printf("Value: %s\n", value.c_str());
		}
	}

};

class InputScanning
{
private:


public:
	static ConnectionDatum parseConnectionDatumFromSentPacket(std::string packetIn);
	static ConnectionCommand resolveConnectionCommand(std::string informationIn);
};