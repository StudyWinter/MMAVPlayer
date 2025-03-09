#pragma once

#include <queue>
#include <mutex>
#include <condition_variable>

/*
MMQueue<MMAVFrame> queue
MMQueue<MMAVPacket> queue
*/

template<typename T>
class MMQueue
{
public:
	MMQueue() = default;
	~MMQueue()
	{
		// 更聪明的锁
		std::lock_guard<std::mutex> lck(myMutex);
		//myMutex.lock();				// 加锁
		while (size() > 0)
		{
			//T* t = nullptr;
			//this->pop(&t);
			//T* t = this->myQueue.front();
			this->myQueue.pop();
		}
		//myMutex.unlock();			// 解锁
	}
	void push(T* t)
	{
		// 更聪明的锁
		std::lock_guard<std::mutex> lck(myMutex);
		//myMutex.lock();				// 加锁
		this->myQueue.push(t);
		//myMutex.unlock();				// 解锁
	}
	bool pop(T** t)
	{
		// 更聪明的锁
		std::lock_guard<std::mutex> lck(myMutex);

		// myMutex.lock();				// 加锁
		int size = this->size();
		if (size > 0)
		{
			*t = this->myQueue.front();
			this->myQueue.pop();
			// myMutex.unlock();		// 解锁
			return true;
		}
		// myMutex.unlock();			// 解锁
		return false;
	}
	int size()
	{
		return this->myQueue.size();
	}

private:
	std::queue<T*> myQueue;
	std::mutex myMutex;
	// 递归锁，允许多次加锁，不推荐大规模使用
	//std::recursive_mutex myMutex;

};