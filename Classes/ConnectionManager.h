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

    // ����
    bool Startup();

    // �ر�
    void Shutdown();

    // ÿ֡����
    void update(float deltaTime);


    // ����ͻ������ӽ���
    bool HandleListenerConnectionAccepted(Socket* ClientSocket, const IPv4Endpoint& ClientEndpoint);

    // ����ͻ��˶Ͽ�
    void HandleClientDisconnected(uint64 clientId);

    // ���ݿͻ���ID���ؿͻ��˶���
    Socket* getClientByID(uint64 clientId);

protected:

    // TCP ���Ӽ������������ͻ������ӡ�
    TcpListener*           _tcpListener;
    ClientMap              _clients;
    std::mutex             _addClientMutex;

    bool                   _bStartup;
};







