#include "VIServer.h"
#include <WinSock2.h>
#include <utility>
#include <iostream>
#include <algorithm>
#include <functional>
#include "FoundationKit/Base/MathEx.h"
#include "FoundationKit/Base/DataStream.h"
#include "FoundationKit/Foundation/unique_id.hpp"
#include "Networking/IProtocol.h"
#include "ConnectionManager.h"

USING_NS_FK;

// һ�������꣬�����󶨷���������ִ�к���
#define BIND_COMMAND(FUN) std::bind(&VIServer::FUN,this)


// ����������������ȡϵͳʱ�䡣
int gettimeofday(struct timeval * val, struct timezone *)
{
    if (val)
    {
        LARGE_INTEGER liTime, liFreq;
        QueryPerformanceFrequency(&liFreq);
        QueryPerformanceCounter(&liTime);
        val->tv_sec = (long)(liTime.QuadPart / liFreq.QuadPart);
        val->tv_usec = (long)(liTime.QuadPart * 1000000.0 / liFreq.QuadPart - val->tv_sec * 1000000.0);
    }
    return 0;
}

// VIServer���캯��
VIServer::VIServer()
    :_blaunched(false)
    , _deltaTime(0.0f)
    , _exit(false)
{
    _lastUpdate = new struct timeval;
    memset(_lastUpdate, 0, sizeof(timeval));
}

// VIServer��������
VIServer::~VIServer()
{
    _readCommandThread.detach();
}

// ����������
void VIServer::setup()
{
    // �����Ϣ
    LOG_INFO(">>��������������......");

    // �������˳�����
    _commandMap["exit"] = [this]()
    {
        this->setExit(true);
    };

    // ��������������
    _commandMap["start"] = BIND_COMMAND(start);
    // ������ֹͣ����
    _commandMap["stop"] = BIND_COMMAND(stop);

    // ����һ���̣߳�������������̨���������
    _readCommandThread = std::thread([this]
    {
        // ��������
        std::string cmd;
        // ѭ���ȴ���������
        while (true)
        {
            // �ȴ�����ȡ��������
            std::cin >> cmd;
            // ���������������̻߳�ȥ��ȡ���ִ��
            pushMessage(cmd);

            // �������̨�������˳����exit��
            // ��ô���˳��߳�
            if (cmd == "exit")
            {
                break;
            }
        }
    });

    // ����������
    start();
    LOG_INFO(">>�������������");
}

// ���������º���
void VIServer::update(bool& bExit)
{
    // ������һ֡����һ֡�ܹ����˶���ʱ��
    calculateDeltaTime();

    // ��ס_commandList��������̷߳��������Դ��ռ��
    std::unique_lock<std::mutex> lock(_commandMutex);
    // ���������б�һ����ʱ������_readCommandThread�߳̿��Լ�����������
    std::list<std::string> copyCommand = _commandList;
    // �����Ѿ���������ʱ������������б��������ˡ�
    _commandList.clear();
    // ����_commandList����_readCommandThread�߳̿��Է���
    lock.unlock();

    // �������������б���ִ�������Ӧ�ĺ�����
    for (auto cmd : copyCommand)
    {
        if (_commandMap.find(cmd) != _commandMap.end())
        {
            _commandMap[cmd]();
        }
    }

    // ���������Ƿ��˳�
    bExit = getExit();

    // �������ӹ������ĸ��º�����
    ConnectionManager::getInstance()->update(_deltaTime);

}

// ����������������һ֡����һ֡��ִ�е�ʱ�䡣
void VIServer::calculateDeltaTime()
{
    struct timeval timeNow;

    if (gettimeofday(&timeNow, nullptr) != 0)
    {
        _deltaTime = 0;
        return;
    }
    _deltaTime = (timeNow.tv_sec - _lastUpdate->tv_sec) + (timeNow.tv_usec - _lastUpdate->tv_usec) / 1000000.0f;
    _deltaTime = MathEx::max(0.f, _deltaTime);

    *_lastUpdate = timeNow;
}

// �������̨���������
void VIServer::pushMessage(std::string& comMsg)
{
    std::lock_guard<std::mutex>  lock(_commandMutex);
    _commandList.push_back(comMsg);
}

// ���÷������Ƿ��˳�
void VIServer::setExit(bool bExit)
{
    _exit = bExit;
}

// ��ȡ�������Ƿ��˳�
bool VIServer::getExit()
{
    return _exit;
}

//����������
void VIServer::start()
{
    //���������û���������򴴽�һ�����ӹ����������ܿ��ƶ�����
    if (!_blaunched)
    {
        ConnectionManager::getInstance()->Startup();
    }

    _blaunched = true;
}

// ֹͣ������
void VIServer::stop()
{
    LOG_INFO(">>����ֹͣ������......");
    _blaunched = false;
    ConnectionManager::getInstance()->Shutdown();

}


