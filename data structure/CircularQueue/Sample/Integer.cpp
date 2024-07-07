#include "Integer.hpp"

#include <utility>
#include <iostream>

std::ostream& operator<<(std::ostream& _stream, \
	const Integer& _integer)
{
	return _stream << _integer._value;
}

Integer::Integer() : _value(0)
{
	std::cout << "default constructor " \
		<< _value << std::endl;
}

Integer::Integer(int _value) : \
	_value(_value)
{
	std::cout << "conversion constructor " \
		<< _value << std::endl;
}

Integer::Integer(const Integer& _another) : \
	_value(_another._value)
{
	std::cout << "copy constructor " \
		<< _value << std::endl;
}

Integer::Integer(Integer&& _another) : \
	_value(std::move(_another._value))
{
	std::cout << "move constructor " \
		<< _value << std::endl;
}

Integer::~Integer()
{
	std::cout << "destructor " \
		<< _value << std::endl;
}
