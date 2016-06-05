
#include "SocketBSD.h"
#include "FoundationKit/Foundation/Logger.h"

USING_NS_FK;

#if ((TARGET_PLATFORM == PLATFORM_ANDROID) ||(TARGET_PLATFORM == PLATFORM_LINUX))

#define PLATFORM_HAS_BSD_SOCKET_FEATURE_IOCTL 1
# include <sys/ioctl.h>

#endif

#if TARGET_PLATFORM == PLATFORM_WIN32
#define PLATFORM_HAS_BSD_SOCKET_FEATURE_WINSOCKETS 1
#endif


/* FSocket overrides
 *****************************************************************************/

bool SocketBSD::Close(void)
{
	if (_Socket != INVALID_SOCKET)
	{
		int32 error = closesocket(_Socket);
		_Socket = INVALID_SOCKET;
		return error == 0;
	}
	return false;
}


bool SocketBSD::Bind(const InternetAddrBSD& addr)
{
	return bind(_Socket, (sockaddr*)(InternetAddrBSD&)addr, sizeof(sockaddr_in)) == 0;
}


bool SocketBSD::Connect(const InternetAddrBSD& addr)
{
	int32 Return = connect(_Socket, (sockaddr*)(InternetAddrBSD&)addr, sizeof(sockaddr_in));
	
	// "would block" is not an error
	return ((Return == 0) || (Return == EWOULDBLOCK));
}


bool SocketBSD::Listen(int32 maxBacklog)
{
	return listen(_Socket, maxBacklog) == 0;
}


bool SocketBSD::HasPendingConnection(bool& bHasPendingConnection)
{
	bool bHasSucceeded = false;
	bHasPendingConnection = false;

	// make sure socket has no error state
	if (HasState(ESocketBSDParam::HasError) == ESocketBSDReturn::No)
	{
		// get the read state
		ESocketBSDReturn State = HasState(ESocketBSDParam::CanRead);
		
		// turn the result into the outputs
		bHasSucceeded = State != ESocketBSDReturn::EncounteredError;
		bHasPendingConnection = State == ESocketBSDReturn::Yes;
	}

	return bHasSucceeded;
}


bool SocketBSD::HasPendingData(uint32& pendingDataSize)
{
	pendingDataSize = 0;

	// make sure socket has no error state
	if (HasState(ESocketBSDParam::CanRead) == ESocketBSDReturn::Yes)
	{
#if TARGET_PLATFORM == PLATFORM_WIN32
		// See if there is any pending data on the read socket
		if (ioctlsocket(_Socket, FIONREAD, (u_long*)(&pendingDataSize)) == 0)
#else
		if (ioctl(_Socket, FIONREAD, (u_long*)(&pendingDataSize)) == 0)
#endif
		{
			return (pendingDataSize > 0);
		}
	}

	return false;
}


Socket* SocketBSD::Accept(const std::string& socketDescription)
{
	SOCKET newSocket = accept(_Socket, NULL, NULL);

	if (newSocket != INVALID_SOCKET)
	{
		return new SocketBSD(newSocket, _SocketType, socketDescription);
	}

	return NULL;
}


Socket* SocketBSD::Accept(InternetAddrBSD& outAddr, const std::string& socketDescription)
{
	int32 aockaddrLen = sizeof(sockaddr_in);
	SOCKET newSocket = accept(_Socket, *(InternetAddrBSD*)(&outAddr), &aockaddrLen);

	if (newSocket != INVALID_SOCKET)
	{
		return new SocketBSD(newSocket, _SocketType, socketDescription);
	}

	return NULL;
}


bool SocketBSD::SendTo(const uint8* data, int32 count, int32& bytesSent, const InternetAddrBSD& destination)
{
	// Write the data and see how much was written
	bytesSent = sendto(_Socket, (const char*)data, count, 0, (InternetAddrBSD&)destination, sizeof(sockaddr_in));

	bool Result = bytesSent >= 0;
	if (Result)
	{
		_LastActivityTime = DateTime::utcNow();
	}
	return Result;
}


bool SocketBSD::Send(const uint8* data, int32 count, int32& bytesSent)
{
	bytesSent = send(_Socket, (const char*)data, count, 0);

	bool Result = bytesSent >= 0;
	if (Result)
	{
		_LastActivityTime = DateTime::utcNow();
	}
	return Result;
}


bool SocketBSD::RecvFrom(uint8* data, int32 bufferSize, int32& bytesRead, InternetAddrBSD& source, ESocketReceiveFlags flags)
{
	int32 aockaddrLen = sizeof(sockaddr_in);
	sockaddr& Addr = *(InternetAddrBSD&)source;

	const int TranslatedFlags = TranslateFlags(flags);

	// Read into the buffer and set the source address
	bytesRead = recvfrom(_Socket, (char*)data, bufferSize, TranslatedFlags, &Addr, &aockaddrLen);

	if (bytesRead < 0)
	{
		bytesRead = 0;
		return false;
	}
	_LastActivityTime = DateTime::utcNow();
	return true;
}


