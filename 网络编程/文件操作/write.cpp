#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>

int main() {
  close(0);
  close(1);
  close(2);
  int fd;    // 定义一个文件描述符/文件句柄。

  // 打开文件，注意，如果创建后的文件没有权限，可以手工授权chmod 777 data.txt。
  fd = open("data.txt", O_CREAT | O_RDWR | O_TRUNC);
  if (fd == -1)
  {
    perror("open(data.txt)"); return -1;
  }

  printf("文件描述符fd = %d\n",fd);

  char buffer[1024];
  strcpy(buffer, "hello world。\n");

  if (write(fd, buffer, strlen(buffer)) == -1)    // 把数据写入文件。
  {
    perror("write()");
    return -1;
  }

  sleep(100);
  close(fd);  // 关闭文件。
}