#pragma once

class MMAVPacketPrivate;
class MMAVStreamPrivate;
class MMAVStream;

class MMAVPacket
{
public:
	MMAVPacket();
	~MMAVPacket();

	int getIndex();

public:
	MMAVPacketPrivate* imp = nullptr;
};

class MMAVFramePrivate;

class MMAVFrame
{
public:
	MMAVFrame();
	~MMAVFrame();

	int videoPrint();			// ��ӡ��Ƶ�������
	int audioPrint();			// ��ӡ��Ƶ�������

	int getWidth();				// ��ȡͼ����
	int getHeight();			// ��ȡͼ��߶�
	int getY(unsigned char* y);	// ��y����������buff��
	int getU(unsigned char* u);	// ��u����������buff��
	int getV(unsigned char* v);	// ��v����������buff��

	long long getPts();			// ��ȡʱ���

public:
	MMAVFramePrivate* imp = nullptr;
};



class MMAVReaderPrivate;

// �ļ���ȡ������
class MMAVReader
{
public:
	MMAVReader();
	~MMAVReader();

	int open(const char* path);							// ��
	int close();										// �ر�
	int read(MMAVPacket* packet);						// ������

	int getStreamCount();								// �ж��ٸ�stream
	int getStream(MMAVStream* avStream, int streamId);	// �����

	int getVideoStreamIndex();							// ��Ƶ������
	int getAudeoStreamIndex();							// ��Ƶ������

	int seek(double time);								// �������

private:
	MMAVReaderPrivate* imp = nullptr;
};


// ���Ͻ��������
class MMAVStreamPrivate;
class MMAVStream
{
public:
	MMAVStream();
	~MMAVStream();


public:
	int streamIndex = -1;
	int timeBaseNum = 0;
	int timeBaseDen = 0;
	MMAVStreamPrivate* imp = nullptr;
};



class MMAVDecoderPrivate;

class MMAVDecoder
{
public:
	MMAVDecoder();
	~MMAVDecoder();

	int init(MMAVStream* stream);				// ��ʼ������
	int sendPacket(MMAVPacket* pkt);			// ���ͺ���
	int recvFrame(MMAVFrame* frame);			// ���պ���
	int close();								// �رպ���


private:
	MMAVDecoderPrivate* imp = nullptr;

	int timeBaseNum = 0;
	int timeBaseDen = 0;
};

// ʱ�������
class MMAVTime
{
public:
	static long long getTime();
};