#pragma once
#include <chrono>
#include "FoundationKit/GenericPlatformMacros.h"
#include "FoundationKit/Base/TimeEx.h"
NS_FK_BEGIN

class Timer
{
public:
    typedef std::chrono::high_resolution_clock clock_type;
    Timer() : _begin(clock_type::now()){}

    void reset()
    {
        _begin = clock_type::now();
    }

    double nanoseconds()const
    {
        return elapsed<nanoseconds_f>();
    }

    double microseconds()const
    {
        return elapsed<microseconds_f>();
    }

    double milliseconds()const
    {
        return elapsed<milliseconds_f>();
    }

    double seconds()const
    {
        return elapsed<seconds_f>();
    }

    double minutes()const
    {
        return elapsed<minutes_f>();
    }

    double hours()const
    {
        return elapsed<hours_f>();
    }

    double days()
    {
        return elapsed< std::chrono::duration<double, std::ratio<3600 * 24> >>();
    }
protected:
    template<typename Duration = milliseconds_f>
    double elapsed()const
    {
        return std::chrono::duration_cast<Duration>(clock_type::now() - _begin).count();
    }
private:
    //std::chrono::time_point<clock_type>   _begin;
    clock_type::time_point _begin;
};

NS_FK_END