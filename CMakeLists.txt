cmake_minimum_required(VERSION 3.24)
project(websocket_server)

set(CMAKE_CXX_STANDARD 17)

link_directories(lib)
include_directories(include)
include_directories(src)

find_package(cpr CONFIG REQUIRED)

set(SOURCE src/EchoServer.cpp src/WebsocketServer.cpp src/base/HttpParser.cpp src/base/HttpParser.h)
set(UNOFFICIAL_LIB cpr::cpr muduo_net muduo_base pthread)

add_executable(${PROJECT_NAME} main.cpp ${SOURCE})
target_link_libraries(${PROJECT_NAME} ${UNOFFICIAL_LIB})

add_executable(test test.cpp ${SOURCE})
target_link_libraries(test ${UNOFFICIAL_LIB})