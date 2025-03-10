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

// ������
int MMAVPlayerDecoderThread::putPacket(MMAVPacket* packet)
{
	return packetQueue.push(packet);
}

int MMAVPlayerDecoderThread::getPacketQueueSize()
{
	return packetQueue.size();
}



// ���麯�����̻߳�������������init֮����run
void MMAVPlayerDecoderThread::run()
{
	int frameCount = 0;
	// ���н�������������߳���
	while (!stopFlag)
	{
		MMAVPacket* packet = nullptr;
		int res = packetQueue.pop(&packet);
		if (res)
		{
			// ����Ϊ��,ȡ����packet
			continue;
		}
		// ����
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