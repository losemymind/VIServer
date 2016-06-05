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


class ConnectionManager : public Singleton<ConnectionManager>
{
    ConnectionManager();
    friend Singleton<ConnectionManager>;
public:
    typedef std::unordered_map<uint64, Socket*>   ClientMap;

    ~ConnectionManager();

    // 启动
    bool Startup();

    // 关闭
    void Shutdown();

    // 每帧更新
    void update(float deltaTime);


    // 处理客户端连接进来
    bool HandleListenerConnectionAccepted(Socket* ClientSocket, const IPv4Endpoint& ClientEndpoint);

    // 处理客户端断开
    void HandleClientDisconnected(uint64 clientId);

    // 根据客户端ID返回客户端对象
    Socket* getClientByID(uint64 clientId);

protected:

    // TCP 连接监听器，监听客户端连接。
    TcpListener*           _tcpListener;
    ClientMap              _clients;
    std::mutex             _addClientMutex;

    bool                   _bStartup;
};







