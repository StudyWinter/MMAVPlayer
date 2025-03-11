#include <iostream>
#include "CMakeProject1/CMakeProject1.h"
#include <windows.h>

int main(int argc, char** argv)
{
	MMPlayer player("C:\\Users\\winter\\Desktop\\1.mp4");
	int res = player.open();
	if (res)
	{
		std::cout << "open failed" << std::endl;
		return -1;
	}

	std::this_thread::sleep_for(std::chrono::seconds(1));



	player.play();

	// wait......
	std::this_thread::sleep_for(std::chrono::seconds(1));

	// ��ͣ
	player.pause();
	
	// wait......
	std::this_thread::sleep_for(std::chrono::seconds(1));

	// ����
	player.play();

	// wait......
	std::this_thread::sleep_for(std::chrono::seconds(1));


	// ֹͣ
	player.stop();

	system("pause");
	return 0;
}