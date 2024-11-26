#include "time.h"
#include <chrono>
#include <assert.h>

// common

//PASS
static uint64_t sub(uint64_t a, uint64_t b) {
    if (a >= b)return a - b;
    return b - a;
}

//PASS
static uint64_t now_milliseconds_since_epoch() {
	return std::chrono::duration_cast<std::chrono::milliseconds>(
		std::chrono::system_clock::now().time_since_epoch())
		.count();
}

//PASS
static uint64_t DateTimeToMilliSeconds(int year, int month, int day, int hour, int minute, int second, int millisecond) {
	assert(x::time::isValidDateTime(year, month, day, hour, minute, second, millisecond));
    
    std::tm tm;
    tm.tm_year = year - 1900;
    tm.tm_mon = month - 1;
    tm.tm_mday = day;
    tm.tm_hour = hour;
    tm.tm_min = minute;
    tm.tm_sec = second;

    std::time_t timeSinceEpoch = mktime(&tm);
    return timeSinceEpoch * 1000ULL + millisecond;
}

//Windos PASS
static std::tm MilliSecondstoTm(uint64_t  milliSecondsSinceEpoch_) {
    std::time_t seconds = milliSecondsSinceEpoch_ / 1000ULL;
    std::tm tm;

#ifdef PLATFORM_WINDOWS
    localtime_s(&tm, &seconds);  // Windows-safe version
#elif defined(PLATFORM_UNIX)
    localtime_r(&seconds, &tm);  // POSIX-safe version
#endif

    return tm;
}

//PASS
bool x::time::isValidDateTime(int year, int month, int day, int hour, int minute, int second, int millisecond)
{
    auto isValidDate = [](int year, int month, int day) {        
		if (month == 2) {
            if ((year % 4 == 0 && year % 100 != 0) || (year % 400 == 0)) {
                return day <= 29;             }
            return day <= 28;        }
        else if (month == 4 || month == 6 || month == 9 || month == 11) {
            return day <= 30;        }
        return day <= 31;        };

    return (year <= x::time::Stamp::MAX_YEAR &&year >= x::time::Stamp::MIN_YEAR) && (1 <= month && month <= 12) && (1 <= day && day <= 31) && (0 <= hour && hour <= 23) && (0 <= minute && minute <= 59) &&(0<= second && second <=59)&& (0 <= millisecond && millisecond <= 999) && isValidDate(year, month, day);
}

// Stamp construct
// PASS

const uint64_t x::time::Stamp::MIN_MILLISECONDS_SINCE_EPOCH = 0;
const uint64_t x::time::Stamp::MAX_MILLISECONDS_SINCE_EPOCH =97858108799999; 
const uint16_t x::time::Stamp::MAX_YEAR = 5000;
const uint16_t x::time::Stamp::MIN_YEAR = 1900;

x::time::Stamp::Stamp(uint64_t m):milliseconds_since_epoch(m){
    assert(m <= MAX_MILLISECONDS_SINCE_EPOCH);
}

// PASS
x::time::Stamp x::time::Stamp::Now()
{
	return Stamp(now_milliseconds_since_epoch());
}

// PASS
x::time::Stamp x::time::Stamp::DateTime(int year, int month, int day, int hour, int minute, int second, int millisecond)
{
	return Stamp(DateTimeToMilliSeconds(year,month,day,hour,minute,second,millisecond));
}

// PASS
x::time::Stamp::Stamp(const Stamp& s):milliseconds_since_epoch(s.milliseconds_since_epoch){
    assert(milliseconds_since_epoch <= MAX_MILLISECONDS_SINCE_EPOCH);
}

// Stamp op
// 
x::time::Gap x::time::Stamp::operator-(const Stamp& s)const {
    return Gap(sub(milliseconds_since_epoch, s.milliseconds_since_epoch));
}

// PASS
x::time::Stamp x::time::Stamp::operator+(const Gap& g) const {
    assert( milliseconds_since_epoch <= MAX_MILLISECONDS_SINCE_EPOCH-g.milliseconds);
    return Stamp(milliseconds_since_epoch + g.milliseconds);
}

// PASS
x::time::Stamp& x::time::Stamp::operator+=(const Gap& g) {
    assert(milliseconds_since_epoch <= MAX_MILLISECONDS_SINCE_EPOCH - g.milliseconds);
    milliseconds_since_epoch += g.milliseconds;
    return *this;
}

// PASS
x::time::Stamp x::time::Stamp::operator-(const Gap& g) const {
    assert(milliseconds_since_epoch >= g.milliseconds);
    return Stamp(milliseconds_since_epoch - g.milliseconds);
}

// PASS
x::time::Stamp& x::time::Stamp::operator-=(const Gap& g) {
    assert(milliseconds_since_epoch >= g.milliseconds);
    milliseconds_since_epoch -= g.milliseconds;
    return *this;
}

