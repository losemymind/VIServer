#ifndef LOSEMYMIND_SOCKETBSD_H
#define LOSEMYMIND_SOCKETBSD_H


#pragma once


#include <string>
#include "Socket.h"
#include "FoundationKit/Base/DateTime.h"


/**
 * Enumerates BSD socket state parameters.
 */
enum class ESocketBSDParam
{
	CanRead,
	CanWrite,
	HasError,
};


/**
 * Enumerates BSD socket state return values.
 */
enum class ESocketBSDReturn
{
	Yes,
	No,
	EncounteredError,
};


/**
 * Implements a BSD network socket.
 */
class SocketBSD : public Socket
{
public:

	/**
	 * Assigns a BSD socket to this object.
	 *
	 * @param InSocket the socket to assign to this object.
	 * @param InSocketType the type of socket that was created.
	 * @param InSocketDescription the debug description of the socket.
	 */
    SocketBSD(SOCKET inSocket, ESocketType inSocketType, const std::string& inSocketDescription)
        : Socket(inSocketType, inSocketDescription)
        , _Socket(inSocket)
	{ }

	/**
	 * Destructor.
	 *
	 * Closes the socket if it is still open
	 */
    virtual ~SocketBSD()
	{
		Close();
	}

public:

	/**
	* Gets the Socket for anyone who knows they have an SocketBSD.
	*
	* @return The native socket.
	*/
	SOCKET GetNativeSocket()
	{
		return _Socket;
	}

public:

	// Socket overrides

	virtual bool Close() override;
	virtual bool Bind(const InternetAddrBSD& addr) override;
    virtual bool Connect(const InternetAddrBSD& addr) override;
	virtual bool Listen(int32 maxBacklog) override;
	virtual bool HasPendingConnection(bool& bHasPendingConnection) override;
	virtual bool HasPendingData(uint32& pendingDataSize) override;
    virtual class Socket* Accept(const std::string& socketDescription) override;
    virtual class Socket* Accept(InternetAddrBSD& outAddr, const std::string& socketDescription) override;
    virtual bool SendTo(const uint8* data, int32 count, int32& bytesSent, const InternetAddrBSD& destination) override;
    virtual bool Send(const uint8* data, int32 count, int32& bytesSent) override;
    virtual bool RecvFrom(uint8* data, int32 bufferSize, int32& bytesRead, InternetAddrBSD& source, ESocketReceiveFlags flags = ESocketReceiveFlags::None) override;
    virtual bool Recv(uint8* data, int32 bufferSize, int32& bytesRead, ESocketReceiveFlags flags = ESocketReceiveFlags::None) override;
	virtual bool Wait(ESocketWaitConditions condition, Timespan waitTime) override;
	virtual ESocketConnectionState GetConnectionState() override;
    virtual void GetAddress(InternetAddrBSD& outAddr) override;
    virtual bool GetPeerAddress(InternetAddrBSD& outAddr) override;
	virtual bool SetNonBlocking(bool bIsNonBlocking = true) override;
	virtual bool SetBroadcast(bool bAllowBroadcast = true) override;
    virtual bool JoinMulticastGroup(const InternetAddrBSD& groupAddress) override;
    virtual bool LeaveMulticastGroup(const InternetAddrBSD& groupAddress) override;
	virtual bool SetMulticastLoopback(bool bLoopback) override;
	virtual bool SetMulticastTtl(uint8 timeToLive) override;
	virtual bool SetReuseAddr(bool bAllowReuse = true) override;
	virtual bool SetLinger(bool bShouldLinger = true, int32 timeout = 0) override;
	virtual bool SetRecvErr(bool bUseErrorQueue = true) override;
	virtual bool SetSendBufferSize(int32 size,int32& newSize) override;
	virtual bool SetReceiveBufferSize(int32 size,int32& newSize) override;
	virtual int32 GetPortNo() override;

protected:

	/** This is generally select(), but makes it easier for platforms without select to replace it. */
	virtual ESocketBSDReturn HasState(ESocketBSDParam State, Timespan WaitTime = Timespan(0));

	/** Updates this socket's time of last activity. */
	void UpdateActivity()
	{
        _LastActivityTime = DateTime::utcNow();
	}

	/** Holds the BSD socket object. */
	SOCKET _Socket;

	/** Last activity time. */
	DateTime _LastActivityTime;

};


#endif // LOSEMYMIND_SOCKETBSD_H
