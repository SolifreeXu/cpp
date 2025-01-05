#pragma once

#include <ostream>

class Integer final
{
	int _value;

private:
	friend std::ostream& operator<<(std::ostream& _stream, \
		const Integer& _integer);

public:
	Integer() noexcept;

	Integer(int _value) noexcept;

	Integer(const Integer& _another) noexcept;

	Integer(Integer&& _another) noexcept;

	~Integer() noexcept;

	operator int() const noexcept
	{
		return _value;
	}
};
