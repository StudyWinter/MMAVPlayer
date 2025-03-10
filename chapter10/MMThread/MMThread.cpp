#include "MMThread.h"


int MMthread::start()
{
	// std::thread t(&MMthread::run, this);			// 创建线程，调用自己的纯虚函数，子类继承父类会重新纯虚函数
	// t.detach();									// 线程分类

	if (t == nullptr)
	{
		stopFlag = 0;
		t = new std::thread(&MMthread::run, this);
	}

	return 0;
}

// 停止操作
int MMthread::stop()
{
	if (t != nullptr)
	{
		stopFlag = 1;
		t->join();			// 阻塞
		delete t;
		t = nullptr;
	}
	return 0;
}
