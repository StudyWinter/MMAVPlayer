#include "CMakeProject1.h"
#include "MMAV/MMAV.h"


MMPlayerReaderThread::MMPlayerReaderThread(std::string _path)
{
	path = _path;
}

MMPlayerReaderThread::~MMPlayerReaderThread()
{

}

// 纯虚函数，线程会调用这个函数
void MMPlayerReaderThread::run()
{
	MMAVReader reader;
	int res = reader.open(path.c_str());
	if (res)
	{
		std::cout << "open failed" << std::endl;
		return;
	}
	// 打开成功，得到音视频两个流
	int audioStreamIndex = reader.getAudeoStreamIndex();
	int videoStreamIndex = reader.getVideoStreamIndex();

	// 初始化解码器


	// 读流
	while (!stopFlag)
	{
		MMAVPacket* packet = new MMAVPacket();
		int res = reader.read(packet);
		if (res)
		{
			delete packet;
			packet = nullptr;
			break;
		}

		// 将packet放入缓存
		std::cout << "reader packet success" << std::endl;
		delete packet;
		packet = nullptr;
	}

	reader.close();
}