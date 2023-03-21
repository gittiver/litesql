/* LiteSQL
 *
 * The list of contributors at http://litesql.sf.net/
 *
 * See LICENSE for copyright information. */

#ifndef litesql_datetime_hpp
#define litesql_datetime_hpp
/** \file datetime.hpp
    contains classes for handling date and time data. */
#include "field.hpp"
#include <ctime>
namespace litesql {

/* holds struct tm */
class TimeStruct {
    struct tm mytm;
public:
    TimeStruct(time_t t=0);
    int day() const;
    int dayOfWeek() const;
    int dayOfYear() const;
    int month() const;
    int year() const;
    int hour() const;
    int min() const;
    int sec() const;
    time_t timeStamp() const;
    TimeStruct& setDay(int day);
    TimeStruct& setMonth(int month);
    TimeStruct& setYear(int year);
    TimeStruct& setHour(int hour);
    TimeStruct& setMin(int min);
    TimeStruct& setSec(int sec);
    TimeStruct& setTimeStamp(time_t t);
};
        
/** holds date */
class Date {
    time_t value;
public:
    /** crops time of day to 00:00:00 */
    Date(time_t t=0);
    Date(int day, int month, int year);
    int day() const;
    int dayOfWeek() const;
    int month() const;
    int year() const;
    time_t timeStamp() const;
    TimeStruct timeStruct() const;
    
    Date& setDay(int d);
    Date& setMonth(int m);
    Date& setYear(int y);
    Date& setTimeStamp(time_t t);
    std::string asString(const std::string& format="%u") const;
};
  
/** holds date and time of day */
class DateTime {
    time_t value;
public:
    DateTime(time_t t=0);
    int day() const;
    int month() const;
    int year() const;
    int hour() const;
    int min() const;
    int sec() const;
    time_t timeStamp() const;
    TimeStruct timeStruct() const;
    std::string asString(const std::string& format="%u") const;

    DateTime& setDay(int d);
    DateTime& setMonth(int m);
    DateTime& setYear(int y);
    DateTime& setHour(int d);
    DateTime& setMin(int m);
    DateTime& setSec(int y);
    Date& setTimeStamp(time_t t);
};
template <>
Date convert<const std::string&, Date>(const std::string& value);
template <>
DateTime convert<const std::string&, DateTime>(const std::string& value);

template <>
std::string convert<const Date&, std::string>(const Date& value);
template <>
std::string convert<const DateTime&, std::string>(const DateTime& value);

std::ostream& operator << (std::ostream& os, const Date& d);
std::ostream& operator << (std::ostream& os, const DateTime& d);
}

#endif
