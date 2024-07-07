#pragma once

#include "Common.hpp"
#include "Version.hpp"

#include <cstddef>
#include <initializer_list>
#include <type_traits>
#include <utility>
#include <memory>
#include <limits>
#include <exception>
#include <stdexcept>
#include <cstring>
#include <iterator>
#include <algorithm>

template <typename _CircularQueue>
class CircularQueueConstIterator
{
	//friend typename _CircularQueue;
	template<typename _Element, typename _Allocator>
	friend class CircularQueue;

public:
#ifdef __cpp_lib_concepts
	using iterator_concept = std::random_access_iterator_tag;
#endif // __cpp_lib_concepts
	using iterator_category = std::random_access_iterator_tag;

	using CircularQueue = _CircularQueue;
	using value_type = typename CircularQueue::value_type;

	using size_type = typename CircularQueue::size_type;
	using difference_type = typename CircularQueue::difference_type;

	using pointer = typename CircularQueue::const_pointer;
	using reference = const value_type&;

private:
	const CircularQueue* _queue;
	size_type _offset;

protected:
	friend CONSTEXPR void verifyContainer(const CircularQueueConstIterator& _left, \
		const CircularQueueConstIterator& _right)
	{
		if (_left._queue != _right._queue) throw std::runtime_error(DIFFERENT_CONTAINER);
	}

	friend CONSTEXPR void verifyRange(const CircularQueueConstIterator& _first, \
		const CircularQueueConstIterator& _last)
	{
		verifyContainer(_first, _last);
		if (_first > _last) throw std::runtime_error(TRANSPOSED_RANGE);
	}

public:
	NODISCARD friend CONSTEXPR CircularQueueConstIterator operator+(difference_type _offset, \
		const CircularQueueConstIterator& _iterator)
	{
		return _iterator + _offset;
	}

public:
	CONSTEXPR CircularQueueConstIterator() noexcept : \
		_queue(nullptr), _offset(0) {}

	CONSTEXPR CircularQueueConstIterator(const CircularQueue* _queue, \
		size_type _offset) noexcept : _queue(_queue), _offset(_offset) {}

	NODISCARD CONSTEXPR reference operator*() const
	{
		return *operator->();
	}

	NODISCARD CONSTEXPR pointer operator->() const;

	CONSTEXPR CircularQueueConstIterator& operator++();

	CONSTEXPR CircularQueueConstIterator operator++(int);

	CONSTEXPR CircularQueueConstIterator& operator--();

	CONSTEXPR CircularQueueConstIterator operator--(int);

	CONSTEXPR CircularQueueConstIterator& operator+=(difference_type _offset);

	CONSTEXPR CircularQueueConstIterator& operator-=(difference_type _offset)
	{
		return *this += -_offset;
	}

	NODISCARD CONSTEXPR CircularQueueConstIterator operator+(difference_type _offset) const
	{
		auto iterator = *this;
		return iterator += _offset;
	}

	NODISCARD CONSTEXPR CircularQueueConstIterator operator-(difference_type _offset) const
	{
		auto iterator = *this;
		return iterator -= _offset;
	}

	NODISCARD CONSTEXPR difference_type operator-(const CircularQueueConstIterator& _iterator) const;

	NODISCARD CONSTEXPR reference operator[](difference_type _offset) const
	{
		return *(*this + _offset);
	}

	NODISCARD CONSTEXPR bool operator==(const CircularQueueConstIterator& _iterator) const
	{
		verifyContainer(*this, _iterator);
		return this->_offset == _iterator._offset;
	}

#ifdef HAS_CXX20
	NODISCARD constexpr auto operator<=>(const CircularQueueConstIterator& _iterator) const
	{
		verifyContainer(*this, _iterator);
		return this->_offset <=> _iterator._offset;
	}

#else // HAS_CXX20
	NODISCARD bool operator!=(const CircularQueueConstIterator& _iterator) const
	{
		return not (*this == _iterator);
	}

	NODISCARD bool operator<(const CircularQueueConstIterator& _iterator) const
	{
		verifyContainer(*this, _iterator);
		return this->_offset < _iterator._offset;
	}

	NODISCARD bool operator<=(const CircularQueueConstIterator& _iterator) const
	{
		return not (_iterator < *this);
	}

	NODISCARD bool operator>(const CircularQueueConstIterator& _iterator) const
	{
		return _iterator < *this;
	}

	NODISCARD bool operator>=(const CircularQueueConstIterator& _iterator) const
	{
		return not (*this < _iterator);
	}
#endif // HAS_CXX20
};

template <typename _CircularQueue>
class CircularQueueIterator : public CircularQueueConstIterator<_CircularQueue>
{
public:
#ifdef __cpp_lib_concepts
	using iterator_concept = std::random_access_iterator_tag;
#endif // __cpp_lib_concepts
	using iterator_category = std::random_access_iterator_tag;

	using CircularQueue = _CircularQueue;
	using value_type = typename CircularQueue::value_type;

	using size_type = typename CircularQueue::size_type;
	using difference_type = typename CircularQueue::difference_type;

	using pointer = typename CircularQueue::pointer;
	using reference = value_type&;

	using const_iterator = typename CircularQueue::const_iterator;

public:
	friend CircularQueueIterator operator+(difference_type _offset, \
		const CircularQueueIterator& _iterator)
	{
		return _iterator + _offset;
	}

public:
	CONSTEXPR CircularQueueIterator() noexcept = default;

	CONSTEXPR CircularQueueIterator(const const_iterator& _iterator) noexcept : \
		const_iterator(_iterator) {}

	CONSTEXPR CircularQueueIterator(const CircularQueue* _queue, \
		size_type _offset) noexcept : const_iterator(_queue, _offset) {}

	NODISCARD CONSTEXPR reference operator*()
	{
		return const_cast<reference>(const_iterator::operator*());
	}

	NODISCARD CONSTEXPR pointer operator->()
	{
		return const_cast<pointer>(const_iterator::operator->());
	}

	CONSTEXPR CircularQueueIterator& operator++()
	{
		const_iterator::operator++();
		return *this;
	}

	CONSTEXPR CircularQueueIterator operator++(int);

	CONSTEXPR CircularQueueIterator& operator--()
	{
		const_iterator::operator--();
		return *this;
	}

	CONSTEXPR CircularQueueIterator operator--(int);

	CONSTEXPR CircularQueueIterator& operator+=(difference_type _offset)
	{
		const_iterator::operator+=(_offset);
		return *this;
	}

