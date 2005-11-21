/* LiteSQL - Date and time classes
 * 
 * By Tero Laitinen 
 * 
 * See LICENSE for copyright information. */
#include "litesql/datetime.hpp"
#include <time.h>
using namespace std;
namespace litesql {
Date::Date(time_t t) : value(t) {
}
Date::Date(int day, int month, int year) {
    struct tm mytm;
    mytm.tm_sec = mytm.tm_min = mytm.tm_hour = 0;
    mytm.tm_mday = day;
    mytm.tm_mon = month - 1;
    mytm.tm_year = year - 1900;
    mytm.tm_isdst = -1;
    value = mktime(&mytm);
}
int Date::day() {
    struct tm mytm;
    localtime_r(&value, &mytm);
}
int Date::month() {
}
int Date::year() {
}
Date& Date::setDay(int d) {
}
Date& Date::setMonth(int m) {
}
Date& Date::setYear(int y) {
}
string Date::asString(string format) {
}
Time::Time(int hour, int min, int sec) {
}
int Time::hour() {
}
int Time::min() {
}
int Time::sec() {
}
int Time::secs() {
    return value;
}
Time& Time::setHour(int d) {
}
Time& Time::setMin(int m) {
}
Time& Time::setSec(int y) {
}
Time& Time::setSecs(int secs) {
}
string Time::asString(string format) {
}
DateTime::DateTime(time_t t) {
}
int DateTime::hour() {
}
int DateTime::min() {
}
int DateTime::sec() {
}
time_t DateTime::timeStamp() {
}
DateTime& DateTime::setDay(int d) {
}
DateTime& DateTime::setMonth(int m) {
}
DateTime& DateTime::setYear(int y) {
}
DateTime& DateTime::setHour(int d) {
}
DateTime& DateTime::setMin(int m) {
}
DateTime& DateTime::setSec(int y) {
}
string DateTime::asString(string format) {

}

}
