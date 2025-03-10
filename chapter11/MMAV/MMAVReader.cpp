#include "MMAV.h"

#include "MMAVReaderPrivate.h"
#include "MMAVPacketPrivate.h"
#include "MMAVStreamPrivate.h"

// ���캯��
MMAVReader::MMAVReader()
{
	// ����
	imp = new MMAVReaderPrivate();
	imp->formatCtx = avformat_alloc_context();
}
// ��������
MMAVReader::~MMAVReader()
{
	// �ͷ�
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

// ��
int MMAVReader::open(const char* path)
{
	if (imp->formatCtx == nullptr || path == nullptr)
	{
		return -1;
	}
	int res = avformat_open_input(&imp->formatCtx, path, nullptr, nullptr);
	// �ɹ�
	if (res == 0)
	{
		avformat_find_stream_info(imp->formatCtx, nullptr);
	}
	return res;
}
// �ر�
int MMAVReader::close()
{
	if (imp->formatCtx == nullptr)
	{
		return -1;
	}
	avformat_close_input(&imp->formatCtx);
	return 0;
}

// ������
int MMAVReader::read(MMAVPacket* packet)
{
	if (imp->formatCtx == nullptr)
	{
		return -1;
	}
	int res = av_read_frame(imp->formatCtx, packet->imp->pkt);
	return res;
}

// �ж��ٸ�stream
int MMAVReader::getStreamCount()
{
	return imp->formatCtx->nb_streams;
}

// �����
int MMAVReader::getStream(MMAVStream* avStream, int streamId)
{
	AVStream* myStream = imp->formatCtx->streams[streamId];
	avStream->streamIndex = myStream->index;
	// ����������ݿ�����ǰ��
	avcodec_parameters_copy(avStream->imp->codecpar, myStream->codecpar);

	return 0;
}

// ��Ƶ������
int MMAVReader::getVideoStreamIndex()
{
	return av_find_best_stream(imp->formatCtx, AVMediaType::AVMEDIA_TYPE_VIDEO, -1, -1, NULL, NULL);
}

// ��Ƶ������
int MMAVReader::getAudeoStreamIndex()
{
	return av_find_best_stream(imp->formatCtx, AVMediaType::AVMEDIA_TYPE_AUDIO, -1, -1, NULL, NULL);
}