cmake_minimum_required(VERSION 3.13)
project(Dim)

set(CMAKE_CXX_STANDARD 14)

add_executable(Dim src/DimProtocol.h src/UDPUtils.h src/UDPUtils.cpp src/Test.cpp src/DimClient.h src/DimClient.cpp src/Arrays.cpp src/Arrays.h src/DateTimeUtils.cpp src/DateTimeUtils.h src/NetUtils.cpp src/NetUtils.h src/NumberUtils.cpp src/NumberUtils.h src/Constants.h src/boot.cpp src/boot.h src/Front.cpp src/Front.h src/debug.h src/StringUtils.cpp src/StringUtils.h src/IteratorHelper.cpp src/IteratorHelper.h src/Collections.cpp src/Collections.h)
#解决对‘pthread_create’未定义的引用
find_package(Threads REQUIRED)
target_link_libraries(Dim Threads::Threads)