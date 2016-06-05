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


#define BIND_COMMAND(FUN) std::bind(&VIServer::FUN,this)



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


VIServer::VIServer()
    :_blaunched(false)
    , _deltaTime(0.0f)
    , _exit(false)
{
    _lastUpdate = new struct timeval;
    memset(_lastUpdate, 0, sizeof(timeval));
}

VIServer::~VIServer()
{
    _readCommandThread.detach();
}


void VIServer::setup()
{
    LOG_INFO(">>服务器正在启动......");

    _commandMap["exit"] = [this]()
    {
        this->setExit(true);
    };

    _commandMap["start"] = BIND_COMMAND(start);
    _commandMap["stop"] = BIND_COMMAND(stop);


    _readCommandThread = std::thread([this]
    {
        std::string cmd;
        while (true)
        {
            std::cin >> cmd;
            pushMessage(cmd);
            if (cmd == "exit")
            {
                break;
            }
        }
    });

    start();
    LOG_INFO(">>服务器启动完成");
}

void VIServer::update(bool& bExit)
{
    calculateDeltaTime();

    std::unique_lock<std::mutex> lock(_commandMutex);
    std::list<std::string> copyCommand = _commandList;
    _commandList.clear();
    lock.unlock();

    for (auto cmd : copyCommand)
    {
        if (_commandMap.find(cmd) != _commandMap.end())
        {
            _commandMap[cmd]();
        }
    }

    bExit = getExit();

    ConnectionManager::getInstance()->update(_deltaTime);

}

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

void VIServer::pushMessage(std::string& comMsg)
{
    std::lock_guard<std::mutex>  lock(_commandMutex);
    _commandList.push_back(comMsg);
}

void VIServer::setExit(bool bExit)
{
    _exit = bExit;
}

bool VIServer::getExit()
{
    return _exit;
}


void VIServer::start()
{
    if (!_blaunched)
    {
        ConnectionManager::getInstance()->Startup();
    }

    _blaunched = true;
}

void VIServer::stop()
{
    LOG_INFO(">>正在停止服务器......");
    _blaunched = false;
    ConnectionManager::getInstance()->Shutdown();

}


