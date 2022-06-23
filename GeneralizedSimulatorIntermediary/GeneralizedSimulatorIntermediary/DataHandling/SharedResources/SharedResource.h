#pragma once

#include <unordered_map>
#include <condition_variable>
#include "ThreadsafeDataValue.h"
#include "../InputScanning.h"

class SharedDataResource
{
private:
	std::unordered_map<std::string, ThreadsafeDataValue *> mainDataResource;

	//2022-06-16 JPL this map contains mutexes that will prevent accessing resources when the system is adding or removing 
	//whole entries from the mainDataResource, the thread that is blocking will remove the element when unlocked
	std::unordered_map<std::string, std::mutex *> mainDataResourcesMutexLocks;
	
	std::mutex MutexLocksMutex;

	void push(std::string tag, std::string value);
	std::string poll(std::string tag);
	void deleteCmd(std::string tag);

	void eraseMainDataResourceMutexLock(std::string tag);

	void lockMainDataResourcesLocks();
	void unlockMainDataResourcesLocks();

public:

	std::string processConnectionDatum(ConnectionDatum datumIn);
	~SharedDataResource();
};