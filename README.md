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

