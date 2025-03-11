#pragma once
extern "C"
{
#include <libavformat/avformat.h>
}

class MMAVFramePrivate
{
public:
	AVFrame* frame = nullptr;
	double ptsSecond = 0.0;

};