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

