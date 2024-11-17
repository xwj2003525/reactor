#pragma once

#include <assert.h>
#include <chrono>
#include <cstdint>
#include <ctime>
#include <iomanip>
#include <sstream>
#include <string>


namespace x {

// unname Time , outside hidden
namespace {
class Time {
public:
  Time(uint64_t milliSecondsSinceEpoch = 0)
      : milliSecondsSinceEpoch_(milliSecondsSinceEpoch) {}

  Time(const Time &t) : milliSecondsSinceEpoch_(t.milliSecondsSinceEpoch_) {}

  // you can compare TimeStamp with TimeStamp , not TimeStamp with TimeGap
  bool operator<(const Time &t) const {
    return milliSecondsSinceEpoch_ < t.milliSecondsSinceEpoch_;
  }

  bool operator>(const Time &t) const {
    return milliSecondsSinceEpoch_ > t.milliSecondsSinceEpoch_;
  }

  bool operator>=(const Time &t) const {
    return milliSecondsSinceEpoch_ >= t.milliSecondsSinceEpoch_;
  }

  bool operator<=(const Time &t) const {
    return milliSecondsSinceEpoch_ <= t.milliSecondsSinceEpoch_;
  }

  bool operator==(const Time &t) const {
    return milliSecondsSinceEpoch_ == t.milliSecondsSinceEpoch_;
  }

  bool operator!=(const Time &t) const { return !(*this == t); }

protected:
  // in timestamp it means milliSecondsSinceEpoch , but int timegap , it means
  // milliseconds
  uint64_t milliSecondsSinceEpoch_;
};
}; // namespace

class TimeGap;

class TimeStamp : public Time {
public:
  TimeStamp();
  explicit TimeStamp(uint64_t milliSecondsSinceEpoch);

  // a - b = b - a
  TimeGap operator-(const TimeStamp &) const;

  // timegap must small than timestamp
  TimeStamp operator+(const TimeGap &) const;
  TimeStamp &operator+=(const TimeGap &);
  TimeStamp operator-(const TimeGap &) const;
  TimeStamp &operator-=(const TimeGap &);

  bool operator<(const TimeGap &) const = delete;
  bool operator>(const TimeGap &) const = delete;
  bool operator>=(const TimeGap &) const = delete;
  bool operator<=(const TimeGap &) const = delete;
  bool operator==(const TimeGap &) const = delete;
  bool operator!=(const TimeGap &) const = delete;

  // if is future or now time , it is valid
  operator bool() const;

  // return real date in correct range , like day in range [1,31]
  int Day() const { return toTm().tm_mday; }
  int Year() const { return toTm().tm_year + 1900; }
  int Month() const { return toTm().tm_mon + 1; }
  int Hour() const { return toTm().tm_hour; }
  int Minute() const { return toTm().tm_min; }
  int Second() const { return toTm().tm_sec; }
  int MilliSecond() const { return milliSecondsSinceEpoch_ % 1000ULL; }

  uint64_t MilliSecondsSinceEpoch() const;
  std::string toString() const;
  std::string toFormattedString() const;

  static TimeStamp now();

private:
  std::tm toTm() const {
    std::time_t seconds = milliSecondsSinceEpoch_ / 1000ULL;
    std::tm tm;
    gmtime_r(&seconds, &tm);
    return tm;
  }
};

// Gap永远大于0
class TimeGap : public Time {
public:
  TimeGap(uint64_t milliseconds);
  static TimeGap Week(int);
  static TimeGap Day(int);
  static TimeGap Hour(int);
  static TimeGap Minute(int);
  static TimeGap Second(int);
  static TimeGap MilliSecond(int);

  TimeGap operator+(const TimeGap &) const;
  TimeGap &operator+=(const TimeGap &);
  TimeGap operator-(const TimeGap &) const;
  TimeGap &operator-=(const TimeGap &);

  // if milliSecondsSinceEpoch_ = 0 , means invalid timegap
  operator bool() const;

  int Week() const;
  int Day() const;
  int Hour() const;
  int Minute() const;
  int Second() const;
  int MilliSecond() const;
};

}; // namespace x
