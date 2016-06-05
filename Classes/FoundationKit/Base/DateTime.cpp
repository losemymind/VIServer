/****************************************************************************
Copyright (c) 2016 libo All rights reserved.

losemymind.libo@gmail.com

****************************************************************************/
#include <vector>
#include <cstdlib>
#include "FoundationKit/Foundation/StringUtils.h"
#include "FoundationKit/Base/MathEx.h"
#include "FoundationKit/Platform/Platform.h"
#include "DateTime.h"

NS_FK_BEGIN

/* DateTime constants
 *****************************************************************************/

const int32 DateTime::DaysPerMonth[] = { 0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };
const int32 DateTime::DaysToMonth[] = { 0, 31, 59, 90, 120, 151, 181, 212, 243, 273, 304, 334, 365 };


/* DateTime structors
 *****************************************************************************/

DateTime::DateTime( int32 year, int32 month, int32 day, int32 hour, int32 minute, int32 second, int32 millisecond )
{
    LOG_ASSERT(validate(year, month, day, hour, minute, second, millisecond), "Input date time is invalid.");

	int32 totalDays = 0;

    if ((month > 2) && isLeapYear(year))
	{
        ++totalDays;
	}

    year--;											// the current year is not a full year yet
    month--;										// the current month is not a full month yet

    totalDays += year * 365;
    totalDays += year / 4;							// leap year day every four years...
    totalDays -= year / 100;						// ...except every 100 years...
    totalDays += year / 400;						// ...but also every 400 years
    totalDays += DaysToMonth[month];				// days in this year up to last month
    totalDays += day - 1;							// days in this month minus today

    _ticks = totalDays   * ETimespan::TicksPerDay
           + hour        * ETimespan::TicksPerHour
           + minute      * ETimespan::TicksPerMinute
           + second      * ETimespan::TicksPerSecond
           + millisecond * ETimespan::TicksPerMillisecond;
}


void DateTime::getDate( int32& outYear, int32& outMonth, int32& outDay ) const
{
	int32 i, j, k, l, n;
    l = MathEx::floorToInt(static_cast<float>(getJulianDay() + 0.5)) + 68569;
	n = 4 * l / 146097;
	l = l - (146097 * n + 3) / 4;
	i = 4000 * (l + 1) / 1461001;
	l = l - 1461 * i / 4 + 31;
	j = 80 * l / 2447;
	k = l - 2447 * j / 80;
	l = j / 11;
	j = j + 2 - 12 * l;
	i = 100 * (n - 49) + i + l;

	outYear = i;
	outMonth = j;
	outDay = k;
}


int32 DateTime::getDay() const
{
	int32 year, month, day;
    getDate(year, month, day);
    return day;
}


EDayOfWeek DateTime::getDayOfWeek() const
{
	// January 1, 0001 was a Monday
	return static_cast<EDayOfWeek>((_ticks / ETimespan::TicksPerDay) % 7);
}


int32 DateTime::getDayOfYear() const
{
    int32 year, month, day;
    getDate(year, month, day);
    for (int32 currentMonth = 1; currentMonth < month; ++currentMonth)
	{
        day += daysInMonth(year, currentMonth);
	}
    return day;
}


int32 DateTime::getHour12() const
{
	int32 hour = getHour();
    if (hour < 1)
	{
		return 12;
	}
    if (hour > 12)
	{
        return (hour - 12);
	}
    return hour;
}


int32 DateTime::getMonth() const
{
    int32 year, month, day;
    getDate(year, month, day);
    return month;
}


int32 DateTime::getYear() const
{
    int32 year, month, day;
    getDate(year, month, day);
    return year;
}


std::string DateTime::toIso8601() const
{
	return toString("%Y-%m-%dT%H:%M:%S.%sZ");
}


std::string DateTime::toString() const
{
	return toString("%Y.%m.%d-%H.%M.%S");
}


