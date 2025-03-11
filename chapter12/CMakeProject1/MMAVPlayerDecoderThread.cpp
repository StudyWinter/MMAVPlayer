#include "CMakeProject1.h"


MMAVPlayerDecoderThread::MMAVPlayerDecoderThread(MMPlayerCtrThread* _playerCtrThread, MMDecoderType _type)
{
	decoder = new MMAVDecoder();
	playerCtrThread = _playerCtrThread;
	type = _type;
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
		// ����1ms
		std::this_thread::sleep_for(std::chrono::milliseconds(1));

		// ͨ��playerCtrThread�ľ������������Frame����
		if (type == MMDecoderType::MMDECODER_TYPE_VIDEO)
		{
			if (playerCtrThread->getVideoQueueSize() > 10)
			{
				continue;
			}
		}
		else if (type == MMDecoderType::MMDECODER_TYPE_AUDIO)
		{
			if (playerCtrThread->getAudioQueueSize() > 20)
			{
				continue;
			}
		}

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
			MMAVFrame* frame = new MMAVFrame();
			res = decoder->recvFrame(frame);
			if (res)
			{
				break;
			}
			frameCount++;
			// std::cout << "Decoder success" << frameCount << std::endl;
			// std::this_thread::sleep_for(std::chrono::seconds(1));

			// ͨ��playerCtrThread�ľ������������Frame����
			if (type == MMDecoderType::MMDECODER_TYPE_VIDEO)
			{
				playerCtrThread->pushFrameToVideoQueue(frame);
			}
			else if (type == MMDecoderType::MMDECODER_TYPE_AUDIO)
			{
				playerCtrThread->pushFrameToAudioQueue(frame);
			}
		}
	}
}