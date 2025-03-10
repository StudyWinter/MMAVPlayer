#include "CMakeProject1.h"

MMPlayer::MMPlayer(std::string _path)
{
	path = _path;
}
MMPlayer::~MMPlayer()
{

}

// 打开
int MMPlayer::open()
{
	if (readerThread == nullptr)
	{
		readerThread = new MMPlayerReaderThread(path);
		readerThread->start();
		readerThread = nullptr;
		return 0;
	}
	
	return -1;
}

// 停止
int MMPlayer::stop()
{
	if (readerThread != nullptr)
	{
		readerThread->stop();
		delete readerThread;
		readerThread = nullptr;
		return 0;
	}
	
	return -1;
}

// 播放
int MMPlayer::play()
{
	return 0;
}

// 暂停
int MMPlayer::pause()
{
	return 0;
}

// 	
int MMPlayer::seek(double second)
{
	return 0;
}