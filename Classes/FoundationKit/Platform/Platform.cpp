/****************************************************************************
Copyright (c) 2015 libo.

losemymind.libo@gmail.com

****************************************************************************/
#include "Platform.h"
NS_FK_BEGIN

Platform::Platform()
{

}

Platform::~Platform()
{

}

// Default getTickCount impl
//int64 Platform::getTickCount()
//{
//    struct timeval tv;
//    struct timezone tz;
//    gettimeofday(&tv, &tz);
//    return (int64)tv.tv_sec * 1000000 + tv.tv_usec;
//}

int64 Platform::getCPUTickCount()
{
#if defined(__i386__)
    int64 x;
    __asm__ volatile (".byte 0x0f, 0x31" : "=A" (x));
    return x;
#elif defined(__x86_64__)
    unsigned hi, lo;
    __asm__ __volatile__("rdtsc" : "=a"(lo), "=d"(hi));
    return (int64)lo | ((int64)hi << 32);
#elif defined(__ppc__)
    int64 result = 0;
    unsigned upper, lower, tmp;
    __asm__ volatile(
        "0:                   \n"
        "\tmftbu   %0         \n"
        "\tmftb    %1         \n"
        "\tmftbu   %2         \n"
        "\tcmpw    %2,%0      \n"
        "\tbne     0b         \n"
        : "=r"(upper), "=r"(lower), "=r"(tmp)
        );
    return lower | ((int64)upper << 32);

#elif defined _MSC_VER && defined WIN32 && defined _M_IX86
    __asm _emit 0x0f;
    __asm _emit 0x31;
#else
    return getTickCount();
#endif
}

NS_FK_END
