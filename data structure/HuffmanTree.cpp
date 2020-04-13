#include <cstddef>
#include <functional>
#include <new>
#include <vector>
#include <queue>
#include <iostream>

/*
 * 哈夫曼树
 * 构建时间复杂度：nlogn
 * 叶子节点数量n
 * 主循环次数n-1
 * 优先级队列
 * 出入队时间复杂度：logn
 */
template <typename Type>
class HuffmanTree
{
	struct Node;
	Node *root;
public:
	using Functor = typename Node::Functor;//std::function<void(Type)>;
	HuffmanTree(const Type *weights, size_t size);
	~HuffmanTree();
	void create(const Type *weights, size_t size);
	void destroy();
	void traversal(const Functor &functor);
};

template <typename Type>
struct HuffmanTree<Type>::Node
{
	Type weight;
	Node *left;
	Node *right;

	using Functor = std::function<void(Type)>;
	Node(Type weight, Node *left = nullptr, Node *right = nullptr)
		: weight(weight), left(left), right(right) {}
	static void destroy(Node *root);
	static void traversal(Node *root, const Functor &functor);
};

template <typename Type>
void HuffmanTree<Type>::Node::destroy(Node *root)
{
	if (root == nullptr)
		return;

	destroy(root->left);
	destroy(root->right);
	delete root;
}

template <typename Type>
void HuffmanTree<Type>::Node::traversal(Node *root, const Functor &functor)
{
	if (root == nullptr)
		return;
	if (root->left == nullptr \
		&& root->right == nullptr)
	{
		functor(root->weight);
		return;
	}

	traversal(root->left, functor);
	traversal(root->right, functor);
}

template <typename Type>
HuffmanTree<Type>::HuffmanTree(const Type *weights, size_t size)
	: root(nullptr)
{
	create(weights, size);
}

template <typename Type>
HuffmanTree<Type>::~HuffmanTree()
{
	destroy();
}

struct Comparator
{
	template <typename Type>
	bool operator()(const Type &left, const Type &right)
	{
		// 较大值在后，相同值先入队列在前
		if (left->weight > right->weight)
			return true;
		return false;
	}
};

template <typename Type>
void HuffmanTree<Type>::create(const Type *weights, size_t size)
{
	if (root)
		destroy();
	if (size == 0)
		return;

	std::priority_queue<Node*, std::vector<Node*>, Comparator> queue;
	for (size_t index = 0; index < size; ++index)
	{
		const Type &weight = weights[index];
		queue.push(new Node(weight));
	}

	while (queue.size() > 1)
	{
		auto left = queue.top();
		queue.pop();
		auto right = queue.top();
		queue.pop();
		queue.push(new Node(left->weight + right->weight, left, right));
	}
	root = queue.top();
}

template <typename Type>
void HuffmanTree<Type>::destroy()
{
	Node::destroy(root);
	root = nullptr;
}

template <typename Type>
void HuffmanTree<Type>::traversal(const Functor &functor)
{
	Node::traversal(root, functor);
}

int main()
{
	int weights[] = { 1, 2, 3, 4, 5, 6, 7, 8, 9 };
	HuffmanTree<int> tree(weights, sizeof(weights) / sizeof(int));
	tree.traversal([](int weight)
	{
		std::cout << weight << ' ';
	});
	return 0;
}
