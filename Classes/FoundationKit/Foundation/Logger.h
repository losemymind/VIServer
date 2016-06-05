/****************************************************************************
  Copyright (c) 2015 libo All rights reserved.

  losemymind.libo@gmail.com

****************************************************************************/
#pragma once
#include "Singleton.h"

NS_FK_BEGIN

class Logger : public Singleton<Logger>
{
    friend class Singleton < Logger >;
    Logger(){};
public:
    /** 
     * Enumeration of valid log levels.
     */
    enum Level
    {
        LV_TRACE = 0,
        LV_DEBUG,
        LV_INFO,
        LV_WARN,
        LV_ERROR,
        LV_FATAL,
    };
private:
    struct State
    {
        State();
        bool enabled;
    };

public:
    /**
     * Logs a message at the specified log level.
     *
     * This method accepts a variable argument list with the same formatting specification
     * as printf. Therefore, the message parameter can include any format specifiers that
     * are supported by printf.
     *
     * @param level Log level.
     * @param message Log message.
     */
    void log(Level level, const char* message, ...);

    /**
     * Determines if logging is currently enabled for the given level.
     *
     * @param level Log level.
     *
     * @return True if logging is enabled for this level, or false if it is disabled.
     */
    bool isEnabled(Level level);

    /**
     * Enables or disables logging at the given level.
     *
     * @param level Log level to enable or disable.
     * @param enabled True to enable the logger for the given level, false to disable it.
     */
    void setEnabled(Level level, bool enabled);
private:
    State _states[6];
};
NS_FK_END

extern void _log_(const char* message, ...);

#ifdef FK_DEBUG
#define LOG_DEBUG(format, ...) Logger::getInstance()->log(Logger::Level::LV_DEBUG, format, ##__VA_ARGS__)
#else
#define LOG_DEBUG(format, ...)
#endif
#define LOG_TRACE(format, ...) Logger::getInstance()->log(Logger::Level::LV_TRACE, format, ##__VA_ARGS__)
#define LOG_INFO( format, ...) Logger::getInstance()->log(Logger::Level::LV_INFO, format, ##__VA_ARGS__)
#define LOG_WARN( format, ...) Logger::getInstance()->log(Logger::Level::LV_WARN, format, ##__VA_ARGS__)
#define LOG_ERROR(format, ...) Logger::getInstance()->log(Logger::Level::LV_ERROR, format, ##__VA_ARGS__)
#define LOG_FATAL(format, ...) Logger::getInstance()->log(Logger::Level::LV_FATAL, format, ##__VA_ARGS__)


