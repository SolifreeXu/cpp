#include "Numeric.hpp"

#include <cctype>
#include <cstring>

bool valid(const char* _string, IntegerType _type)
{
	auto size = std::strlen(_string);
	if (size <= 0)
		return false;

	char sign = '\0', digit = '\0';
	decltype(size) sum = 0;
	for (decltype(size) index = 0; index < size; ++index)
	{
		auto character = _string[index];
		if (std::isspace(character))
		{
			if (digit)
				break;

			if (sign)
				return false;
			continue;
		}

		if (character == '+' || character == '-')
		{
			if (sign || digit)
				return false;

			sign = character;
			continue;
		}

		if (!isdigit(character))
			return false;
		sum += (digit = character) - '0';
	}

	switch (_type)
	{
	case IntegerType::ALL:
		break;
	case IntegerType::NATURAL:
		if (sign == '-' && sum != 0)
			return false;
		break;
	case IntegerType::POSITIVE:
		if (sum == 0)
			return false;
		if (sign == '-')
			return false;
		break;
	case IntegerType::NEGATIVE:
		if (sum == 0)
			return false;
		if (sign != '-')
			return false;
		break;
	default:
		break;
	}
	return true;
}
