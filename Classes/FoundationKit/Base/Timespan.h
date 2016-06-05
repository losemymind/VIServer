/****************************************************************************
Copyright (c) 2016 libo All rights reserved.

losemymind.libo@gmail.com

****************************************************************************/

#ifndef LOSEMYMIND_TIMESPAN_H
#define LOSEMYMIND_TIMESPAN_H
#pragma once

#include <functional>
#include <string>
#include <chrono>
#include "FoundationKit/GenericPlatformMacros.h"
#include "FoundationKit/Base/Types.h"

NS_FK_BEGIN

namespace ETimespan
{
	/** The number of timespan ticks per millisecond. */
	const int64 TicksPerMillisecond = 10000;
    /** The number of timespan ticks per second. */
    const int64 TicksPerSecond = 10000000;
	/** The number of timespan ticks per minute. */
	const int64 TicksPerMinute = 600000000;
    /** The number of timespan ticks per hour. */
    const int64 TicksPerHour = 36000000000;
    /** The number of timespan ticks per day. */
    const int64 TicksPerDay = 864000000000;
	/** The number of timespan ticks per week. */
	const int64 TicksPerWeek = 6048000000000;
}


/**
 * Implements a time span.
 *
 * A time span is the difference between two dates and times. For example, the time span between
 * 12:00:00 January 1, 2000 and 18:00:00 January 2, 2000 is 30.0 hours. Time spans are measured in
 * positive or negative ticks depending on whether the difference is measured forward or backward.
 * Each tick has a resolution of 0.1 microseconds (= 100 nanoseconds).
 *
 * In conjunction with the companion class FDateTime, time spans can be used to perform date and time
 * based arithmetic, such as calculating the difference between two dates or adding a certain amount
 * of time to a given date.
 *
 * @see DateTime
 */
class Timespan
{
public:

	/** Default constructor (no initialization). */
    Timespan() { }

	/**
	 * Creates and initializes a new time interval with the specified number of ticks.
	 *
	 * @param inTicks The number of ticks.
	 */
    explicit Timespan(int64 inTicks)
        : _ticks(inTicks)
	{ }

	/**
	 * Creates and initializes a new time interval with the specified number of hours, minutes and seconds.
	 *
	 * @param hours   The hours component.
	 * @param minutes The minutes component.
	 * @param seconds The seconds component.
	 */
    Timespan(int32 hours, int32 minutes, int32 seconds)
	{
        assign(0, hours, minutes, seconds, 0);
	}

	/**
	 * Creates and initializes a new time interval with the specified number of days, hours, minutes and seconds.
	 *
	 * @param days    The days component.
	 * @param hours   The hours component.
	 * @param minutes The minutes component.
	 * @param seconds The seconds component.
	 */
    Timespan(int32 days, int32 hours, int32 minutes, int32 seconds)
	{
        assign(days, hours, minutes, seconds, 0);
	}

	/**
	 * Creates and initializes a new time interval with the specified number of days, hours, minutes and seconds.
	 *
	 * @param days         The days component.
	 * @param hours        The hours component.
	 * @param minutes      The minutes component.
	 * @param seconds      The seconds component.
	 * @param milliseconds The milliseconds component.
	 */
    Timespan(int32 days, int32 hours, int32 minutes, int32 seconds, int32 milliseconds)
	{
        assign(days, hours, minutes, seconds, milliseconds);
	}

public:

	/**
	 * Returns result of adding the given time span to this time span.
	 *
	 * @return A time span whose value is the sum of this time span and the given time span.
	 */
    Timespan operator+(const Timespan& other) const
	{
        return Timespan(_ticks + other._ticks);
	}

	/**
	 * Adds the given time span to this time span.
	 *
	 * @return This time span.
	 */
    Timespan& operator+=(const Timespan& other)
	{
        _ticks += other._ticks;
		return *this;
	}

	/**
	 * Returns the inverse of this time span.
	 *
	 * The value of this time span must be greater than Timespan::MinValue(), or else an overflow will occur.
	 *
	 * @return Inverse of this time span.
	 */
    Timespan operator-() const
	{
        return Timespan(-_ticks);
	}

	/**
	 * Returns the result of subtracting the given time span from this time span.
	 *
	 * @param Other The time span to compare with.
	 * @return A time span whose value is the difference of this time span and the given time span.
	 */
    Timespan operator-(const Timespan& other) const
	{
        return Timespan(_ticks - other._ticks);
	}

	/**
	 * Subtracts the given time span from this time span.
	 *
	 * @param Other The time span to subtract.
	 * @return This time span.
	 */
    Timespan& operator-=(const Timespan& other)
	{
        _ticks -= other._ticks;
		return *this;
	}

	/**
	 * Returns the result of multiplying the this time span with the given scalar.
	 *
	 * @param Scalar The scalar to multiply with.
	 * @return A time span whose value is the product of this time span and the given scalar.
	 */
    Timespan operator*(float scalar) const
	{
        return Timespan(static_cast<int64>(_ticks * scalar));
	}

