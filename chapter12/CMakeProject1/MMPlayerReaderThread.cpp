#include "CMakeProject1.h"
#include "MMAV/MMAV.h"


MMPlayerReaderThread::MMPlayerReaderThread(std::string _path, MMPlayerCtrThread* _playerCtrThread)
{
	path = _path;
	playerCtrThread = _playerCtrThread;
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
	MMAVPlayerDecoderThread* audioDecoderThread = 
		new MMAVPlayerDecoderThread(playerCtrThread, MMDecoderType::MMDECODER_TYPE_AUDIO);
	MMAVPlayerDecoderThread* videoDecoderThread = 
		new MMAVPlayerDecoderThread(playerCtrThread, MMDecoderType::MMDECODER_TYPE_VIDEO);

	// 初始化
	MMAVStream audioStream;
	reader.getStream(&audioStream, audioStreamIndex);

	MMAVStream videoStream;
	reader.getStream(&videoStream, videoStreamIndex);

	audioDecoderThread->init(&audioStream);
	videoDecoderThread->init(&videoStream);

	// 启动线程
	audioDecoderThread->start();
	videoDecoderThread->start();

	// 读流
	while (!stopFlag)
	{
		// 进行判断，此时解码线程慢
		if (videoDecoderThread->getPacketQueueSize() > 5 && audioDecoderThread->getPacketQueueSize() > 5)
		{
			// 进入自璇等待
			continue;
		}

		MMAVPacket* packet = new MMAVPacket();
		int res = reader.read(packet);
		if (res)
		{
			delete packet;
			packet = nullptr;
			break;
		}

		// 区分是音频流还是视频流
		if (packet->getIndex() == audioStreamIndex)
		{
			audioDecoderThread->putPacket(packet);
		}
		if (packet->getIndex() == videoStreamIndex)
		{
			videoDecoderThread->putPacket(packet);
		}

		// 将packet放入缓存
		// std::cout << "reader packet success" << std::endl;
		// delete packet;
		// packet = nullptr;
	}

	// 停止线程
	audioDecoderThread->stop();
	videoDecoderThread->stop();

	reader.close();
}