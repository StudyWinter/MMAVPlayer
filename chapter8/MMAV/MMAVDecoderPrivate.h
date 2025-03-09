#pragma once
extern "C"
{
#include <libavcodec/avcodec.h>
}

class MMAVDecoderPrivate
{
public:
	AVCodecContext* codecContext = nullptr;
};