bool SocketBSD::Recv(uint8* data, int32 bufferSize, int32& bytesRead, ESocketReceiveFlags flags)
{
	const int TranslatedFlags = TranslateFlags(flags);
	bytesRead = recv(_Socket, (char*)data, bufferSize, TranslatedFlags);

	bool Result = bytesRead >= 0;
	if (Result)
	{
		_LastActivityTime = DateTime::utcNow();
	}
	return Result;
}


bool SocketBSD::Wait(ESocketWaitConditions condition, Timespan waitTime)
{
	if ((condition == ESocketWaitConditions::WaitForRead) || (condition == ESocketWaitConditions::WaitForReadOrWrite))
	{
		if (HasState(ESocketBSDParam::CanRead, waitTime) == ESocketBSDReturn::Yes)
		{
			return true;
		}
	}

	if ((condition == ESocketWaitConditions::WaitForWrite) || (condition == ESocketWaitConditions::WaitForReadOrWrite))
	{
		if (HasState(ESocketBSDParam::CanWrite, waitTime) == ESocketBSDReturn::Yes)
		{
			return true;
		}
	}

	return false;
}


ESocketConnectionState SocketBSD::GetConnectionState(void)
{
	ESocketConnectionState CurrentState = ESocketConnectionState::ConnectionError;

	// look for an existing error
	if (HasState(ESocketBSDParam::HasError) == ESocketBSDReturn::No)
	{
		if (DateTime::utcNow() - _LastActivityTime > Timespan::fromSeconds(5))
		{
			// get the write state
			ESocketBSDReturn WriteState = HasState(ESocketBSDParam::CanWrite, Timespan::fromMilliseconds(1));
			ESocketBSDReturn ReadState = HasState(ESocketBSDParam::CanRead, Timespan::fromMilliseconds(1));
		
			// translate yes or no (error is already set)
			if (WriteState == ESocketBSDReturn::Yes || ReadState == ESocketBSDReturn::Yes)
			{
				CurrentState = ESocketConnectionState::Connected;
				_LastActivityTime = DateTime::utcNow();
			}
			else if (WriteState == ESocketBSDReturn::No && ReadState == ESocketBSDReturn::No)
			{
				CurrentState = ESocketConnectionState::NotConnected;
			}
		}
		else
		{
			CurrentState = ESocketConnectionState::Connected;
		}
	}

	return CurrentState;
}


void SocketBSD::GetAddress(InternetAddrBSD& outAddr)
{
	InternetAddrBSD& addr = (InternetAddrBSD&)outAddr;
	int32 aockaddrLen = sizeof(sockaddr_in);

	// Figure out what ip/port we are bound to
	int Result = getsockname(_Socket, addr, &aockaddrLen);

	if (Result != 0)
	{
		LOG_ERROR("***** Failed to read address for socket with for error:%d", Result);
	}
}


bool SocketBSD::GetPeerAddress(InternetAddrBSD& OutAddr)
{
	InternetAddrBSD& Addr = (InternetAddrBSD&)OutAddr;
	int32 aockaddrLen = sizeof(sockaddr_in);

	// Figure out what ip/port we are bound to
	int Result = getpeername(_Socket, Addr, &aockaddrLen);

	if (Result != 0)
	{
		LOG_ERROR("***** Failed to read address for socket with for error:%d", Result);
	}
	return Result == 0;
}

bool SocketBSD::SetNonBlocking(bool bIsNonBlocking)
{
	u_long Value = bIsNonBlocking ? true : false;

#if PLATFORM_HAS_BSD_SOCKET_FEATURE_WINSOCKETS
	return ioctlsocket(_Socket,FIONBIO,&Value) == 0;
#else 
	int Flags = fcntl(Socket, F_GETFL, 0);
	//Set the flag or clear it, without destroying the other flags.
	Flags = bIsNonBlocking ? Flags | O_NONBLOCK : Flags ^ (Flags & O_NONBLOCK);
	int err = fcntl(Socket, F_SETFL, Flags);
	return (err == 0 ? true : false);
#endif

}


bool SocketBSD::SetBroadcast(bool bAllowBroadcast)
{
	int Param = bAllowBroadcast ? 1 : 0;
	return setsockopt(_Socket,SOL_SOCKET,SO_BROADCAST,(char*)&Param,sizeof(Param)) == 0;
}


bool SocketBSD::JoinMulticastGroup(const InternetAddrBSD& groupAddress)
{
	ip_mreq imr;

	imr.imr_interface.s_addr = INADDR_ANY;
	imr.imr_multiaddr = ((sockaddr_in*)&**((InternetAddrBSD*)(&groupAddress)))->sin_addr;
	return (setsockopt(_Socket, IPPROTO_IP, IP_ADD_MEMBERSHIP, (char*)&imr, sizeof(imr)) == 0);
}


