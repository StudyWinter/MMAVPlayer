#include "MMAV.h"

#include "MMAVReaderPrivate.h"
#include "MMAVPacketPrivate.h"

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