	/**
	 * Multiplies this time span with the given scalar.
	 *
	 * @param Scalar The scalar to multiply with.
	 * @return This time span.
	 */
    Timespan& operator*=(float scalar)
	{
        _ticks = (int64)(_ticks * scalar);
		return *this;
	}

	/**
	 * Compares this time span with the given time span for equality.
	 *
	 * @param Other The time span to compare with.
	 * @return true if the time spans are equal, false otherwise.
	 */
    bool operator==(const Timespan& other) const
	{
        return (_ticks == other._ticks);
	}

	/**
	 * Compares this time span with the given time span for inequality.
	 *
	 * @param Other The time span to compare with.
	 * @return true if the time spans are not equal, false otherwise.
	 */
    bool operator!=(const Timespan& other) const
	{
        return (_ticks != other._ticks);
	}

	/**
	 * Checks whether this time span is greater than the given time span.
	 *
	 * @param Other The time span to compare with.
	 * @return true if this time span is greater, false otherwise.
	 */
    bool operator>(const Timespan& other) const
	{
        return (_ticks > other._ticks);
	}

	/**
	 * Checks whether this time span is greater than or equal to the given time span.
	 *
	 * @param Other The time span to compare with.
	 * @return true if this time span is greater or equal, false otherwise.
	 */
    bool operator>=(const Timespan& other) const
	{
        return (_ticks >= other._ticks);
	}

	/**
	 * Checks whether this time span is less than the given time span.
	 *
	 * @param Other The time span to compare with.
	 * @return true if this time span is less, false otherwise.
	 */
    bool operator<(const Timespan& other) const
	{
        return (_ticks < other._ticks);
	}

	/**
	 * Checks whether this time span is less than or equal to the given time span.
	 *
	 * @param Other The time span to compare with.
	 * @return true if this time span is less or equal, false otherwise.
	 */
	bool operator<=( const Timespan& other ) const
	{
        return (_ticks <= other._ticks);
	}

public:

	/**
	 * Returns a time span with the absolute value of this time span.
	 *
	 * This method may overflow the timespan if its value is equal to MinValue.
	 *
	 * @return Duration of this time span.
	 * @see minValue
	 */
	Timespan getDuration()
	{
        return Timespan(_ticks >= 0 ? _ticks : -_ticks);
	}

   /**
    * Gets the days component of this time span.
    *
    * @return Days component.
    */
    int32 getDays() const
    {
        return static_cast<int32>(_ticks / ETimespan::TicksPerDay);
    }

	/**
	 * Gets the hours component of this time span.
	 *
	 * @return Hours component.
	 * @see getTotalHours
	 */
	int32 getHours() const
	{
        return static_cast<int32>((_ticks / ETimespan::TicksPerHour) % 24);
	}

   /**
    * Gets the minutes component of this time span.
    *
    * @return Minutes component.
    * @see getTotalMinutes
    */
    int32 getMinutes() const
    {
        return static_cast<int32>((_ticks / ETimespan::TicksPerMinute) % 60);
    }

   /**
    * Gets the seconds component of this time span.
    *
    * @return Seconds component.
    * @see getTotalSeconds
    */
    int32 getSeconds() const
    {
        return static_cast<int32>((_ticks / ETimespan::TicksPerSecond) % 60);
    }

	/**
	 * Gets the milliseconds component of this time span.
	 *
	 * @return Milliseconds component.
	 * @see getTotalMilliseconds
	 */
	int32 getMilliseconds() const
	{
        return static_cast<int32>((_ticks / ETimespan::TicksPerMillisecond) % 1000);
	}

	/**
	 * Gets the number of ticks represented by this time span.
	 *
	 * @return Number of ticks.
	 */
	int64 getTicks() const
	{
		return _ticks;
	}

	/**
	 * Gets the total number of days represented by this time span.
	 *
	 * @return Number of days.
	 * @see getDays
	 */
	double getTotalDays() const
	{
        return static_cast<double>(_ticks / ETimespan::TicksPerDay);
	}

	/**
	 * Gets the total number of hours represented by this time span.
	 *
	 * @return Number of hours.
	 * @see getHours
	 */
	double getTotalHours() const
	{
        return static_cast<double>(_ticks / ETimespan::TicksPerHour);
	}

	/**
	 * Gets the total number of milliseconds represented by this time span.
	 *
	 * @return Number of milliseconds.
	 * @see getMilliseconds
	 */
	double getTotalMilliseconds() const
	{
        return static_cast<double>(_ticks / ETimespan::TicksPerMillisecond);
	}

	/**
	 * Gets the total number of minutes represented by this time span.
	 *
	 * @return Number of minutes.
	 * @see getMinutes
	 */
	double getTotalMinutes() const
	{
        return static_cast<double>(_ticks / ETimespan::TicksPerMinute);
	}

	/**
	 * Gets the total number of seconds represented by this time span.
	 *
	 * @return Number of seconds.
	 * @see getSeconds
	 */
	double getTotalSeconds() const
	{
        return static_cast<double>(_ticks / ETimespan::TicksPerSecond);
	}

