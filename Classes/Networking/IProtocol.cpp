
#include "IProtocol.h"
#include "FoundationKit/Foundation/Logger.h"

// 协议池实例
IProtocol::PROTOCOLS & IProtocol::GetProtoclsPool ()
{
    static IProtocol::PROTOCOLS protocols;
    return protocols;
}

// 每个协议一个协议ID
IProtocol::IProtocol(int32 idx)
{
    //LOG_ASSERT(GetMatchedProtocol(idx) == nullptr, "***** 协议id 已经存在，请重新指定。");
    PROTOCOLS & pool = GetProtoclsPool();
    pool.insert(std::pair< int32, IProtocol* >(idx, this));
}

// 根据协议ID获得协议的实例
IProtocol * IProtocol::GetMatchedProtocol(int32 idx)
{
    PROTOCOLS & pool = GetProtoclsPool();
    PROTOCOLS::iterator it = pool.find( idx );
    if ( it != pool.end() )
        return it->second;
    return nullptr;
}

void IProtocol::DispathStreamProtocol(uint64 clientID, DataStream & stream)
{
    int32 idx = stream.read<int32>();
    IProtocol * pProtocol = GetMatchedProtocol( idx );
    if ( pProtocol )
    {
        pProtocol->ProcessStreamProtocol(clientID, stream);
    }
    else
    {
        LOG_ERROR("***** Cannot found procotol by id[%d]", idx);
    }
}
