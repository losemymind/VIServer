/****************************************************************************
Copyright (c) 2016 libo All rights reserved.

losemymind.libo@gmail.com

****************************************************************************/
#include "TimeEx.h"
NS_FK_BEGIN

	Time::Time()
		: m_TimeDuration()
	{

	}

	Time::Time(const Time & t)
		: m_TimeDuration(t.m_TimeDuration)
	{

	}

	Time::Time(Time && t)
		: m_TimeDuration(t.m_TimeDuration)
	{

	}

    Time::Time(const std::chrono::high_resolution_clock::time_point& timePoint)
        : m_TimeDuration(timePoint.time_since_epoch())
    {

    }

	Time::~Time()
	{

	}

	Time & Time::operator=(const Time & t)
	{
		m_TimeDuration = t.m_TimeDuration;
		return * this;
	}

	Time & Time::operator=(Time && t)
	{
		m_TimeDuration = t.m_TimeDuration;
		return * this;
	}

	Time & Time::operator+=(const Time & t)
	{
		m_TimeDuration += t.m_TimeDuration;
		return * this;
	}

	Time & Time::operator-=(const Time & t)
	{
		m_TimeDuration -= t.m_TimeDuration;
		return * this;
	}

	Time & Time::operator*=(const std::chrono::high_resolution_clock::duration::rep & t)
	{
		m_TimeDuration *= t;
		return * this;
	}

	Time & Time::operator/=(const std::chrono::high_resolution_clock::duration::rep & t)
	{
		m_TimeDuration /= t;
		return * this;
	}

	Time & Time::operator%=(const std::chrono::high_resolution_clock::duration::rep & t)
	{
		m_TimeDuration %= t;
		return * this;
	}

	Time & Time::operator%=(const Time & t)
	{
		m_TimeDuration %= t.m_TimeDuration;
		return * this;
	}

	Time Time::operator+(const Time & t) const
	{
		Time time(*this);
		time += t;
		return time;
	}

	Time Time::operator-(const Time & t) const
	{
		Time time(*this);
		time -= t;
		return time;
	}

	Time & Time::operator++()
	{
		++m_TimeDuration;
		return * this;
	}

	Time Time::operator++(int32)
	{
		Time time(*this);
		time.m_TimeDuration++;
		return time;
	}

	Time & Time::operator--()
	{
		--m_TimeDuration;
		return * this;
	}

	Time Time::operator--(int32)
	{
		Time time(*this);
		time.m_TimeDuration--;
		return time;
	}

    double Time::hours() const
	{
        //return (std::chrono::duration_cast<std::chrono::hours> (m_TimeDuration).count());
        return (std::chrono::duration_cast<hours_f> (m_TimeDuration).count());
	}

    double Time::minutes() const
	{
        //return (std::chrono::duration_cast<std::chrono::minutes> (m_TimeDuration).count());
        return (std::chrono::duration_cast<minutes_f> (m_TimeDuration).count());
	}

    double Time::seconds() const
	{
        //return (std::chrono::duration_cast<std::chrono::seconds> (m_TimeDuration).count());
        return (std::chrono::duration_cast<seconds_f> (m_TimeDuration).count());
	}

    double Time::milliseconds() const
	{
        //return (std::chrono::duration_cast<std::chrono::milliseconds> (m_TimeDuration).count());
        return (std::chrono::duration_cast<milliseconds_f> (m_TimeDuration).count());
	}

    double Time::microseconds() const
	{
        //return (std::chrono::duration_cast<std::chrono::microseconds> (m_TimeDuration).count());
        return (std::chrono::duration_cast<microseconds_f> (m_TimeDuration).count());
	}

    double Time::nanoseconds() const
	{
        //return (std::chrono::duration_cast<std::chrono::nanoseconds> (m_TimeDuration).count());
        return (std::chrono::duration_cast<nanoseconds_f> (m_TimeDuration).count());
	}

    Time Time::now()
    {
        return Time(std::chrono::high_resolution_clock::now());
    }

NS_FK_END




