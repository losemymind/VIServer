#ifndef LOSEMYMIND_IPROTOCOL_H
#define LOSEMYMIND_IPROTOCOL_H

#include <unordered_map>
#include "FoundationKit/Base/Types.h"
#include "FoundationKit/Base/DataStream.h"

USING_NS_FK;

/**
@brief 实现接收客户端协议的接口 
*/
class IProtocol
{
    typedef std::unordered_map< int32, IProtocol * > PROTOCOLS;
private:
    /**
     * @brief		禁止默认建构 
     */
    IProtocol() {}
        
    /**
     * @brief		管理各个协议的MAP 
     */
    static PROTOCOLS & GetProtoclsPool ();
        
    /**
     * @brief		根据协议ID获得协议的指针 
     */
    static IProtocol * GetMatchedProtocol(int32 idx);
public:
    IProtocol(int32 idx);
    virtual ~IProtocol() {}

    /**
     * @brief		分发原始流协议 
     */
    static void DispathStreamProtocol(uint64 clientID, DataStream & stream);
        
 
    /**
     * @brief		处理原始流协议 
     */
    virtual void ProcessStreamProtocol(uint64 clientID, DataStream & stream) {}
};

#define IMPLEMENT_PROTOCOL(CLS,IDX) CLS G_PROTOCOL_##CLS(IDX)

#endif // LOSEMYMIND_IPROTOCOL_H
