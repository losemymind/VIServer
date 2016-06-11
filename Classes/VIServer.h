
/**
 * �����������߼��������Ĺ��ܣ�
 * 1.�������̨��������
 * 2.���ܺʹ�����ƶ���������
 * 3.���ܺʹ����豸���������ӡ�
 */

#pragma once
// �߳̿�
#include <thread>
// ��������
#include <mutex>
// ����
#include <list>
// �ַ�����
#include <string>
// unordered_map��
#include <unordered_map>
// ������
#include <functional>
// ����ƽ̨���
#include "FoundationKit/GenericPlatformMacros.h"
// ����ģ�����
#include "FoundationKit/Foundation/Singleton.h"
// TCP���Ӽ�����
#include "Networking/TcpListener.h"
// IPv4 ��ַ������
#include "Networking/IPv4Address.h"
// IPv4 ��ַ������
#include "Networking/IPv4Endpoint.h"
// ����socket
#include "Networking/SocketBSD.h"

USING_NS_FK;

// ͨ��ģ����Singleton����VIServerΪ��������
class VIServer : public Singleton<VIServer>
{
    //��VIServer�Ĺ��캯������Ϊprivate��������˴���VIServer
    VIServer();
    // ʹ����Ԫ����Singleton����Ե���VIServer�Ĺ��캯��
    friend Singleton<VIServer>;
public:
    // ��������������������̨�����б������ִ�еĺ�����
    typedef std::unordered_map<std::string, std::function<void()> > CommandMap;

    // VIServer����������������ִ���ڴ��ͷš�
    ~VIServer();

    // ��ʼ��������
    void setup();

    // ���·������߼�
    void update(bool& bExit);

    // ������̨���������浽 _commandList, ���߳�
    // ��ȥ��ȡ��Щ����ȥִ��.
    void pushMessage(std::string& comMsg);

    // ���÷������Ƿ�Ҫ�˳�
    void setExit(bool bExit);
    // ��ȡ�������Ƿ��˳�
    bool getExit();

    //=========�����������=============
    // ����������
    void start();
    // ֹͣ������
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







