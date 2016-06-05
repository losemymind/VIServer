#ifndef LOSEMYMIND_IPV4ENDPOINT_H
#define LOSEMYMIND_IPV4ENDPOINT_H



#pragma once
#include <memory>
#include "IPv4Address.h"
#include "IPAddressBSD.h"

/**
 * Implements an endpoint for IPv4 networks.
 *
 * An endpoint consists of an IPv4 address and a port number.
 *
 * @todo gmp: add IPv6 support and rename this to FIpEndpoint?
 */
struct IPv4Endpoint
{
	/** Holds the endpoint's IP address. */
	IPv4Address Address;

	/** Holds the endpoint's port number. */
	uint16 Port; 

public:

	/** Default constructor. */
    IPv4Endpoint() { }

	/**
	 * Creates and initializes a new IPv4 endpoint with the specified NetID and port.
	 *
	 * @param InAddress The endpoint's IP address.
	 * @param InPort The endpoint's port number.
	 */
    IPv4Endpoint(const IPv4Address& InAddress, uint16 InPort)
		: Address(InAddress)
		, Port(InPort)
	{ }

	/**
	 * Creates and initializes a new IPv4 endpoint from a given FInternetAddr object.
	 *
	 * Note: this constructor will be removed after the socket subsystem has been refactored.
	 *
	 * @param InternetAddrBSD The Internet address.
	 */
    IPv4Endpoint(const std::shared_ptr<InternetAddrBSD>& InternetAddr)
	{
		int32 OutPort;
		InternetAddr->GetIp(Address.Value);
		InternetAddr->GetPort(OutPort);
		Port = OutPort;
	}

public:

	/**
	 * Compares this IPv4 endpoint with the given endpoint for equality.
	 *
	 * @param Other The endpoint to compare with.
	 * @return true if the endpoints are equal, false otherwise.
	 */
	bool operator==(const IPv4Endpoint& Other) const
	{
		return ((Address == Other.Address) && (Port == Other.Port));
	}

	/**
	 * Compares this IPv4 address with the given endpoint for inequality.
	 *
	 * @param Other The endpoint to compare with.
	 * @return true if the endpoints are not equal, false otherwise.
	 */
	bool operator!=(const IPv4Endpoint& Other) const
	{
		return ((Address != Other.Address) || (Port != Other.Port));
	}


public:

	/**
	 * Converts this endpoint to an FInternetAddr object.
	 *
	 * Note: this method will be removed after the socket subsystem is refactored.
	 *
	 * @return Internet address object representing this endpoint.
	 */
    std::shared_ptr<InternetAddrBSD> ToInternetAddr() const
	{
        std::shared_ptr<InternetAddrBSD> InternetAddr = std::shared_ptr<InternetAddrBSD>(new InternetAddrBSD);
		{
			InternetAddr->SetIp(Address.Value);
			InternetAddr->SetPort(Port);
		}
		return InternetAddr;
	}

	/**
	 * Gets a string representation for this endpoint.
	 *
	 * @return String representation.
	 * @see Parse, ToText
	 */
    std::string ToString() const
    {
        return StringUtils::format("%s:%i", Address.ToString().c_str(), Port);
    }

public:

	/**
	 * Converts a string to an IPv4 endpoint.
	 *
	 * @param EndpointString The string to convert.
	 * @param OutEndpoint Will contain the parsed endpoint.
	 * @return true if the string was converted successfully, false otherwise.
	 * @see ToString
	 */
    static  bool Parse(const std::string& EndpointString, IPv4Endpoint& OutEndpoint)
    {
        std::vector<std::string> Tokens = StringUtils::split(EndpointString, ':');

        if (Tokens.size() == 2)
        {
            if (IPv4Address::Parse(Tokens[0], OutEndpoint.Address))
            {
                OutEndpoint.Port = std::atoi(Tokens[1].c_str());

                return true;
            }
        }

        return false;
    }

public:

	/** Defines the wild card endpoint, which is 0.0.0.0:0 */
	static const IPv4Endpoint Any;

};





#endif // LOSEMYMIND_IPV4ENDPOINT_H
