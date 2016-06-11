#include "NetworkProtocols.h"
#include <string>
#include "Networking/IProtocol.h"
#include "Networking/SocketBSD.h"
#include "VIServer.h"
#include "ClientProtocolDefines.h"
#include "ConnectionManager.h"


// ÁÄÌìÐ­Òé
class ClientChat : IProtocol
{
public:
    ClientChat(int32 idx) :IProtocol(idx){}

    virtual void ProcessStreamProtocol(uint64 clientID, DataStream & stream)
    {
        std::string msg;
        stream >> msg;
        LOG_INFO(">>RECV:%s", msg.c_str());

        auto client = ConnectionManager::getInstance()->getClientByID(clientID);

        int bytesSend;
        char* sendMsg = "I recv you send msg.";
        client->Send((unsigned char*)sendMsg, strlen(sendMsg), bytesSend);
    }
};


IMPLEMENT_PROTOCOL(ClientChat, CLIENT_CHAT);