	CONSTEXPR CircularQueueIterator& operator-=(difference_type _offset)
	{
		const_iterator::operator-=(_offset);
		return *this;
	}

	NODISCARD CONSTEXPR CircularQueueIterator operator+(difference_type _offset) const
	{
		auto iterator = *this;
		return iterator += _offset;
	}

	NODISCARD CONSTEXPR CircularQueueIterator operator-(difference_type _offset) const
	{
		auto iterator = *this;
		return iterator -= _offset;
	}

	using const_iterator::operator-;

	NODISCARD CONSTEXPR reference operator[](difference_type _offset)
	{
		return const_cast<reference>(const_iterator::operator[](_offset));
	}
};

template<typename _Element, typename _Allocator = std::allocator<_Element>>
class CircularQueue
{
	struct ValueTag
	{
		explicit ValueTag() = default;
	};

public:
	using value_type = _Element;
	using allocator_type = _Allocator;

private:
	using AllocatorTraits = std::allocator_traits<allocator_type>;

public:
	using size_type = std::size_t;
	using difference_type = std::ptrdiff_t;

	using reference = value_type&;
	using const_reference = const value_type&;
	using pointer = typename AllocatorTraits::pointer;
	using const_pointer = typename AllocatorTraits::const_pointer;

	using iterator = CircularQueueIterator<CircularQueue>;
	using const_iterator = CircularQueueConstIterator<CircularQueue>;
	using reverse_iterator = std::reverse_iterator<iterator>;
	using const_reverse_iterator = std::reverse_iterator<const_iterator>;

private:
	allocator_type _allocator;
	pointer _pointer;
	size_type _capacity;

	size_type _size;
	size_type _head;
	size_type _tail;

private:
	friend const_pointer const_iterator::operator->() const;

private:
	static CONSTEXPR void construct(pointer _pointer, const value_type& _value);

	static CONSTEXPR void construct(pointer _pointer, value_type&& _value);

	template <typename _ValueType>
	static CONSTEXPR void construct(pointer _pointer, \
		size_type _count, const _ValueType& _value);

	static CONSTEXPR void deconstruct(pointer _pointer)
	{
		if constexpr (not is_base_v<value_type>)
			std::destroy_at(_pointer);
	}

	static CONSTEXPR void deconstruct(pointer _pointer, size_type _count) noexcept;

	static CONSTEXPR void move(pointer _left, pointer _right);

private:
	NODISCARD CONSTEXPR value_type* allocate(size_type _size)
	{
		return _allocator.allocate(_size);
	}

	CONSTEXPR void deallocate(value_type* _pointer, size_type _size) noexcept;

	template <typename _Iterator>
	CONSTEXPR void construct(size_type _offset, \
		_Iterator _first, _Iterator _last);

	CONSTEXPR void initialize() noexcept;

	NODISCARD CONSTEXPR bool forward(size_type _offset, \
		size_type _size = 0) const noexcept;

	NODISCARD CONSTEXPR size_type offset(size_type _position, \
		size_type _offset, bool _forward) const noexcept;

	template <typename _Iterator>
	CONSTEXPR void assign(size_type _size, _Iterator _first, _Iterator _last);

	template <typename _Iterator>
	CONSTEXPR void move(size_type _offset, \
		bool _forward, _Iterator _first, _Iterator _last);

	CONSTEXPR void move(const_iterator _iterator, \
		size_type _offset, bool _outward, bool _forward);

	CONSTEXPR void move(pointer _pointer, \
		size_type _size, bool _partial = false);

	CONSTEXPR void expand(size_type _size);

	CONSTEXPR void adjust(size_type _size) noexcept;

	template <typename _ValueType>
	CONSTEXPR void adjust(size_type _size, const _ValueType& _value);

	template <typename _Iterator>
	CONSTEXPR iterator insert(const_iterator _where, \
		size_type _size, _Iterator _first, _Iterator _last);

public:
	CONSTEXPR CircularQueue() noexcept(std::is_nothrow_default_constructible_v<allocator_type>) : \
		_pointer(nullptr), _capacity(0), _size(0), _head(0), _tail(0) {}

	CONSTEXPR explicit CircularQueue(const allocator_type& _allocator) noexcept : \
		_allocator(_allocator), _pointer(nullptr), _capacity(0), _size(0), _head(0), _tail(0) {}

	CONSTEXPR explicit CircularQueue(size_type _count, \
		const allocator_type& _allocator = allocator_type()) : \
		_allocator(_allocator), _pointer(nullptr), _capacity(0), \
		_size(0), _head(0), _tail(0)
	{
		resize(_count);
	}

	CONSTEXPR CircularQueue(size_type _count, const value_type& _value, \
		const allocator_type& _allocator = allocator_type()) : \
		_allocator(_allocator), _pointer(nullptr), _capacity(0), \
		_size(0), _head(0), _tail(0)
	{
		resize(_count, _value);
	}

	template <typename _Iterator>
	CONSTEXPR CircularQueue(_Iterator _first, _Iterator _last, \
		const allocator_type& _allocator = allocator_type());

	CONSTEXPR CircularQueue(std::initializer_list<value_type> _list, \
		const allocator_type& _allocator = allocator_type()) : \
		_allocator(_allocator), _pointer(nullptr), _capacity(0), \
		_size(0), _head(0), _tail(0)
	{
		assign(_list.size(), _list.begin(), _list.end());
	}

	CONSTEXPR CircularQueue(const CircularQueue& _another) : \
		_pointer(nullptr), _capacity(0), _size(0), _head(0), _tail(0)
	{
		assign(_another.size(), _another.begin(), _another.end());
	}

	CONSTEXPR CircularQueue(const CircularQueue& _another, \
		const allocator_type& _allocator) : \
		_allocator(_allocator), _pointer(nullptr), _capacity(0), \
		_size(0), _head(0), _tail(0)
	{
		assign(_another.size(), _another.begin(), _another.end());
	}

	CONSTEXPR CircularQueue(CircularQueue&& _another) noexcept : \
		_allocator(std::move(_another._allocator)), \
		_pointer(_another._pointer), _capacity(_another.capacity()), \
		_size(_another.size()), _head(_another._head), _tail(_another._tail)
	{
		_another.initialize();
	}

	CONSTEXPR CircularQueue(CircularQueue&& _another, \
		const allocator_type& _allocator);

	CONSTEXPR ~CircularQueue() noexcept;

