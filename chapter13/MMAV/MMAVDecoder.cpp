#include "MMAV.h"
#include "MMAVDecoderPrivate.h"
#include "MMAVStreamPrivate.h"
#include "MMAVPacketPrivate.h"
#include "MMAVFramePrivate.h"
#include <iostream>

// 构造函数
MMAVDecoder::MMAVDecoder()
{
	imp = new MMAVDecoderPrivate();
	// 分配
	imp->codecContext = avcodec_alloc_context3(nullptr);

}

// 析构函数
MMAVDecoder::~MMAVDecoder()
{
	// 释放
	if (imp->codecContext != nullptr) {
		avcodec_free_context(&imp->codecContext);
		imp->codecContext = nullptr;
	}

	if (imp != nullptr) {
		delete imp;
		imp = nullptr;
	}
}

// 初始化函数
int MMAVDecoder::init(MMAVStream* stream)
{
	timeBaseNum = stream->timeBaseNum;
	timeBaseDen = stream->timeBaseDen;

	// 1 avcodec_parameters_to_context
	avcodec_parameters_to_context(imp->codecContext, stream->imp->codecpar);

	// 2 avcodec_find_decoder
	AVCodec* avCodec = avcodec_find_decoder(imp->codecContext->codec_id);

	// 3 avcodec_open2
	int res = avcodec_open2(imp->codecContext, avCodec, nullptr);
	if (res)
	{
		std::cout << "avcodec_open2 failed:" << res << std::endl;
		return -1;
	}
	return 0;
}

// 发送函数
int MMAVDecoder::sendPacket(MMAVPacket* pkt)
{
	int res = 0;
	if (pkt == nullptr)
	{
		res = avcodec_send_packet(imp->codecContext, nullptr);
	}
	else
	{
		res = avcodec_send_packet(imp->codecContext, pkt->imp->pkt);
	}
	return res;
}

// 接收函数
int MMAVDecoder::recvFrame(MMAVFrame* frame)
{
	int res = avcodec_receive_frame(imp->codecContext, frame->imp->frame);
	if (!res)
	{
		// 将秒级时间戳计算并且赋值
		frame->imp->ptsSecond = frame->imp->frame->pts * 1.0 * timeBaseNum / timeBaseDen;
	}
	return res;
}

// 关闭函数
int MMAVDecoder::close()
{
	return avcodec_close(imp->codecContext);
}