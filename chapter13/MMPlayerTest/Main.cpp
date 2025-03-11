#include <iostream>
#include "CMakeProject1/CMakeProject1.h"
#include <windows.h>

int main(int argc, char** argv)
{
	MMPlayer player("C:\\Users\\winter\\Desktop\\video1.mp4");
	int res = player.open();
	if (res)
	{
		std::cout << "open failed" << std::endl;
		return -1;
	}

	// std::this_thread::sleep_for(std::chrono::seconds(1));


	player.play();
	std::cout << "player.play();-----------------------1" << std::endl;

	// wait......
	std::this_thread::sleep_for(std::chrono::seconds(1));


	// seekµ½5Ãë
	player.seek(5);
	std::this_thread::sleep_for(std::chrono::seconds(2));


	// ÔÝÍ£
	player.pause();
	std::cout << "player.pause();-----------------------1" << std::endl;
	
	// wait......
	std::this_thread::sleep_for(std::chrono::seconds(1));

	// ²¥·Å
	player.play();
	std::cout << "player.play();-----------------------2" << std::endl;


	// wait......
	std::this_thread::sleep_for(std::chrono::seconds(60 * 60));


	// Í£Ö¹
	player.stop();

	system("pause");
	return 0;
}