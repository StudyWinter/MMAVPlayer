#include "MMAV.h"

#include "MMAVReaderPrivate.h"
#include "MMAVPacketPrivate.h"
#include "MMAVStreamPrivate.h"
#include <iostream>

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
	if (imp != nullptr)
	{
		delete imp;
		imp = nullptr;
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

// 有多少个stream
int MMAVReader::getStreamCount()
{
	return imp->formatCtx->nb_streams;
}

// 获得流
int MMAVReader::getStream(MMAVStream* avStream, int streamId)
{
	AVStream* myStream = imp->formatCtx->streams[streamId];

	// std::cout << "time base num: " << myStream->time_base.num << std::endl;
	// std::cout << "time base den: " << myStream->time_base.den << std::endl;

	avStream->timeBaseNum = myStream->time_base.num;
	avStream->timeBaseDen = myStream->time_base.den;

	avStream->streamIndex = myStream->index;
	// 将后面的数据拷贝到前面
	avcodec_parameters_copy(avStream->imp->codecpar, myStream->codecpar);

	return 0;
}

// 视频流索引
int MMAVReader::getVideoStreamIndex()
{
	if (imp->formatCtx == nullptr)
	{
		return -1;
	}
	return av_find_best_stream(imp->formatCtx, AVMediaType::AVMEDIA_TYPE_VIDEO, -1, -1, NULL, NULL);
}

// 音频流索引
int MMAVReader::getAudeoStreamIndex()
{
	if (imp->formatCtx == nullptr)
	{
		return -1;
	}
	return av_find_best_stream(imp->formatCtx, AVMediaType::AVMEDIA_TYPE_AUDIO, -1, -1, NULL, NULL);
}

// 快进后退
int MMAVReader::seek(double time)
{
	if (imp->formatCtx == nullptr)
	{
		return -1;
	}
	
	int64_t timestamp = (int64_t)(time * AV_TIME_BASE);
	av_seek_frame(imp->formatCtx, -1, timestamp, AVSEEK_FLAG_BACKWARD);			// seek到之前的I帧上去
	return 0;
}