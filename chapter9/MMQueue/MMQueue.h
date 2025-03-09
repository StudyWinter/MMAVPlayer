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
		// ����������
		std::lock_guard<std::mutex> lck(myMutex);
		//myMutex.lock();				// ����
		while (size() > 0)
		{
			//T* t = nullptr;
			//this->pop(&t);
			//T* t = this->myQueue.front();
			this->myQueue.pop();
		}
		//myMutex.unlock();			// ����
	}
	void push(T* t)
	{
		// ����������
		std::lock_guard<std::mutex> lck(myMutex);
		//myMutex.lock();				// ����
		this->myQueue.push(t);
		//myMutex.unlock();				// ����
	}
	bool pop(T** t)
	{
		// ����������
		std::lock_guard<std::mutex> lck(myMutex);

		// myMutex.lock();				// ����
		int size = this->size();
		if (size > 0)
		{
			*t = this->myQueue.front();
			this->myQueue.pop();
			// myMutex.unlock();		// ����
			return true;
		}
		// myMutex.unlock();			// ����
		return false;
	}
	int size()
	{
		return this->myQueue.size();
	}

private:
	std::queue<T*> myQueue;
	std::mutex myMutex;
	// �ݹ����������μ��������Ƽ����ģʹ��
	//std::recursive_mutex myMutex;

};