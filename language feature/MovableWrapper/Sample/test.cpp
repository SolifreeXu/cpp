#include "Integer.hpp"
#include "MovableWrapper.hpp"

#include <cstdlib>
#include <memory>
#include <utility>
#include <iostream>

using UniquePointer = std::unique_ptr<Integer>;
using UniqueWrapper = MovableWrapper<UniquePointer>;

static void print(const UniquePointer& _pointer)
{
	if (_pointer)
		std::cout << *_pointer << std::endl;
}

int main()
{
	UniquePointer pointer(new Integer(1));
	UniqueWrapper wrapper(std::move(pointer));

	pointer.reset(new Integer(2));
	decltype(wrapper) image(std::move(pointer));

	std::swap(wrapper, image);
	print(*wrapper);

	pointer = image.release();
	print(pointer);

	pointer.reset(new Integer(3));
	wrapper.reset(std::move(pointer));
	print(*wrapper);

	image = wrapper;
	print(*wrapper);
	return EXIT_SUCCESS;
}
