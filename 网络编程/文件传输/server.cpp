#include <iostream>
#include <fstream>
#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <unistd.h>
#include <netdb.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <signal.h>
using namespace std;

class ctcpserver         // TCP通讯的服务端类。
{
private:
  int    m_listenfd;        // 监听的socket，-1表示未初始化。
  int    m_clientfd;        // 客户端连上来的socket，-1表示客户端未连接。
  string m_clientip;        // 客户端字符串格式的IP。
  unsigned short m_port;    // 服务端用于通讯的端口。
public:
  ctcpserver() : m_listenfd(-1), m_clientfd(-1) {}

  // 初始化服务端用于监听的socket。
  bool initserver(const unsigned short in_port)
  {
    // 第1步：创建服务端的socket。 
    if ( (m_listenfd = socket(AF_INET, SOCK_STREAM, 0)) == -1) return false;

    m_port = in_port;
  
    // 第2步：把服务端用于通信的IP和端口绑定到socket上。 
    struct sockaddr_in servaddr;                // 用于存放协议、端口和IP地址的结构体。
    memset(&servaddr, 0, sizeof(servaddr));
    servaddr.sin_family = AF_INET;                // ①协议族，固定填AF_INET。
    servaddr.sin_port = htons(m_port);            // ②指定服务端的通信端口。
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY); // ③如果操作系统有多个IP，全部的IP都可以用于通讯。

    // 绑定服务端的IP和端口（为socket分配IP和端口）。
    if (bind(m_listenfd, (struct sockaddr *)&servaddr, sizeof(servaddr)) == -1) { 
      close(m_listenfd);
      m_listenfd = -1;
      return false; 
    }
 
    // 第3步：把socket设置为可连接（监听）的状态。
    if (listen(m_listenfd, 5) == -1 ) { 
      close(m_listenfd);
      m_listenfd=-1;
      return false;
    }

    return true;
  }

  // 受理客户端的连接（从已连接的客户端中取出一个客户端），
  // 如果没有已连接的客户端，accept()函数将阻塞等待。
  bool accept() {
    struct sockaddr_in caddr;        // 客户端的地址信息。  
    socklen_t addrlen = sizeof(caddr); // struct sockaddr_in的大小。
    if ((m_clientfd = ::accept(m_listenfd, (struct sockaddr *)&caddr, &addrlen)) == -1) return false;

    m_clientip = inet_ntoa(caddr.sin_addr);  // 把客户端的地址从大端序转换成字符串。

    return true;
  }

  // 获取客户端的IP(字符串格式)。
  const string & clientip() const {
    return m_clientip;
  }

  // 向对端发送报文，成功返回true，失败返回false。
  bool send(const string &buffer) {
    if (m_clientfd == -1) return false;

    if ( (::send(m_clientfd, buffer.data(), buffer.size(), 0)) <= 0) return false;
   
    return true;
  }

  // 接收对端的报文，成功返回true，失败返回false。
  // buffer-存放接收到的报文的内容，maxlen-本次接收报文的最大长度。
  bool recv(string &buffer,const size_t maxlen)
  { 
    buffer.clear();         // 清空容器。
    buffer.resize(maxlen);  // 设置容器的大小为maxlen。
    int readn = ::recv(m_clientfd, &buffer[0], buffer.size(), 0);  // 直接操作buffer的内存。
    if (readn <= 0) { buffer.clear(); return false; }
    buffer.resize(readn);   // 重置buffer的实际大小。

    return true;
  }

  bool recv(void* buffer, const size_t size) 
  {
    int readn = ::recv(m_clientfd, buffer, size, 0);
    if (readn <= 0) {
      return false;
    }
    return true;
  }

  bool recvFile(const std::string& file_name, const size_t& file_size) {
    std::ofstream fout;
    fout.open(file_name, std::ios::binary);
    if (fout.is_open() == false) {
      std::cout << "open file failed\n";
      return false;
    }

    int totalbytes = 0;
    int onread = 0;
    char buffer[256];

    while (true) {
      onread = (file_size - totalbytes) > 256 ? 256 : file_size - totalbytes;
      if (recv(buffer, onread) == false) {
        return false;
      }

      fout.write(buffer, onread);

      totalbytes += onread;
      if (totalbytes == file_size) break;

    }

    return true;
  }
  
  // 关闭监听的socket。
  bool closelisten() {
    if (m_listenfd == -1) {
      return false; 
    }
    ::close(m_listenfd);
    m_listenfd = -1;
    return true;
  }

  // 关闭客户端连上来的socket。
  bool closeclient() {
    if (m_clientfd == -1) {
      return false;
    }
    ::close(m_clientfd);
    m_clientfd = -1;
    return true;
  }

 ~ctcpserver() { closelisten(); closeclient(); }
};

ctcpserver tcpserver;

void ChildExit(int sig) {
  signal(SIGINT, SIG_IGN);
  signal(SIGTERM, SIG_IGN);
  std::cout << "child process " << getpid() << "exit, sig = " << sig << std::endl;
  tcpserver.closeclient();
  exit(0);
}

void FathExit(int sig) {
  signal(SIGINT, SIG_IGN);
  signal(SIGTERM, SIG_IGN);
  std::cout << "parent process exit, sig = " << sig << std::endl;
  kill(0, SIGTERM);
  tcpserver.closelisten();
  exit(0);
}

struct st_fileinfo {
  char filename[256];
  int filesize;
}fileinfo;
 
int main(int argc,char *argv[]) {
  if (argc != 3)
  {
    return -1;
  }

  for (int i = 1; i <= 64; ++i) {
    signal(i, SIG_IGN);
  }

  signal(SIGTERM, FathExit);
  signal(SIGINT, FathExit);

  if (tcpserver.initserver(atoi(argv[1])) == false) // 初始化服务端用于监听的socket。
  {
    perror("initserver()"); return -1;
  }

  while (true) {
    // 受理客户端的连接（从已连接的客户端中取出一个客户端），  
    // 如果没有已连接的客户端，accept()函数将阻塞等待。
    if (tcpserver.accept() == false)
    {
      perror("accept()"); return -1;
    }

    int pid = fork();
    if (pid == -1) {
      return -1;
    }
    if (pid > 0) {
      tcpserver.closeclient();
      continue;
    }

    tcpserver.closelisten();
    signal(SIGTERM, ChildExit);
    signal(SIGINT, SIG_IGN);
    cout << "客户端已连接(" << tcpserver.clientip() << ")。\n";

    //1.接收文件名和文件大小
    memset(&fileinfo, 0, sizeof fileinfo);
    if (tcpserver.recv(&fileinfo, sizeof fileinfo) == false) {
      perror("recv");
      return -1;
    }

    std::cout << "LOG: " << "fileSize = " << fileinfo.filesize << ", fileName = " << fileinfo.filename << std::endl;

    //2.给客户端回复报文，表示客户端可以发送文件了
    if (tcpserver.send("OK") == false) {
      perror("send");
      return -1;
    }

    //3.接收文件内容
    if (tcpserver.recvFile(std::string(argv[2]) + "/" + fileinfo.filename, fileinfo.filesize) == false) {
      std::cout << "recv file failed\n";
      return -1;
    }

    std::cout << "LOG recv file success\n";

    //4.给客户端回复确认报文
    if (tcpserver.send("OK") == false) {
      perror("send");
      return -1;
    }
    return 0;
  }

}

