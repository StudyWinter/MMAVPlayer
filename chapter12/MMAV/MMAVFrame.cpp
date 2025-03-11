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

// ��ӡ��Ƶ�������
int MMAVFrame::videoPrint()
{
	// ��ӡ���Ϳ�
	std::cout << "Width: " << getWidth() << std::endl;
	std::cout << "Height: " << getHeight() << std::endl;

	// ��ӡ��ʽ������ǰ��yuv���ݣ������ԭ��yuv��ʽ
	AVPixelFormat format = (AVPixelFormat)(imp->frame->format);
	char* str = (char*)malloc(128);
	str = av_get_pix_fmt_string(str, 128, format);

	std::cout << "Pixel format: " << str << std::endl;

	free(str);

	// ��ӡline size
	for (int i = 0; i < AV_NUM_DATA_POINTERS; i++)
	{
		std::cout << "Line size[" << i << "]: " << imp->frame->linesize[i] << std::endl;
	}

	return 0;
}

// ��ӡ��Ƶ�������
int MMAVFrame::audioPrint()
{
	// ������˫����
	int channel = imp->frame->channels;
	std::cout << "channel: " << channel << std::endl;
	std::cout << "nb_samples: " << imp->frame->nb_samples << std::endl;
	std::cout << "sample_rate: " << imp->frame->sample_rate << std::endl;

	// ��ӡ��ʽ������ǰ��yuv���ݣ������ԭ��yuv��ʽ
	AVSampleFormat format = (AVSampleFormat)(imp->frame->format);
	char* str = (char*)malloc(128);
	str = av_get_sample_fmt_string(str, 128, format);

	std::cout << "Sample format: " << str << std::endl;


	// ��ӡline size
	for (int i = 0; i < AV_NUM_DATA_POINTERS; i++)
	{
		std::cout << "Line size[" << i << "]: " << imp->frame->linesize[i] << std::endl;
	}
	free(str);

	return 0;
}

// ��ȡͼ����
int MMAVFrame::getWidth()
{
	return imp->frame->width;
}

// ��ȡͼ��߶�
int MMAVFrame::getHeight()
{
	return imp->frame->height;
}

// ��y����������buff��
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

// ��u����������buff��
int MMAVFrame::getU(unsigned char* u)
{
	// ��Ϊ��yuv420������/2
	int width = getWidth() / 2;
	int height = getHeight() / 2;
	//memcpy(y, imp->frame->data[0], width * height);

	for (int i = 0; i < height; i++)
	{
		memcpy(u + i * width, imp->frame->data[1] + i * imp->frame->linesize[1], width);
	}

	return 0;
}

// ��v����������buff��
int MMAVFrame::getV(unsigned char* v)
{
	// ��Ϊ��yuv420������/2
	int width = getWidth() / 2;
	int height = getHeight() / 2;
	//memcpy(y, imp->frame->data[0], width * height);

	for (int i = 0; i < height; i++)
	{
		memcpy(v + i * width, imp->frame->data[2] + i * imp->frame->linesize[2], width);
	}

	return 0;
}

// ��ȡʱ���
long long MMAVFrame::getPts()
{
	// std::cout << "pts: " << imp->frame->pts << std::endl;
	// std::cout << "pts second: " << imp->ptsSecond << std::endl;
	return (long long)(imp->ptsSecond * 1000);
}