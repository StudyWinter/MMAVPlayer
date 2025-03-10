#pragma once
#include <thread>

// 定义一个类
class MMthread
{
public:
	virtual void run() = 0;		// 纯虚函数，线程会调用这个函数
	int start();				// 启动函数
	int stop();					// 停止操作

private:
	std::thread* t = nullptr;

public:
	int stopFlag = 0;
};