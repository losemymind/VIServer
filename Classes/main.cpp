

#include <windows.h>
#include "VIServer.h"


DWORD g_dwMainThreadId;
bool  g_appExit = false;
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




static LARGE_INTEGER G_FrameInterval;
static void setFrameInterval(float interval)
{
    LARGE_INTEGER nFreq;
    QueryPerformanceFrequency(&nFreq);
    G_FrameInterval.QuadPart = (LONGLONG)(interval * nFreq.QuadPart);
}


int main(int argv, char** argc)
{

    g_dwMainThreadId = GetCurrentThreadId(); 
    SetConsoleCtrlHandler(ConsoleEventCallback, TRUE);


    //MSG msg;
    //while (GetMessage(&msg, NULL, 0, 0))
    //{
    //    TranslateMessage(&msg);
    //    DispatchMessage(&msg);
    //}
    
    LARGE_INTEGER nLast;
    LARGE_INTEGER nNow;
    QueryPerformanceCounter(&nLast);
    setFrameInterval(0.0016f);
    VIServer::getInstance()->setup();
    bool bExit = false;

    while (!bExit && !g_appExit)
    {
        QueryPerformanceCounter(&nNow);
        if (nNow.QuadPart - nLast.QuadPart > G_FrameInterval.QuadPart)
        {
            nLast.QuadPart = nNow.QuadPart - (nNow.QuadPart % G_FrameInterval.QuadPart);
            VIServer::getInstance()->update(bExit);
        }
        else
        {
            Sleep(1);
        }
    }

    int exitCode = 0;
    SetConsoleCtrlHandler(ConsoleEventCallback, FALSE);
    system("pause");
    return exitCode;

}