
#ifndef LOSEMYMIND_GENERICPLATFORMMACROS_H
#define LOSEMYMIND_GENERICPLATFORMMACROS_H
#pragma once
#include <cassert>
// namespace FoundationKit {}
#ifdef __cplusplus
    #define NS_FK_BEGIN                     namespace FoundationKit {
    #define NS_FK_END                       }
    #define USING_NS_FK                     using namespace FoundationKit
#else
    #define NS_FK_BEGIN 
    #define NS_FK_END 
    #define USING_NS_FK 
#endif 

#if __cplusplus < 201103L
#define constexpr
#define noexcept
#endif

// see https://sourceforge.net/p/predef/wiki/Compilers/
// or boost\config\select_platform_config.hpp
// define supported target platform macro which to uses.
#define PLATFORM_UNKNOWN            0
#define PLATFORM_IOS                1
#define PLATFORM_ANDROID            2
#define PLATFORM_WIN32              3
#define PLATFORM_LINUX              4
#define PLATFORM_MAC                5

#define TARGET_PLATFORM PLATFORM_UNKNOWN

#if defined(macintosh) || defined(__APPLE__) || defined(__APPLE_CC__)
// TARGET_OS_IPHONE inlcudes TARGET_OS_IOS TARGET_OS_TV and TARGET_OS_WATCH. see TargetConditionals.h
#include <TargetConditionals.h>
#endif

#if (TARGET_OS_IOS )
    #undef TARGET_PLATFORM
    #define TARGET_PLATFORM PLATFORM_IOS
   /**
    * Macro for marking up deprecated code, functions and types.
    *
    * Features that are marked as deprecated are scheduled to be removed from the code base
    * in a future release. If you are using a deprecated feature in your code, you should
    * replace it before upgrading to the next release. See the Upgrade Notes in the release
    * notes for the release in which the feature was marked deprecated.
    *
    * Sample usage (note the slightly different syntax for classes and structures):
    *
    *		DEPRECATED(4.6, "Message")
    *		void Function();
    *
    *		struct DEPRECATED(4.6, "Message") MODULE_API MyStruct
    *		{
    *			// StructImplementation
    *		};
    *		class DEPRECATED(4.6, "Message") MODULE_API MyClass
    *		{
    *			// ClassImplementation
    *		};
    *
    * @param VERSION The release number in which the feature was marked deprecated.
    * @param MESSAGE A message containing upgrade notes.
    */
    #define DEPRECATED(VERSION, MESSAGE) __attribute__((deprecated(MESSAGE " Please update your code to the new API before upgrading to the next release, otherwise your project will no longer compile.")))
    // Symbol export and import definitions
    #define SYMBOL_EXPORT
    #define SYMBOL_IMPORT
    // Alignment.
    #define ATTRIBUTE_PACK(n) __attribute__((packed,aligned(n)))
    #define ATTRIBUTE_ALIGN(n) __attribute__((aligned(n)))
    #define ATTRIBUTE_UNUSED __attribute__((unused))//__attribute__((__unused__)) ?
    #define FORCEINLINE inline __attribute__ ((always_inline))	/* Force code to be inline */
    #define FORCENOINLINE __attribute__((noinline))	            /* Force code to NOT be inline */
    #define THREAD_LOCAL __thread
    #define FILEPATH_MAX MAX_PATH


#elif defined(__ANDROID__) || defined(ANDROID)
    #undef TARGET_PLATFORM
    #define TARGET_PLATFORM PLATFORM_ANDROID
    #define DEPRECATED(VERSION, MESSAGE) __attribute__((deprecated(MESSAGE " Please update your code to the new API before upgrading to the next release, otherwise your project will no longer compile.")))
    // Symbol export and import definitions
    #define SYMBOL_EXPORT		__attribute__((visibility("default")))
    #define SYMBOL_IMPORT		__attribute__((visibility("default")))
    // Alignment.
    #define ATTRIBUTE_PACK(n) __attribute__((packed,aligned(n)))
    #define ATTRIBUTE_ALIGN(n) __attribute__((aligned(n)))
    #define ATTRIBUTE_UNUSED __attribute__((unused))//__attribute__((__unused__)) ?
    #define FORCEINLINE inline __attribute__ ((always_inline)) /* Force code to be inline */
    #define FORCENOINLINE __attribute__((noinline))	           /* Force code to NOT be inline */
    #define THREAD_LOCAL __thread
    #define FILEPATH_MAX PATH_MAX


#elif defined(_WIN32) || defined(__WIN32__) || defined(WIN32)
    #undef TARGET_PLATFORM
    #define TARGET_PLATFORM PLATFORM_WIN32
    #define DEPRECATED(VERSION, MESSAGE) __declspec(deprecated(MESSAGE " Please update your code to the new API before upgrading to the next release, otherwise your project will no longer compile."))
    // Symbol export and import definitions
    #define SYMBOL_EXPORT __declspec(dllexport)
    #define SYMBOL_IMPORT __declspec(dllimport)
    // Alignment.
    #define ATTRIBUTE_PACK(n)
    #define ATTRIBUTE_ALIGN(n) __declspec(align(n))
    #define ATTRIBUTE_UNUSED
    #define FORCEINLINE __forceinline				  /* Force code to be inline */
    #define FORCENOINLINE __declspec(noinline)        /* Force code to NOT be inline */	
    #define THREAD_LOCAL __declspec(thread)
    #define FILEPATH_MAX MAX_PATH

