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

// ���麯�����̻߳�����������
void MMPlayerReaderThread::run()
{
	MMAVReader reader;
	int res = reader.open(path.c_str());
	if (res)
	{
		std::cout << "open failed" << std::endl;
		return;
	}
	// �򿪳ɹ����õ�����Ƶ������
	int audioStreamIndex = reader.getAudeoStreamIndex();
	int videoStreamIndex = reader.getVideoStreamIndex();

	// ��ʼ��������
	MMAVPlayerDecoderThread* audioDecoderThread = 
		new MMAVPlayerDecoderThread(playerCtrThread, MMDecoderType::MMDECODER_TYPE_AUDIO);
	MMAVPlayerDecoderThread* videoDecoderThread = 
		new MMAVPlayerDecoderThread(playerCtrThread, MMDecoderType::MMDECODER_TYPE_VIDEO);

	// ��ʼ��
	MMAVStream audioStream;
	reader.getStream(&audioStream, audioStreamIndex);

	MMAVStream videoStream;
	reader.getStream(&videoStream, videoStreamIndex);

	audioDecoderThread->init(&audioStream);
	videoDecoderThread->init(&videoStream);

	// �����߳�
	audioDecoderThread->start();
	videoDecoderThread->start();

	// ����
	while (!stopFlag)
	{
		// �����жϣ���ʱ�����߳���
		if (videoDecoderThread->getPacketQueueSize() > 5 && audioDecoderThread->getPacketQueueSize() > 5)
		{
			// ������诵ȴ�
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

		// ��������Ƶ��������Ƶ��
		if (packet->getIndex() == audioStreamIndex)
		{
			audioDecoderThread->putPacket(packet);
		}
		if (packet->getIndex() == videoStreamIndex)
		{
			videoDecoderThread->putPacket(packet);
		}

		// ��packet���뻺��
		// std::cout << "reader packet success" << std::endl;
		// delete packet;
		// packet = nullptr;
	}

	// ֹͣ�߳�
	audioDecoderThread->stop();
	videoDecoderThread->stop();

	reader.close();
}