#pragma once

// ����һ����
class MMthread
{
public:
	virtual void run() = 0;		// ���麯�����̻߳�����������
	int start();				// ��������
};