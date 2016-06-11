

#include <windows.h>
#include "VIServer.h"


// �������߳�ID
DWORD g_dwMainThreadId;

// �Ƿ��˳����� true�˳���false���˳���
bool  g_appExit = false;

// ����̨�����¼��ص�����
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

// ��������֡�ص�ʱ�䡣
static LARGE_INTEGER G_FrameInterval;

// ���ó���ÿ֡���õ�ʱ��
// setFrameInterval(1/60.f) // 60֡
// setFrameInterval(1/30.f) // 30֡
static void setFrameInterval(float interval)
{
    LARGE_INTEGER nFreq;
    QueryPerformanceFrequency(&nFreq);
    G_FrameInterval.QuadPart = (LONGLONG)(interval * nFreq.QuadPart);
}

// ���������
int main(int argv, char** argc)
{

    // ��ȡ���߳�ID��������
    g_dwMainThreadId = GetCurrentThreadId(); 

    // ����ϵͳ����SetConsoleCtrlHandler ���ÿ���̨�¼��ص�������
    SetConsoleCtrlHandler(ConsoleEventCallback, TRUE);

    // ���浱ǰʱ���֡��������ʱ��
    LARGE_INTEGER nNow;
    LARGE_INTEGER nLast;
    
    //��ȡʱ��
    QueryPerformanceCounter(&nLast);

    // ���ó���Ϊ60֡����1���ӵ���60�Ρ�
    setFrameInterval(1/60.f);

    // ��ʼ��������
    VIServer::getInstance()->setup();

    // ��ʶ����Ϊ����״̬�����˳���
    // ���������ֵ��������������exit�����ʱ��ֵ
    bool bExit = false;

    // ������ѭ��������Ϊû������exit�����û�е���رհ�ť
    while (!bExit && !g_appExit)
    {
        //��ȡʱ�丳��nNow
        QueryPerformanceCounter(&nNow);

        // �����ǰʱ���ȥ֡�����µ�ʱ����� 1/60����ô�͵��÷��������º�����
        if (nNow.QuadPart - nLast.QuadPart > G_FrameInterval.QuadPart)
        {
            // ����֡���õ����ʱ��
            nLast.QuadPart = nNow.QuadPart - (nNow.QuadPart % G_FrameInterval.QuadPart);
            // ���÷������ĸ��º������·�����״̬��
            VIServer::getInstance()->update(bExit);
        }
        else
        {
            Sleep(1);
        }
    }

    // ��������˳���
    int exitCode = 0;
    SetConsoleCtrlHandler(ConsoleEventCallback, FALSE);
    system("pause");
    return exitCode;

}