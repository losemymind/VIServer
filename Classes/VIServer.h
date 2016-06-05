
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

    // ������̨���������浽 _commandList, ���߳�
    // ��ȥ��ȡ��Щ����ȥִ��.
    void pushMessage(std::string& comMsg);
    void setExit(bool bExit);
    bool getExit();

    //����������

    void start();
    void stop();



private:
    // ����ÿ֡���е�ʱ��
    void calculateDeltaTime();

    // �������Ƿ��Ѿ�����
    bool  _blaunched;

    // ����ÿ֡���е�ʱ�䣬��λ���롣
    float _deltaTime;

    // ��¼��ѭ�������µ�ʱ��
    struct timeval *_lastUpdate;

    // ���_exitΪtrue,���˳���������
    bool            _exit;

    // �������̨�������ڿ��Ʒ����������
    std::list<std::string> _commandList;

    // ������󣬱�֤����_commandList���̰߳�ȫ�ġ�
    std::mutex             _commandMutex;

    // ����������ϣ���¼������֧�ֵ������Ѿ�ִ�еķ�����
    CommandMap             _commandMap;

    // ���ڶ�ȡ����̨�����������̡߳�
    std::thread            _readCommandThread;

};







