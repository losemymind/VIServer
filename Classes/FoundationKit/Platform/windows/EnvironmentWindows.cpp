

#include <sstream>
#include "FoundationKit/Platform/Environment.h"
#include "FoundationKit/Foundation/Exception.h"
#include "FoundationKit/Foundation/Logger.h"

NS_FK_BEGIN

std::string Environment::GetEnvironmentVariable(const std::string& variable)
{
    DWORD len = GetEnvironmentVariableA(variable.c_str(), 0, 0);
    if (len == 0) return ""; //throw NotFoundException(variable);
    char* buffer = new char[len];
    GetEnvironmentVariableA(variable.c_str(), buffer, len);
    std::string result(buffer);
    delete[] buffer;
    return result;
}

bool Environment::HasEnvironmentVariable(const std::string& variable)
{
    DWORD len = GetEnvironmentVariableA(variable.c_str(), 0, 0);
    return len > 0;
}

void Environment::SetEnvironmentVariable(const std::string& variable, const std::string& value)
{
    if (SetEnvironmentVariableA(variable.c_str(), value.c_str()) == 0)
    {
        std::string msg = "cannot set environment variable: ";
        msg.append(variable);
        throw SystemException(msg);
    }
}

typedef LONG(NTAPI* fnRtlGetVersion)(PRTL_OSVERSIONINFOW lpVersionInformation);
std::string Environment::GetMachineName()
{
    RTL_OSVERSIONINFOEXW verInfo = { 0 };
    verInfo.dwOSVersionInfoSize = sizeof(verInfo);
    static auto RtlGetVersion = (fnRtlGetVersion)GetProcAddress(GetModuleHandleA("ntdll.dll"), "RtlGetVersion");
    if (RtlGetVersion != 0)
        RtlGetVersion((PRTL_OSVERSIONINFOW)&verInfo);

    switch (verInfo.dwMajorVersion)
    {
    case 10:
        switch (verInfo.dwMinorVersion)
        {
        case 0:
            return verInfo.wProductType == VER_NT_WORKSTATION ? "Windows 10" : "Windows Server 2016";
        }
    case 6:
        switch (verInfo.dwMinorVersion)
        {
        case 0:
            return verInfo.wProductType == VER_NT_WORKSTATION ? "Windows Vista" : "Windows Server 2008";
        case 1:
            return verInfo.wProductType == VER_NT_WORKSTATION ? "Windows 7" : "Windows Server 2008 R2";
        case 2:
            return verInfo.wProductType == VER_NT_WORKSTATION ? "Windows 8" : "Windows Server 2012";
        case 3:
            return verInfo.wProductType == VER_NT_WORKSTATION ? "Windows 8.1" : "Windows Server 2012 R2";
        default:
            return "Unknown";
        }
    case 5:
        switch (verInfo.dwMinorVersion)
        {
        case 0:
            return "Windows 2000";
        case 1:
            return "Windows XP";
        case 2:
            return "Windows Server 2003/Windows Server 2003 R2";
        default:
            return "Unknown";
        }
    default:
        return "Unknown";
    }
}

std::string Environment::GetOSVersion()
{
    RTL_OSVERSIONINFOEXW verInfo = { 0 };
    verInfo.dwOSVersionInfoSize = sizeof(verInfo);
    static auto RtlGetVersion = (fnRtlGetVersion)GetProcAddress(GetModuleHandleA("ntdll.dll"), "RtlGetVersion");
    if (RtlGetVersion != 0 && RtlGetVersion((PRTL_OSVERSIONINFOW)&verInfo) == 0)
    {
        std::ostringstream str;
        str << verInfo.dwMajorVersion << "." << verInfo.dwMinorVersion << " (Build " << (verInfo.dwBuildNumber & 0xFFFF);
        if (verInfo.szCSDVersion[0]) str << ": " << verInfo.szCSDVersion;
        str << ")";
        return str.str();
    }
    else
    {
        //throw SystemException("Cannot get OS version information");
        LOG_ERROR("***** Cannot get OS version information.");
    }
    return "";
}



NS_FK_END




