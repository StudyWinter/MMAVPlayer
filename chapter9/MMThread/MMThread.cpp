#include "MMThread.h"
#include <thread>


int MMthread::start()
{
	std::thread t(&MMthread::run, this);			// �����̣߳������Լ��Ĵ��麯��������̳и�������´��麯��
	t.detach();										// �̷߳���

	return 0;
}