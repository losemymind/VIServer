
#pragma once
#include <thread>
#include <mutex>
#include <list>
#include <string>
#include <unordered_map>
#include <functional>
#include "FoundationKit/GenericPlatformMacros.h"
#include "FoundationKit/Foundation/Singleton.h"
#include "Networking/TcpListener.h"
#include "Networking/IPv4Address.h"
#include "Networking/IPv4Endpoint.h"
#include "Networking/SocketBSD.h"

USING_NS_FK;

class VIServer : public Singleton<VIServer>
{
    VIServer();
    friend Singleton<VIServer>;
public:
    typedef std::unordered_map<std::string, std::function<void()> > CommandMap;


    ~VIServer();
    void setup();
    void update(bool& bExit);

    // 将控制台输入的命令保存到 _commandList, 主线程
    // 回去读取这些命令去执行.
    void pushMessage(std::string& comMsg);
    void setExit(bool bExit);
    bool getExit();

    //服务器命令

    void start();
    void stop();



private:
    // 计算每帧运行的时间
    void calculateDeltaTime();

    // 服务器是否已经启动
    bool  _blaunched;

    // 保存每帧运行的时间，单位毫秒。
    float _deltaTime;

    // 记录主循环最后更新的时间
    struct timeval *_lastUpdate;

    // 如果_exit为true,则退出服务器。
    bool            _exit;

    // 保存控制台输入用于控制服务器的命令。
    std::list<std::string> _commandList;

    // 互斥对象，保证操作_commandList是线程安全的。
    std::mutex             _commandMutex;

    // 服务器命令集合，记录服务器支持的命令已经执行的方法。
    CommandMap             _commandMap;

    // 用于读取控制台输入的命令的线程。
    std::thread            _readCommandThread;

};







