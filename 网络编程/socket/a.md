# 创建socket
```cpp
int socket(int domain, int type, int protocol);
//成功返回一个有效的socket，失败返回-1，errno被设置
//全部网络编程函数，失败时基本上都是返回-1，errno被设置
//单个进程中创建的socket数量与受系统参数open files的限制
```
## domain协议族
```cpp
PF_INET       //IPv4
PF_INET6      //IPv6
PF_LOCAL      //本地
PF_PACKET     //内核底层
PF_IPX        //IPX Novell
```

## type 数据传输的类型
```cpp
SOCK_STREAM //面向socket，数据不会丢失，数据顺序不会错乱，双向通道
SOCK_DGRAM  //无连接的socket，数据可能丢失，数据顺序可能会错乱，传输的效率更高
```

## prorocol 最终使用的协议
在IPv4网络协议家族中，数据传输方式为SOCK_STREAM的协议只有IPPROTO_TCP,数据传输方式为SOCK_DGRAM的协议只有IPPROTO_UDP

本参数也可以填0


