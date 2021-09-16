#include <cstddef>
#include <cstdint>
#include <iostream>

#include "Sorter.hpp"

struct Record
{
	uint_least64_t _id;
	uint_least64_t _integral;
	uint_least64_t _timestamp;

	bool operator<(const Record& _another) const noexcept
	{
		return this->_integral < _another._integral
			|| this->_integral == _another._integral
			&& (this->_timestamp > _another._timestamp
				|| this->_timestamp == _another._timestamp
				&& this->_id > _another._id);
	}

private:
	friend std::ostream& operator<<(std::ostream& _ostream, const Record& _record)
	{
		return _ostream << _record._id << '\t' << _record._integral << '\t' << _record._timestamp;
	}
};

static void load(Sorter<uint_least64_t, Record>& _sorter)
{
	for (size_t index = 1; index <= 100; ++index)
		_sorter.update(index, { index, 101 - index, index });
}

static void update(Sorter<uint_least64_t, Record>& _sorter)
{
	_sorter.update(10, { 10, 100, 101 });
	_sorter.update(100, { 100, 100, 101 });
}

int main()
{
	Sorter<uint_least64_t, Record> sorter;
	load(sorter);

	auto snapshot = sorter.backup();
	update(sorter);

	if (auto recordList = sorter.get(0, 10))
	for (const auto& record : *recordList)
		std::cout << record << std::endl;

	if (auto pointer = snapshot->find(10))
		std::cout << *pointer << std::endl;
	return 0;
}
