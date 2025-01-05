#pragma once

#include <iostream>

class Time
{
private:
	long long totalSeconds;
	long long hours;
	int minutes;
	int seconds;

	friend std::istream& operator>>(std::istream&, Time&);
	friend std::ostream& operator<<(std::ostream&, const Time&);
public:
	Time();
	Time(const long long &seconds);
	Time(const long long &hours, const int &minutes, const int &seconds);
	bool setTime(const long long &seconds);
	long long getTime()const
	{
		return totalSeconds;
	}
	bool setHours(const long long &hours);
	long long getHours()const
	{
		return hours;
	}
	bool setMinutes(const int &minutes);
	int getMinutes()const
	{
		return minutes;
	}
	bool setSeconds(const int &seconds);
	int getSeconds()const
	{
		return seconds;
	}
	friend Time operator+(const Time &left, const Time &right);
	friend Time operator-(const Time &left, const Time &right);
	friend bool operator>(const Time &left, const Time &right);
	friend bool operator<(const Time &left, const Time &right);
	friend bool operator==(const Time &left, const Time &right);
	friend bool operator>=(const Time &left, const Time &right);
	friend bool operator<=(const Time &left, const Time &right);
};