	CONSTEXPR CircularQueue& operator=(const CircularQueue& _queue);

	CONSTEXPR CircularQueue& operator=(CircularQueue&& _queue) noexcept;

	CONSTEXPR CircularQueue& operator=(std::initializer_list<value_type> _list);

	NODISCARD CONSTEXPR reference operator[](size_type _position)
	{
		return at(_position);
	}

	NODISCARD CONSTEXPR const_reference operator[](size_type _position) const
	{
		return at(_position);
	}

	CONSTEXPR void assign(size_type _count, const value_type& _value);

	template <typename _Iterator>
	CONSTEXPR void assign(_Iterator _first, _Iterator _last);

	CONSTEXPR void assign(std::initializer_list<value_type> _list)
	{
		clear();
		assign(_list.size(), _list.begin(), _list.end());
	}

	NODISCARD CONSTEXPR allocator_type get_allocator() const noexcept
	{
		return _allocator;
	}

	NODISCARD CONSTEXPR reference at(size_type _position);

	NODISCARD CONSTEXPR const_reference at(size_type _position) const;

	NODISCARD CONSTEXPR reference front();

	NODISCARD CONSTEXPR const_reference front() const;

	NODISCARD CONSTEXPR reference back();

	NODISCARD CONSTEXPR const_reference back() const;

	NODISCARD CONSTEXPR iterator begin() noexcept
	{
		return iterator(this, 0);
	}

	NODISCARD CONSTEXPR const_iterator begin() const noexcept
	{
		return const_iterator(this, 0);
	}

	NODISCARD CONSTEXPR iterator end() noexcept
	{
		return iterator(this, size());
	}

	NODISCARD CONSTEXPR const_iterator end() const noexcept
	{
		return const_iterator(this, size());
	}

	NODISCARD CONSTEXPR reverse_iterator rbegin() noexcept
	{
		return reverse_iterator(end());
	}

	NODISCARD CONSTEXPR const_reverse_iterator rbegin() const noexcept
	{
		return const_reverse_iterator(end());
	}

	NODISCARD CONSTEXPR reverse_iterator rend() noexcept
	{
		return reverse_iterator(begin());
	}

	NODISCARD CONSTEXPR const_reverse_iterator rend() const noexcept
	{
		return const_reverse_iterator(begin());
	}

	NODISCARD CONSTEXPR const_iterator cbegin() const noexcept
	{
		return begin();
	}

	NODISCARD CONSTEXPR const_iterator cend() const noexcept
	{
		return end();
	}

	NODISCARD CONSTEXPR const_reverse_iterator crbegin() const noexcept
	{
		return rbegin();
	}

	NODISCARD CONSTEXPR const_reverse_iterator crend() const noexcept
	{
		return rend();
	}

	NODISCARD CONSTEXPR bool empty() const noexcept { return size() == 0; }

	NODISCARD CONSTEXPR size_type size() const noexcept { return _size; }

	NODISCARD CONSTEXPR size_type capacity() const noexcept { return _capacity; }

	NODISCARD CONSTEXPR size_type max_size() const noexcept;

	CONSTEXPR void reserve(size_type _capacity);

	CONSTEXPR void shrink_to_fit();

	CONSTEXPR void clear() noexcept;

	CONSTEXPR iterator insert(const_iterator _where, const value_type& _value);

	CONSTEXPR iterator insert(const_iterator _where, value_type&& _value);

	CONSTEXPR iterator insert(const_iterator _where, \
		size_type _count, const value_type& _value);

	template <typename _Iterator>
	CONSTEXPR iterator insert(const_iterator _where, \
		_Iterator _first, _Iterator _last);

	CONSTEXPR iterator insert(const_iterator _where, \
		std::initializer_list<value_type> _list);

	template <typename... _Args>
	CONSTEXPR iterator emplace(const_iterator _where, _Args&&... _args);

	CONSTEXPR iterator erase(const_iterator _where);

	CONSTEXPR iterator erase(const_iterator _first, const_iterator _last);

	template <typename _ValueType>
	CONSTEXPR size_type erase(const _ValueType& _value);

	template <typename _Predicate>
	CONSTEXPR size_type erase_if(_Predicate _predicate);

	CONSTEXPR void push_front(const value_type& _value);

	CONSTEXPR void push_front(value_type&& _value);

	template <typename... _Args>
	CONSTEXPR reference emplace_front(_Args&&... _args);

	CONSTEXPR void pop_front();

	CONSTEXPR void push_back(const value_type& _value);

	CONSTEXPR void push_back(value_type&& _value);

	template <typename... _Args>
	CONSTEXPR reference emplace_back(_Args&&... _args);

	CONSTEXPR void pop_back();

	CONSTEXPR void resize(size_type _count);

	CONSTEXPR void resize(size_type _count, const value_type& _value);

	CONSTEXPR void swap(CircularQueue& _queue) noexcept;
};

template <typename _CircularQueue>
NODISCARD CONSTEXPR auto CircularQueueConstIterator<_CircularQueue>::operator->() const \
-> pointer
{
	if (_offset >= _queue->size()) throw std::out_of_range(DEREFERENCE_OUT_OF_RANGE);

	auto offset = _queue->offset(_queue->_head, _offset, true);
	return _queue->_pointer + offset;
}

template <typename _CircularQueue>
CONSTEXPR auto CircularQueueConstIterator<_CircularQueue>::operator++() \
-> CircularQueueConstIterator&
{
	if (_offset >= _queue->size())
		throw std::out_of_range(INCREMENT_OUT_OF_RANGE);

	++_offset;
	return *this;
}

template <typename _CircularQueue>
CONSTEXPR auto CircularQueueConstIterator<_CircularQueue>::operator++(int) \
-> CircularQueueConstIterator
{
	auto iterator = *this;
	++*this;
	return iterator;
}

template <typename _CircularQueue>
CONSTEXPR auto CircularQueueConstIterator<_CircularQueue>::operator--() \
-> CircularQueueConstIterator&
{
	if (_offset <= 0) throw std::out_of_range(DECREMENT_OUT_OF_RANGE);

	--_offset;
	return *this;
}

template <typename _CircularQueue>
CONSTEXPR auto CircularQueueConstIterator<_CircularQueue>::operator--(int) \
-> CircularQueueConstIterator
{
	auto iterator = *this;
	--*this;
	return iterator;
}

