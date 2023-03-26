cmake_minimum_required(VERSION 3.24)
project(websocket_server)

set(CMAKE_CXX_STANDARD 17)

link_directories(lib)
include_directories(include)
include_directories(.)

set(SOURCE EchoServer.cpp
        WebsocketServer/WebsocketServer.cpp
        base/HttpParser.cpp
        base/sha1_portable.cpp
        base/base64.cpp
        WebsocketServer/DataFrameCodec.cpp)
set(UNOFFICIAL_LIB muduo_net muduo_base pthread)

add_executable(${PROJECT_NAME} main.cpp ${SOURCE})
target_link_libraries(${PROJECT_NAME} ${UNOFFICIAL_LIB})
