# time_t  别名
time_t用于表示时间类型，它是一个long类型，在头文件<time.h>中定义，表示从1970年1月1日0时0分0秒到现在的秒数


# time()库函数，用于获取操作系统的当前时间
## 声明
time_t time(time_t* tloc);
## 调用方法
第一种
```cpp
time_t now = time(0);
```
第二种
```cpp
time_t now;
time(&now);
```

# tm结构体
定义
```cpp
struct tm
{
  int tm_sec;			/* Seconds.	[0-60] (1 leap second) */
  int tm_min;			/* Minutes.	[0-59] */
  int tm_hour;			/* Hours.	[0-23] */
  int tm_mday;			/* Day.		[1-31] */
  int tm_mon;			/* Month.	[0-11] */
  int tm_year;			/* Year	- 1900.  */
  int tm_wday;			/* Day of week.	[0-6] */
  int tm_yday;			/* Days in year.[0-365]	*/
  int tm_isdst;			/* DST.		[-1/0/1]*/

# ifdef	__USE_MISC
  long int tm_gmtoff;		/* Seconds east of UTC.  */
  const char *tm_zone;		/* Timezone abbreviation.  */
# else
  long int __tm_gmtoff;		/* Seconds east of UTC.  */
  const char *__tm_zone;	/* Timezone abbreviation.  */
# endif
};
```

# localtime()
localtime()将time_t表示的时间转换为tm表示的时间

localtime()不是线程安全的，localtime_r()是线程安全的

## 声明
```cpp
struct tm *localtime (const time_t *__timer)
```

```cpp
#include <time.h>
#include <iostream>

int main() {
    time_t now;
    time(&now);
    std::cout << now << "\n";
    tm* tmm = localtime(&now);
    std::string stime = std::to_string(tmm->tm_year + 1900) + "-" + std::to_string(tmm->tm_mon + 1) + "-"
                        + std::to_string(tmm->tm_mday) + "-" + std::to_string(tmm->tm_hour) + "-" +
                        std::to_string(tmm->tm_min) + "-" + std::to_string(tmm->tm_sec);
    std::cout << "stime = " << stime << "\n";
}
```

# mktime()
mktime()作用和localtime()作用相反,该函数主要用于时间的运算
## 声明
```cpp
time_t mktime (struct tm *__tp);
```

# gettimeofday()
用于获取1970年1月1日到现在的秒数和当前秒中逝去的微秒数，头文件<sys/time.h>
```cpp
gettimeofday (struct timeval *__restrict __tv,
			 void *__restrict __tz)
```
```cpp
struct timeval
{
#ifdef __USE_TIME_BITS64
  __time64_t tv_sec;		/* Seconds.  */
  __suseconds64_t tv_usec;	/* Microseconds.  */
#else
  __time_t tv_sec;		/* Seconds.  */
  __suseconds_t tv_usec;	/* Microseconds.  */
#endif
};
```

# 程序睡眠sleep() usleep()
两个函数的参数分别是秒和微秒