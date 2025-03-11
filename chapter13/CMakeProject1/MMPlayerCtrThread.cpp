#include "CMakeProject1.h"

MMPlayerCtrThread::MMPlayerCtrThread(double _seekTime)
{
	seekTime = _seekTime;
}

MMPlayerCtrThread::~MMPlayerCtrThread()
{

}

void MMPlayerCtrThread::run()
{
	// �����߳�ҲӦ�ÿ��ƶ��߳�
	MMPlayerReaderThread readerThread("C:\\Users\\winter\\Desktop\\video1.mp4", seekTime, this);
	readerThread.start();


	// ��ȡ�߳�����ʱ��ʱ�� startTime
	long long startTime = MMAVTime::getTime();

	MMAVFrame* videoFrame = nullptr;
	MMAVFrame* audioFrame = nullptr;

	long long sleepCountTime = 0;

	while (!stopFlag)
	{
		std::this_thread::sleep_for(std::chrono::milliseconds(1));

		long long sleepTimeStart = MMAVTime::getTime();
		// ��ͣ״̬��ѭ���ȴ�
		while (status == MMPlayCtrStatus::MMPLAYER_CTR_STATUS_PAUSEING)
		{
			std::this_thread::sleep_for(std::chrono::milliseconds(1));
		}
		long long sleepTimeEnd = MMAVTime::getTime();
		long long sleepDTime = sleepTimeEnd - sleepTimeStart;
		sleepCountTime += sleepDTime;


		// ��ȡ��ǰ��ʱ�� nowTime
		long long nowTime = MMAVTime::getTime();

		// ��ȡ����ǰʱ��Ϳ�ʼʱ��Ĳ�ֵ dTime
		long long dTime = nowTime - startTime;
		dTime = dTime - sleepCountTime;
		// std::cout << "dTime :" << dTime << std::endl;
		dTime = dTime + (long long)(seekTime * 1000);

		// ����Ƶ��������У���ȡһ֡��Ƶ frame_pts
		if (videoFrame == nullptr)
		{
			// ����ȡһ֡
			videoQueue.pop(&videoFrame);
		}

		// std::cout << "Video Queue size:" << videoQueue.size() << std::endl;

		// �ӵ�
		if (videoFrame != nullptr){
			if (videoFrame->getPts() < (long long)(seekTime * 1000))
			{
				delete videoFrame;
				videoFrame = nullptr;
			}
		}

		if (videoFrame != nullptr)
		{
			if (videoFrame->getPts() <= dTime)
			{
				// ���frame_pts <= d_time����֡��ƵӦ���������ų�����
				std::cout << "video frame:" << videoFrame->getPts() << std::endl;
				delete videoFrame;
				videoFrame = nullptr;
			}
			else
			{
				// ������֡��Ƶ���������ŵ�ʱ�򣬳��������读���ֱ�Ӵ�����Ƶ
			}
		}


		// ����Ƶ��������У���ȡһ֡��Ƶ frame_pts
		if (audioFrame == nullptr)
		{
			// ����ȡһ֡
			audioQueue.pop(&audioFrame);
		}

		// �ӵ�
		if (audioFrame != nullptr) {
			if (audioFrame->getPts() < (long long)(seekTime * 1000))
			{
				delete audioFrame;
				audioFrame = nullptr;
			}
		}


		if (audioFrame != nullptr)
		{
			if (audioFrame->getPts() <= dTime)
			{
				// ���frame_pts <= d_time����֡��ƵӦ���������ų�����
				// std::cout << "audio frame " << audioFrame->getPts() << std::endl;
				delete audioFrame;
				audioFrame = nullptr;
			}
			else
			{
				// ������֡��Ƶ���������ŵ�ʱ�򣬳���������
			}
		}
	}

	// �ر��߳�
	readerThread.stop();
}

// ��ȡ���еĳ���
int MMPlayerCtrThread::getVideoQueueSize()
{
	return videoQueue.size();
}
int MMPlayerCtrThread::getAudioQueueSize()
{
	return audioQueue.size();
}

// ������
int MMPlayerCtrThread::pushFrameToVideoQueue(MMAVFrame* videoFrame)
{
	return videoQueue.push(videoFrame);
}
int MMPlayerCtrThread::pushFrameToAudioQueue(MMAVFrame* audioFrame)
{
	return audioQueue.push(audioFrame);
}

// ����
int MMPlayerCtrThread::play()
{
	status = MMPlayCtrStatus::MMPLAYER_CTR_STATUS_PLAYING;
	return 0;
}

// ��ͣ
int MMPlayerCtrThread::pause()
{
	status = MMPlayCtrStatus::MMPLAYER_CTR_STATUS_PAUSEING;
	return 0;
}