std::string DateTime::toString( const char* format ) const
{
    std::string result;

    if (format != nullptr)
	{
        while (*format != '\0')
		{
            if ((*format == '%') && (*(++format) != '\0'))
			{
                switch (*format)
				{
                case 'a': result += isMorning() ? "am" : "pm"; break;
                case 'A': result += isMorning() ? "AM" : "PM"; break;
                case 'd': result += StringUtils::format("%02i", getDay()); break;
                case 'D': result += StringUtils::format("%03i", getDayOfYear()); break;
                case 'm': result += StringUtils::format("%02i", getMonth()); break;
                case 'y': result += StringUtils::format("%02i", getYear() % 100); break;
                case 'Y': result += StringUtils::format("%04i", getYear()); break;
                case 'h': result += StringUtils::format("%02i", getHour12()); break;
                case 'H': result += StringUtils::format("%02i", getHour()); break;
                case 'M': result += StringUtils::format("%02i", getMinute()); break;
                case 'S': result += StringUtils::format("%02i", getSecond()); break;
                case 's': result += StringUtils::format("%03i", getMillisecond()); break;
                default:		 result += *format;
				}
			}
			else
			{
                result += *format;
			}

			// move to the next one
            format++;
		}
	}

    return result;
}


/* DateTime static interface
 *****************************************************************************/

int32 DateTime::daysInMonth( int32 year, int32 month )
{
    LOG_ASSERT((month >= 1) && (month <= 12), "The month param is invaild.");
    if ((month == 2) && isLeapYear(year))
	{
		return 29;
	}
    return DaysPerMonth[month];
}


int32 DateTime::daysInYear( int32 year )
{
    if (isLeapYear(year))
	{
		return 366;
	}
	return 365;
}


bool DateTime::isLeapYear( int32 year )
{
    if ((year % 4) == 0)
	{
        return (((year % 100) != 0) || ((year % 400) == 0));
	}
	return false;
}


DateTime DateTime::now()
{
	int32 year, month, day, dayOfWeek;
	int32 hour, minute, second, millisecond;
    Platform::systemTime(year, month, dayOfWeek, day, hour, minute, second, millisecond);
    return DateTime(year, month, day, hour, minute, second, millisecond);
}

DateTime DateTime::utcNow()
{
    int32 year, month, day, dayOfWeek;
    int32 hour, minute, second, millisecond;
    Platform::utcTime(year, month, dayOfWeek, day, hour, minute, second, millisecond);
    return DateTime(year, month, day, hour, minute, second, millisecond);
}

bool DateTime::parse( const std::string& dateTimeString, DateTime& outDateTime )
{
	// first replace -, : and . with space
    std::string fixedString = dateTimeString;
    std::replace(fixedString.begin(), fixedString.end(), '-', ' ');
    std::replace(fixedString.begin(), fixedString.end(), ':', ' ');
    std::replace(fixedString.begin(), fixedString.end(), '.', ' ');

	std::vector<std::string> tokens;

	// split up on a single delimiter
    StringUtils::split(fixedString, ' ', tokens);
	// make sure it parsed it properly (within reason)
    if ((tokens.size() < 6) || (tokens.size() > 7))
	{
		return false;
	}

    const int32 year = std::atoi(tokens[0].c_str());
    const int32 month = std::atoi(tokens[1].c_str());
    const int32 day = std::atoi(tokens[2].c_str());
    const int32 hour = std::atoi(tokens[3].c_str());
    const int32 minute = std::atoi(tokens[4].c_str());
    const int32 second = std::atoi(tokens[5].c_str());
    const int32 millisecond = tokens.size() > 6 ? std::atoi(tokens[6].c_str()) : 0;

    if (!validate(year, month, day, hour, minute, second, millisecond))
	{
		return false;
	}

	// convert the tokens to numbers
    outDateTime._ticks = DateTime(year, month, day, hour, minute, second, millisecond)._ticks;

	return true;
}