	/**
	 * Returns the string representation of this time span using a default format.
	 *
	 * The returned string has the following format:
	 *		[-][d.]hh:mm:ss.fff
	 *
	 * @return String representation.
	 * @see parse
	 */
	std::string toString() const;

	/**
	 * Converts this time span to its string representation.
	 *
	 * The following formatting codes are available:
	 *		%n - prints the minus sign (for negative time spans only)
	 *		%N - prints the minus or plus sign (always)
	 *		%d - prints the time span's days part
	 *		%h - prints the time span's hours part (0..23)
	 *		%m - prints the time span's minutes part (0..59)
	 *		%s - prints the time span's seconds part (0..59)
	 *		%f - prints the time span's milliseconds part (0..999)
	 *		%D - prints the total number of days (without minus sign)
	 *		%H - prints the total number of hours (without minus sign)
	 *		%M - prints the total number of minutes (without minus sign)
	 *		%S - prints the total number of seconds (without minus sign)
	 *		%F - prints the total number of milliseconds (without minus sign)
	 *
	 * @param Format - The format of the returned string.
	 * @return String representation.
	 * @see parse
	 */
	 std::string toString( const char* format ) const;

public:

	/**
	 * Creates a time span that represents the specified number of days.
	 *
	 * @param Days The number of days.
	 * @return Time span.
	 * @see fromHours, fromMilliseconds, fromMinutes, fromSeconds
	 */
	static Timespan fromDays( double days );

	/**
	 * Creates a time span that represents the specified number of hours.
	 *
	 * @param Hours The number of hours.
	 * @return Time span.
	 * @see fromDays, fromMilliseconds, fromMinutes, fromSeconds
	 */
	static Timespan fromHours( double hours );

   /**
    * Creates a time span that represents the specified number of minutes.
    *
    * @param Minutes The number of minutes.
    * @return Time span.
    * @see fromDays, fromHours, fromMilliseconds, fromSeconds
    */
    static Timespan fromMinutes(double minutes);

   /**
    * Creates a time span that represents the specified number of seconds.
    *
    * @param Seconds The number of seconds.
    * @return Time span.
    * @see fromDays, fromHours, fromMilliseconds, fromMinutes
    */
    static Timespan fromSeconds(double seconds);

	/**
	 * Creates a time span that represents the specified number of milliseconds.
	 *
	 * @param Milliseconds The number of milliseconds.
	 * @return Time span.
	 * @see fromDays, fromHours, fromMinutes, fromSeconds
	 */
	static Timespan fromMilliseconds( double milliseconds );

	/**
	 * Returns the maximum time span value.
	 *
	 * The maximum time span value is slightly more than 10,675,199 days.
	 *
	 * @return Maximum time span.
	 * @see minValue,Zero
	 */
	static Timespan maxValue()
	{
		return Timespan(9223372036854775807);
	}

	/**
	 * Returns the minimum time span value.
	 *
	 * The minimum time span value is slightly less than -10,675,199 days.
	 *
	 * @return Minimum time span.
	 * @see maxValue, zeroValue
	 */
	static Timespan minValue()
	{
		return Timespan(-9223372036854775807 - 1);
	}

	/**
	 * Converts a string to a time span.
	 *
	 * Currently, the string must be in the format written by Timespan.ToString().
	 * Other formats are not supported at this time.
	 *
	 * @param TimespanString The string to convert.
	 * @param OutTimespan Will contain the parsed time span.
	 * @return true if the string was converted successfully, false otherwise.
	 * @see ToString
	 */
	static bool parse( const std::string& timespanString, Timespan& outTimespan );

	/**
	 * Returns the zero time span value.
	 *
	 * The zero time span value can be used in comparison operations with other time spans.
	 *
	 * @return Zero time span.
	 * @see MaxValue, MinValue
	 */
	static Timespan zero()
	{
		return Timespan(0);
	}

	/**
	 * Gets the hash for the specified time span.
	 *
	 * @param Timespan The timespan to get the hash for.
	 * @return Hash value.
	 */
	size_t getHash()
	{
        return std::hash<int64>()(this->_ticks);
	}

protected:

	/**
	 * Assigns the specified components to this time span.
	 *
	 * @param days         The days component.
	 * @param hours        The hours component.
	 * @param minutes      The minutes component.
	 * @param seconds      The seconds component.
	 * @param milliseconds The milliseconds component.
	 */
	void assign( int32 days, int32 hours, int32 minutes, int32 seconds, int32 milliseconds );

private:

	/** Holds the time span in 100 nanoseconds resolution. */
	int64 _ticks;
};


/**
 * Pre-multiplies a time span with the given scalar.
 *
 * @param Scalar The scalar to pre-multiply with.
 * @param Timespan The time span to multiply.
 */
inline Timespan operator*( float scalar, const Timespan& timespan )
{
    return timespan.operator*(scalar);
}


NS_FK_END



#endif // LOSEMYMIND_TIMESPAN_H
