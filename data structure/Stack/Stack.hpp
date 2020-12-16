#pragma once

#include <cstddef>
#include <memory>
#include <atomic>

//template <typename Type>
//class Stack
//{
//	struct Node
//	{
//		std::shared_ptr<Type> data;
//		std::shared_ptr<Node> next;
//
//		Node(const Type& data)
//			: data(std::make_shared<Type>(data)) {}
//	};
//
//	std::shared_ptr<Node> head;
//public:
//	void push(const Type& data)
//	{
//		const std::shared_ptr<Node> node = std::make_shared<Node>(data);
//		node->next = std::atomic_load(&head);
//		while (!std::atomic_compare_exchange_weak(&head, &node->next, node));
//	}
//
//	std::shared_ptr<Type> pop()
//	{
//		std::shared_ptr<Node> node = std::atomic_load(&head);
//		while (node && !std::atomic_compare_exchange_weak(&head, &node, node->next));
//		return node ? node->data : std::shared_ptr<Type>();
//	}
//};

template <typename Type>
class Stack
{
	struct Node;
	struct counting_pointer
	{
		size_t external;
		Node* node;
	};

	struct Node
	{
		std::shared_ptr<Type> data;
		std::atomic<size_t> internal;
		counting_pointer next;

		Node(const Type& data)
			: data(std::make_shared<Type>(data)), internal(0) {}
	};

	std::atomic<counting_pointer> head;
private:
	void increase(counting_pointer& old_pointer)
	{
		counting_pointer new_pointer;
		do
		{
			new_pointer = old_pointer;
			++new_pointer.external;
		} while (!head.compare_exchange_strong(old_pointer, new_pointer, \
			std::memory_order_acquire, std::memory_order_relaxed));
		old_pointer.external = new_pointer.external;
	}
public:
	~Stack() { while (pop()); }

	void push(const Type& data)
	{
		counting_pointer pointer;
		pointer.external = 0;
		pointer.node = new Node(data);
		pointer.node->next = head.load(std::memory_order_relaxed);
		while (!head.compare_exchange_weak(pointer.node->next, pointer, \
			std::memory_order_release, std::memory_order_relaxed));
	}

	std::shared_ptr<Type> pop()
	{
		counting_pointer pointer = head.load(std::memory_order_relaxed);
		while (true)
		{
			increase(pointer);
			Node* const node = pointer.node;
			if (node == nullptr)
				return std::shared_ptr<Type>();
			if (head.compare_exchange_strong(pointer, node->next, std::memory_order_relaxed))
			{
				std::shared_ptr<Type> data;
				data.swap(node->data);
				const size_t counter = pointer.external - 1;
				if (node->internal.fetch_add(counter, std::memory_order_release) == -counter)
					delete node;
				return data;
			}
			else if (node->internal.fetch_sub(1, std::memory_order_relaxed) == 1)
			{
				node->internal.load(std::memory_order_acquire);
				delete node;
			}
		}
	}
};
