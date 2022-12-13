//Copyright 2022, Battelle Energy Alliance, LLC
#include "InputScanning.h"

ConnectionDatum InputScanning::parseConnectionDatumFromSentPacket(std::string packetIn)
{
	std::regex re(R"((?:\{\s*?"command"\s*?:\s*?")([^"]*?)(?:"\s*?,\s*?"tag":")([^"]*?)(?:")(?:(?:\s*?,\s*?"value"\s*?:\s*?")([^"]*?)(?:"))?(?:\s*?\}))");

	std::smatch match;
	std::regex_match(packetIn, match, re);

	if (match.size() < 3)
	{
		return ConnectionDatum();
	}
	
	ConnectionDatum retVal;
	retVal.command = resolveConnectionCommand(match[1]);
	retVal.tag = match[2];
	if (retVal.command == ConnectionCommand::Push)
	{
		if (match[3].length() == 0)
		{
			retVal.command = ConnectionCommand::Invalid_Parse;
			return retVal;
		}
		retVal.value = match[3];
	}
	else
	{
		if (match[3].length() != 0)
		{
			retVal.command = ConnectionCommand::Invalid_Parse;
			return retVal;
		}
	}

	return retVal;
}

ConnectionCommand InputScanning::resolveConnectionCommand(std::string informationIn)
{
	if (informationIn == "poll") return ConnectionCommand::Poll;
	if (informationIn == "push") return ConnectionCommand::Push;
	if (informationIn == "delete") return ConnectionCommand::Delete;
	return ConnectionCommand::Invalid_Parse;
}

