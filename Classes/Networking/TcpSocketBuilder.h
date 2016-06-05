#ifndef LOSEMYMIND_TCPSOCKETBUILDER_H
#define LOSEMYMIND_TCPSOCKETBUILDER_H


#pragma once
#include <string>
#include "FoundationKit/Base/Types.h"
#include "FoundationKit/Foundation/Logger.h"
#include "IPv4Endpoint.h"
#include "SocketBSD.h"
USING_NS_FK;

/**
 * Implements a fluent builder for TCP sockets.
 */
class TcpSocketBuilder
{
public:

	/**
	 * Creates and initializes a new instance.
	 *
	 * @param InDescription Debug description for the socket.
	 */
    TcpSocketBuilder(const std::string& InDescription)
        : _Blocking(false)
        , _Bound(false)
        , _BoundEndpoint(IPv4Address::Any, 0)
        , _Description(InDescription)
        , _Linger(false)
        , _LingerTimeout(0)
        , _Listen(false)
        , _ReceiveBufferSize(0)
        , _Reusable(false)
        , _SendBufferSize(0)
	{ }

public:

	/**
	 * Sets socket operations to be blocking.
	 *
	 * @return This instance (for method chaining).
	 * @see AsNonBlocking, AsReusable
	 */
    TcpSocketBuilder AsBlocking()
	{
        _Blocking = true;

		return *this;
	}

	/**
	 * Sets socket operations to be non-blocking.
	 *
	 * @return This instance (for method chaining).
	 * @see AsBlocking, AsReusable
	 */
    TcpSocketBuilder AsNonBlocking()
	{
        _Blocking = false;

		return *this;
	}

	/**
	 * Makes the bound address reusable by other sockets.
	 *
	 * @return This instance (for method chaining).
	 * @see AsNonBlocking, AsNonBlocking
	 */
    TcpSocketBuilder AsReusable()
	{
        _Reusable = true;

		return *this;
	}

	/**
 	 * Sets the local address to bind the socket to.
	 *
	 * Unless specified in a subsequent call to BoundToPort(), a random
	 * port number will be assigned by the underlying provider.
	 *
	 * @param Address The IP address to bind the socket to.
	 * @return This instance (for method chaining).
	 * @see BoundToEndpoint, BoundToPort
	 */
    TcpSocketBuilder BoundToAddress(const IPv4Address& Address)
	{
        _BoundEndpoint = IPv4Endpoint(Address, _BoundEndpoint.Port);
        _Bound = true;

		return *this;
	}

	/**
 	 * Sets the local endpoint to bind the socket to.
	 *
	 * @param Endpoint The IP endpoint to bind the socket to.
	 * @return This instance (for method chaining).
	 * @see BoundToAddress, BoundToPort
	 */
    TcpSocketBuilder BoundToEndpoint(const IPv4Endpoint& Endpoint)
	{
        _BoundEndpoint = Endpoint;
        _Bound = true;

		return *this;
	}

	/**
	 * Sets the local port to bind the socket to.
	 *
	 * Unless specified in a subsequent call to BoundToAddress(), the local
	 * address will be determined automatically by the underlying provider.
	 *
	 * @param Port The local port number to bind the socket to.
	 * @return This instance (for method chaining).
	 * @see BoundToAddress, BoundToEndpoint
	 */
    TcpSocketBuilder BoundToPort(int32 Port)
	{
        _BoundEndpoint = IPv4Endpoint(_BoundEndpoint.Address, Port);
        _Bound = true;

		return *this;
	}

	/**
	 * Sets how long the socket will linger after closing.
	 *
	 * @param Timeout The amount of time to linger before closing.
	 * @return This instance (for method chaining).
	 */
    TcpSocketBuilder Lingering(int32 Timeout)
	{
        _Linger = true;
        _LingerTimeout = Timeout;

		return *this;
	}

	/**
	 * Sets the socket into a listening state for incoming connections.
	 *
	 * @param MaxBacklog The number of connections to queue before refusing them.
	 * @return This instance (for method chaining).
	 */
    TcpSocketBuilder Listening(int32 MaxBacklog)
	{
        _Listen = true;
        _ListenBacklog = MaxBacklog;

		return *this;
	}

