#include "MMAV.h"

#include "MMAVReaderPrivate.h"
#include "MMAVPacketPrivate.h"

// 构造函数
MMAVReader::MMAVReader()
{
	// 分配
	imp = new MMAVReaderPrivate();
	imp->formatCtx = avformat_alloc_context();
}
// 析构函数
MMAVReader::~MMAVReader()
{
	// 释放
	if (imp->formatCtx != nullptr)
	{
		avformat_free_context(imp->formatCtx);
		imp->formatCtx = nullptr;
	}
}

// 打开
int MMAVReader::open(const char* path)
{
	if (imp->formatCtx == nullptr || path == nullptr)
	{
		return -1;
	}
	int res = avformat_open_input(&imp->formatCtx, path, nullptr, nullptr);
	// 成功
	if (res == 0)
	{
		avformat_find_stream_info(imp->formatCtx, nullptr);
	}
	return res;
}
// 关闭
int MMAVReader::close()
{
	if (imp->formatCtx == nullptr)
	{
		return -1;
	}
	avformat_close_input(&imp->formatCtx);
	return 0;
}

// 读数据
int MMAVReader::read(MMAVPacket* packet)
{
	if (imp->formatCtx == nullptr)
	{
		return -1;
	}
	int res = av_read_frame(imp->formatCtx, packet->imp->pkt);
	return res;
}