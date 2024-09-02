#include <iostream>
#include <sys/socket.h>

int main() {
  int buf_size = 0;
  socklen_t optlen = sizeof buf_size;
  int sockfd = socket(AF_INET, SOCK_STREAM, 0);

  getsockopt(sockfd, SOL_SOCKET, SO_SNDBUF , &buf_size, &optlen);
  std::cout << "send buf_size = " << buf_size  << std::endl;

  getsockopt(sockfd, SOL_SOCKET, SO_RCVBUF, &buf_size, &optlen);
  std::cout << "recv buf_size = " << buf_size << std::endl;
}