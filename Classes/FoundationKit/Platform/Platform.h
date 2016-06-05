/****************************************************************************
Copyright (c) 2015 libo.

losemymind.libo@gmail.com

****************************************************************************/
#ifndef LOSEMYMIND_PLATFORM_H
#define LOSEMYMIND_PLATFORM_H

#pragma once
#include <string>
#include <functional>
#include <vector>
#include "FoundationKit/GenericPlatformMacros.h"
#include "FoundationKit/Base/Types.h"

NS_FK_BEGIN

class Platform
{
public:
	Platform();
	~Platform();

	// MB
	static float getTotalMemory();

	// MB
    static float getFreeMemory();

	// MB
    static float getProcessMemory();


    static std::string getMacAddress(bool isRaw = false);

    static std::string getDeviceId();

    static std::string getDeviceName();

    static std::string getOperatingSystemVersion();

    static std::string getCPUArchitecture();

    static void systemTime(int32& year, int32& month, int32& dayOfWeek, int32& day, int32& hour, int32& min, int32& sec, int32& msec);
    static void utcTime(int32& year, int32& month, int32& dayOfWeek, int32& day, int32& hour, int32& min, int32& sec, int32& msec);
    static int64 getTickCount();
    static int64 getCPUTickCount();
    
    static std::vector<uint8> getMacAddressRaw();

};

NS_FK_END
#endif // LOSEMYMIND_PLATFORM_H


