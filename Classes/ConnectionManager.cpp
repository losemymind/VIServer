
#include "ConnectionManager.h"
#include <utility>
#include <iostream>
#include <algorithm>
#include <functional>
#include "FoundationKit/Base/MathEx.h"
#include "FoundationKit/Base/DataStream.h"
#include "FoundationKit/Foundation/unique_id.hpp"
#include "Networking/IProtocol.h"

#define LISTENSERVER_DEFAULT_PORT 4159
#define LISTENSERVER_DEFAULT_ENDPOINT IPv4Endpoint(IPv4Address(127, 0, 0, 1), LISTENSERVER_DEFAULT_PORT)

ConnectionManager::ConnectionManager()
    :_tcpListener(nullptr)
    , _bStartup(false)
{

}

ConnectionManager::~ConnectionManager()
{

}

// 启动控制端连接监听器
bool ConnectionManager::Startup()
{
    // 如果已经启动了，那么什么也不做
    if (_bStartup)return true;
    // 创建一个TCP监听器监听控制端连接
    // LISTENSERVER_DEFAULT_ENDPOINT为监听的IP地址和端口。
    _tcpListener = new TcpListener(LISTENSERVER_DEFAULT_ENDPOINT);
    // 绑定处理客户端连接进来的函数，当有客户端连接进来，TcpListener会调用 
    // HandleListenerConnectionAccepted方法。
    _tcpListener->OnConnectionAccepted() = std::bind(&ConnectionManager::HandleListenerConnectionAccepted, this, std::placeholders::_1, std::placeholders::_2);
    _bStartup = true;
    LOG_INFO(">>Server listen endpoint[%s]", _tcpListener->GetLocalEndpoint().ToString().c_str());
    return true;
}

// 连接管理器关闭函数，在这里出来资源释放。
void ConnectionManager::Shutdown()
{
    _tcpListener->Stop();
    SAFE_DELETE(_tcpListener);
    for (auto iter : _clients)
    {
        SAFE_DELETE(iter.second);
    }
    _bStartup = false;
}

// 接收并处理客户端消息
void ConnectionManager::update(float deltaTime)
{
    std::unique_lock<std::mutex> uniqueLock(_addClientMutex);
    ClientMap  tempClients = _clients;
    uniqueLock.unlock();

    for (auto clientPair : tempClients)
    {
        Socket *client = clientPair.second;
        uint32 dataSize = 0;
        // 客户端是否是连接状态
        if (client->GetConnectionState() == ESocketConnectionState::Connected)
        {
            // 客户端是否有可读取的数据
            if (client->HasPendingData(dataSize))
            {
                std::vector<uint8> datagram(MathEx::min(dataSize, 65507u));
                int32 bytesRead = 0;
                // 接收数据
                if (client->Recv(datagram.data(), datagram.size(), bytesRead))
                {
                    // 分发协议
                    DataStream dataStream;
                    dataStream.reset(datagram.data(), datagram.size());
                    IProtocol::DispathStreamProtocol(clientPair.first, dataStream);
                }
            }
            else
            {

            }
        }
        else
        {
            // 删除连接断开的客户端
            HandleClientDisconnected(clientPair.first);
        }
    }
}

// 处理客户端连接进来
bool ConnectionManager::HandleListenerConnectionAccepted(Socket* ClientSocket, const IPv4Endpoint& ClientEndpoint)
{
    std::lock_guard<std::mutex>   lockClient(_addClientMutex);
    _clients.insert(std::make_pair(unique_id::create(), ClientSocket));
    return true;
}

// 处理客户端断开
void ConnectionManager::HandleClientDisconnected(uint64 clientId)
{
    std::lock_guard<std::mutex>   lockClient(_addClientMutex);
    auto iterFind = _clients.find(clientId);
    if (iterFind != _clients.end())
    {
        _clients.erase(clientId);
    }
}

// 根据客户端ID返回客户端对象
Socket* ConnectionManager::getClientByID(uint64 clientId)
{
    std::lock_guard<std::mutex>   lockClient(_addClientMutex);
    auto iterFind = _clients.find(clientId);
    if (iterFind != _clients.end())
    {
        return iterFind->second;
    }
    return nullptr;
}

