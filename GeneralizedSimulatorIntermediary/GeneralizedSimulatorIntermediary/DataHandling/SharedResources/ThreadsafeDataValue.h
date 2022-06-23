#pragma once

#include <mutex>
#include <string>

class ThreadsafeDataValue
{
private:
	std::string Tag;
	std::string Value;
	std::mutex valueLock;
	bool currentlyBeingDeleted;
public:
	std::string getValue();
	void setValue(std::string valueIn);
	std::string getTag();
	std::mutex * getMutex();
	ThreadsafeDataValue(std::string valueIn);
	ThreadsafeDataValue() {};
	ThreadsafeDataValue(const ThreadsafeDataValue& data) = delete;
};