template <typename _CircularQueue>
CONSTEXPR auto CircularQueueConstIterator<_CircularQueue>::operator+=(difference_type _offset) \
-> CircularQueueConstIterator&
{
	if (_offset >= 0)
	{
		if ((static_cast<size_type>(_offset) > _queue->size() - this->_offset))
			throw std::out_of_range(SEEK_AFTER_END);
	}
	else
	{
		if ((static_cast<size_type>(-_offset) > this->_offset))
			throw std::out_of_range(SEEK_BEFORE_BEGIN);
	}

	this->_offset += _offset;
	return *this;
}

template <typename _CircularQueue>
NODISCARD CONSTEXPR auto CircularQueueConstIterator<_CircularQueue>::operator-(const CircularQueueConstIterator& _iterator) const \
-> difference_type
{
	verifyContainer(*this, _iterator);

	if (this->_offset >= _iterator._offset)
		return static_cast<difference_type>(this->_offset - _iterator._offset);
	return -static_cast<difference_type>(_iterator._offset - this->_offset);
}

template <typename _CircularQueue>
CONSTEXPR auto CircularQueueIterator<_CircularQueue>::operator++(int) \
-> CircularQueueIterator
{
	auto iterator = *this;
	++*this;
	return iterator;
}

template <typename _CircularQueue>
CONSTEXPR auto CircularQueueIterator<_CircularQueue>::operator--(int) \
-> CircularQueueIterator
{
	auto iterator = *this;
	--*this;
	return iterator;
}

template <typename _Element, typename _Allocator>
CONSTEXPR void CircularQueue<_Element, _Allocator>::construct(pointer _pointer, \
	const value_type& _value)
{
	if constexpr (is_base_v<value_type>)
		*_pointer = _value;
	else
		std::construct_at(_pointer, _value);
}

template <typename _Element, typename _Allocator>
CONSTEXPR void CircularQueue<_Element, _Allocator>::construct(pointer _pointer, \
	value_type&& _value)
{
	if constexpr (is_base_v<value_type>)
		*_pointer = std::forward<value_type>(_value);
	else
		std::construct_at(_pointer, std::forward<value_type>(_value));
}

template <typename _Element, typename _Allocator>
template <typename _ValueType>
CONSTEXPR void CircularQueue<_Element, _Allocator>::construct(pointer _pointer, \
	size_type _count, const _ValueType& _value)
{
	if constexpr (std::is_same_v<_ValueType, ValueTag>)
	{
		//if (_count > 0)
		//{
		//	auto size = sizeof(value_type) * _count;
		//	std::memset(_pointer, 0, size);
		//}
	}
	else
	{
		for (decltype(_count) index = 0; index < _count; ++index)
			construct(_pointer + index, _value);
	}
}

template <typename _Element, typename _Allocator>
CONSTEXPR void CircularQueue<_Element, _Allocator>::deconstruct(pointer _pointer, \
	size_type _count) noexcept
{
	if constexpr (not is_base_v<value_type>)
		for (decltype(_count) index = 0; index < _count; ++index)
		{
			try
			{
				std::destroy_at(_pointer + index);
			}
			catch (std::exception&) {}
		}
}

template <typename _Element, typename _Allocator>
CONSTEXPR void CircularQueue<_Element, _Allocator>::move(pointer _left, \
	pointer _right)
{
	if constexpr (is_base_v<value_type>)
		*_left = *_right;
	if constexpr (std::is_move_constructible_v<value_type>)
	{
		//new(_left)value_type(std::move(*_right));
		std::construct_at(_left, std::move(*_right));
	}
	else
	{
		//new(_left)value_type(*_right);
		std::construct_at(_left, *_right);
	}

	if constexpr (not is_base_v<value_type>)
	{
		//_right->~value_type();
		std::destroy_at(_right);
	}
}

template <typename _Element, typename _Allocator>
CONSTEXPR void CircularQueue<_Element, _Allocator>::deallocate(value_type* _pointer, \
	size_type _size) noexcept
{
	try
	{
		_allocator.deallocate(_pointer, _size);
	}
	catch (std::exception&) {}
}

template <typename _Element, typename _Allocator>
template <typename _Iterator>
CONSTEXPR void CircularQueue<_Element, _Allocator>::construct(size_type _offset, \
	_Iterator _first, _Iterator _last)
{
	auto capacity = this->capacity();
	for (auto iterator = _first; iterator != _last; ++iterator)
	{
		construct(_pointer + _offset, *iterator);
		++_offset %= capacity;
	}
}

template <typename _Element, typename _Allocator>
CONSTEXPR void CircularQueue<_Element, _Allocator>::initialize() noexcept
{
	_pointer = nullptr;
	_capacity = _size = 0;
	_head = _tail = 0;
}

template <typename _Element, typename _Allocator>
NODISCARD CONSTEXPR bool CircularQueue<_Element, _Allocator>::forward(size_type _offset, \
	size_type _size) const noexcept
{
	auto count = _offset;
	if (count >= _head) count -= _head;
	else count += capacity() - _head;

	auto forward = count > (size() - _size) / 2;
	return _size == 0 ? forward : not forward;
}

template <typename _Element, typename _Allocator>
NODISCARD CONSTEXPR auto CircularQueue<_Element, _Allocator>::offset(size_type _position, \
	size_type _offset, bool _forward) const noexcept -> size_type
{
	if (_forward)
	{
		auto offset = capacity() - _position;
		return _offset < offset ? _position + _offset : _offset - offset;
	}

	if (_offset > _position)
	{
		_offset -= _position;
		_position = capacity();
	}
	return _position - _offset;
}

template <typename _Element, typename _Allocator>
template <typename _Iterator>
CONSTEXPR void CircularQueue<_Element, _Allocator>::assign(size_type _size, \
	_Iterator _first, _Iterator _last)
{
	if (_size > 0)
	{
		expand(_size);
		construct(0, _first, _last);
		this->_size = _tail = _size;
		_tail %= capacity();
	}
}

template <typename _Element, typename _Allocator>
template <typename _Iterator>
CONSTEXPR void CircularQueue<_Element, _Allocator>::move(size_type _offset, \
	bool _forward, _Iterator _first, _Iterator _last)
{
	for (auto iterator = _first; iterator != _last; ++iterator)
	{
		if (not _forward)
		{
			if (_offset <= 0) _offset = capacity();
			--_offset;
		}

		move(_pointer + _offset, const_cast<pointer>(iterator.operator->()));

		if (_forward) ++_offset %= capacity();
	}
}

