#pragma once

class MMAVPacketPrivate;

class MMAVPacket
{
public:
	MMAVPacket();
	~MMAVPacket();

public:
	MMAVPacketPrivate* imp = nullptr;
};


class MMAVReaderPrivate;

// 文件读取工具类
class MMAVReader
{
public:
	MMAVReader();
	~MMAVReader();

	int open(const char* path);			// 打开
	int close();						// 关闭
	int read(MMAVPacket* packet);		// 读数据

private:
	MMAVReaderPrivate* imp = nullptr;
};