/****************************************************************************
Copyright (c) 2015 libo.

losemymind.libo@gmail.com

****************************************************************************/
#ifdef WIN32
#include <windows.h>
#include <WindowsX.h>
#include <psapi.h>
#include <vector>
#include <memory>
#include "FoundationKit/GenericPlatformMacros.h"
#include "FoundationKit/Platform/Platform.h"
#include "FoundationKit/Platform/OpenGL.h"
#include "FoundationKit/Platform/Environment.h"
#include "FoundationKit/Foundation/Logger.h"
#include "FoundationKit/Foundation/StringUtils.h"
#include "FoundationKit/Base/Types.h"
#include "FoundationKit/Crypto/md5.hpp"


#pragma   comment(lib,"Psapi.lib")

// for getMacAddressUseGetAdaptersInfo
#include <Iphlpapi.h>
#pragma comment(lib, "iphlpapi.lib")


NS_FK_BEGIN

float Platform::getTotalMemory()
{
    long megabyte = 1024 * 1024;
	MEMORYSTATUSEX memInfo;
	memInfo.dwLength = sizeof(MEMORYSTATUSEX);
	GlobalMemoryStatusEx(&memInfo);
	DWORDLONG physical_memory = memInfo.ullTotalPhys;
    return static_cast<float>((physical_memory / megabyte));
}

float Platform::getFreeMemory()
{
    long megabyte = 1024 * 1024;
	MEMORYSTATUSEX memInfo;
	memInfo.dwLength = sizeof(MEMORYSTATUSEX);
	GlobalMemoryStatusEx(&memInfo);
	DWORDLONG free_memory = memInfo.ullAvailPhys;
    return static_cast<float>(free_memory / megabyte);
}

float Platform::getProcessMemory()
{
    long megabyte = 1024 * 1024;
	PROCESS_MEMORY_COUNTERS_EX pmc;
	GetProcessMemoryInfo(GetCurrentProcess(), (PROCESS_MEMORY_COUNTERS*)&pmc, sizeof(pmc));
	SIZE_T used_memory = pmc.WorkingSetSize;
    return static_cast<float>(used_memory / megabyte);
}



std::string Platform::getMacAddress(bool isRaw /*= false*/)
{
    std::string result;
    IP_ADAPTER_INFO IpAddresses[16];
    ULONG OutBufferLength = sizeof(IP_ADAPTER_INFO) * 16;
    // Read the adapters
    uint32 RetVal = GetAdaptersInfo(IpAddresses, &OutBufferLength);
    if (RetVal == NO_ERROR)
    {
        PIP_ADAPTER_INFO AdapterList = IpAddresses;
        // Walk the set of addresses copying each one
        while (AdapterList)
        {
            // If there is an address to read
            if (AdapterList->AddressLength > 0)
            {
                if (isRaw)
                {
                    result = std::string((char*)&AdapterList->Address[0], 6);
                }
                else
                {
                    // Copy the data and say we did
                    result = StringUtils::format("%02X%02X%02X%02X%02X%02X"
                        , AdapterList->Address[0]
                        , AdapterList->Address[1]
                        , AdapterList->Address[2]
                        , AdapterList->Address[3]
                        , AdapterList->Address[4]
                        , AdapterList->Address[5]);
                }

                break;
            }
            AdapterList = AdapterList->Next;
        }
    }
    return result;
}

std::string Platform::getDeviceId()
{
    return MD5::md5_hash_hex(getMacAddress().c_str());
}

std::string Platform::getDeviceName()
{
    return Environment::GetMachineName();
}

std::string Platform::getOperatingSystemVersion()
{
    return Environment::GetOSVersion();
}

std::string Platform::getCPUArchitecture()
{
    SYSTEM_INFO stInfo;
    GetSystemInfo(&stInfo);
    //GetNativeSystemInfo(&stInfo);
    WORD procArch = stInfo.wProcessorArchitecture;
    if (procArch == PROCESSOR_ARCHITECTURE_INTEL)         return "Intel x86";
    if (procArch == PROCESSOR_ARCHITECTURE_MIPS)          return "mips";
    if (procArch == PROCESSOR_ARCHITECTURE_ALPHA)         return "alpha x86";
    if (procArch == PROCESSOR_ARCHITECTURE_PPC)           return "ppc";
    if (procArch == PROCESSOR_ARCHITECTURE_SHX)           return "shx";
    if (procArch == PROCESSOR_ARCHITECTURE_ARM)           return "arm";
    if (procArch == PROCESSOR_ARCHITECTURE_IA64)          return "Intel x64";
    if (procArch == PROCESSOR_ARCHITECTURE_ALPHA64)       return "alpha x64";
    if (procArch == PROCESSOR_ARCHITECTURE_MSIL)          return "msil";
    if (procArch == PROCESSOR_ARCHITECTURE_AMD64)         return "amd64";
    if (procArch == PROCESSOR_ARCHITECTURE_IA32_ON_WIN64) return "wow64";
    if (procArch == PROCESSOR_ARCHITECTURE_UNKNOWN)       return "Intel x86";
    return "Unknown";
}

void Platform::systemTime(int32& year, int32& month, int32& dayOfWeek, int32& day, int32& hour, int32& min, int32& sec, int32& msec)
{
    SYSTEMTIME st;
    GetLocalTime(&st);
    year = st.wYear;
    month = st.wMonth;
    dayOfWeek = st.wDayOfWeek;
    day = st.wDay;
    hour = st.wHour;
    min = st.wMinute;
    sec = st.wSecond;
    msec = st.wMilliseconds;
}

void Platform::utcTime(int32& year, int32& month, int32& dayOfWeek, int32& day, int32& hour, int32& min, int32& sec, int32& msec)
{
    SYSTEMTIME st;
    GetSystemTime(&st);

    year = st.wYear;
    month = st.wMonth;
    dayOfWeek = st.wDayOfWeek;
    day = st.wDay;
    hour = st.wHour;
    min = st.wMinute;
    sec = st.wSecond;
    msec = st.wMilliseconds;
}

int64 Platform::getTickCount()
{
    LARGE_INTEGER counter;
    QueryPerformanceCounter(&counter);
    return (int64)counter.QuadPart;
}


std::vector<uint8> Platform::getMacAddressRaw()
{
    std::vector<uint8> result;
    IP_ADAPTER_INFO IpAddresses[16];
    ULONG OutBufferLength = sizeof(IP_ADAPTER_INFO) * 16;
    // Read the adapters
    uint32 RetVal = GetAdaptersInfo(IpAddresses, &OutBufferLength);
    if (RetVal == NO_ERROR)
    {
        PIP_ADAPTER_INFO AdapterList = IpAddresses;
        // Walk the set of addresses copying each one
        while (AdapterList)
        {
            // If there is an address to read
            if (AdapterList->AddressLength > 0)
            {
                result.resize(AdapterList->AddressLength);
                std::memcpy(result.data(), AdapterList->Address, result.size());
                break;
            }
            AdapterList = AdapterList->Next;
        }
    }
    return result;
}


NS_FK_END

#endif









