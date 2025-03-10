// CMakeProject1.h: 标准系统包含文件的包含文件
// 或项目特定的包含文件。

#pragma once
#include <iostream>
#include "MMthread/MMthread.h"
#include "MMAV/MMAV.h"
#include "MMQueue/MMQueue.h"


// 读取流的线程
class MMPlayerReaderThread : public MMthread
{
public:
	MMPlayerReaderThread(std::string path);
	~MMPlayerReaderThread();

	virtual void run() override;		  // 纯虚函数，线程会调用这个函数

private:
	std::string path;

};


// 解码线程
class MMAVPlayerDecoderThread : public MMthread
{
public:
	MMAVPlayerDecoderThread();
	~MMAVPlayerDecoderThread();

	virtual void run() override;		  // 纯虚函数，线程会调用这个函数
	int init(MMAVStream* avStream);

	int putPacket(MMAVPacket* packet);	  // 存数据
	int getPacketQueueSize();

private:
	MMAVDecoder* decoder = nullptr;		  // 解码器
	MMQueue<MMAVPacket> packetQueue;
};


class MMPlayer
{
public:
	MMPlayer(std::string path);
	~MMPlayer();

	int open();					// 打开
	int stop();					// 停止
	int play();					// 播放
	int pause();				// 暂停
	int seek(double second);	// 

private:
	std::string path;
	MMPlayerReaderThread* readerThread = nullptr;
};
