//Copyright 2022, Battelle Energy Alliance, LLC
#include "ThreadsafeDataValue.h"

ThreadsafeDataValue::~ThreadsafeDataValue()
{
}

ThreadsafeDataValue::ThreadsafeDataValue(std::string valueIn)
{
	this->setValue(valueIn);
}

std::string ThreadsafeDataValue::getValue()
{
	valueLock.lock();
	std::string returnValue = std::string(Value);
	valueLock.unlock();
	return returnValue;
}

std::string ThreadsafeDataValue::getTag()
{
	valueLock.lock();
	std::string returnValue = std::string(Tag);
	valueLock.unlock();
	return returnValue;
}

void ThreadsafeDataValue::setValue(std::string valueIn)
{
	valueLock.lock();
	Value = valueIn;
	valueLock.unlock();
}

std::mutex* ThreadsafeDataValue::getMutex()
{
	return &valueLock;
}

