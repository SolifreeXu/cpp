#include "Stack.hpp"

#include <chrono>
#include <cstddef>
#include <memory>
#include <algorithm>
#include <iostream>
#include <fstream>
#include <thread>

Stack<int> stack;
Stack<int> stream;

static constexpr size_t THREADS = 10;
static constexpr size_t NUMBERS = 100;
static constexpr size_t TOTAL_NUMBERS = THREADS * NUMBERS;

static void execute(size_t start, size_t stop)
{
	for (size_t index = start; index < stop; ++index)
	{
		stack.push(index);
		std::this_thread::sleep_for(std::chrono::milliseconds(1));
		stream.push(*stack.pop());
	}
}

static void lock_free()
{
	std::shared_ptr<Stack<int>> shared_pointer;
	std::cout << std::boolalpha << std::atomic_is_lock_free(&shared_pointer) << std::endl;
}

static void generate()
{
	using std::cout;
	std::thread threads[THREADS];
	for (size_t index = 0; index < THREADS; ++index)
		threads[index] = std::thread(execute, index * NUMBERS, (index + 1) * NUMBERS);

	for (size_t index = 0; index < THREADS; ++index)
		if (threads[index].joinable())
			threads[index].join();

	std::ofstream output("input.txt");
	std::streambuf* std_cout = cout.rdbuf(output.rdbuf());
	for (size_t index = 0; index < TOTAL_NUMBERS; ++index)
		cout << *stream.pop() << ' ';
	cout.rdbuf(std_cout);
}

static void validate()
{
	using std::cin;
	using std::cout;
	std::ifstream input("input.txt");
	std::streambuf* std_cin = cin.rdbuf(input.rdbuf());
	std::ofstream output("output.txt");
	std::streambuf* std_cout = cout.rdbuf(output.rdbuf());

	bool result = true;
	static size_t numbers[TOTAL_NUMBERS];
	for (size_t index = 0; index < TOTAL_NUMBERS; ++index)
		cin >> numbers[index];
	std::sort(numbers, numbers + TOTAL_NUMBERS);
	for (size_t index = 0; index < TOTAL_NUMBERS; ++index)
	{
		result = result && (index <= 0 || numbers[index] - numbers[index - 1] == 1);
		cout << numbers[index] << ' ';
	}

	cin.rdbuf(std_cin);
	cout.rdbuf(std_cout);
	cout << std::boolalpha << result;
}

int main()
{
	lock_free();
	generate();
	validate();
	return 0;
}
