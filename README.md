 ## WebsocketServer

使用 muduo 网络库简单实现的 WebSocket 服务器。支持自定义消息到达回调。

## 使用方法

```cpp
#include "WebSocketServer/WebsocketServer.h"

using namespace muduo::net;

int main()
{
    // Logger::setLogLevel(Logger::DEBUG);
    EventLoop loop;
    WebsocketServer websocketServer(&loop, InetAddress(9000));
    websocketServer.setMessageCallback([]
        (const auto &conn, const auto &msg, auto time){
            printf("%s", msg.c_str());
        });
    websocketServer.start();
    loop.loop();

    return 0;
}
```

## TODO

- 消息到达回调函数的接口优化

  隐藏回调函数的conn接口，将函数返回类型改为std::string

  将返回值打包成数据帧并发送给ws客户端

## 开发日志

- 2023.3.24

  - 开发了 HTTP 请求解析器 HttpParsser 以及字符串处理工具类 StringExtra ，用于处理 Websoket 协议的一次握手。

  - 引入muduo网络库，并且开发了 EchoServer 用于测试 muduo 网络库是否正常运转，以及用于调试 HTTP 请求头的解析功能。

- 2023.3.26

  - 引入了 Base64 等编码库，解决握手过程的编码问题。

  - 定义服务器的两个状态：WAIT_HANDSHAKE、ESTABLISHED，将WAIT_HANDSHAKE状态接收到的数据交给 handleHandshake() 处理；将ESTABLISHED状态接收到的数据交给数据帧解码器 DataFrameCodec::decodeDataFrame处理。

  - 开发DataFrameCodec用于数据帧的解码，解码完成时将回调WebsocketServer中定义的onMessage方法。



