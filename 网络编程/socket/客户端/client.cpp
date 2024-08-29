#include <iostream>
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

int main(int argc, char* argv[]) {
  ctcpclient tcpclient;
  if (!tcpclient.connect(argv[1], atoi(argv[2]))) {
    perror("connect");
    return -1;
  }

  std::string buffer;
  for (int i = 0; i < 10; ++i) {
    int iret;
    buffer = "this is " + std::to_string(i +1) + "th\n";
    if (!tcpclient.send(buffer)) {
      perror("sned");
      return false;
    }
    std::cout << "send " << buffer << std::endl;

    if (tcpclient.recv(buffer, 1024) <= 0) {
      std::cout << "iret = " << iret << std::endl;
      break;
    }
    std::cout<< "received " << buffer << std::endl;

    sleep(1);
  }
}

bool ctcpclient::close() {
  if (m_clientfd == -1) {
    return false;
  }
  ::close(m_clientfd);
  m_clientfd = -1;
  return true;
}