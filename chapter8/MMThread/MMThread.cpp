#include "MMThread.h"
#include <thread>


int MMthread::start()
{
	std::thread t(&MMthread::run, this);			// 创建线程，调用自己的纯虚函数，子类继承父类会重新纯虚函数
	t.detach();										// 线程分类

	return 0;
}