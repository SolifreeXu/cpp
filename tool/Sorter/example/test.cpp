#include "Sorter.hpp"

#include <cstdlib>
#include <cstdint>
#include <iostream>

#define SHARED

struct Record
{
	using SizeType = std::uint_least64_t;

	SizeType _id;
	SizeType _integral;
	SizeType _timestamp;

	explicit operator SizeType() const noexcept { return _id; }

	bool operator<(const Record& _another) const noexcept;

private:
	friend std::ostream& operator<<(std::ostream& _ostream, \
		const Record& _record)
	{
		return _ostream << _record._id << '\t' \
			<< _record._integral << '\t' << _record._timestamp;
	}
};

bool Record::operator<(const Record& _another) const noexcept
{
	return this->_integral < _another._integral \
		|| this->_integral == _another._integral \
		&& (this->_timestamp > _another._timestamp \
			|| this->_timestamp == _another._timestamp \
			&& this->_id > _another._id);
}

using SizeType = Record::SizeType;

#ifndef SHARED
using SorterType = Sorter<SizeType, Record>;

#else // SHARED
using SorterType = SharedSorter<SizeType, Record>;
#endif // !SHARED

static constexpr SizeType SIZE = 100 + 1;

static void load(SorterType& _sorter)
{
	for (SizeType index = 1; index < SIZE; ++index)
		_sorter.update({ index, SIZE - index, index });
}

static void update(SorterType& _sorter)
{
	_sorter.update({ 10, 100, SIZE });
	_sorter.update({ 100, 100, SIZE });
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
	SorterType::RecordList recordList(0);
	sorter.get(recordList, 0, 10);
	for (const auto& record : recordList)
		std::cout << record << std::endl;

	std::cout << "\nRemove 9" << std::endl;
	sorter.remove(9);
	std::cout << "Sorter Size: " << sorter.size() << std::endl;

	std::cout << "\nClear Sorter" << std::endl;
	sorter.clear();
	std::cout << "Sorter Size: " << sorter.size() << std::endl;

	std::cout << "\nImage Size: " << image.size() << std::endl;
	std::cout << "The 9 ranks " << image.rank(9) << "th" << std::endl;
	return EXIT_SUCCESS;
}