#elif (defined(linux) || defined(__linux) || defined(__linux__) || defined(__GNU__) || defined(__GLIBC__)) && !defined(_CRAYC)
    #undef TARGET_PLATFORM    
    #define TARGET_PLATFORM PLATFORM_LINUX
    #define DEPRECATED(VERSION, MESSAGE) __attribute__((deprecated(MESSAGE " Please update your code to the new API before upgrading to the next release, otherwise your project will no longer compile.")))
    // Symbol export and import definitions
    #define SYMBOL_EXPORT			__attribute__((visibility("default")))
    #define SYMBOL_IMPORT			__attribute__((visibility("default")))
    // Alignment.
    #define ATTRIBUTE_PACK(n) __attribute__((packed,aligned(n)))
    #define ATTRIBUTE_ALIGN(n) __attribute__((aligned(n)))
    #define ATTRIBUTE_UNUSED __attribute__((unused))//__attribute__((__unused__)) ?
    #define FORCEINLINE inline __attribute__ ((always_inline)) /* Force code to be inline */
    #define FORCENOINLINE __attribute__((noinline))	           /* Force code to NOT be inline */
    #define THREAD_LOCAL __thread
    #define FILEPATH_MAX PATH_MAX

#elif(TARGET_OS_MAC)
    #undef TARGET_PLATFORM
    #define TARGET_PLATFORM PLATFORM_MAC
    #define DEPRECATED(VERSION, MESSAGE) __attribute__((deprecated(MESSAGE " Please update your code to the new API before upgrading to the next release, otherwise your project will no longer compile.")))
    // Symbol export and import definitions
    #define SYMBOL_EXPORT
    #define SYMBOL_IMPORT
    // Alignment.
    #define ATTRIBUTE_PACK(n) __attribute__((packed,aligned(n)))
    #define ATTRIBUTE_ALIGN(n) __attribute__((aligned(n)))
    #define ATTRIBUTE_UNUSED __attribute__((unused)) //__attribute__((__unused__)) ?
    #define FORCEINLINE inline __attribute__ ((always_inline))  /* Force code to be inline */
    #define FORCENOINLINE __attribute__((noinline))	            /* Force code to NOT be inline */
    #define THREAD_LOCAL __thread
    #define FILEPATH_MAX MAX_PATH

#else
    #error Unknown platform
#endif

#define UNUSED_ARG(arg) do{(void)(arg);}while(0)

#define SAFE_DELETE(p)           do { delete (p); (p) = nullptr; } while(0)
#define SAFE_DELETE_ARRAY(p)     do { if(p) { delete[] (p); (p) = nullptr; } } while(0)
#define SAFE_FREE(p)             do { if(p) { free(p); (p) = nullptr; } } while(0)
#define SAFE_RELEASE(p)          do { if(p) { (p)->release(); } } while(0)
#define SAFE_RELEASE_NULL(p)     do { if(p) { (p)->release(); (p) = nullptr; } } while(0)
#define SAFE_RETAIN(p)           do { if(p) { (p)->retain(); } } while(0)
#define BREAK_IF(cond)           if(cond) break

// Make a string to Wide string
#define TEXT_HELPER(a,b) a ## b
#define MAKE_TEXT(s) TEXT_HELPER(L, s)

// Helper macro STRINGIZE:
// Converts the parameter X to a string after macro replacement
// on X has been performed.
#define STRINGIZE_HELPER(X) #X
#define STRINGIZE(X) STRINGIZE_HELPER(X)

//#pragma message(COMPILE_MSG "Show compile message")
#define COMPILE_MSG __FILE__ "(" STRINGIZE(__LINE__) "):Warning:" 

extern void __fail__(const char* expr, const char* file, int line);
extern void __log__(const char* file, int line, const char* message, ...);
#if defined(_DEBUG) || defined(DEBUG)
    #define ASSERTED_EXPRESSION(CHECK, EXPR) ((CHECK) ? (EXPR) : (__fail__(#CHECK, __FILE__, __LINE__), (EXPR)))
    #define LOG_ASSERT(cond, msg,...) do{if (!(cond)){ __log__(__FILE__, __LINE__, msg, ##__VA_ARGS__);assert(cond);}} while (false)
    #define DEBUG_MODE 1
#else
    #define ASSERTED_EXPRESSION(CHECK, EXPR) (EXPR)
    #define LOG_ASSERT(cond, msg,...) do{}while(false)
    #define DEBUG_MODE 0
#endif
#define LOG_CONDITION(cond,msg,...) do{if ((cond)){ __log__(__FILE__, __LINE__, msg, ##__VA_ARGS__);}} while (false)

//===============================================================================================
// Platform Pre-Setup
//===============================================================================================
#if (TARGET_PLATFORM == PLATFORM_WIN32)
__pragma (warning(disable:4127))
#define _XKEYCHECK_H // disable windows xkeycheck.h

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN 1
#endif

#ifndef NOMINMAX
#define NOMINMAX
#endif

#ifndef _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS 1
#endif

#ifndef _SCL_SECURE_NO_WARNINGS
#define _SCL_SECURE_NO_WARNINGS 1
#endif

#include <windows.h>

#endif //(TARGET_PLATFORM == PLATFORM_WIN32)

// IOS,ANDROID,MAC platform must be defined USE_FILE32API
//#ifndef USE_FILE32API
//#define USE_FILE32API 1
//#endif


#endif // #ifndef LOSEMYMIND_GENERICPLATFORMMACROS_H

