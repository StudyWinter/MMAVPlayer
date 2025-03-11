#include "MMAV.h"
#include "MMAVFramePrivate.h"
#include <iostream>

extern "C"
{
#include <libavutil/pixdesc.h>
}



MMAVFrame::MMAVFrame()
{
	imp = new MMAVFramePrivate();
	imp->frame = av_frame_alloc();

}


MMAVFrame::~MMAVFrame()
{
	if (imp->frame != nullptr)
	{
		av_frame_free(&imp->frame);
		imp->frame = nullptr;
	}
	if (imp != nullptr)
	{
		delete imp;
		imp = nullptr;
	}
}

// 打印视频相关属性
int MMAVFrame::videoPrint()
{
	// 打印长和宽
	std::cout << "Width: " << getWidth() << std::endl;
	std::cout << "Height: " << getHeight() << std::endl;

	// 打印格式。编码前是yuv数据，解码后还原成yuv格式
	AVPixelFormat format = (AVPixelFormat)(imp->frame->format);
	char* str = (char*)malloc(128);
	str = av_get_pix_fmt_string(str, 128, format);

	std::cout << "Pixel format: " << str << std::endl;

	free(str);

	// 打印line size
	for (int i = 0; i < AV_NUM_DATA_POINTERS; i++)
	{
		std::cout << "Line size[" << i << "]: " << imp->frame->linesize[i] << std::endl;
	}

	return 0;
}

// 打印音频相关属性
int MMAVFrame::audioPrint()
{
	// 声道、双声道
	int channel = imp->frame->channels;
	std::cout << "channel: " << channel << std::endl;
	std::cout << "nb_samples: " << imp->frame->nb_samples << std::endl;
	std::cout << "sample_rate: " << imp->frame->sample_rate << std::endl;

	// 打印格式。编码前是yuv数据，解码后还原成yuv格式
	AVSampleFormat format = (AVSampleFormat)(imp->frame->format);
	char* str = (char*)malloc(128);
	str = av_get_sample_fmt_string(str, 128, format);

	std::cout << "Sample format: " << str << std::endl;


	// 打印line size
	for (int i = 0; i < AV_NUM_DATA_POINTERS; i++)
	{
		std::cout << "Line size[" << i << "]: " << imp->frame->linesize[i] << std::endl;
	}
	free(str);

	return 0;
}

// 获取图像宽度
int MMAVFrame::getWidth()
{
	return imp->frame->width;
}

// 获取图像高度
int MMAVFrame::getHeight()
{
	return imp->frame->height;
}

// 把y分量拷贝到buff上
int MMAVFrame::getY(unsigned char* y)
{
	int width = getWidth();
	int height = getHeight();
	//memcpy(y, imp->frame->data[0], width * height);

	for (int i = 0; i < height; i++)
	{
		memcpy(y + i * width, imp->frame->data[0] + i * imp->frame->linesize[0], width);
	}

	return 0;
}

// 把u分量拷贝到buff上
int MMAVFrame::getU(unsigned char* u)
{
	// 因为是yuv420，所以/2
	int width = getWidth() / 2;
	int height = getHeight() / 2;
	//memcpy(y, imp->frame->data[0], width * height);

	for (int i = 0; i < height; i++)
	{
		memcpy(u + i * width, imp->frame->data[1] + i * imp->frame->linesize[1], width);
	}

	return 0;
}

// 把v分量拷贝到buff上
int MMAVFrame::getV(unsigned char* v)
{
	// 因为是yuv420，所以/2
	int width = getWidth() / 2;
	int height = getHeight() / 2;
	//memcpy(y, imp->frame->data[0], width * height);

	for (int i = 0; i < height; i++)
	{
		memcpy(v + i * width, imp->frame->data[2] + i * imp->frame->linesize[2], width);
	}

	return 0;
}

// 获取时间戳
long long MMAVFrame::getPts()
{
	// std::cout << "pts: " << imp->frame->pts << std::endl;
	// std::cout << "pts second: " << imp->ptsSecond << std::endl;
	return (long long)(imp->ptsSecond * 1000);
}