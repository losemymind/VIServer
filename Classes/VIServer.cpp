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

// 一个帮助宏，用来绑定服务器命令执行函数
#define BIND_COMMAND(FUN) std::bind(&VIServer::FUN,this)


// 帮助函数，用来获取系统时间。
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

// VIServer构造函数
VIServer::VIServer()
    :_blaunched(false)
    , _deltaTime(0.0f)
    , _exit(false)
{
    _lastUpdate = new struct timeval;
    memset(_lastUpdate, 0, sizeof(timeval));
}

// VIServer析构函数
VIServer::~VIServer()
{
    _readCommandThread.detach();
}

// 启动服务器
void VIServer::setup()
{
    // 输出信息
    LOG_INFO(">>服务器正在启动......");

    // 服务器退出命令
    _commandMap["exit"] = [this]()
    {
        this->setExit(true);
    };

    // 服务器启动命令
    _commandMap["start"] = BIND_COMMAND(start);
    // 服务器停止命令
    _commandMap["stop"] = BIND_COMMAND(stop);

    // 启动一个线程，用来监听控制台输入的命令
    _readCommandThread = std::thread([this]
    {
        // 保存命令
        std::string cmd;
        // 循环等待命令输入
        while (true)
        {
            // 等待并获取输入命令
            std::cin >> cmd;
            // 保存输入的命令，主线程会去读取命令并执行
            pushMessage(cmd);

            // 如果控制台输入了退出命令（exit）
            // 那么就退出线程
            if (cmd == "exit")
            {
                break;
            }
        }
    });

    // 启动服务器
    start();
    LOG_INFO(">>服务器启动完成");
}

// 服务器更新函数
void VIServer::update(bool& bExit)
{
    // 计算上一帧到这一帧总共用了多少时间
    calculateDeltaTime();

    // 锁住_commandList，避免多线程访问造成资源抢占。
    std::unique_lock<std::mutex> lock(_commandMutex);
    // 拷贝命令列表到一个临时表，好让_readCommandThread线程可以继续保存命令
    std::list<std::string> copyCommand = _commandList;
    // 由于已经保存了临时命令，所以命令列表可以清空了。
    _commandList.clear();
    // 解锁_commandList，让_readCommandThread线程可以访问
    lock.unlock();

    // 遍历整个命令列表，并执行命令对应的函数。
    for (auto cmd : copyCommand)
    {
        if (_commandMap.find(cmd) != _commandMap.end())
        {
            _commandMap[cmd]();
        }
    }

    // 检测服务器是否退出
    bExit = getExit();

    // 调用连接管理器的更新函数。
    ConnectionManager::getInstance()->update(_deltaTime);

}

// 帮助函数，计算上一帧到这一帧所执行的时间。
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

// 保存控制台输入的命令
void VIServer::pushMessage(std::string& comMsg)
{
    std::lock_guard<std::mutex>  lock(_commandMutex);
    _commandList.push_back(comMsg);
}

// 设置服务器是否退出
void VIServer::setExit(bool bExit)
{
    _exit = bExit;
}

// 获取服务器是否退出
bool VIServer::getExit()
{
    return _exit;
}

//启动服务器
void VIServer::start()
{
    //如果服务器没有启动，则创建一个连接管理器来接受控制端连接
    if (!_blaunched)
    {
        ConnectionManager::getInstance()->Startup();
    }

    _blaunched = true;
}

// 停止服务器
void VIServer::stop()
{
    LOG_INFO(">>正在停止服务器......");
    _blaunched = false;
    ConnectionManager::getInstance()->Shutdown();

}