bool DateTime::parseIso8601( const char* dateTimeString, DateTime& outDateTime )
{
	// DateOnly: YYYY-MM-DD
	// DateTime: YYYY-mm-ddTHH:MM:SS(.ssss)(Z|+th:tm|-th:tm)

    const char* dtPtr = dateTimeString;
	char* next = nullptr;

	int32 year = 0, month = 0, day = 0;
	int32 hour = 0, minute = 0, second = 0, millisecond = 0;
	int32 tzHour = 0, tzMinute = 0;

	// get date
    year = static_cast<int32>(std::strtol(dtPtr, &next, 10));

    if ((next <= dtPtr) || (*next == '\0'))
	{
		return false;
	}

    dtPtr = next + 1; // skip separator
    month = static_cast<int32>(std::strtol(dtPtr, &next, 10));

    if ((next <= dtPtr) || (*next == '\0'))
	{
		return false;
	}

    dtPtr = next + 1; // skip separator
    day = static_cast<int32>(std::strtol(dtPtr, &next, 10));

    if (next <= dtPtr)
	{
		return false;
	}

	// check whether this is date and time
    if (*next == 'T')
	{
        dtPtr = next + 1;

		// parse time
        hour = static_cast<int32>(std::strtol(dtPtr, &next, 10));

        if ((next <= dtPtr) || (*next == '\0'))
		{
			return false;
		}

        dtPtr = next + 1; // skip separator
        minute = static_cast<int32>(std::strtol(dtPtr, &next, 10));

        if ((next <= dtPtr) || (*next == '\0'))
		{
			return false;
		}

        dtPtr = next + 1; // skip separator
        second = static_cast<int32>(std::strtol(dtPtr, &next, 10));

        if (next <= dtPtr)
		{
			return false;
		}

		// check for milliseconds
        if (*next == '.')
		{
            dtPtr = next + 1;
            millisecond = static_cast<int32>(std::strtol(dtPtr, &next, 10));

			// should be no more than 3 digits
            if ((next <= dtPtr) || (next > dtPtr + 3))
			{
				return false;
			}

            for (PTRINT Digits = next - dtPtr; Digits < 3; ++Digits)
			{
				millisecond *= 10;
			}
		}

		// see if the timezone offset is included
        if (*next == '+' || *next == '-')
		{
			// include the separator since it's + or -
            dtPtr = next;

			// parse the timezone offset
            tzHour = static_cast<int32>(std::strtol(dtPtr, &next, 10));

            if ((next <= dtPtr) || (*next == '\0'))
			{
				return false;
			}

            dtPtr = next + 1; // skip separator
            tzMinute = static_cast<int32>(std::strtol(dtPtr, &next, 10));

            if (next <= dtPtr)
			{
				return false;
			}
		}
        else if ((*next != '\0') && (*next != 'Z'))
		{
			return false;
		}
	}
    else if (*next != '\0')
	{
		return false;
	}

	if (!validate(year, month, day, hour, minute, second, millisecond))
	{
		return false;
	}

	DateTime Final(year, month, day, hour, minute, second, millisecond);

	// adjust for the timezone (bringing the DateTime into UTC)
	int32 TzOffsetMinutes = (tzHour < 0) ? tzHour * 60 - tzMinute : tzHour * 60 + tzMinute;
	Final -= Timespan(0, TzOffsetMinutes, 0);
	outDateTime = Final;

	return true;
}

bool DateTime::validate( int32 year, int32 month, int32 day, int32 hour, int32 minute, int32 second, int32 millisecond )
{
    return (year >= 1) && (year <= 9999) &&
        (month >= 1) && (month <= 12) &&
        (day >= 1) && (day <= daysInMonth(year, month)) &&
        (hour >= 0) && (hour <= 23) &&
        (minute >= 0) && (minute <= 59) &&
        (second >= 0) && (second <= 59) &&
        (millisecond >= 0) && (millisecond <= 999);
}


NS_FK_END




