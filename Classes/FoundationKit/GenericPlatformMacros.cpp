#include "GenericPlatformMacros.h"
#include "FoundationKit/Foundation/StringUtils.h"
#include <cstdarg>
#include <cstdio>
#if (TARGET_PLATFORM == PLATFORM_ANDROID)
#include <android/log.h>
#elif (TARGET_PLATFORM == PLATFORM_WIN32)
#include <Windows.h>
#endif
USING_NS_FK;


void __fail__(const char* expr, const char* file, int line)
{
#if (DEBUG_MODE==1)

# if defined(EMSCRIPTEN) && EMSCRIPTEN
    __assert_fail(expr, file, line, "");
# elif defined __native_client__
    __assert(expr, line, file); // WHY.
# elif defined __ANDROID__
    __assert(file, line, expr);
# elif defined __clang__ || defined __GNU_LIBRARY__ || (defined __GNUC__ && defined __APPLE__)
    __assert(expr, file, line);
# elif defined __GNUC__
    _assert(expr, file, line);
# elif defined _MSC_VER
    _CrtDbgReport(_CRT_ASSERT, file, line, expr, "");
# else
#   error UNSUPPORTED COMPILER
# endif
#endif // (DEBUG_MODE==1)

}

void __log__(const char* file, int line, const char* message, ...)
{
    static const size_t MAX_LOG_LENGTH = 1024;
    // Declare a moderately sized buffer on the stack that should be
    // large enough to accommodate most log requests.
    int size = MAX_LOG_LENGTH;
    //char stackBuffer[MAX_LOG_LENGTH] = { 0 };
    std::vector<char> dynamicBuffer(MAX_LOG_LENGTH);
    char* str = &dynamicBuffer[0];
    std::string logMessage;
    logMessage += file ? file : "";
    logMessage += ":";
    logMessage += StringUtils::to_string(line);
    va_list args;
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
    logMessage += str;

#if (TARGET_PLATFORM == PLATFORM_ANDROID)
    __android_log_print(ANDROID_LOG_INFO, "LoseMyMind", "%s", logMessage.c_str());
#elif TARGET_PLATFORM ==  PLATFORM_WIN32
    std::wstring wstr = StringUtils::string2UTF8wstring(logMessage.c_str());
    OutputDebugStringW(wstr.c_str());
    printf("%s", logMessage.c_str());
    fflush(stdout);
#else
    // Linux, Mac, iOS, etc
    fprintf(stdout, "%s", logMessage.c_str());
    fflush(stdout);
#endif
}




