#include "Date.h"

std::istream& operator>> (std::istream &input, Date &object)
{
	char delimiter;
	input >> object.year >> delimiter >> object.month >> delimiter >> object.day;
	return input;
}

std::ostream& operator<< (std::ostream &output, const Date &object)
{
	output << object.year << '/';
	if (object.month < 10)
		output << '0';
	output << object.month << '/';
	if (object.day < 10)
		output << '0';
	output << object.day;
	return output;
}

#include <time.h>

Date::Date()
{
	time_t timen;
	struct tm *tb = localtime(&(timen = time(NULL)));
	year = tb->tm_year + 1900;
	month = tb->tm_mon + 1;
	day = tb->tm_mday;
}

Date::Date(int year, unsigned month, unsigned day)
{
	setYear(year);
	setMonth(month);
	setDay(day);
}

Date& Date::operator = (const Date &other)
{
	year = other.year;
	month = other.month;
	day = other.day;
	return *this;
}

bool operator==(const Date &left, const Date &right)
{
	if (left.year == right.year && left.month == right.month && left.day == right.day)
		return true;
	return false;
}

bool operator>(const Date &left, const Date &right)
{
	if (left.year > right.year)
		return true;
	if (left.year == right.year && left.month > right.month)
		return true;
	if (left.year == right.year && left.month == right.month && left.day > right.day)
		return true;
	return false;
}

bool operator<(const Date &left, const Date &right)
{
	if (left.year < right.year)
		return true;
	if (left.year == right.year && left.month < right.month)
		return true;
	if (left.year == right.year && left.month == right.month && left.day < right.day)
		return true;
	return false;
}

bool operator>=(const Date &left, const Date &right)
{
	if (left.year > right.year)
		return true;
	else if (left.year == right.year && left.month > right.month)
		return true;
	else if (left.year == right.year && left.month == right.month && left.day >= right.day)
		return true;
	return false;
}

bool operator<=(const Date &left, const Date &right)
{
	if (left.year < right.year)
		return true;
	else if (left.year == right.year && left.month < right.month)
		return true;
	else if (left.year == right.year && left.month == right.month && left.day <= right.day)
		return true;
	return false;
}

void Date::setYear(const int year)
{
	this->year = year;
	if (year % 4 == 0 && month == 2 && day > 28)
		day = 28;
}

bool Date::setMonth(const unsigned month)
{
	if (month > 0 && month <= 12)
	{
		this->month = month;
		if (month == 2)
		{
			if (isLeapYear())
			{
				if (day > 29)
					day = 29;
			}
			else if (day > 28)
				day = 28;
		}
		else if ((month == 4 || month == 6 || month == 9 || month == 11) && day > 30)
			day = 30;
		return true;
	}
	return false;
}

bool Date::setDay(const unsigned day)
{
	if (month == 2 && day <= 29)
	{
		if (isLeapYear())
		{
			this->day = day;
			return true;
		}
		else if (day < 29)
		{
			this->day = day;
			return true;
		}
	}
	else if ((month > 0 && month < 8 && month % 2 == 1 || month >= 8 && month <= 12 && month % 2 == 0) && day <= 31)
	{
		this->day = day;
		return true;
	}
	else if ((month == 4 || month == 6 || month == 9 || month == 11) && day < 31)
	{
		this->day = day;
		return true;
	}
	return false;
}