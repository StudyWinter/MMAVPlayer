#pragma once
#include <thread>

// ����һ����
class MMthread
{
public:
	virtual void run() = 0;		// ���麯�����̻߳�����������
	int start();				// ��������
	int stop();					// ֹͣ����

private:
	std::thread* t = nullptr;

public:
	int stopFlag = 0;
};