#include "../include/time.h"

x::TimeStamp::TimeStamp() {
  auto now = std::chrono::system_clock::now();
  auto milliseconds = std::chrono::duration_cast<std::chrono::milliseconds>(
                          now.time_since_epoch())
                          .count();
  milliSecondsSinceEpoch_ = milliseconds;
}

x::TimeStamp x::TimeStamp::now() { return TimeStamp(); }

x::TimeStamp::TimeStamp(uint64_t milliSecondsSinceEpoch)
    : Time(milliSecondsSinceEpoch) {}

x::TimeGap x::TimeStamp::operator-(const TimeStamp &t) const {
  return TimeGap((milliSecondsSinceEpoch_ >= t.milliSecondsSinceEpoch_
                      ? milliSecondsSinceEpoch_ - t.milliSecondsSinceEpoch_
                      : t.milliSecondsSinceEpoch_ - milliSecondsSinceEpoch_));
}

x::TimeStamp x::TimeStamp::operator+(const TimeGap &t) const {
  assert(t.MilliSecond() < milliSecondsSinceEpoch_);
  return TimeStamp(milliSecondsSinceEpoch_ + t.MilliSecond());
}

x::TimeStamp &x::TimeStamp::operator+=(const TimeGap &t) {
  assert(t.MilliSecond() < milliSecondsSinceEpoch_);
  milliSecondsSinceEpoch_ += t.MilliSecond();
  return *this;
}

x::TimeStamp &x::TimeStamp::operator-=(const TimeGap &t) {
  assert(t.MilliSecond() < milliSecondsSinceEpoch_);
  milliSecondsSinceEpoch_ -= t.MilliSecond();
  return *this;
}

x::TimeStamp x::TimeStamp::operator-(const TimeGap &t) const {
  assert(t.MilliSecond() < milliSecondsSinceEpoch_);
  return TimeStamp(milliSecondsSinceEpoch_ - t.MilliSecond());
}

x::TimeStamp::operator bool() const { return *this >= TimeStamp(); }

uint64_t x::TimeStamp::MilliSecondsSinceEpoch() const {
  return milliSecondsSinceEpoch_;
}

std::string x::TimeStamp::toString() const {
  std::ostringstream oss;
  uint64_t seconds = milliSecondsSinceEpoch_ / 1000;
  uint64_t milliseconds = milliSecondsSinceEpoch_ % 1000;
  oss << seconds << "." << std::setw(3) << std::setfill('0') << milliseconds;
  return oss.str();
}

std::string x::TimeStamp::toFormattedString() const {
  char buf[64];
  std::time_t seconds = milliSecondsSinceEpoch_ / 1000;
  int milliseconds = milliSecondsSinceEpoch_ % 1000;
  std::tm tm_time;
  gmtime_r(&seconds, &tm_time);

  snprintf(buf, sizeof(buf), "%4d-%02d-%02d %02d:%02d:%02d.%03d",
           tm_time.tm_year + 1900, tm_time.tm_mon + 1, tm_time.tm_mday,
           tm_time.tm_hour, tm_time.tm_min, tm_time.tm_sec, milliseconds);
  return buf;
}

x::TimeGap::TimeGap(uint64_t milliseconds) : Time(milliseconds) {}

x::TimeGap::operator bool() const { return milliSecondsSinceEpoch_ != 0; }

x::TimeGap x::TimeGap::Week(int w) {
  return TimeGap(7 * 24 * 3600 * w * 1000ULL);
}

x::TimeGap x::TimeGap::Day(int d) { return TimeGap(24 * d * 3600 * 1000ULL); }
x::TimeGap x::TimeGap::Hour(int h) { return TimeGap(h * 3600 * 1000ULL); }
x::TimeGap x::TimeGap::Minute(int m) { return TimeGap(m * 60 * 1000ULL); }
x::TimeGap x::TimeGap::Second(int s) { return TimeGap(s * 1000ULL); }
x::TimeGap x::TimeGap::MilliSecond(int m) { return TimeGap(m); }

int x::TimeGap::Week() const {
  return milliSecondsSinceEpoch_ / 7 / 24 / 3600 / 1000;
}

int x::TimeGap::Day() const {
  return milliSecondsSinceEpoch_ / 24 / 3600 / 1000;
}

int x::TimeGap::Hour() const { return milliSecondsSinceEpoch_ / 3600 / 1000; }

int x::TimeGap::Minute() const { return milliSecondsSinceEpoch_ / 60 / 1000; }

int x::TimeGap::Second() const { return milliSecondsSinceEpoch_ / 1000; }

int x::TimeGap::MilliSecond() const { return milliSecondsSinceEpoch_; }

x::TimeGap x::TimeGap::operator+(const TimeGap &t) const {
  return TimeGap(milliSecondsSinceEpoch_ + t.milliSecondsSinceEpoch_);
}

x::TimeGap &x::TimeGap::operator+=(const TimeGap &t) {
  milliSecondsSinceEpoch_ += t.milliSecondsSinceEpoch_;
  return *this;
}

x::TimeGap x::TimeGap::operator-(const TimeGap &t) const {
  return TimeGap((milliSecondsSinceEpoch_ >= t.milliSecondsSinceEpoch_
                      ? (milliSecondsSinceEpoch_ - t.milliSecondsSinceEpoch_)
                      : t.milliSecondsSinceEpoch_ - milliSecondsSinceEpoch_));
}

x::TimeGap &x::TimeGap::operator-=(const TimeGap &t) {
  (milliSecondsSinceEpoch_ >= t.milliSecondsSinceEpoch_
       ? (milliSecondsSinceEpoch_ - t.milliSecondsSinceEpoch_)
       : t.milliSecondsSinceEpoch_ - milliSecondsSinceEpoch_);

  return *this;
}
