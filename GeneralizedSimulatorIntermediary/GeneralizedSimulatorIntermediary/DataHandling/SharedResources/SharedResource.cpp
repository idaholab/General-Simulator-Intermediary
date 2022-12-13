//Copyright 2022, Battelle Energy Alliance, LLC
#include "SharedResource.h"

SharedDataResource::~SharedDataResource()
{
	for (std::pair<std::string, ThreadsafeDataValue*> ptr : mainDataResource)
	{
		delete ptr.second;
	}

	for (std::pair<std::string, std::mutex*> ptr : mainDataResourcesMutexLocks)
	{
		delete ptr.second;
	}
}

//JPL 2022-06-16 This connection shall never recieve an invalid parsed element
std::string SharedDataResource::processConnectionDatum(ConnectionDatum datumIn)
{
	std::string returnValue = "Command Completed Sucessfully";

	switch (datumIn.command)
	{
	case Push: push(datumIn.tag,datumIn.value); break;
	case Poll: returnValue = poll(datumIn.tag); break;
	case Delete: deleteCmd(datumIn.tag); break;
	}

	return returnValue;
}

void SharedDataResource::count()
{
	lockMainDataResourcesLocks();

	printf("Current Stored Elements %ld\n", mainDataResource.size());

	unlockMainDataResourcesLocks();
}

void SharedDataResource::check()
{
	lockMainDataResourcesLocks();

	for (std::pair<std::string, ThreadsafeDataValue*> ptr : mainDataResource)
	{
		printf("%s:%s\n", ptr.first.c_str(), ptr.second->getValue().c_str());
	}

	unlockMainDataResourcesLocks();
}

void SharedDataResource::clear()
{
	lockMainDataResourcesLocks();

	for (std::pair<std::string, ThreadsafeDataValue*> ptr : mainDataResource)
	{
		delete ptr.second;
	}
	mainDataResource.clear();
	
	for (std::pair<std::string, std::mutex*> ptr : mainDataResourcesMutexLocks)
	{
		delete ptr.second;
	}

	unlockMainDataResourcesLocks();
}

void SharedDataResource::lockMainDataResourcesLocks()
{
	MutexLocksMutex.lock();
}

void SharedDataResource::unlockMainDataResourcesLocks()
{
	MutexLocksMutex.unlock();
}

void SharedDataResource::eraseMainDataResourceMutexLock(std::string tag)
{
	lockMainDataResourcesLocks();
	if (mainDataResourcesMutexLocks.contains(tag))
	{
		mainDataResourcesMutexLocks[tag]->lock();
		mainDataResourcesMutexLocks[tag]->unlock();
		delete mainDataResourcesMutexLocks[tag];
		mainDataResourcesMutexLocks.erase(tag);
	}
	unlockMainDataResourcesLocks();
}

void SharedDataResource::push(std::string tag, std::string value)
{
	eraseMainDataResourceMutexLock(tag);

	if (mainDataResource.contains(tag))mainDataResource[tag]->setValue(value);
	else
	{
		lockMainDataResourcesLocks();
		mainDataResourcesMutexLocks.emplace( tag, new std::mutex() );
		mainDataResourcesMutexLocks[tag]->lock();
		unlockMainDataResourcesLocks();

		mainDataResource.emplace( tag, new ThreadsafeDataValue(value) );
		mainDataResourcesMutexLocks[tag]->unlock();

		eraseMainDataResourceMutexLock(tag);
	}
}

void SharedDataResource::deleteCmd(std::string tag)
{
	eraseMainDataResourceMutexLock(tag);

	lockMainDataResourcesLocks();
	mainDataResourcesMutexLocks.emplace( tag,new std::mutex() );
	mainDataResourcesMutexLocks[tag]->lock();
	unlockMainDataResourcesLocks();

	delete mainDataResource[tag];
	mainDataResource.erase(tag);

	mainDataResourcesMutexLocks[tag]->unlock();
	eraseMainDataResourceMutexLock(tag);
}


std::string SharedDataResource::poll(std::string tag)
{
	std::string returnValue = "Value does not exist";
	
	if (mainDataResource.contains(tag)) returnValue = mainDataResource[tag]->getValue();

	return returnValue;
}


