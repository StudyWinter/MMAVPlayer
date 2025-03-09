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
#include <vector>

int main(void)
{
	MMAVReader reader;
	int res = reader.open("C:\\Users\\winter\\Desktop\\1.mp4");
	if (res)
	{
		std::cout << "open file failed" << std::endl;
		return -1;
	}

	int vedioStreamIndex = reader.getVideoStreamIndex();
	int audioStreamIndex = reader.getAudeoStreamIndex();

	std::cout << "vedioStreamIndex: " << vedioStreamIndex << std::endl;
	std::cout << "audioStreamIndex: " << audioStreamIndex << std::endl;


	std::vector<MMAVDecoder*> decodeList;

	// 得到所有的流
	int streamCount = reader.getStreamCount();
	for (int i = 0; i < streamCount; i++)
	{
		MMAVStream avStream;
		reader.getStream(&avStream, i);
		// 打印index
		std::cout << "StreamIndex: " << avStream.streamIndex << std::endl;

		// 初始化编码器
		MMAVDecoder* decoder = new MMAVDecoder();
		int res = decoder->init(&avStream);
		if (res)
		{
			std::cout << "init decoder failed: "<< std::endl;
		}
		// 添加
		decodeList.push_back(decoder);
	}

	FILE* file = fopen("C:\\Users\\winter\\Desktop\\1.yuv", "wb");

	// 读取
	while (1)
	{
		MMAVPacket pkt;
		res = reader.read(&pkt);
		if (res)
		{
			break;
		}
		//std::cout << "read packet success" << std::endl;
		int streamIndex = pkt.getIndex();
		MMAVDecoder* decoder = decodeList[streamIndex];

		// 解码
		res = decoder->sendPacket(&pkt);
		if (res)
		{
			continue;
		}

		// 循环
		while (1)
		{
			MMAVFrame frame;
			res = decoder->recvFrame(&frame);
			if (res)
			{
				break;
			}
			// Recv成功
			// 需要判断是音频还是视频
			if (streamIndex == vedioStreamIndex)
			{
				// 打印视频信息
				//frame.videoPrint();
				// 获取长和宽
				int widht = frame.getWidth();
				int height = frame.getHeight();
				// 分频yuv的buf
				unsigned char* y = (unsigned char*)malloc(widht * height);
				unsigned char* u = (unsigned char*)malloc(widht / 2 * height / 2);
				unsigned char* v = (unsigned char*)malloc(widht / 2 * height / 2);

				frame.getY(y);
				frame.getU(u);
				frame.getV(v);

				// 写成文件
				fwrite(y, widht * height, 1, file);
				fwrite(u, widht / 2 * height / 2, 1, file);
				fwrite(v, widht / 2 * height / 2, 1, file);


				free(y);
				free(u);
				free(v);
			}
			if (streamIndex == audioStreamIndex)
			{
				// 打印音频信息
				frame.audioPrint();

			}

		}
	}

	// 读不到帧了，还需要send
	for (int i = 0; i < decodeList.size(); i++)
	{
		MMAVDecoder* decoder = decodeList[i];
		res = decoder->sendPacket(nullptr);

		// 循环
		while (1)
		{
			MMAVFrame frame;
			res = decoder->recvFrame(&frame);
			if (res)
			{
				break;
			}
			// Recv成功

		}
	}


	reader.close();
	// 释放
	for (int i = 0; i < decodeList.size(); i++)
	{
		MMAVDecoder* decoder = decodeList[i];
		decoder->close();
		delete decoder;
		decoder = nullptr;
	}
	decodeList.clear();
	fclose(file);
	system("pause");
	return 0;
}
