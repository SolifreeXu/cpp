#include <cstddef>
#include <cstdint>
#include <iostream>

#include "Leaderboard.hpp"

struct Record
{
	uint_least64_t id;
	uint_least64_t integral;
	uint_least64_t timestamp;

	bool operator<(const Record& another) const
	{
		return this->integral < another.integral
			|| this->integral == another.integral
			&& (this->timestamp > another.timestamp
				|| this->timestamp == another.timestamp
				&& this->id > another.id);
	}

private:
	friend std::ostream& operator<<(std::ostream& ostream, const Record& record)
	{
		ostream << record.id << '\t' << record.integral << '\t' << record.timestamp;
		return ostream;
	}
};

static void load(Leaderboard<uint_least64_t, Record>& leaderboard)
{
	for (size_t index = 1; index <= 100; ++index)
		leaderboard.update(index, { index, 101 - index, index });
}

static void update(Leaderboard<uint_least64_t, Record>& leaderboard)
{
	leaderboard.update(10, { 10, 100, 101 });
	leaderboard.update(100, { 100, 100, 101 });
}

int main()
{
	Leaderboard<uint_least64_t, Record> leaderboard;
	load(leaderboard);
	auto snapshot = leaderboard.backup();
	update(leaderboard);

	auto records = leaderboard.get(10);
	for (const auto& record : *records)
		std::cout << record << std::endl;

	auto pointer = snapshot->find(10);
	if (pointer)
		std::cout << *pointer << std::endl;
	return 0;
}
