# CMakeList.txt: CMakeProject1 的 CMake 项目，在此处包括源代码并定义
# 项目特定的逻辑。
#
cmake_minimum_required (VERSION 3.8)

include_directories(../3rdpart/ffmpeg/include)


# 将源代码添加到此项目的可执行文件。
# 将.cpp/.c/.cc文件生成.a静态库
# 注意，库文件名称通常为libxxx.so，在这里只要写xxx即可
add_library(MMAV
			MMAVReader.cpp
			MMAVPacket.cpp
			MMAVDecoder.cpp
			MMAVStream.cpp
			MMAVFrame.cpp
)

# TODO: 如有需要，请添加测试并安装目标。

