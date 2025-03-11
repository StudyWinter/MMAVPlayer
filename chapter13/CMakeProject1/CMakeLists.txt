# CMakeList.txt: CMakeProject1 的 CMake 项目，在此处包括源代码并定义
# 项目特定的逻辑。
#
cmake_minimum_required (VERSION 3.8)

# include_directories(../3rdpart/ffmpeg/include)
# 从上一级开始查找
include_directories(../)

link_directories(../3rdpart/ffmpeg/lib)

# 将源代码添加到此项目的可执行文件。
add_library (
	CMakeProject1 
	CMakeProject1.cpp
	CMakeProject1.h
	MMPlayerReaderThread.cpp
	MMAVPlayerDecoderThread.cpp
	MMPlayerCtrThread.cpp
)

# TODO: 如有需要，请添加测试并安装目标。

# 添加依赖的库
target_link_libraries(CMakeProject1 MMThread)
target_link_libraries(CMakeProject1 MMAV)

target_link_libraries(CMakeProject1 avcodec)
target_link_libraries(CMakeProject1 avdevice)
target_link_libraries(CMakeProject1 avformat)
target_link_libraries(CMakeProject1 avutil)
target_link_libraries(CMakeProject1 swresample)
target_link_libraries(CMakeProject1 swscale)
