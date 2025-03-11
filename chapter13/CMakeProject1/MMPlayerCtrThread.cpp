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
	// 控制线程也应该控制读线程
	MMPlayerReaderThread readerThread("C:\\Users\\winter\\Desktop\\video1.mp4", seekTime, this);
	readerThread.start();


	// 获取线程启动时的时间 startTime
	long long startTime = MMAVTime::getTime();

	MMAVFrame* videoFrame = nullptr;
	MMAVFrame* audioFrame = nullptr;

	long long sleepCountTime = 0;

	while (!stopFlag)
	{
		std::this_thread::sleep_for(std::chrono::milliseconds(1));

		long long sleepTimeStart = MMAVTime::getTime();
		// 暂停状态，循环等待
		while (status == MMPlayCtrStatus::MMPLAYER_CTR_STATUS_PAUSEING)
		{
			std::this_thread::sleep_for(std::chrono::milliseconds(1));
		}
		long long sleepTimeEnd = MMAVTime::getTime();
		long long sleepDTime = sleepTimeEnd - sleepTimeStart;
		sleepCountTime += sleepDTime;


		// 获取当前的时间 nowTime
		long long nowTime = MMAVTime::getTime();

		// 获取到当前时间和开始时间的差值 dTime
		long long dTime = nowTime - startTime;
		dTime = dTime - sleepCountTime;
		// std::cout << "dTime :" << dTime << std::endl;
		dTime = dTime + (long long)(seekTime * 1000);

		// 从视频缓存队列中，获取一帧视频 frame_pts
		if (videoFrame == nullptr)
		{
			// 尝试取一帧
			videoQueue.pop(&videoFrame);
		}

		// std::cout << "Video Queue size:" << videoQueue.size() << std::endl;

		// 扔掉
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
				// 如果frame_pts <= d_time，这帧视频应该立即播放出来；
				std::cout << "video frame:" << videoFrame->getPts() << std::endl;
				delete videoFrame;
				videoFrame = nullptr;
			}
			else
			{
				// 否则这帧视频还不到播放的时候，程序进行自璇或者直接处理音频
			}
		}


		// 从音频缓存队列中，获取一帧音频 frame_pts
		if (audioFrame == nullptr)
		{
			// 尝试取一帧
			audioQueue.pop(&audioFrame);
		}

		// 扔掉
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
				// 如果frame_pts <= d_time，这帧音频应该立即播放出来；
				// std::cout << "audio frame " << audioFrame->getPts() << std::endl;
				delete audioFrame;
				audioFrame = nullptr;
			}
			else
			{
				// 否则这帧音频还不到播放的时候，程序进行自璇
			}
		}
	}

	// 关闭线程
	readerThread.stop();
}

// 获取队列的长度
int MMPlayerCtrThread::getVideoQueueSize()
{
	return videoQueue.size();
}
int MMPlayerCtrThread::getAudioQueueSize()
{
	return audioQueue.size();
}

// 加数据
int MMPlayerCtrThread::pushFrameToVideoQueue(MMAVFrame* videoFrame)
{
	return videoQueue.push(videoFrame);
}
int MMPlayerCtrThread::pushFrameToAudioQueue(MMAVFrame* audioFrame)
{
	return audioQueue.push(audioFrame);
}

// 播放
int MMPlayerCtrThread::play()
{
	status = MMPlayCtrStatus::MMPLAYER_CTR_STATUS_PLAYING;
	return 0;
}

// 暂停
int MMPlayerCtrThread::pause()
{
	status = MMPlayCtrStatus::MMPLAYER_CTR_STATUS_PAUSEING;
	return 0;
}