// CMakeProject1.cpp: 定义应用程序的入口点。
//

#include "CMakeProject1.h"
#include <MMThread/MMThread.h>
#include <chrono>
#include <thread>
using namespace std;

class MyMMThread : public MMthread
{
public:
	// 构造函数
	MyMMThread(int _a) : a(_a) {}

	// 实现父类的纯虚函数
	void run() override
	{
		std::cout << "MyMMthread: " << a << std::endl;
	}
	
private:
	int a;
};


int main2()
{
	MyMMThread myThread1(10);			// 创建线程
	// 会调用父类的start函数，父类的start函数创建线程，调用纯虚函数，子类实现了纯虚函数，因此会打印
	myThread1.start();


	MyMMThread myThread2(20);			// 创建线程
	myThread2.start();

	// 休眠2s
	std::this_thread::sleep_for(chrono::seconds(2));


	system("pause");
	return 0;
}

#include "MMAV/MMAV.h"

int main(void)
{
	MMAVReader reader;
	int res = reader.open("C:\\Users\\winter\\Desktop\\1.mp4");
	if (res)
	{
		std::cout << "open file failed" << std::endl;
		return -1;
	}
	// 读取
	while (1)
	{
		MMAVPacket pkt;
		res = reader.read(&pkt);
		if (res)
		{
			break;
		}
		std::cout << "read packet success" << std::endl;
	}

	reader.close();

	system("pause");
	return 0;
}
