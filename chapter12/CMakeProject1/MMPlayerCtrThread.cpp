#include "CMakeProject1.h"

MMPlayerCtrThread::MMPlayerCtrThread()
{

}

MMPlayerCtrThread::~MMPlayerCtrThread()
{

}

void MMPlayerCtrThread::run()
{
	// 控制线程也应该控制读线程
	MMPlayerReaderThread readerThread("C:\\Users\\winter\\Desktop\\video1.mp4", this);
	readerThread.start();


	// 获取线程启动时的时间 startTime
	long long startTime = MMAVTime::getTime();

	MMAVFrame* videoFrame = nullptr;
	MMAVFrame* audioFrame = nullptr;


	while (!stopFlag)
	{
		std::this_thread::sleep_for(std::chrono::milliseconds(1));
		// 获取当前的时间 nowTime
		long long nowTime = MMAVTime::getTime();

		// 获取到当前时间和开始时间的差值 dTime
		long long dTime = nowTime - startTime;
		// std::cout << "dTime :" << dTime << std::endl;

		// 从视频缓存队列中，获取一帧视频 frame_pts
		if (videoFrame == nullptr)
		{
			// 尝试取一帧
			videoQueue.pop(&videoFrame);
		}

		// std::cout << "Video Queue size:" << videoQueue.size() << std::endl;

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