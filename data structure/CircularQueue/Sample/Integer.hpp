#pragma once

#include <ostream>

class Integer final
{
	int _value;

private:
	friend std::ostream& operator<<(std::ostream& _stream, \
		const Integer& _integer);

public:
	Integer();

	Integer(int _value);

	Integer(const Integer& _another);

	Integer(Integer&& _another);

	~Integer();

	operator int() const noexcept
	{
		return _value;
	}
};
