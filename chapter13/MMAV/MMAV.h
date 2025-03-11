#pragma once

class MMAVPacketPrivate;
class MMAVStreamPrivate;
class MMAVStream;

class MMAVPacket
{
public:
	MMAVPacket();
	~MMAVPacket();

	int getIndex();

public:
	MMAVPacketPrivate* imp = nullptr;
};

class MMAVFramePrivate;

class MMAVFrame
{
public:
	MMAVFrame();
	~MMAVFrame();

	int videoPrint();			// 打印视频相关属性
	int audioPrint();			// 打印音频相关属性

	int getWidth();				// 获取图像宽度
	int getHeight();			// 获取图像高度
	int getY(unsigned char* y);	// 把y分量拷贝到buff上
	int getU(unsigned char* u);	// 把u分量拷贝到buff上
	int getV(unsigned char* v);	// 把v分量拷贝到buff上

	long long getPts();			// 获取时间戳

public:
	MMAVFramePrivate* imp = nullptr;
};



class MMAVReaderPrivate;

// 文件读取工具类
class MMAVReader
{
public:
	MMAVReader();
	~MMAVReader();

	int open(const char* path);							// 打开
	int close();										// 关闭
	int read(MMAVPacket* packet);						// 读数据

	int getStreamCount();								// 有多少个stream
	int getStream(MMAVStream* avStream, int streamId);	// 获得流

	int getVideoStreamIndex();							// 视频流索引
	int getAudeoStreamIndex();							// 音频流索引

	int seek(double time);								// 快进后退

private:
	MMAVReaderPrivate* imp = nullptr;
};


// 加上解码的内容
class MMAVStreamPrivate;
class MMAVStream
{
public:
	MMAVStream();
	~MMAVStream();


public:
	int streamIndex = -1;
	int timeBaseNum = 0;
	int timeBaseDen = 0;
	MMAVStreamPrivate* imp = nullptr;
};



class MMAVDecoderPrivate;

class MMAVDecoder
{
public:
	MMAVDecoder();
	~MMAVDecoder();

	int init(MMAVStream* stream);				// 初始化函数
	int sendPacket(MMAVPacket* pkt);			// 发送函数
	int recvFrame(MMAVFrame* frame);			// 接收函数
	int close();								// 关闭函数


private:
	MMAVDecoderPrivate* imp = nullptr;

	int timeBaseNum = 0;
	int timeBaseDen = 0;
};

// 时间戳代码
class MMAVTime
{
public:
	static long long getTime();
};