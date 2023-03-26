cmake_minimum_required(VERSION 3.24)
project(websocket_server)

set(CMAKE_CXX_STANDARD 17)

link_directories(lib)
include_directories(include)
include_directories(src)

find_package(cpr CONFIG REQUIRED)

set(SOURCE EchoServer.cpp src/WebSocketServer/WebsocketServer.cpp src/base/HttpParser.cpp src/base/HttpParser.h src/base/sha1_portable.cpp src/base/base64.cpp)
set(UNOFFICIAL_LIB cpr::cpr muduo_net muduo_base pthread)

add_executable(${PROJECT_NAME} main.cpp ${SOURCE})
target_link_libraries(${PROJECT_NAME} ${UNOFFICIAL_LIB})

add_executable(test test.cpp ${SOURCE})
target_link_libraries(test ${UNOFFICIAL_LIB})