cmake_minimum_required(VERSION 3.10)

project(WebCamRtspServer LANGUAGES CXX)

# 设置C++标准
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 查找OpenCV
find_package(OpenCV REQUIRED)

include_directories(
    ${CMAKE_SOURCE_DIR}/liveMedia/include
    ${CMAKE_SOURCE_DIR}/BasicUsageEnvironment/include
    ${CMAKE_SOURCE_DIR}/UsageEnvironment/include
    ${CMAKE_SOURCE_DIR}/groupsock/include
    ${OpenCV_INCLUDE_DIRS}
)

file(GLOB MEDIA_SERVER_SRCS "*.cpp" "*.hpp")

add_executable(WebCamRtspServer ${MEDIA_SERVER_SRCS})

target_link_libraries(WebCamRtspServer PRIVATE 
    liveMedia 
    BasicUsageEnvironment 
    groupsock 
    UsageEnvironment
    ${OpenCV_LIBS}
)

# 设置输出目录
set(LIBRARY_OUTPUT_PATH ${CMAKE_BINARY_DIR}/lib)
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR}/bin)
