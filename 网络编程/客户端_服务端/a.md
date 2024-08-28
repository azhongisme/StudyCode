# 客户端
## 创建客户端的socket
```cpp
int sockfd = socket(AF_INET, SOCK_STREAM, 0);
if (sockfd == -1) {
    perror("socket");
    return -1;
}
```
## 向服务端发起连接请求
```cpp
    struct hostent* h;
    // 把字符串格式的IP转换成结构体。
    if ( (h = gethostbyname(argv[1])) == 0 )  { 
        std::cout << "gethostbyname failed.\n" << std::endl; close(sockfd); return -1;
    }
    struct sockaddr_in servaddr;              // 用于存放服务端IP和端口的结构体。
    memset(&servaddr,0,sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    memcpy(&servaddr.sin_addr,h->h_addr,h->h_length); // 指定服务端的IP地址。
    servaddr.sin_port = htons(atoi(argv[2]));         // 指定服务端的通信端口。

    // 向服务端发起连接清求。
    if (connect(sockfd,(struct sockaddr *)&servaddr,sizeof(servaddr))!=0)  { 
        perror("connect");
        close(sockfd);
        return -1; 
    }
```
## 通讯
通讯的方式有很多种
```cpp
     // 第3步：与服务端通讯，客户发送一个请求报文后等待服务端的回复，收到回复后，再发下一个请求报文。
    char buffer[1024];
    // 循环3次，将与服务端进行三次通讯。
    for (int ii = 0;ii < 3; ii++) {
        int iret;
        memset(buffer, 0, sizeof(buffer));
        sprintf(buffer,"这是第%d个超级女生，编号%03d。", ii+1, ii+1);  // 生成请求报文内容。
        // 向服务端发送请求报文。
        if ( (iret = send(sockfd,buffer,strlen(buffer),0)) <= 0) { 
            perror("send"); break; 
        }
        std::cout << "发送：" << buffer << std::endl;

        memset(buffer,0,sizeof(buffer));
        // 接收服务端的回应报文，如果服务端没有发送回应报文，recv()函数将阻塞等待。
        if ( (iret = recv(sockfd,buffer,sizeof(buffer),0)) <= 0) {
        std::cout << "iret=" << iret << std::endl; break;
        }
        std::cout << "接收：" << buffer << std::endl;
        sleep(1);
    }
```
## 释放资源

# 服务端
## 创建socket
```cpp
  // 第1步：创建服务端的socket。 
  int listenfd = socket(AF_INET,SOCK_STREAM,0);
  if (listenfd == -1) { 
    perror("socket"); return -1; 
  }
```
## 绑定到socket
```cpp
  // 第2步：把服务端用于通信的IP和端口绑定到socket上。 
  struct sockaddr_in servaddr;          // 用于存放服务端IP和端口的数据结构。
  memset(&servaddr,0,sizeof(servaddr));
  servaddr.sin_family = AF_INET;        // 指定协议。
  servaddr.sin_addr.s_addr = htonl(INADDR_ANY); // 服务端任意网卡的IP都可以用于通讯。
  servaddr.sin_port = htons(atoi(argv[1]));     // 指定通信端口，普通用户只能用1024以上的端口。
  // 绑定服务端的IP和端口。
  if (bind(listenfd,(struct sockaddr *)&servaddr,sizeof(servaddr)) != 0 ) { 
    perror("bind"); close(listenfd); return -1; 
  }
```
## 把socket设置为可连接（监听）的状态。
```cpp
  if (listen(listenfd,5) != 0 ) { 
    perror("listen"); close(listenfd);
    return -1; 
  }
```
## 受理客户端的连接请求，如果没有客户端连上来，accept()函数将阻塞等待。
```cpp
  int clientfd=accept(listenfd,0,0);
  if (clientfd==-1) {
    perror("accept");
    close(listenfd);
    return -1; 
  }
```
## 与客户端通信，接收客户端发过来的报文后，回复ok。
```cpp
  char buffer[1024];
  while (true) {
    int iret;
    memset(buffer,0,sizeof(buffer));
    // 接收客户端的请求报文，如果客户端没有发送请求报文，recv()函数将阻塞等待。
    // 如果客户端已断开连接，recv()函数将返回0。
    if ( (iret=recv(clientfd,buffer,sizeof(buffer),0))<=0) {
       std::cout << "iret=" << iret << std::endl;  break;   
    }
    std::cout << "接收：" << buffer << std::endl;
 
    strcpy(buffer,"ok");  // 生成回应报文内容。
    // 向客户端发送回应报文。
    if ( (iret=send(clientfd,buffer,strlen(buffer),0))<=0) { 
      perror("send"); break; 
    }
    std::cout << "发送：" << buffer << std::endl;
  }
```
## 关闭socket，释放资源。
```cpp
  close(listenfd);   // 关闭服务端用于监听的socket。
  close(clientfd);   // 关闭客户端连上来的socket。
```