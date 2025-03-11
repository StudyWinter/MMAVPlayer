#include "MMAV.h"
#include "MMAVDecoderPrivate.h"
#include "MMAVStreamPrivate.h"
#include "MMAVPacketPrivate.h"
#include "MMAVFramePrivate.h"
#include <iostream>

// ���캯��
MMAVDecoder::MMAVDecoder()
{
	imp = new MMAVDecoderPrivate();
	// ����
	imp->codecContext = avcodec_alloc_context3(nullptr);

}

// ��������
MMAVDecoder::~MMAVDecoder()
{
	// �ͷ�
	if (imp->codecContext != nullptr) {
		avcodec_free_context(&imp->codecContext);
		imp->codecContext = nullptr;
	}

	if (imp != nullptr) {
		delete imp;
		imp = nullptr;
	}
}

// ��ʼ������
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

// ���ͺ���
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

// ���պ���
int MMAVDecoder::recvFrame(MMAVFrame* frame)
{
	int res = avcodec_receive_frame(imp->codecContext, frame->imp->frame);
	if (!res)
	{
		// ���뼶ʱ������㲢�Ҹ�ֵ
		frame->imp->ptsSecond = frame->imp->frame->pts * 1.0 * timeBaseNum / timeBaseDen;
	}
	return res;
}

// �رպ���
int MMAVDecoder::close()
{
	return avcodec_close(imp->codecContext);
}