template <typename _Element, typename _Allocator>
CONSTEXPR void CircularQueue<_Element, _Allocator>::move(const_iterator _iterator, \
	size_type _offset, bool _outward, bool _forward)
{
	if (_outward)
	{
		if (_forward)
		{
			auto offset = this->offset(_tail, _offset, _forward);
			auto end = const_reverse_iterator(_iterator);
			move(offset, not _forward, crbegin(), end);

			_tail = offset;
		}
		else
		{
			auto offset = this->offset(_head, _offset, _forward);
			auto end = _iterator;
			move(offset, not _forward, cbegin(), end);

			_head = offset;
		}

		_size += _offset;
	}
	else
	{
		if (_forward)
		{
			auto offset = this->offset(_head, _iterator._offset, true);
			offset = this->offset(offset, _offset, _forward);
			auto begin = const_reverse_iterator(_iterator);
			move(offset, not _forward, begin, crend());

			_head = this->offset(_head, _offset, _forward);
		}
		else
		{
			auto offset = this->offset(_head, _iterator._offset, true);
			auto begin = _iterator + _offset;
			move(offset, not _forward, begin, cend());

			_tail = this->offset(_tail, _offset, _forward);
		}

		_size -= _offset;
	}
}

template <typename _Element, typename _Allocator>
CONSTEXPR void CircularQueue<_Element, _Allocator>::move(pointer _pointer, \
	size_type _size, bool _partial)
{
	if (empty())
		_head = _tail = 0;
	else
	{
		decltype(_size) counter = 0;
		if (_head < _tail)
		{
			if constexpr (is_base_v<value_type>)
			{
				counter = _tail - _head;
				auto size = sizeof(value_type) * counter;
				std::memcpy(_pointer, this->_pointer + _head, size);
			}
			else
			{
				for (auto index = _head; index < _tail; ++index)
					move(_pointer + counter++, this->_pointer + index);
			}
		}
		else
		{
			if constexpr (is_base_v<value_type>)
			{
				counter = capacity() - _head;
				auto size = sizeof(value_type) * counter;
				std::memcpy(_pointer, this->_pointer + _head, size);

				size = sizeof(value_type) * _tail;
				std::memcpy(_pointer + counter, this->_pointer, size);

				counter += _tail;
			}
			else
			{
				for (auto index = _head; index < capacity(); ++index)
					move(_pointer + counter++, this->_pointer + index);

				for (decltype(_tail) index = 0; index < _tail; ++index)
					move(_pointer + counter++, this->_pointer + index);
			}
		}

		_head = 0;
		_tail = counter % _size;
	}

	if (not _partial)
	{
		deallocate(this->_pointer, capacity());
		this->_pointer = _pointer;
		_capacity = _size;
	}
}

template <typename _Element, typename _Allocator>
CONSTEXPR void CircularQueue<_Element, _Allocator>::expand(size_type _size)
{
	auto capacity = this->capacity();
	auto size = capacity - this->size();
	if (_size <= size) return;

	_size = std::max(capacity / 2, _size - size);
	if (_size > max_size() - capacity)
		throw std::length_error("expansion size exceeds maximum size");

	reserve(capacity + _size);
}

template <typename _Element, typename _Allocator>
CONSTEXPR void CircularQueue<_Element, _Allocator>::adjust(size_type _size) noexcept
{
	auto size = this->size() - _size;
	if (_head < _tail)
	{
		_tail -= size;
		deconstruct(_pointer + _tail, size);
	}
	else
	{
		auto count = std::min(size, _tail);
		_tail -= count;
		deconstruct(_pointer + _tail, count);

		if (count < size)
		{
			count = size - count;
			_tail = capacity() - count;
			deconstruct(_pointer + _tail, count);
		}
	}

	this->_size = _size;
}

template <typename _Element, typename _Allocator>
template <typename _ValueType>
CONSTEXPR void CircularQueue<_Element, _Allocator>::adjust(size_type _size, \
	const _ValueType& _value)
{
	auto capacity = this->capacity();
	if (_size > capacity) expand(_size - capacity);

	auto size = _size - this->size();
	if (_head > _tail)
	{
		auto count = std::min(size, _head - _tail);
		construct(_pointer + _tail, count, _value);
		_tail += count;
	}
	else
	{
		auto count = std::min(size, capacity - _tail);
		construct(_pointer + _tail, count, _value);

		if (count >= size)
			_tail = offset(_tail, count, true);
		else
		{
			count = size - count;
			construct(_pointer, count, _value);
			_tail = count;
		}
	}

	this->_size = _size;
}

template <typename _Element, typename _Allocator>
template <typename _Iterator>
CONSTEXPR auto CircularQueue<_Element, _Allocator>::insert(const_iterator _where, \
	size_type _size, _Iterator _first, _Iterator _last) -> iterator
{
	if (_size <= 0) return _where;

	expand(_size);

	auto offset = this->offset(_head, _where._offset, true);
	move(_where, _size, true, forward(offset));

	offset = this->offset(_head, _where._offset, true);
	construct(offset, _first, _last);
	return _where;
}

template <typename _Element, typename _Allocator>
template <typename _Iterator>
CONSTEXPR CircularQueue<_Element, _Allocator>::CircularQueue(_Iterator _first, _Iterator _last, \
	const allocator_type& _allocator) : \
	_allocator(_allocator), _pointer(nullptr), _capacity(0), _size(0), _head(0), _tail(0)
{
	auto count = std::distance(_first, _last);
	if (count < 0) count = -count;
	assign(static_cast<size_type>(count), _first, _last);
}

template <typename _Element, typename _Allocator>
CONSTEXPR CircularQueue<_Element, _Allocator>::CircularQueue(CircularQueue&& _another, \
	const allocator_type& _allocator) : \
	_allocator(_allocator), _pointer(nullptr), _capacity(0), _size(0), _head(0), _tail(0)
{
	auto size = _another.size();
	this->reserve(size);

	_another.move(this->_pointer, size, true);
	this->_size = size;
	this->_head = _another._head;
	this->_tail = _another._tail;

	_another.deallocate(_another._pointer, _another.capacity());
	_another.initialize();
}

template <typename _Element, typename _Allocator>
CONSTEXPR CircularQueue<_Element, _Allocator>::~CircularQueue() noexcept
{
	if (_pointer != nullptr)
	{
		clear();
		deallocate(_pointer, capacity());
	}
}

