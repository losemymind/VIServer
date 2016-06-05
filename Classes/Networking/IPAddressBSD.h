#ifndef LOSEMYMIND_IPADDRESSBSD_H
#define LOSEMYMIND_IPADDRESSBSD_H



#pragma once
#include <vector>
#include <string>
#include "socket_types.hpp"
#include "FoundationKit/Base/Types.h"
#include "FoundationKit/Foundation/StringUtils.h"

USING_NS_FK;

/**
 * Represents an internet ip address, using the relatively standard SOCKADDR_IN structure. All data is in network byte order
 */
class InternetAddrBSD 
{
protected:
	/** The internet ip address structure */
	sockaddr_in _Addr;

public:
	/**
	 * Constructor. Sets address to default state
	 */
    InternetAddrBSD()
	{
        memset(&_Addr, 0x00, sizeof(sockaddr_in));
        _Addr.sin_family = AF_INET;
	}
	/**
	 * Sets the ip address from a host byte order uint32
	 *
	 * @param inAddr the new address to use (must convert to network byte order)
	 */
    virtual void SetIp(uint32 inAddr)
	{
        _Addr.sin_addr.s_addr = htonl(inAddr);
	}

	/**
	 * Sets the ip address from a string ("A.B.C.D:PORT")
	 *
	 * @param inAddr the string containing the new ip address to use
	 */
	virtual void SetIp(const char* inAddr, bool& bIsValid)
	{
		int32 A, B, C, D;
		int32 port = 0;

        std::string addressString = inAddr;

		std::vector<std::string> portTokens = StringUtils::split(addressString, ':');

		
		// look for a port number
        if (portTokens.size() > 1)
		{
            port = std::atoi(portTokens[1].c_str());
		}

		// now split the part before the : into a.b.c.d

        std::vector<std::string> addrTokens = StringUtils::split(portTokens[0], ':');
	

        if (addrTokens.size() < 4)
		{
			bIsValid = false;
			return;
		}

        A = std::atoi(addrTokens[0].c_str());
        B = std::atoi(addrTokens[1].c_str());
        C = std::atoi(addrTokens[2].c_str());
        D = std::atoi(addrTokens[3].c_str());

		// Make sure the address was valid
		if ((A & 0xFF) == A && (B & 0xFF) == B && (C & 0xFF) == C && (D & 0xFF) == D)
		{
			SetIp((A << 24) | (B << 16) | (C << 8) | (D << 0));

            if (port != 0)
			{
                SetPort(port);
			}

			bIsValid = true;
		}
		else
		{
			//debugf(TEXT("Invalid IP address string (%s) passed to SetIp"),InAddr);
			bIsValid = false;
		}
	}

	/**
	 * Sets the ip address using a network byte order ip address
	 *
	 * @param IpAddr the new ip address to use
	 */
	void SetIp(const in_addr& ipAddr)
 	{
        _Addr.sin_addr = ipAddr;
 	}


	/**
	 * Copies the network byte order ip address to a host byte order dword
	 *
	 * @param OutAddr the out param receiving the ip address
	 */
	virtual void GetIp(uint32& outAddr) const
	{ 
        outAddr = ntohl(_Addr.sin_addr.s_addr);
	}

	/**
	 * Copies the network byte order ip address 
	 *
	 * @param OutAddr the out param receiving the ip address
	 */
    void GetIp(in_addr& outAddr) const
 	{
        outAddr = _Addr.sin_addr;
 	}

	/**
	 * Sets the port number from a host byte order int
	 *
	 * @param InPort the new port to use (must convert to network byte order)
	 */
	virtual void SetPort(int32 InPort)
	{
        _Addr.sin_port = htons(InPort);
	}

	/**
	 * Copies the port number from this address and places it into a host byte order int
	 *
	 * @param OutPort the host byte order int that receives the port
	 */
	virtual void GetPort(int32& OutPort) const
	{
        OutPort = ntohs(_Addr.sin_port);
	}

	/**
	 * Returns the port number from this address in host byte order
	 */
	virtual int32 GetPort(void) const
	{
        return ntohs(_Addr.sin_port);
	}

	/** Sets the address to be any address */
	virtual void SetAnyAddress(void)
	{
		SetIp(INADDR_ANY);
		SetPort(0);
	}

	/** Sets the address to broadcast */
	virtual void SetBroadcastAddress()
	{
		SetIp(INADDR_BROADCAST);
		SetPort(0);
	}

	/**
	 * Converts this internet ip address to string form
	 *
	 * @param bAppendPort whether to append the port information or not
	 */
	virtual std::string ToString(bool bAppendPort) const
	{
        uint32 LocalAddr = ntohl(_Addr.sin_addr.s_addr);

		// Get the individual bytes
		const int32 A = (LocalAddr >> 24) & 0xFF;
		const int32 B = (LocalAddr >> 16) & 0xFF;
		const int32 C = (LocalAddr >>  8) & 0xFF;
		const int32 D = (LocalAddr >>  0) & 0xFF;
		if (bAppendPort)
		{
			return StringUtils::format("%i.%i.%i.%i:%i",A,B,C,D,GetPort());
		}
		else
		{
            return StringUtils::format("%i.%i.%i.%i", A, B, C, D);
		}
	}

	/**
	 * Compares two internet ip addresses for equality
	 *
	 * @param Other the address to compare against
	 */
    virtual bool operator==(const InternetAddrBSD& Other) const
	{
        InternetAddrBSD& OtherBSD = (InternetAddrBSD&)Other;
        return _Addr.sin_addr.s_addr == OtherBSD._Addr.sin_addr.s_addr &&
            _Addr.sin_port == OtherBSD._Addr.sin_port &&
            _Addr.sin_family == OtherBSD._Addr.sin_family;
	}

	/**
	 * Is this a well formed internet address
	 *
	 * @return true if a valid IP, false otherwise
	 */
	virtual bool IsValid() const
	{
        return _Addr.sin_addr.s_addr != 0;
	}

 	operator sockaddr*(void)
 	{
        return (sockaddr*)&_Addr;
 	}

 	operator const sockaddr*(void) const
 	{
        return (const sockaddr*)&_Addr;
 	}
};

#endif // LOSEMYMIND_IPADDRESSBSD_H
