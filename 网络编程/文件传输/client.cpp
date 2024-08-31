#include <iostream>
#include <fstream>
#include <cstring>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <unistd.h>

class ctcpclient {
public:
  int m_clientfd;
  std::string m_ip;
  unsigned short m_port;
  ctcpclient();
  ctcpclient(const ctcpclient&) = delete;
  ctcpclient& operator = (const ctcpclient&) = delete;
  ~ctcpclient();
  bool connect(const std::string& in_ip, const unsigned short in_port);
  bool send(const std::string& buffer);
  bool send(void* buffer, size_t size);
  bool sendFile(const std::string& file_name, const size_t& file_size);
  bool recv(std::string& buffer, const size_t maxlen);
  bool close();
};

ctcpclient::ctcpclient() : m_clientfd(-1) { close(); }
ctcpclient::~ctcpclient() {}

bool ctcpclient::connect(const std::string& in_ip, const unsigned short in_port) {
  if (m_clientfd != -1) {
    return false;
  }

  m_ip = in_ip, m_port = in_port;

  m_clientfd = socket(AF_INET, SOCK_STREAM, 0);
  if (m_clientfd == -1) {
    return false;
  }

  struct sockaddr_in servaddr;
  memset(&servaddr, 0, sizeof(servaddr));
  servaddr.sin_family = AF_INET;
  servaddr.sin_port = htons(in_port);

  struct hostent* h;
  if ((h = gethostbyname(m_ip.c_str())) == nullptr) {
    ::close(m_clientfd);
    m_clientfd = -1;
    return false;
  }
  memcpy(&servaddr.sin_addr, h->h_addr, h->h_length);

  if (::connect(m_clientfd, (struct sockaddr*)&servaddr, sizeof(servaddr)) == -1) {
    ::close(m_clientfd);
    m_clientfd = -1;
    return false;
  }

  return true;
}

bool ctcpclient::send(const std::string& buffer) {
  if (m_clientfd == -1) {
    return false;
  }

  if (::send(m_clientfd, buffer.data(), buffer.size(), 0) <= 0) {
    return false;
  }

  return true;
}

bool ctcpclient::send(void* buffer, size_t size) {
  if (m_clientfd == -1) {
    return false;
  }

  if (::send(m_clientfd, buffer, size, 0) <= 0) {
    return false;
  }

  return true;
}

bool ctcpclient::sendFile(const std::string& file_name, const size_t& file_size) {
  std::ifstream fin(file_name, std::ios::binary);
  if (fin.is_open() == false) {
    std::cout << "open file failed\n";
    return false;
  }

  int onread = 0;
  int totalbytes = 0;
  char buffer[256];
  
  while (true) {
    memset(buffer, 0, sizeof buffer);
    
    onread = (file_size - totalbytes) > 256 ? 256 : file_size - totalbytes;

    fin.read(buffer, onread);

    if (send(buffer, onread) == false) {
      return false;
    }

    totalbytes = totalbytes + onread;

    if (totalbytes == file_size) break;
  }

  return true;
}

bool ctcpclient::recv(std::string& buffer, const size_t maxlen) {
  buffer.clear();
  buffer.resize(maxlen);
  int readen = ::recv(m_clientfd, &buffer[0], buffer.size(), 0);
  if (readen <= 0) {
    buffer.clear();
    return false;
  }
  buffer.resize(readen);

  return true;
}

bool ctcpclient::close() {
  if (m_clientfd == -1) {
    return false;
  }
  ::close(m_clientfd);
  m_clientfd = -1;
  return true;
}

struct st_fileinfo {
  char filename[256];
  int filesize;
}fileinfo;

int main(int argc, char* argv[]) {
  ctcpclient tcpclient;
  if (!tcpclient.connect(argv[1], atoi(argv[2]))) {
    perror("connect");
    return -1;
  }
  //1.把待传输的文件名和文件大小告诉服务端
  memset(&fileinfo, 0, sizeof(fileinfo));
  strcpy(fileinfo.filename, argv[3]);
  fileinfo.filesize = atoi(argv[4]);

  if (tcpclient.send(&fileinfo, sizeof fileinfo) == false) {
    perror("send");
    return -1;
  }

  //2.等待服务端的确认报文
  std::string buffer;
  if (tcpclient.recv(buffer, 2) == false) {
    perror("recv");
    return -1;
  }
  if (buffer != "OK") {
    std::cout << "the answer is wrong\n";
  }

  //3.发送文件内容
  if (tcpclient.sendFile(fileinfo.filename, fileinfo.filesize) == false) {
    perror("sendFile");
    return -1;
  }

  //4.等待服务端的确认报文
  if (tcpclient.recv(buffer, 2) == false) {
    perror("recv");
    return -1;
  }
  if (buffer != "OK") {
    std::cout << "the answer is wrong\n";
  }
  std::cout << "LOG recv file success\n";
}