template <typename _Element, typename _Allocator>
CONSTEXPR auto CircularQueue<_Element, _Allocator>::operator=(const CircularQueue& _queue) \
-> CircularQueue&
{
	if (this != &_queue)
	{
		this->clear();
		this->assign(_queue.size(), _queue.begin(), _queue.end());
	}
	return *this;
}

template <typename _Element, typename _Allocator>
CONSTEXPR auto CircularQueue<_Element, _Allocator>::operator=(CircularQueue&& _queue) noexcept \
-> CircularQueue&
{
	if (this != &_queue)
	{
		this->clear();
		this->deallocate(this->_pointer, this->capacity());

		this->_allocator = std::move(_queue._allocator);
		this->_pointer = _queue._pointer;
		this->_capacity = _queue.capacity();

		this->_size = _queue.size();
		this->_head = _queue._head;
		this->_tail = _queue._tail;

		_queue.initialize();
	}
	return *this;
}

template <typename _Element, typename _Allocator>
CONSTEXPR auto CircularQueue<_Element, _Allocator>::operator=(std::initializer_list<value_type> _list) \
-> CircularQueue&
{
	clear();
	assign(_list.size(), _list.begin(), _list.end());
	return *this;
}

template <typename _Element, typename _Allocator>
CONSTEXPR void CircularQueue<_Element, _Allocator>::assign(size_type _count, \
	const value_type& _value)
{
	clear();

	if (_count > 0)
	{
		expand(_count);
		construct(_pointer, _count, _value);
		_size = _tail = _count;
		_tail %= capacity();
	}
}

template <typename _Element, typename _Allocator>
template <typename _Iterator>
CONSTEXPR void CircularQueue<_Element, _Allocator>::assign(_Iterator _first, _Iterator _last)
{
	clear();

	auto count = std::distance(_first, _last);
	if (count < 0) count = -count;
	assign(static_cast<size_type>(count), _first, _last);
}

template <typename _Element, typename _Allocator>
NODISCARD CONSTEXPR auto CircularQueue<_Element, _Allocator>::at(size_type _position) -> reference
{
	auto functor = static_cast<const_reference(CircularQueue::*)(size_type) const>(&CircularQueue::at);
	auto& value = (this->*functor)(_position);
	return const_cast<reference>(value);
}

template <typename _Element, typename _Allocator>
NODISCARD CONSTEXPR auto CircularQueue<_Element, _Allocator>::at(size_type _position) const \
-> const_reference
{
	if (_position >= size()) throw std::out_of_range(SUBSCRIPT_OUT_OF_RANGE);

	auto offset = this->offset(_head, _position, true);
	return _pointer[offset];
}

template <typename _Element, typename _Allocator>
NODISCARD CONSTEXPR auto CircularQueue<_Element, _Allocator>::front() -> reference
{
	auto functor = static_cast<const_reference(CircularQueue::*)() const>(&CircularQueue::front);
	auto& value = (this->*functor)();
	return const_cast<reference>(value);
}

template <typename _Element, typename _Allocator>
NODISCARD CONSTEXPR auto CircularQueue<_Element, _Allocator>::front() const \
-> const_reference
{
	if (empty()) throw std::runtime_error(FRONT_ON_EMPTY_CONTAINER);
	return _pointer[_head];
}

template <typename _Element, typename _Allocator>
NODISCARD CONSTEXPR auto CircularQueue<_Element, _Allocator>::back() -> reference
{
	auto functor = static_cast<const_reference(CircularQueue::*)() const>(&CircularQueue::back);
	auto& value = (this->*functor)();
	return const_cast<reference>(value);
}

template <typename _Element, typename _Allocator>
NODISCARD CONSTEXPR auto CircularQueue<_Element, _Allocator>::back() const \
-> const_reference
{
	if (empty()) throw std::runtime_error(BACK_ON_EMPTY_CONTAINER);

	auto tail = _tail > 0 ? _tail : capacity();
	return _pointer[tail - 1];
}

template <typename _Element, typename _Allocator>
NODISCARD CONSTEXPR auto CircularQueue<_Element, _Allocator>::max_size() const noexcept \
-> size_type
{
	constexpr auto MAX_SIZE = static_cast<size_type>(-1) / sizeof(value_type);
	auto size = std::numeric_limits<difference_type>::max();
	return std::min(static_cast<size_type>(size), MAX_SIZE);
}

template <typename _Element, typename _Allocator>
CONSTEXPR void CircularQueue<_Element, _Allocator>::reserve(size_type _capacity)
{
	if (_capacity > max_size())
		throw std::length_error(RESERVE_EXCEED_MAXIMUM_SIZE);

	if (_capacity > capacity())
	{
		auto pointer = allocate(_capacity);
		move(pointer, _capacity);
	}
}

template <typename _Element, typename _Allocator>
CONSTEXPR void CircularQueue<_Element, _Allocator>::shrink_to_fit()
{
	auto size = this->size();
	if (size <= 0)
	{
		if (_pointer != nullptr)
		{
			deallocate(_pointer, capacity());
			initialize();
		}
		return;
	}

	if (size < capacity())
	{
		auto pointer = allocate(size);
		move(pointer, size);
	}
}

template <typename _Element, typename _Allocator>
CONSTEXPR void CircularQueue<_Element, _Allocator>::clear() noexcept
{
	if (empty()) return;

	if (_head < _tail)
		deconstruct(_pointer + _head, _tail - _head);
	else
	{
		deconstruct(_pointer + _head, capacity() - _head);
		deconstruct(_pointer, _tail);
	}

	_size = 0;
	_head = _tail = 0;
}

template <typename _Element, typename _Allocator>
CONSTEXPR auto CircularQueue<_Element, _Allocator>::insert(const_iterator _where, \
	const value_type& _value) -> iterator
{
	if (_where > cend()) throw std::out_of_range(INSERT_OUTSIDE_RANGE);

	expand(1);

	auto offset = this->offset(_head, _where._offset, true);
	move(_where, 1, true, forward(offset));

	offset = this->offset(_head, _where._offset, true);
	construct(_pointer + offset, _value);
	return _where;
}

template <typename _Element, typename _Allocator>
CONSTEXPR auto CircularQueue<_Element, _Allocator>::insert(const_iterator _where, \
	value_type&& _value) -> iterator
{
	if (_where > cend()) throw std::out_of_range(INSERT_OUTSIDE_RANGE);

	expand(1);

	auto offset = this->offset(_head, _where._offset, true);
	move(_where, 1, true, forward(offset));

	offset = this->offset(_head, _where._offset, true);
	construct(_pointer + offset, std::forward<value_type>(_value));
	return _where;
}

