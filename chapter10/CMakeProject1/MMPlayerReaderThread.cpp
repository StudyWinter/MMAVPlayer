#include "CMakeProject1.h"
#include "MMAV/MMAV.h"


MMPlayerReaderThread::MMPlayerReaderThread(std::string _path)
{
	path = _path;
}

MMPlayerReaderThread::~MMPlayerReaderThread()
{

}

// ���麯�����̻߳�����������
void MMPlayerReaderThread::run()
{
	MMAVReader reader;
	int res = reader.open(path.c_str());
	if (res)
	{
		std::cout << "open failed" << std::endl;
		return;
	}
	// �򿪳ɹ����õ�����Ƶ������
	int audioStreamIndex = reader.getAudeoStreamIndex();
	int videoStreamIndex = reader.getVideoStreamIndex();

	// ��ʼ��������


	// ����
	while (!stopFlag)
	{
		MMAVPacket* packet = new MMAVPacket();
		int res = reader.read(packet);
		if (res)
		{
			delete packet;
			packet = nullptr;
			break;
		}

		// ��packet���뻺��
		std::cout << "reader packet success" << std::endl;
		delete packet;
		packet = nullptr;
	}

	reader.close();
}