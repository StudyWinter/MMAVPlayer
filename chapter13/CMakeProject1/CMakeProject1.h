// CMakeProject1.h: 标准系统包含文件的包含文件
// 或项目特定的包含文件。

#pragma once
#include <iostream>
#include "MMthread/MMthread.h"
#include "MMAV/MMAV.h"
#include "MMQueue/MMQueue.h"

// 枚举
enum MMDecoderType
{
	MMDECODER_TYPE_VIDEO = 0,
	MMDECODER_TYPE_AUDIO = 1
};

// 枚举
enum MMPlayCtrStatus
{
	MMPLAYER_CTR_STATUS_PLAYING = 0,
	MMPLAYER_CTR_STATUS_PAUSEING = 1
};


// 播放控制线程
class MMPlayerCtrThread : public MMthread
{
public:
	MMPlayerCtrThread(double seekTime = 0.0);
	~MMPlayerCtrThread();

	virtual void run() override;		  // 纯虚函数，线程会调用这个函数

	int play();						      // 播放
	int pause();						  // 暂停

	// 获取队列的长度
	int getVideoQueueSize();
	int getAudioQueueSize();

	// 加数据
	int pushFrameToVideoQueue(MMAVFrame* videoFrame);
	int pushFrameToAudioQueue(MMAVFrame* audioFrame);

private:
	MMQueue<MMAVFrame> videoQueue;
	MMQueue<MMAVFrame> audioQueue;

	// 信号量，默认是播放中
	MMPlayCtrStatus status = MMPlayCtrStatus::MMPLAYER_CTR_STATUS_PLAYING;
	double seekTime = 0.0;
};

// 读取流的线程
class MMPlayerReaderThread : public MMthread
{
public:
	MMPlayerReaderThread(std::string path, double seekTime, MMPlayerCtrThread* _playerCtrThread);
	~MMPlayerReaderThread();

	virtual void run() override;		  // 纯虚函数，线程会调用这个函数

private:
	std::string path;
	MMPlayerCtrThread* playerCtrThread = nullptr;
	double seekTime = 0.0;
};


// 解码线程
class MMAVPlayerDecoderThread : public MMthread
{
public:
	MMAVPlayerDecoderThread(MMPlayerCtrThread* _playerCtrThread, MMDecoderType type);
	~MMAVPlayerDecoderThread();

	virtual void run() override;		  // 纯虚函数，线程会调用这个函数
	int init(MMAVStream* avStream);

	int putPacket(MMAVPacket* packet);	  // 存数据
	int getPacketQueueSize();

private:
	MMAVDecoder* decoder = nullptr;		  // 解码器
	MMQueue<MMAVPacket> packetQueue;
	MMPlayerCtrThread* playerCtrThread = nullptr;
	MMDecoderType type;

};


class MMPlayer
{
public:
	MMPlayer(std::string path);
	~MMPlayer();

	int open(double second = 0.0);	// 打开
	int stop();						// 停止
	int play();						// 播放
	int pause();					// 暂停
	int seek(double second);		// 进度条

private:
	std::string path;
	MMPlayerReaderThread* readerThread = nullptr;
	MMPlayerCtrThread* playerCtrThread = nullptr;
};