template <typename _Element, typename _Allocator>
CONSTEXPR auto CircularQueue<_Element, _Allocator>::insert(const_iterator _where, \
	size_type _count, const value_type& _value) -> iterator
{
	if (_where > cend()) throw std::out_of_range(INSERT_OUTSIDE_RANGE);

	if (_count <= 0) return _where;

	expand(_count);

	auto offset = this->offset(_head, _where._offset, true);
	move(_where, _count, true, forward(offset));

	offset = this->offset(_head, _where._offset, true);
	auto count = std::min(_count, capacity() - offset);
	construct(_pointer + offset, count, _value);

	if (count < _count)
		construct(_pointer, _count - count, _value);
	return _where;
}

template <typename _Element, typename _Allocator>
template <typename _Iterator>
CONSTEXPR auto CircularQueue<_Element, _Allocator>::insert(const_iterator _where, \
	_Iterator _first, _Iterator _last) -> iterator
{
	if (_where > cend()) throw std::out_of_range(INSERT_OUTSIDE_RANGE);

	auto count = std::distance(_first, _last);
	if (count < 0) count = -count;
	return insert(_where, static_cast<size_type>(count), _first, _last);
}

template <typename _Element, typename _Allocator>
CONSTEXPR auto CircularQueue<_Element, _Allocator>::insert(const_iterator _where, \
	std::initializer_list<value_type> _list) -> iterator
{
	if (_where > cend()) throw std::out_of_range(INSERT_OUTSIDE_RANGE);
	return insert(_where, _list.size(), _list.begin(), _list.end());
}

template <typename _Element, typename _Allocator>
template <typename... _Args>
CONSTEXPR auto CircularQueue<_Element, _Allocator>::emplace(const_iterator _where, \
	_Args&&... _args) -> iterator
{
	if (_where > cend()) throw std::out_of_range(EMPLACE_OUTSIDE_RANGE);

	expand(1);

	auto offset = this->offset(_head, _where._offset, true);
	move(_where, 1, true, forward(offset));

	offset = this->offset(_head, _where._offset, true);
	std::construct_at(_pointer + offset, std::forward<_Args>(_args)...);
	return _where;
}

template <typename _Element, typename _Allocator>
CONSTEXPR auto CircularQueue<_Element, _Allocator>::erase(const_iterator _where) \
-> iterator
{
	if (_where >= cend()) throw std::out_of_range(ERASE_OUTSIDE_RANGE);

	auto offset = this->offset(_head, _where._offset, true);
	deconstruct(_pointer + offset);

	move(_where, 1, false, forward(offset, 1));
	return _where;
}

template <typename _Element, typename _Allocator>
CONSTEXPR auto CircularQueue<_Element, _Allocator>::erase(const_iterator _first, \
	const_iterator _last) -> iterator
{
	verifyRange(_first, _last);

	auto end = cend();
	if (_first >= end || _last > end) throw std::out_of_range(ERASE_OUTSIDE_RANGE);

	auto difference = _last - _first;
	if (difference <= 0) return _first;

	auto size = static_cast<size_type>(difference);
	auto offset = this->offset(_head, _first._offset, true);
	auto count = std::min(size, capacity() - offset);
	deconstruct(_pointer + offset, count);

	if (count < size)
		deconstruct(_pointer, size - count);

	move(_first, size, false, forward(offset, size));
	return _first;
}

template <typename _Element, typename _Allocator>
template <typename _ValueType>
CONSTEXPR auto CircularQueue<_Element, _Allocator>::erase(const _ValueType& _value) \
-> size_type
{
	auto equal = [&_value](const value_type& _element)
	{
		return _element == _value;
	};

	return erase_if(equal);
}

//template <typename _Element, typename _Allocator>
//template <typename _Predicate>
//CONSTEXPR auto CircularQueue<_Element, _Allocator>::erase_if(_Predicate _predicate) \
//-> size_type
//{
//	if (empty()) return 0;
//
//	size_type counter = 0;
//
//	auto capacity = this->capacity();
//	for (auto index = _head, offset = capacity; index != _tail; ++index %= capacity)
//	{
//		auto pointer = _pointer + index;
//		if (_predicate(*pointer))
//		{
//			deconstruct(pointer);
//			if (offset == capacity) offset = index;
//			++counter;
//			continue;
//		}
//
//		if (offset != capacity and index != offset)
//		{
//			move(_pointer + offset, _pointer + index);
//			++offset %= capacity;
//		}
//	}
//
//	_tail = offset(_tail, counter, false);
//	_size -= counter;
//	return counter;
//}

template <typename _Element, typename _Allocator>
template <typename _Predicate>
CONSTEXPR auto CircularQueue<_Element, _Allocator>::erase_if(_Predicate _predicate) \
-> size_type
{
	if (empty()) return 0;

	size_type size = 0;
	auto capacity = this->capacity();
	auto position = offset(_head, this->size() / 2, true);

	for (auto index = this->offset(position, 1, false), \
		cursor = this->offset(_head, 1, false), offset = capacity; \
		index != cursor; index = this->offset(index, 1, false))
	{
		auto pointer = _pointer + index;
		if (_predicate(*pointer))
		{
			deconstruct(pointer);
			if (offset == capacity) offset = index;
			++size;
			continue;
		}

		if (offset != capacity and index != offset)
		{
			move(_pointer + offset, _pointer + index);
			offset = this->offset(offset, 1, false);
		}
	}

	_head = offset(_head, size, true);

	size_type counter = 0;

	for (auto index = position, offset = capacity; \
		index != _tail; ++index %= capacity)
	{
		auto pointer = _pointer + index;
		if (_predicate(*pointer))
		{
			deconstruct(pointer);
			if (offset == capacity) offset = index;
			++counter;
			continue;
		}

		if (offset != capacity and index != offset)
		{
			move(_pointer + offset, _pointer + index);
			++offset %= capacity;
		}
	}

	_tail = offset(_tail, counter, false);

	counter += size;
	_size -= counter;
	return counter;
}

template <typename _Element, typename _Allocator>
CONSTEXPR void CircularQueue<_Element, _Allocator>::push_front(const value_type& _value)
{
	expand(1);

	if (_head <= 0) _head = capacity();
	--_head;

	construct(_pointer + _head, _value);
	++_size;
}

