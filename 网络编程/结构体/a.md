# sockaddr
存放协议族、端口和地址信息，客户端和connect函数和服务端bind函数需要这个结构体

支持IPv6
```cpp
struct sockaddr {
    unsigned short sa_family;  //协议族 IPv4 填AF_INET
    unsigned char sa_data[14]; //14字节的端口和地址
};
```
# sockaddr_in
可以强制转换成sockaddr

只支持IPv4
```cpp
struct sockaddr_in {
    unsigned short sin_family; //协议族
    unsigned short sin_port;   //16位端口号
    struct in_addr sin_addr;   //32位地址
    unsigned char sin_zero[8]; //未使用，为保持sockaddr一样的长度而添加
};
struct in_addr {
    unsigned int s_addr;       //32位地址
};
```
# gethostbyname
用域名、主机名、字符串IP获取大端序IP,用于网络通信的客户端程序中
```cpp
struct hostent* gethostbyname(const char* name);
struct hostent {
    char* h_name;              //主机名
    char** h_aliases;          //主机所有别名构成的字符串数组，同意IP可以绑定多个域名
    short h_addrtype;          //主机IP地址的类型 IPv4（AF_INET）
    short h_length;            //主机IP地址长度 IPv4为4 IPv6为6
    char** h_addr_list         //主机IP地址，以网络字节序存储
};
#define h_addr h_addr_list[0]
```
转换后，用以下代码把大端序地址复制到sockaddr_in的sin_addr成员
```cpp
memcpy(&servaddr.sin_addr, h->h_addr, h->h_length);
```
# 字符串IP与大端序IP的转换
下面几个函数主要用于服务端程序中
```cpp
typedef unsigned int in_addr_t;   //32位大端序IP地址

//把字符串格式的IP转换成大端序的IP，转换后的IP赋给sockadd_in.sin_addr.s_addr
in_addr_t inet_addr(const char* cp);

//把字符串格式的IP转换成大端序的IP，转换后的IP填充到sockaddr_in.sin_addr.s_addr
int inet_aton(const char* cp, struct in_addr* inp);

//把大端序IP转换成字符串格式的IP，用于服务端中解析客户端的IP地址
char* inet_ntoa(struct in_addr in);