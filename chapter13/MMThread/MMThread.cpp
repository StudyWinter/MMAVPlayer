#include "MMThread.h"


int MMthread::start()
{
	// std::thread t(&MMthread::run, this);			// �����̣߳������Լ��Ĵ��麯��������̳и�������´��麯��
	// t.detach();									// �̷߳���

	if (t == nullptr)
	{
		stopFlag = 0;
		t = new std::thread(&MMthread::run, this);
	}

	return 0;
}

// ֹͣ����
int MMthread::stop()
{
	if (t != nullptr)
	{
		stopFlag = 1;
		t->join();			// ����
		delete t;
		t = nullptr;
	}
	return 0;
}
