/****************************************************************************
Copyright (c) 2016 libo All rights reserved.

losemymind.libo@gmail.com

****************************************************************************/
#ifndef LOSEMYMIND_TIME_H
#define LOSEMYMIND_TIME_H

#pragma once
#include "FoundationKit/GenericPlatformMacros.h"
#include "FoundationKit/Base/Types.h"
#include <chrono>
NS_FK_BEGIN

typedef std::chrono::duration<double, std::nano> nanoseconds_f;
typedef std::chrono::duration<double, std::micro> microseconds_f;
typedef std::chrono::duration<double, std::milli> milliseconds_f;
typedef std::chrono::duration<double> seconds_f;
typedef std::chrono::duration<double, std::ratio<60> > minutes_f;
typedef std::chrono::duration<double, std::ratio<3600> > hours_f;

class Time final
{
public:
    Time();
    Time(const Time & t);
	Time(Time && t);
    Time(const std::chrono::high_resolution_clock::time_point& timePoint);

	~Time();
	
    static Time now();
	Time & operator=(const Time & t);
	Time & operator=(Time && t);
	Time & operator+=(const Time & t);
	Time & operator-=(const Time & t);
	Time & operator*=(const std::chrono::high_resolution_clock::duration::rep & t);
	Time & operator/=(const std::chrono::high_resolution_clock::duration::rep & t);
	Time & operator%=(const std::chrono::high_resolution_clock::duration::rep & t);
	Time & operator%=(const Time & t);
	Time & operator++();
	Time & operator--();
    Time   operator+(const Time & t) const;
    Time   operator-(const Time & t) const;
    Time   operator++(int32 t);
	Time   operator--(int32 t);
    double hours() const;
    double minutes() const;
    double seconds() const;
    double milliseconds() const;
    double microseconds() const;
    double nanoseconds() const;
private:
	std::chrono::high_resolution_clock::duration m_TimeDuration;

};

NS_FK_END



#endif // LOSEMYMIND_TIME_H
