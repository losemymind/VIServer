/****************************************************************************
Copyright (c) 2016 libo All rights reserved.

losemymind.libo@gmail.com

****************************************************************************/
#ifndef LOSEMYMIND_DATETIME_H
#define LOSEMYMIND_DATETIME_H

#pragma once
#include <string>
#include "FoundationKit/GenericPlatformMacros.h"
#include "FoundationKit/Base/Types.h"
#include "Timespan.h"

NS_FK_BEGIN

/**
 * Enumerates the days of the week in 7-day calendars.
 */
enum class EDayOfWeek
{
	Monday = 0,
	Tuesday,
	Wednesday,
	Thursday,
	Friday,
	Saturday,
	Sunday
};


/**
 * Enumerates the months of the year in 12-month calendars.
 */
enum class EMonthOfYear
{
	January = 1,
	February,
	March,
	April,
	May,
	June,
	July,
	August,
	September,
	October,
	November,
	December
};


/**
 * Implements a date and time.
 *
 * Values of this type represent dates and times between Midnight 00:00:00, January 1, 0001 and
 * Midnight 23:59:59.9999999, December 31, 9999 in the Gregorian calendar. Internally, the time
 * values are stored in ticks of 0.1 microseconds (= 100 nanoseconds) since January 1, 0001.
 *
 * To retrieve the current local date and time, use the DateTime.Now() method. To retrieve the
 * current UTC time, use the DateTime.UtcNow() method instead.
 *
 * This class also provides methods to convert dates and times from and to string representations,
 * calculate the number of days in a given month and year, check for leap years and determine the
 * time of day, day of week and month of year of a given date and time.
 *
 * The companion struct Timespan is provided for enabling date and time based arithmetic, such as
 * calculating the difference between two dates or adding a certain amount of time to a given date.
 *
 * Ranges of dates and times can be represented by the FDateRange class.
 *
 * @see Timespan
 */
class DateTime
{
public:

	/** Default constructor (no initialization). */
	DateTime() { }

	/**
	 * Creates and initializes a new instance with the specified number of ticks.
	 *
	 * @param Ticks The ticks representing the date and time.
	 */
	explicit DateTime( int64 inTicks )
        : _ticks(inTicks)
	{ }

	/**
	 * Creates and initializes a new instance with the specified year, month, day, hour, minute, second and millisecond.
	 *
	 * @param year The year.
	 * @param month The month.
	 * @param day The day.
	 * @param hour The hour (optional).
	 * @param minute The minute (optional).
	 * @param second The second (optional).
	 * @param millisecond The millisecond (optional).
	 */
	DateTime( int32 year, int32 month, int32 day, int32 hour = 0, int32 minute = 0, int32 second = 0, int32 millisecond = 0 );

public:

	/**
	 * Returns result of adding the given time span to this date.
	 *
	 * @return A date whose value is the sum of this date and the given time span.
	 * @see Timespan
	 */
	DateTime operator+( const Timespan& other ) const
	{
        return DateTime(_ticks + other.getTicks());
	}

	/**
	 * Adds the given time span to this date.
	 *
	 * @return This date.
	 * @see Timespan
	 */
    DateTime& operator+=(const Timespan& other)
	{
        _ticks += other.getTicks();
		return *this;
	}

	/**
	 * Returns time span between this date and the given date.
	 *
	 * @return A time span whose value is the difference of this date and the given date.
	 * @see Timespan
	 */
    Timespan operator-(const DateTime& other) const
	{
        return Timespan(_ticks - other.getTicks());
	}

	/**
	 * Returns result of subtracting the given time span from this date.
	 *
	 * @return A date whose value is the difference of this date and the given time span.
	 * @see Timespan
	 */
    DateTime operator-(const Timespan& other) const
	{
		return DateTime(_ticks - other.getTicks());
	}

	/**
	 * Subtracts the given time span from this date.
	 *
	 * @return This date.
	 * @see Timespan
	 */
    DateTime& operator-=(const Timespan& other)
	{
		_ticks -= other.getTicks();
		return *this;
	}

	/**
	 * Compares this date with the given date for equality.
	 *
	 * @param other The date to compare with.
	 * @return true if the dates are equal, false otherwise.
	 */
    bool operator==(const DateTime& other) const
	{
        return (_ticks == other._ticks);
	}

	/**
	 * Compares this date with the given date for inequality.
	 *
	 * @param other The date to compare with.
	 * @return true if the dates are not equal, false otherwise.
	 */
	bool operator!=( const DateTime& Other ) const
	{
		return (_ticks != Other._ticks);
	}

