#include "CMakeProject1.h"


MMAVPlayerDecoderThread::MMAVPlayerDecoderThread()
{
	decoder = new MMAVDecoder();
}

MMAVPlayerDecoderThread::~MMAVPlayerDecoderThread()
{
	if (decoder != nullptr)
	{
		delete decoder;
		decoder = nullptr;
	}
}

int MMAVPlayerDecoderThread::init(MMAVStream* avStream)
{
	return decoder->init(avStream);
}

// 存数据
int MMAVPlayerDecoderThread::putPacket(MMAVPacket* packet)
{
	return packetQueue.push(packet);
}

int MMAVPlayerDecoderThread::getPacketQueueSize()
{
	return packetQueue.size();
}



// 纯虚函数，线程会调用这个函数，init之后在run
void MMAVPlayerDecoderThread::run()
{
	int frameCount = 0;
	// 进行解码操作，解码线程慢
	while (!stopFlag)
	{
		MMAVPacket* packet = nullptr;
		int res = packetQueue.pop(&packet);
		if (res)
		{
			// 队列为空,取不到packet
			continue;
		}
		// 解码
		decoder->sendPacket(packet);
		while (1)
		{
			MMAVFrame frame;
			res = decoder->recvFrame(&frame);
			if (res)
			{
				break;
			}
			frameCount++;
			std::cout << "Decoder success" << frameCount << std::endl;
			std::this_thread::sleep_for(std::chrono::seconds(1));
		}
	}
}