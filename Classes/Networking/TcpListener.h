#ifndef LOSEMYMIND_TCPLISTENER_H
#define LOSEMYMIND_TCPLISTENER_H



#pragma once
#include <functional>
#include <thread>
#include "FoundationKit/Base/Timespan.h"
#include "Socket.h"
#include "IPv4Address.h"
#include "IPv4Endpoint.h"
#include "TcpSocketBuilder.h"

USING_NS_FK;

typedef std::function<bool(Socket*, const IPv4Endpoint&)> OnTcpListenerConnectionAccepted;



/**
 * Implements a runnable that listens for incoming TCP connections.
 */
class TcpListener
{
public:

	/**
	 * Creates and initializes a new instance from the specified IP endpoint.
	 *
	 * @param LocalEndpoint The local IP endpoint to listen on.
	 * @param SleepTime The time to sleep between checking for pending connections (default = 0 seconds).
	 */
	TcpListener(const IPv4Endpoint& LocalEndpoint, const Timespan& InSleepTime = Timespan::zero())
        : _DeleteSocket(true)
        , _Endpoint(LocalEndpoint)
        , _SleepTime(InSleepTime)
        , _Socket(nullptr)
        , _Stopping(false)
	{
        _Thread = std::thread(std::bind(&TcpListener::Run, this));
	}

	/**
	 * Creates and initializes a new instance from the specified socket.
	 *
	 * @param InSocket The socket to listen on.
	 * @param SleepTime The time to sleep between checking for pending connections (default = 0 seconds).
	 */
	TcpListener(Socket& InSocket, const Timespan& InSleepTime = Timespan::zero())
        : _DeleteSocket(false)
        , _SleepTime(InSleepTime)
        , _Socket(&InSocket)
        , _Stopping(false)
	{
		std::shared_ptr<InternetAddrBSD> LocalAddress = std::shared_ptr<InternetAddrBSD>(new InternetAddrBSD);
        _Socket->GetAddress(*LocalAddress);
        _Endpoint = IPv4Endpoint(LocalAddress);
        _Thread = std::thread(std::bind(&TcpListener::Run, this));
	}

	/** Destructor. */
	~TcpListener()
	{
        Stop();
        _Thread.join();
        if (_DeleteSocket && (_Socket != nullptr))
		{
            delete _Socket;
            _Socket = nullptr;
		}
	}

public:

	/**
	 * Gets the listener's local IP endpoint.
	 *
	 * @return IP endpoint.
	 */
	const IPv4Endpoint& GetLocalEndpoint() const
	{
        return _Endpoint;
	}

	/**
	 * Gets the listener's network socket.
	 *
	 * @return Network socket.
	 */
	Socket* GetSocket() const
	{
        return _Socket;
	}

	/**
	 * Checks whether the listener is listening for incoming connections.
	 *
	 * @return true if it is listening, false otherwise.
	 */
	bool IsActive() const
	{
        return ((_Socket != nullptr) && !_Stopping);
	}

    virtual void Stop()
    {
        _Stopping = true;
    }
public:

	/**
	 * Gets a delegate to be invoked when an incoming connection has been accepted.
	 *
	 * If this delegate is not bound, the listener will reject all incoming connections.
	 * To temporarily disable accepting connections, use the Enable() and Disable() methods.
	 *
	 * @return The delegate.
	 * @see Enable, Disable
	 */
	OnTcpListenerConnectionAccepted& OnConnectionAccepted()
	{
		return ConnectionAcceptedDelegate;
	}

public:


	virtual uint32 Run()
	{
        if (_Socket == nullptr)
		{
            _Socket = TcpSocketBuilder("TcpListener server")
				.AsReusable()
				.BoundToEndpoint(_Endpoint)
				.Listening(8);

			int32 NewSize = 0;
			_Socket->SetReceiveBufferSize(2 * 1024 * 1024, NewSize);
		}

        std::shared_ptr<InternetAddrBSD> RemoteAddress = std::shared_ptr<InternetAddrBSD>(new InternetAddrBSD);

		while (!_Stopping)
		{
			bool Pending;

			// handle incoming connections
			if (_Socket->HasPendingConnection(Pending) && Pending)
			{
                Socket* ConnectionSocket = _Socket->Accept(*RemoteAddress, "TcpListener client");

				if (ConnectionSocket != nullptr)
				{
					bool Accepted = false;

					if (ConnectionAcceptedDelegate)
					{
						Accepted = ConnectionAcceptedDelegate(ConnectionSocket, IPv4Endpoint(RemoteAddress));
					}

					if (!Accepted)
					{
						ConnectionSocket->Close();
                        delete ConnectionSocket;
					}
				}
			}

            std::this_thread::sleep_for(std::chrono::seconds(_SleepTime.getSeconds()));
		}

		return 0;
	}

private:

	/** Holds a flag indicating whether the socket should be deleted in the destructor. */
    bool _DeleteSocket;

	/** Holds the server endpoint. */
    IPv4Endpoint _Endpoint;

	/** Holds the time to sleep between checking for pending connections. */
    Timespan _SleepTime;

	/** Holds the server socket. */
    Socket* _Socket;

	/** Holds a flag indicating that the thread is stopping. */
    bool _Stopping;

	/** Holds the thread object. */
	std::thread _Thread;

private:

	/** Holds a delegate to be invoked when an incoming connection has been accepted. */
	OnTcpListenerConnectionAccepted ConnectionAcceptedDelegate;
};
#endif // LOSEMYMIND_TCPLISTENER_H
