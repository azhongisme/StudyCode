#include <stdio.h>
#include <sys/time.h>

int main() {
    struct timeval tv;
    struct timezone tz;

    // 获取当前时间
    int result = gettimeofday(&tv, &tz);

    if (result != 0) {
        perror("gettimeofday");
        return 1;
    }

    // 打印时间
    printf("Seconds since epoch: %ld\n", tv.tv_sec);
    printf("Microseconds: %ld\n", tv.tv_usec);

    return 0;
}
