#include "MMAV.h"
#include "MMAVPacketPrivate.h"

MMAVPacket::MMAVPacket()
{
	imp = new MMAVPacketPrivate();
	// ·ÖÅä
	imp->pkt = av_packet_alloc();
}

MMAVPacket::~MMAVPacket()
{
	if (imp->pkt != nullptr)
	{
		// ÊÍ·Å
		av_packet_free(&imp->pkt);
		imp->pkt = nullptr;
	}
	if (imp != nullptr)
	{
		delete imp;
		imp = nullptr;
	}
}

int MMAVPacket::getIndex()
{
	return imp->pkt->stream_index;
}