	/**
	 * Checks whether this date is greater than the given date.
	 *
	 * @param other The date to compare with.
	 * @return true if this date is greater, false otherwise.
	 */
	bool operator>( const DateTime& Other ) const
	{
		return (_ticks > Other._ticks);
	}

	/**
	 * Checks whether this date is greater than or equal to the date span.
	 *
	 * @param other The date to compare with.
	 * @return true if this date is greater or equal, false otherwise.
	 */
	bool operator>=( const DateTime& Other ) const
	{
		return (_ticks >= Other._ticks);
	}

	/**
	 * Checks whether this date is less than the given date.
	 *
	 * @param other The date to compare with.
	 * @return true if this date is less, false otherwise.
	 */
	bool operator<( const DateTime& Other ) const
	{
		return (_ticks < Other._ticks);
	}

	/**
	 * Checks whether this date is less than or equal to the given date.
	 *
	 * @param other The date to compare with.
	 * @return true if this date is less or equal, false otherwise.
	 */
	bool operator<=( const DateTime& Other ) const
	{
		return (_ticks <= Other._ticks);
	}

public:

	/**
	 * Gets the date part of this date.
	 *
	 * The time part is truncated and becomes 00:00:00.000.
	 *
	 * @return A DateTime object containing the date.
	 */
	DateTime getDate() const
	{
		return DateTime(_ticks - (_ticks % ETimespan::TicksPerDay));
	}

	/**
	 * Gets the date components of this date.
	 *
	 * @param outYear  Will contain the year.
	 * @param outMonth Will contain the number of the month (1-12).
	 * @param outDay   Will contain the number of the day (1-31).
	 */
	void getDate( int32& outYear, int32& outMonth, int32& outDay ) const;

	/**
	 * Gets this date's day part (1 to 31).
	 *
	 * @return Day of the month.
	 * @see getHour, getHour12, getMillisecond, getMinute, getMonth, getSecond, getYear
	 */
	int32 getDay() const;

	/**
	 * Calculates this date's day of the week (Sunday - Saturday).
	 *
	 * @return The week day.
	 * @see getDayOfYear, getMonthOfYear, getTimeOfDay
	 */
	EDayOfWeek getDayOfWeek() const;

	/**
	 * Gets this date's day of the year.
	 *
	 * @return The day of year.
	 * @see getDayOfWeek, getMonthOfYear, getTimeOfDay
	 */
	int32 getDayOfYear() const;

	/**
	 * Gets this date's hour part in 24-hour clock format (0 to 23).
	 *
	 * @return The hour.
	 * @see getDay, getDayOfWeek, getDayOfYear, getHour12, getMillisecond, getMinute, getMonth, getSecond, getYear
	 */
	int32 getHour() const
	{
		return (int32)((_ticks / ETimespan::TicksPerHour) % 24);
	}

	/**
	 * Gets this date's hour part in 12-hour clock format (1 to 12).
	 *
	 * @return The hour in AM/PM format.
	 * @see getDay, getHour, getMillisecond, getMinute, getMonth, getSecond, getYear
	 */
	int32 getHour12() const;

	/**
	 * Returns the Julian Day for this date.
	 *
	 * The Julian Day is the number of days since the inception of the Julian calendar at noon on
	 * Monday, January 1, 4713 B.C.E. The minimum Julian Day that can be represented in DateTime is
	 * 1721425.5, which corresponds to Monday, January 1, 0001 in the Gregorian calendar.
	 *
	 * @return Julian Day.
	 * @see fromJulianDay, getModifiedJulianDay
	 */
	double getJulianDay() const
	{
		return (double)(1721425.5 + _ticks / ETimespan::TicksPerDay);
	}

	/**
	 * Returns the Modified Julian day.
	 *
	 * The Modified Julian Day is calculated by subtracting 2400000.5, which corresponds to midnight UTC on
	 * November 17, 1858 in the Gregorian calendar.
	 *
	 * @return Modified Julian Day
	 * @see getJulianDay
	 */
	double getModifiedJulianDay() const
	{
		return (getJulianDay() - 2400000.5);
	}

	/**
	 * Gets this date's millisecond part (0 to 999).
	 *
	 * @return The millisecond.
	 * @see getDay, getHour, getHour12, getMinute, getMonth, getSecond, getYear
	 */
	int32 getMillisecond() const
	{
		return (int32)((_ticks / ETimespan::TicksPerMillisecond) % 1000);
	}

