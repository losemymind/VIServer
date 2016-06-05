/****************************************************************************
  Copyright (c) 2015 libo All rights reserved.

  losemymind.libo@gmail.com

****************************************************************************/
#include <vector>
#include <stdarg.h>
#include "Logger.h"
#include "FoundationKit/Foundation/StringUtils.h"

#if (TARGET_PLATFORM == PLATFORM_WIN32)
#include <Winsock2.h>
#endif

#if (TARGET_PLATFORM == PLATFORM_ANDROID)
#include <android/log.h>
#endif


NS_FK_BEGIN

static const size_t MAX_LOG_LENGTH = 1024;
static const char*  LevelMsg[] = {"TRACE", "DEBUG", "INFO", "WARN", "ERROR", "FATAL"};


Logger::State::State():enabled(true){}

void Logger::log( Level level, const char* message, ... )
{
    Logger::State& state = _states[level];
    if (!state.enabled)
        return;
    std::string strPreMsg = LevelMsg[level];
    strPreMsg += ":";
    va_list args;

    // Declare a moderately sized buffer on the stack that should be
    // large enough to accommodate most log requests.
    int size = MAX_LOG_LENGTH;
    //char stackBuffer[MAX_LOG_LENGTH] = { 0 };
    std::vector<char> dynamicBuffer(MAX_LOG_LENGTH);
    char* str = &dynamicBuffer[0];
    for (;;)
    {
        va_start(args, message);
        // Pass one less than size to leave room for NULL terminator
        int needed = vsnprintf(str, size - 1, message, args);
        va_end(args);
        // NOTE: Some platforms return -1 when vsnprintf runs out of room, while others return
        // the number of characters actually needed to fill the buffer.
        if (needed >= 0 && needed < size)
        {
            // Successfully wrote buffer. Added a NULL terminator in case it wasn't written.
            str[needed] = '\0';
            break;
        }
        size = needed > 0 ? (needed + 1) : (size * 2);
        dynamicBuffer.resize(size);
        str = &dynamicBuffer[0];
    }
    strPreMsg += str;
    strPreMsg += "\n";

#if (TARGET_PLATFORM == PLATFORM_ANDROID)
    __android_log_print(ANDROID_LOG_DEBUG, "FoundationKit", "%s", strPreMsg.c_str());
#elif TARGET_PLATFORM ==  PLATFORM_WIN32
    std::wstring wstr = StringUtils::string2UTF8wstring(strPreMsg);
    OutputDebugStringW(wstr.c_str());
    printf("%s", strPreMsg.c_str());
    fflush(stdout);
#else
    // Linux, Mac, iOS, etc
    fprintf(stdout, "%s", strPreMsg.c_str());
    fflush(stdout);
#endif
}

bool Logger::isEnabled( Level level )
{
    return _states[level].enabled;
}

void Logger::setEnabled( Level level, bool enabled )
{
    _states[level].enabled = enabled;
}


NS_FK_END


