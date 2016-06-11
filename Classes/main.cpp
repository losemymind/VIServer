

#include <windows.h>
#include "VIServer.h"


// 保存主线程ID
DWORD g_dwMainThreadId;

// 是否退出程序？ true退出，false不退出。
bool  g_appExit = false;

// 控制台程序事件回调函数
BOOL WINAPI ConsoleEventCallback(DWORD dwCtrlType)
{
    if (dwCtrlType == CTRL_CLOSE_EVENT)  
    {       
        PostThreadMessage(g_dwMainThreadId, WM_QUIT, 0, 0);  
        g_appExit = true;
        return FALSE;
    }  
    return FALSE;
}

// 保存程序的帧回调时间。
static LARGE_INTEGER G_FrameInterval;

// 设置程序每帧调用的时间
// setFrameInterval(1/60.f) // 60帧
// setFrameInterval(1/30.f) // 30帧
static void setFrameInterval(float interval)
{
    LARGE_INTEGER nFreq;
    QueryPerformanceFrequency(&nFreq);
    G_FrameInterval.QuadPart = (LONGLONG)(interval * nFreq.QuadPart);
}

// 程序主入口
int main(int argv, char** argc)
{

    // 获取主线程ID保存起来
    g_dwMainThreadId = GetCurrentThreadId(); 

    // 调用系统函数SetConsoleCtrlHandler 设置控制台事件回调函数。
    SetConsoleCtrlHandler(ConsoleEventCallback, TRUE);

    // 保存当前时间和帧调用最后的时间
    LARGE_INTEGER nNow;
    LARGE_INTEGER nLast;
    
    //获取时间
    QueryPerformanceCounter(&nLast);

    // 设置程序为60帧，即1秒钟调用60次。
    setFrameInterval(1/60.f);

    // 初始化服务器
    VIServer::getInstance()->setup();

    // 标识程序为运行状态（不退出）
    // 这个变量的值会在命令行输入exit命令的时候赋值
    bool bExit = false;

    // 主程序循环，条件为没有输入exit命令和没有点击关闭按钮
    while (!bExit && !g_appExit)
    {
        //获取时间赋予nNow
        QueryPerformanceCounter(&nNow);

        // 如果当前时间减去帧最后更新的时间大于 1/60，那么就调用服务器更新函数。
        if (nNow.QuadPart - nLast.QuadPart > G_FrameInterval.QuadPart)
        {
            // 更新帧调用的最后时间
            nLast.QuadPart = nNow.QuadPart - (nNow.QuadPart % G_FrameInterval.QuadPart);
            // 调用服务器的更新函数更新服务器状态。
            VIServer::getInstance()->update(bExit);
        }
        else
        {
            Sleep(1);
        }
    }

    // 处理程序退出。
    int exitCode = 0;
    SetConsoleCtrlHandler(ConsoleEventCallback, FALSE);
    system("pause");
    return exitCode;

}