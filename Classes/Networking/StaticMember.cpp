
#include "IPv4Address.h"
#include "IPv4Endpoint.h"

const IPv4Address IPv4Address::Any(0, 0, 0, 0);
const IPv4Address IPv4Address::InternalLoopback(127, 0, 0, 1);
const IPv4Address IPv4Address::LanBroadcast(255, 255, 255, 255);

const IPv4Endpoint IPv4Endpoint::Any(IPv4Address(0, 0, 0, 0), 0);