	/**
	 * Gets this date's minute part (0 to 59).
	 *
	 * @return The minute.
	 * @see getDay, getHour, getHour12, getMillisecond, getMonth, getSecond, getYear
	 */
	int32 getMinute() const
	{
		return (int32)((_ticks / ETimespan::TicksPerMinute) % 60);
	}

	/**
	 * Gets this date's the month part (1 to 12).
	 *
	 * @return The month.
	 * @see getDay, getHour, getHour12, getMillisecond, getMinute, getSecond, getYear
	 */
	int32 getMonth() const;

	/**
	 * Gets the date's month of the year (January to December).
	 *
	 * @return Month of year.
	 * @see getDayOfWeek, getDayOfYear, getTimeOfDay
	 */
	EMonthOfYear getMonthOfYear() const
	{
		return static_cast<EMonthOfYear>(getMonth());
	}

	/**
	 * Gets this date's second part.
	 *
	 * @return The second.
	 * @see getDay, getHour, getHour12, getMillisecond, getMinute, getMonth, getYear
	 */
	int32 getSecond() const
	{
		return (int32)((_ticks / ETimespan::TicksPerSecond) % 60);
	}

	/**
	 * Gets this date's representation as number of ticks.
	 *
	 * @return Number of ticks since midnight, January 1, 0001.
	 */
	int64 getTicks() const
	{
		return _ticks;
	}

	/**
	 * Gets the time elapsed since midnight of this date.
	 *
	 * @param Time of day since midnight.
	 * @see getDayOfWeek, getDayOfYear, getMonthOfYear
	 */
	Timespan getTimeOfDay() const
	{
		return Timespan(_ticks % ETimespan::TicksPerDay);
	}

	/**
	 * Gets this date's year part.
	 *
	 * @return The year.
	 * @see GetDay, GetHour, GetHour12, GetMillisecond, GetMinute, GetMonth, GetSecond
	 */
	int32 getYear() const;

	/**
	 * Gets whether this date's time is in the afternoon.
	 *
	 * @param true if it is in the afternoon, false otherwise.
	 * @see isMorning
	 */
	bool isAfternoon() const
	{
		return (getHour() >= 12);
	}

	/**
	 * Gets whether this date's time is in the morning.
	 *
	 * @param true if it is in the morning, false otherwise.
	 * @see isAfternoon
	 */
	bool isMorning() const
	{
		return (getHour() < 12);
	}

	/**
	 * Returns the ISO-8601 string representation of the DateTime.
	 *
	 * The resulting string assumes that the DateTime is in UTC.
	 * 
	 * @return String representation.
	 * @see parseIso8601, toString
	 */
    std::string toIso8601() const;

	/**
	 * Returns the string representation of this date using a default format.
	 *
	 * The returned string has the following format:
	 *		yyyy.mm.dd-hh.mm.ss
	 *
	 * @return String representation.
	 * @see parse, toIso8601
	 */
    std::string toString() const;

	/**
	 * Returns the string representation of this date.
	 *
	 * @param format The format of the returned string.
	 * @return String representation.
	 * @see parse, toIso8601
	 */
	std::string toString( const char* format ) const;

	/**
	 * Returns this date as the number of seconds since the Unix Epoch (January 1st of 1970).
	 *
	 * @return Time of day.
	 * @see fromUnixTimestamp
	 */
	int64 toUnixTimestamp() const
	{
		return (_ticks - DateTime(1970, 1, 1)._ticks) / ETimespan::TicksPerSecond;
	}

public:

	/**
	 * Gets the number of days in the year and month.
	 *
	 * @param year The year.
	 * @param month The month.
	 * @return The number of days
	 * @see daysInYear
	 */
	static int32 daysInMonth( int32 year, int32 month );

	/**
	 * Gets the number of days in the given year.
	 *
	 * @param year The year.
	 * @return The number of days.
	 * @see daysInMonth
	 */
	static int32 daysInYear( int32 year );

	/**
	 * Returns the proleptic Gregorian date for the given Julian Day.
	 *
	 * @param JulianDay The Julian Day.
	 * @return Gregorian date and time.
	 * @see getJulianDay
	 */
	static DateTime fromJulianDay( double julianDay )
	{
        return DateTime((int64)((julianDay - 1721425.5) * ETimespan::TicksPerDay));
	}

