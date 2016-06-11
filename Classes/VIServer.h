
/**
 * 服务器处理逻辑，保护的功能：
 * 1.处理控制台输入的命令。
 * 2.接受和处理控制端网络连接
 * 3.接受和处理设备端网络连接。
 */

#pragma once
// 线程库
#include <thread>
// 互斥对象库
#include <mutex>
// 链表
#include <list>
// 字符串库
#include <string>
// unordered_map库
#include <unordered_map>
// 函数库
#include <functional>
// 处理平台相关
#include "FoundationKit/GenericPlatformMacros.h"
// 单例模板对象
#include "FoundationKit/Foundation/Singleton.h"
// TCP连接监听器
#include "Networking/TcpListener.h"
// IPv4 地址帮助类
#include "Networking/IPv4Address.h"
// IPv4 地址帮助类
#include "Networking/IPv4Endpoint.h"
// 基础socket
#include "Networking/SocketBSD.h"

USING_NS_FK;

// 通过模板类Singleton定义VIServer为单例对象
class VIServer : public Singleton<VIServer>
{
    //将VIServer的构造函数设置为private，避免别人创建VIServer
    VIServer();
    // 使用友元，让Singleton类可以调用VIServer的构造函数
    friend Singleton<VIServer>;
public:
    // 定义别名，用来保存控制台命令列表和命令执行的函数。
    typedef std::unordered_map<std::string, std::function<void()> > CommandMap;

    // VIServer析构函数，在这里执行内存释放。
    ~VIServer();

    // 初始化服务器
    void setup();

    // 更新服务器逻辑
    void update(bool& bExit);

    // 将控制台输入的命令保存到 _commandList, 主线程
    // 会去读取这些命令去执行.
    void pushMessage(std::string& comMsg);

    // 设置服务器是否要退出
    void setExit(bool bExit);
    // 获取服务器是否退出
    bool getExit();

    //=========服务器命令函数=============
    // 启动服务器
    void start();
    // 停止服务器
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