bool SocketBSD::LeaveMulticastGroup(const InternetAddrBSD& groupAddress)
{
	ip_mreq imr;
	imr.imr_interface.s_addr = INADDR_ANY;
	imr.imr_multiaddr = ((sockaddr_in*)&**((InternetAddrBSD*)(&groupAddress)))->sin_addr;
	return (setsockopt(_Socket, IPPROTO_IP, IP_DROP_MEMBERSHIP, (char*)&imr, sizeof(imr)) == 0);
}


bool SocketBSD::SetMulticastLoopback(bool bLoopback)
{
	return (setsockopt(_Socket, IPPROTO_IP, IP_MULTICAST_LOOP, (char*)&bLoopback, sizeof(bLoopback)) == 0);
}


bool SocketBSD::SetMulticastTtl(uint8 timeToLive)
{
	return (setsockopt(_Socket, IPPROTO_IP, IP_MULTICAST_TTL, (char*)&timeToLive, sizeof(timeToLive)) == 0);
}


bool SocketBSD::SetReuseAddr(bool bAllowReuse)
{
	int Param = bAllowReuse ? 1 : 0;
	return setsockopt(_Socket,SOL_SOCKET,SO_REUSEADDR,(char*)&Param,sizeof(Param)) == 0;
}


bool SocketBSD::SetLinger(bool bShouldLinger,int32 timeout)
{
	linger ling;
	ling.l_onoff = bShouldLinger;
	ling.l_linger = timeout;
	return setsockopt(_Socket,SOL_SOCKET,SO_LINGER,(char*)&ling,sizeof(ling)) == 0;
}


bool SocketBSD::SetRecvErr(bool bUseErrorQueue)
{
	// Not supported, but return true to avoid spurious log messages
	return true;
}


bool SocketBSD::SetSendBufferSize(int32 size,int32& newSize)
{
	int32 typeSize = sizeof(int32);
	bool bOk = setsockopt(_Socket, SOL_SOCKET, SO_SNDBUF, (char*)&size, sizeof(int32)) == 0;

	// Read the value back in case the size was modified
	getsockopt(_Socket, SOL_SOCKET, SO_SNDBUF, (char*)&newSize, &typeSize);

	return bOk;
}


bool SocketBSD::SetReceiveBufferSize(int32 Size,int32& NewSize)
{
	int32 typeSize = sizeof(int32);
	bool bOk = setsockopt(_Socket,SOL_SOCKET,SO_RCVBUF,(char*)&Size,sizeof(int32)) == 0;

	// Read the value back in case the size was modified
	getsockopt(_Socket, SOL_SOCKET, SO_RCVBUF, (char*)&NewSize, &typeSize);

	return bOk;
}


int32 SocketBSD::GetPortNo(void)
{
	sockaddr_in Addr;

	int32 sockaddrLen = sizeof(sockaddr_in);

	// Figure out what ip/port we are bound to
	bool bOk = getsockname(_Socket, (sockaddr*)&Addr, &sockaddrLen) == 0;
	
	if (bOk == false)
	{
		LOG_ERROR("***** Failed to read address for socket ");
	}

	// Read the port number
	return ntohs(Addr.sin_port);
}


/* SocketBSD implementation
*****************************************************************************/

ESocketBSDReturn SocketBSD::HasState(ESocketBSDParam state, Timespan waitTime)
{
//#if PLATFORM_HAS_BSD_SOCKET_FEATURE_SELECT
	// convert WaitTime to a timeval
	timeval Time;
	Time.tv_sec = (int32)waitTime.getTotalSeconds();
	Time.tv_usec = waitTime.getMilliseconds() * 1000;

	fd_set SocketSet;

	// Set up the socket sets we are interested in (just this one)
	FD_ZERO(&SocketSet);
	FD_SET(_Socket, &SocketSet);

	// Check the status of the state
	int32 SelectStatus = 0;
	switch (state)
	{
	case ESocketBSDParam::CanRead:
		SelectStatus = select(_Socket + 1, &SocketSet, NULL, NULL, &Time);
		break;

	case ESocketBSDParam::CanWrite:
		SelectStatus = select(_Socket + 1, NULL, &SocketSet, NULL, &Time);
		break;

	case ESocketBSDParam::HasError:
		SelectStatus = select(_Socket + 1, NULL, NULL, &SocketSet, &Time);
		break;
	}

	// if the select returns a positive number, the socket had the state, 0 means didn't have it, and negative is API error condition (not socket's error state)
	return SelectStatus > 0 ? ESocketBSDReturn::Yes :
		SelectStatus == 0 ? ESocketBSDReturn::No :
		ESocketBSDReturn::EncounteredError;
//#else
	//LOG_ERROR("***** This platform doesn't support select(), but SocketBSD::HasState was not overridden");
	//return ESocketBSDReturn::EncounteredError;
//#endif
}