// PASS
x::time::StampView x::time::Stamp::View()const {
    auto tm = MilliSecondstoTm(milliseconds_since_epoch);
    return {
        static_cast<uint16_t>(tm.tm_year + 1900),
        static_cast<uint8_t>(tm.tm_mon + 1),
        static_cast<uint8_t>(tm.tm_mday),
        static_cast<uint8_t>(tm.tm_hour),
        static_cast<uint8_t>(tm.tm_min),
        static_cast<uint8_t>(tm.tm_sec),
        static_cast<uint16_t>(milliseconds_since_epoch % 1000)
    };
}


// gap construct
const uint64_t x::time::Gap::MAX_MILLISECONDS = 1210694400000;
const uint16_t x::time::Gap::MAX_WEEK = 2003;

// PASS
x::time::Gap::Gap(uint64_t m):milliseconds(m){
    assert(m <= MAX_MILLISECONDS);
}

// PASS
x::time::Gap::Gap(const Gap&g):milliseconds(g.milliseconds){
    assert(g.milliseconds <= MAX_MILLISECONDS);
}

// gap factory construct
// PASS
x::time::Gap x::time::Gap::MilliSeconds(uint64_t m)
{
    return Gap(m);
}

// PASS
x::time::Gap x::time::Gap::Seconds(uint64_t s)
{
    return Gap(s*1000ULL);
}

// PASS
x::time::Gap x::time::Gap::Minutes(uint64_t m)
{
    return Gap(m * 60 * 1000ULL);
}

// PASS
x::time::Gap x::time::Gap::Hours(uint64_t h) {
    return Gap(h * 60 * 60 * 1000ULL);
}

// PASS
x::time::Gap x::time::Gap::Days(uint64_t d) {
    return Gap(d * 24 * 60 * 60 * 1000ULL);
}

// PASS
x::time::Gap x::time::Gap::Weeks(uint64_t w) {
    return Gap(w * 7 * 24 * 60 * 60 * 1000ULL);
}

// PASS
x::time::Stamp x::time::Gap::operator+(const Stamp&s)const {
    assert(milliseconds <= Stamp::MAX_MILLISECONDS_SINCE_EPOCH - s.milliseconds_since_epoch);
    return Stamp(milliseconds + s.milliseconds_since_epoch);
}

// PASS
x::time::Gap x::time::Gap::operator+(const Gap& g) const {
    assert(milliseconds <= MAX_MILLISECONDS - g.milliseconds);
    return Gap(milliseconds + g.milliseconds);
}

// PASS
x::time::Gap& x::time::Gap::operator+=(const Gap& g) {
    assert(milliseconds <= MAX_MILLISECONDS - g.milliseconds);
    milliseconds += g.milliseconds;
    return *this;
}

// PASS
x::time::GapView x::time::Gap::View()const {
    uint64_t remainingTime = milliseconds;
    uint16_t weeks = remainingTime / (24ULL * 7 * 3600 * 1000);
    remainingTime %= (24ULL * 7 * 3600 * 1000);
    uint8_t days = remainingTime / (24ULL * 3600 * 1000);
    remainingTime %= (24ULL * 3600 * 1000);
    uint8_t hours = remainingTime / (3600 * 1000);
    remainingTime %= (3600 * 1000);
    uint8_t minutes = remainingTime / (60 * 1000);
    remainingTime %= (60 * 1000);
    uint8_t seconds = remainingTime / 1000; remainingTime %= 1000;
    uint16_t milliseconds_ = remainingTime;
    return { weeks, days, hours, minutes, seconds, milliseconds_ };
}

std::string x::time::StampView::toString() const
{
    std::ostringstream oss; oss << std::setw(4) << std::setfill('0') << year << "-" << std::setw(2) << std::setfill('0') << static_cast<uint16_t>(month) << "-" << std::setw(2) << std::setfill('0') << static_cast<uint16_t>(day) << " " << std::setw(2) << std::setfill('0') << static_cast<uint16_t>(hour) << ":" << std::setw(2) << std::setfill('0') << static_cast<uint16_t>(minute) << ":" << std::setw(2) << std::setfill('0') << static_cast<uint16_t>(second) << "." << std::setw(3) << std::setfill('0') << millisecond; return oss.str();
}

std::string x::time::GapView::toString() const
{
    std::ostringstream oss; oss << week << "w" << std::setw(1) << static_cast<uint16_t>(day) << "d" << std::setw(2) << std::setfill('0') << static_cast<uint16_t>(hour) << "h" << std::setw(2) << std::setfill('0') << static_cast<uint16_t>(minute) << "m" << std::setw(2) << std::setfill('0') << static_cast<uint16_t>(second) << "s" << std::setw(3) << std::setfill('0') << millisecond << "ms"; return oss.str(); 
}
