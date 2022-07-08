#include "Sorter.hpp"

#include <cstdint>
#include <iostream>

struct Record
{
	std::uint_least64_t _id;
	std::uint_least64_t _integral;
	std::uint_least64_t _timestamp;

	bool operator<(const Record& _another) const noexcept
	{
		return this->_integral < _another._integral \
			|| this->_integral == _another._integral \
			&& (this->_timestamp > _another._timestamp \
				|| this->_timestamp == _another._timestamp \
				&& this->_id > _another._id);
	}

private:
	friend std::ostream& operator<<(std::ostream& _ostream, \
		const Record& _record)
	{
		return _ostream << _record._id << '\t' \
			<< _record._integral << '\t' << _record._timestamp;
	}
};

using SorterType = Sorter<std::uint_least64_t, Record>;

static constexpr std::uint_least64_t SIZE = 100 + 1;

static void load(SorterType& _sorter)
{
	for (std::uint_least64_t index = 1; index < SIZE; ++index)
		_sorter.update(index, { index, SIZE - index, index });
}

static void update(SorterType& _sorter)
{
	_sorter.update(10, { 10, 100, SIZE });
	_sorter.update(100, { 100, 100, SIZE });
}

int main()
{
	std::cout << "Load Sorter" << std::endl;
	SorterType sorter;
	load(sorter);

	std::cout << "Sorter Size: " << sorter.size() << std::endl;
	std::cout << "No.9" << std::endl;
	if (auto pointer = sorter.find(9))
		std::cout << *pointer << std::endl;

	std::cout << "\nGenerate Image\n" << std::endl;
	auto image = sorter;

	std::cout << "Update Sorter" << std::endl;
	update(sorter);

	std::cout << "Sorter Size: " << sorter.size() << std::endl;
	std::cout << "Top 10" << std::endl;
	if (auto recordList = sorter.get(0, 10))
		for (const auto& record : *recordList)
			std::cout << record << std::endl;

	std::cout << "\nRemove 9" << std::endl;
	sorter.remove(9);
	std::cout << "Sorter Size: " << sorter.size() << std::endl;

	std::cout << "\nClear Sorter" << std::endl;
	sorter.clear();
	std::cout << "Sorter Size: " << sorter.size() << std::endl;

	std::cout << "\nImage Size: " << image.size() << std::endl;
	std::cout << "The 9 ranks " << image.rank(9) << "th" << std::endl;
	return 0;
}
