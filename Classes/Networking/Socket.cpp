
#include "Socket.h"
#include "FoundationKit/Foundation/Logger.h"

//
// Socket stats implementation
//

bool Socket::SendTo(const uint8* data, int32 count, int32& bytesSent, const InternetAddrBSD& destination)
{
    LOG_INFO("***** Socket '%s' SendTo %i Bytes", _SocketDescription.c_str(), bytesSent);
	return true;
}


bool Socket::Send(const uint8* data, int32 count, int32& bytesSent)
{
    LOG_INFO("***** Socket '%s' Send %i Bytes", _SocketDescription.c_str(), bytesSent);
	return true;
}


bool Socket::RecvFrom(uint8* data, int32 bufferSize, int32& bytesRead, InternetAddrBSD& source, ESocketReceiveFlags flags)
{
    if (bytesRead > 0)
	{
        LOG_INFO("***** Socket '%s' RecvFrom %i Bytes", _SocketDescription.c_str(), bytesRead);
	}
	return true;
}


bool Socket::Recv(uint8* data, int32 bufferSize, int32& bytesRead, ESocketReceiveFlags flags)
{
    if (bytesRead > 0)
	{
        LOG_INFO("***** Socket '%s' Recv %i Bytes", _SocketDescription.c_str(), bytesRead);
	}
	return true;
}