	/**
	 * Specifies the desired size of the receive buffer in bytes (0 = default).
	 *
	 * The socket creation will not fail if the desired size cannot be set or
	 * if the actual size is less than the desired size.
	 *
	 * @param SizeInBytes The size of the buffer.
	 * @return This instance (for method chaining).
	 * @see WithSendBufferSize
	 */
    TcpSocketBuilder WithReceiveBufferSize(int32 SizeInBytes)
	{
        _ReceiveBufferSize = SizeInBytes;

		return *this;
	}

	/**
	 * Specifies the desired size of the send buffer in bytes (0 = default).
	 *
	 * The socket creation will not fail if the desired size cannot be set or
	 * if the actual size is less than the desired size.
	 *
	 * @param SizeInBytes The size of the buffer.
	 * @return This instance (for method chaining).
	 * @see WithReceiveBufferSize
	 */
    TcpSocketBuilder WithSendBufferSize(int32 SizeInBytes)
	{
        _SendBufferSize = SizeInBytes;

		return *this;
	}

public:

	/**
	 * Implicit conversion operator that builds the socket as configured.
	 *
	 * @return The built socket.
	 */
	operator Socket*() const
	{
		return Build();
	}

	/**
	 * Builds the socket as configured.
	 *
	 * @return The built socket.
	 */
	Socket* Build() const
	{


        SOCKET nativeSocket = INVALID_SOCKET;
        // Creates a stream (TCP) socket
        nativeSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
        Socket* NewSocket = (nativeSocket != INVALID_SOCKET) ? new SocketBSD(nativeSocket, ESocketType::Streaming, _Description) : nullptr;

        if (NewSocket != nullptr)
		{
            ::SetHandleInformation((HANDLE)nativeSocket, HANDLE_FLAG_INHERIT, 0);

            bool Error = !NewSocket->SetReuseAddr(_Reusable) ||
                !NewSocket->SetLinger(_Linger, _LingerTimeout) ||
                !NewSocket->SetRecvErr();

			if (!Error)
			{
                Error = _Bound && !NewSocket->Bind(*_BoundEndpoint.ToInternetAddr());
			}

			if (!Error)
			{
                Error = _Listen && !NewSocket->Listen(_ListenBacklog);
			}

			if (!Error)
			{
                Error = !NewSocket->SetNonBlocking(!_Blocking);
			}

			if (!Error)
			{
				int32 OutNewSize;

                if (_ReceiveBufferSize > 0)
				{
                    NewSocket->SetReceiveBufferSize(_ReceiveBufferSize, OutNewSize);
				}

                if (_SendBufferSize > 0)
				{
                    NewSocket->SetSendBufferSize(_SendBufferSize, OutNewSize);
				}
			}

			if (Error)
			{
                LOG_ERROR("***** TcpSocketBuilder: Failed to create the socket %s as configured", _Description.c_str());
                SAFE_DELETE(NewSocket);
			}
		}


        return NewSocket;
	}

private:

	/** Holds a flag indicating whether socket operations are blocking. */
    bool _Blocking;

	/** Holds a flag indicating whether the socket should be bound. */
    bool _Bound;

	/** Holds the IP address (and port) that the socket will be bound to. */
    IPv4Endpoint _BoundEndpoint;

	/** Holds the socket's debug description text. */
    std::string _Description;

	/** Holds a flag indicating whether the socket should linger after closing. */
    bool _Linger;

	/** Holds the amount of time the socket will linger before closing. */
    int32 _LingerTimeout;

	/** Holds a flag indicating whether the socket should listen for incoming connections. */
    bool _Listen;

	/** Holds the number of connections to queue up before refusing them. */
    int32 _ListenBacklog;

	/** The desired size of the receive buffer in bytes (0 = default). */
    int32 _ReceiveBufferSize;

	/** Holds a flag indicating whether the bound address can be reused by other sockets. */
    bool _Reusable;

	/** The desired size of the send buffer in bytes (0 = default). */
	int32 _SendBufferSize;
};
#endif // LOSEMYMIND_TCPSOCKETBUILDER_H