template <typename _Element, typename _Allocator>
CONSTEXPR void CircularQueue<_Element, _Allocator>::push_front(value_type&& _value)
{
	expand(1);

	if (_head <= 0) _head = capacity();
	--_head;

	construct(_pointer + _head, std::forward<value_type>(_value));
	++_size;
}

template <typename _Element, typename _Allocator>
template <typename... _Args>
CONSTEXPR auto CircularQueue<_Element, _Allocator>::emplace_front(_Args&&... _args) \
-> reference
{
	expand(1);

	if (_head <= 0) _head = capacity();
	--_head;

	std::construct_at(_pointer + _head, std::forward<_Args>(_args)...);
	++_size;
	return front();
}

template <typename _Element, typename _Allocator>
CONSTEXPR void CircularQueue<_Element, _Allocator>::pop_front()
{
	if (empty())
		throw std::runtime_error(POP_FRONT_ON_EMPTY_CONTAINER);

	deconstruct(_pointer + _head);

	++_head %= capacity();
	--_size;
}

template <typename _Element, typename _Allocator>
CONSTEXPR void CircularQueue<_Element, _Allocator>::push_back(const value_type& _value)
{
	expand(1);
	construct(_pointer + _tail, _value);

	++_tail %= capacity();
	++_size;
}

template <typename _Element, typename _Allocator>
CONSTEXPR void CircularQueue<_Element, _Allocator>::push_back(value_type&& _value)
{
	expand(1);
	construct(_pointer + _tail, std::forward<value_type>(_value));

	++_tail %= capacity();
	++_size;
}

template <typename _Element, typename _Allocator>
template <typename... _Args>
CONSTEXPR auto CircularQueue<_Element, _Allocator>::emplace_back(_Args&&... _args) \
-> reference
{
	expand(1);
	std::construct_at(_pointer + _tail, std::forward<_Args>(_args)...);

	++_tail %= capacity();
	++_size;
	return back();
}

template <typename _Element, typename _Allocator>
CONSTEXPR void CircularQueue<_Element, _Allocator>::pop_back()
{
	if (empty())
		throw std::runtime_error(POP_BACK_ON_EMPTY_CONTAINER);

	if (_tail <= 0) _tail = capacity();
	--_tail;

	deconstruct(_pointer + _tail);
	--_size;
}

template <typename _Element, typename _Allocator>
CONSTEXPR void CircularQueue<_Element, _Allocator>::resize(size_type _count)
{
	auto size = this->size();
	if (_count == size) return;

	if (_count < size)
	{
		adjust(_count);
		return;
	}

	if constexpr (is_base_v<value_type>)
		adjust(_count, ValueTag());
	else
		adjust(_count, value_type());
}

template <typename _Element, typename _Allocator>
CONSTEXPR void CircularQueue<_Element, _Allocator>::resize(size_type _count, \
	const value_type& _value)
{
	auto size = this->size();
	if (_count == size) return;

	if (_count < size) adjust(_count);
	else adjust(_count, _value);
}

template <typename _Element, typename _Allocator>
CONSTEXPR void CircularQueue<_Element, _Allocator>::swap(CircularQueue& _queue) noexcept
{
	if (this != &_queue)
	{
		std::swap(this->_allocator, _queue._allocator);
		std::swap(this->_pointer, _queue._pointer);
		std::swap(this->_capacity, _queue._capacity);

		std::swap(this->_size, _queue._size);
		std::swap(this->_head, _queue._head);
		std::swap(this->_tail, _queue._tail);
	}
}

namespace std
{
	template <typename _Element, typename _Allocator>
	NODISCARD CONSTEXPR bool operator==(const CircularQueue<_Element, _Allocator>& _left, \
		const CircularQueue<_Element, _Allocator>& _right)
	{
		if (&_left == &_right) return true;
		if (_left.size() != _right.size()) return false;

		return equal(_left.begin(), _left.end(), _right.begin());
	}

#ifdef HAS_CXX20
	template <typename _Element, typename _Allocator>
	NODISCARD constexpr auto operator<=>(const CircularQueue<_Element, _Allocator>& _left, \
		const CircularQueue<_Element, _Allocator>& _right)
	{
		return lexicographical_compare_three_way(_left.begin(), _left.end(), \
			_right.begin(), _right.end());
	}

#else // HAS_CXX20
	template <typename _Element, typename _Allocator>
	NODISCARD bool operator!=(const CircularQueue<_Element, _Allocator>& _left, \
		const CircularQueue<_Element, _Allocator>& _right)
	{
		return not (_left == _right);
	}

	template <typename _Element, typename _Allocator>
	NODISCARD bool operator<(const CircularQueue<_Element, _Allocator>& _left, \
		const CircularQueue<_Element, _Allocator>& _right)
	{
		if (&_left == &_right) return false;

		return lexicographical_compare(_left.begin(), _left.end(), \
			_right.begin(), _right.end());
	}

	template <typename _Element, typename _Allocator>
	NODISCARD bool operator<=(const CircularQueue<_Element, _Allocator>& _left, \
		const CircularQueue<_Element, _Allocator>& _right)
	{
		return not (_right < _left);
	}

	template <typename _Element, typename _Allocator>
	NODISCARD bool operator>(const CircularQueue<_Element, _Allocator>& _left, \
		const CircularQueue<_Element, _Allocator>& _right)
	{
		return _right < _left;
	}

	template <typename _Element, typename _Allocator>
	NODISCARD bool operator>=(const CircularQueue<_Element, _Allocator>& _left, \
		const CircularQueue<_Element, _Allocator>& _right)
	{
		return not (_left < _right);
	}
#endif // HAS_CXX20

	template <typename _Element, typename _Allocator>
	CONSTEXPR void swap(CircularQueue<_Element, _Allocator>& _left, \
		CircularQueue<_Element, _Allocator>& _right) noexcept
	{
		_left.swap(_right);
	}

	template <typename _Element, typename _Allocator, typename _ValueType>
	CONSTEXPR typename CircularQueue<_Element, _Allocator>::size_type \
		erase(CircularQueue<_Element, _Allocator>& _queue, const _ValueType& _value)
	{
		return _queue.erase(_value);
	}

	template <typename _Element, typename _Allocator, typename _Predicate>
	CONSTEXPR typename CircularQueue<_Element, _Allocator>::size_type \
		erase_if(CircularQueue<_Element, _Allocator>& _queue, _Predicate _predicate)
	{
		return _queue.erase_if(_predicate);
	}
}
