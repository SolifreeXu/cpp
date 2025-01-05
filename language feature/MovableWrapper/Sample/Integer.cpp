#include "Integer.hpp"

#include <exception>
#include <utility>
#include <iostream>

std::ostream& operator<<(std::ostream& _stream, \
	const Integer& _integer)
{
	return _stream << _integer._value;
}

Integer::Integer() noexcept : _value(0)
{
	try
	{
		std::cout << "default constructor " \
			<< _value << std::endl;
	}
	catch (std::exception&) {}
}

Integer::Integer(int _value) noexcept : \
	_value(_value)
{
	try
	{
		std::cout << "conversion constructor " \
			<< _value << std::endl;
	}
	catch (std::exception&) {}
}

Integer::Integer(const Integer& _another) noexcept : \
	_value(_another._value)
{
	try
	{
		std::cout << "copy constructor " \
			<< _value << std::endl;
	}
	catch (std::exception&) {}
}

Integer::Integer(Integer&& _another) noexcept : \
	_value(std::move(_another._value))
{
	try
	{
		std::cout << "move constructor " \
			<< _value << std::endl;
	}
	catch (std::exception&) {}
}

Integer::~Integer() noexcept
{
	try
	{
		std::cout << "destructor " \
			<< _value << std::endl;
	}
	catch (std::exception&) {}
}
