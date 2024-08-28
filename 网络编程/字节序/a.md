# 大端序
```
0x00000001        0x12
0x00000002        0x34
0x00000003        0x56
0x00000004        0x78
```
# 小端序
```
0x00000001        0x78
0x00000002        0x56
0x00000003        0x34
0x00000004        0x12
```
Intel系列CPU采用小端序

# 网络字节序
为了解决不同字节序的计算机之间传输数据的问题，约定采用网络字节序
C语言提供四个函数，用于在主机字节序和网络字节序之间的转换
```cpp
uint16_t htons(uint16_t hostshort);
uint32_t htonl(uint32_t hostlong);
uint16_t ntohs(uint16_t netshort);
uint32_t ntohl(uint32_t netlong);
//h host 主机
//to 转换
//n network
//s short
//l long
```

# IP地址和通讯端口
在计算机中，IPv4的地址用4字节对策整数存放，通讯端口用2字节的整数存放（0-65535）

# 处理大小端序
在网络编程中，数据收发的时候有自动转换机制，只有向sockaddr_in结构体成员变量填充数据时，才需要考虑字节序的问题
