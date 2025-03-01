#pragma once

class MMAVPacketPrivate;

class MMAVPacket
{
public:
	MMAVPacket();
	~MMAVPacket();

public:
	MMAVPacketPrivate* imp = nullptr;
};


class MMAVReaderPrivate;

// �ļ���ȡ������
class MMAVReader
{
public:
	MMAVReader();
	~MMAVReader();

	int open(const char* path);			// ��
	int close();						// �ر�
	int read(MMAVPacket* packet);		// ������

private:
	MMAVReaderPrivate* imp = nullptr;
};