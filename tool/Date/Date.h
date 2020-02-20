#pragma once

#include <iostream>

class Date
{
	int year;
	unsigned month;
	unsigned day;

	friend std::istream& operator>> (std::istream&, Date&);
	friend std::ostream& operator<< (std::ostream&, const Date&);
public:
	Date();
	Date(int, unsigned, unsigned);
	Date& operator = (const Date&);
	friend bool operator == (const Date &left, const Date &right);
	friend bool operator > (const Date &left, const Date &right);
	friend bool operator < (const Date &left, const Date &right);
	friend bool operator >= (const Date &left, const Date &right);
	friend bool operator <= (const Date &left, const Date &right);
	int getYear()
	{
		return year;
	}
	unsigned getMonth()
	{
		return month;
	}
	unsigned getDay()
	{
		return day;
	}
	void setYear(const int year);
	bool setMonth(const unsigned month);
	bool setDay(const unsigned day);
	bool isLeapYear()
	{
		return year % 400 == 0 || year % 4 == 0 && year % 100 != 0;
	}
};