	/**
	 * Returns the date from Unix time (seconds from midnight 1970-01-01)
	 *
	 * @param UnixTime Unix time (seconds from midnight 1970-01-01)
	 * @return Gregorian date and time.
	 * @see toUnixTimestamp
	 */
	static DateTime fromUnixTimestamp( int64 unixTime )
	{
        return DateTime(1970, 1, 1) + Timespan(unixTime * ETimespan::TicksPerSecond);
	}

	/**
	 * Checks whether the given year is a leap year.
	 *
	 * A leap year is a year containing one additional day in order to keep the calendar synchronized
	 * with the astronomical year. All years divisible by 4, but not divisible by 100 - except if they
	 * are also divisible by 400 - are leap years.
	 *
	 * @param year The year to check.
	 * @return true if the year is a leap year, false otherwise.
	 */
	static bool isLeapYear( int32 year );

	/**
	 * Returns the maximum date value.
	 *
	 * The maximum date value is December 31, 9999, 23:59:59.9999999.
	 *
	 * @see minValue
	 */
	static DateTime maxValue()
	{
		return DateTime(3652059 * ETimespan::TicksPerDay - 1);
	}

	/**
	 * Returns the minimum date value.
	 *
	 * The minimum date value is January 1, 0001, 00:00:00.0.
	 *
	 * @see maxValue
	 */
	static DateTime minValue()
	{
		return DateTime(0);
	}

	/**
	 * Gets the local date and time on this computer.
	 *
	 * This method takes into account the local computer's time zone and daylight saving
	 * settings. For time zone independent time comparisons, and when comparing times
	 * between different computers, please use UtcNow() instead.
	 *
	 * @return Current date and time.
	 * @see Today, utcNow
	 */
	static DateTime now();

    /**
     * Gets the UTC date and time on this computer.
     *
     * This method returns the Coordinated Universal Time (UTC), which does not take the
     * local computer's time zone and daylight savings settings into account. It should be
     * used when comparing dates and times that should be independent of the user's locale.
     * To get the date and time in the current locale, use Now() instead.
     *
     * @return Current date and time.
     * @see Now
     */
    static DateTime utcNow();


	/**
	 * Converts a string to a date and time.
	 *
	 * Currently, the string must be in the format written by either DateTime.ToString() or
	 * FTimeStamp.TimestampToFString(). Other formats are not supported at this time.
	 *
	 * @param dateTimeString The string to convert.
	 * @param outDateTime Will contain the parsed date and time.
	 * @return true if the string was converted successfully, false otherwise.
	 */
	static bool parse( const std::string& dateTimeString, DateTime& outDateTime );

	/**
	 * Parses a date string in ISO-8601 format.
	 * 
	 * @param dateTimeString The string to be parsed
	 * @param outDateTime DateTime object (in UTC) corresponding to the input string (which may have been in any timezone).
	 * @return true if the string was converted successfully, false otherwise.
	 * @see Parse, ToIso8601
	 */
	static bool parseIso8601( const char* dateTimeString, DateTime& outDateTime );

	/**
	 * Gets the local date on this computer.
	 *
	 * The time component is set to 00:00:00
	 *
	 * @return Current date.
	 * @see Now, UtcNow
	 */
	static DateTime today()
	{
		return now().getDate();
	}

	/**
	 * Validates the given components of a date and time value.
	 *
	 * The allow ranges for the components are:
	 *		year: 1 - 9999
	 *		month: 1 - 12
	 *		day: 1 - daysInMonth(month)
	 *		hour: 0 - 23
	 *		minute: 0 - 59
	 *		second: 0 - 59
	 *		millisecond: 0 - 999
	 *
	 * @return true if the components are valid, false otherwise.
	 */
	static bool validate( int32 year, int32 month, int32 day, int32 hour, int32 minute, int32 second, int32 millisecond );

public:

	/**
	 * Gets the hash for the specified date and time.
	 *
	 * @param dateTime The date and time to get the hash for.
	 * @return Hash value.
	 */
    size_t getHash()
	{
        return std::hash<int64>()(this->_ticks);
	}

protected:

	/** Holds the days per month in a non-leap year. */
	static const int32 DaysPerMonth[];

	/** Holds the cumulative days per month in a non-leap year. */
	static const int32 DaysToMonth[];

private:

	/** Holds the ticks in 100 nanoseconds resolution since January 1, 0001 A.D. */
	int64 _ticks;
};


NS_FK_END


#endif // LOSEMYMIND_DATETIME_H


