/*
* 文件：State.hpp
* 
* 版本：v1.0
* 作者：许聪
* 邮箱：2592419242@qq.com
* 创建日期：2021年01月07日
* 
* 摘要：
* 1.抽象思想：主动胜于被动。自发性组织，无需管理者。状态衍生多状态，多状态形成状态机。
* 3.多状态为状态机前提，多状态的虚函数支持状态机的递归接口。
*/

#pragma once

#include <cstddef>
#include <vector>

#include "Transition.hpp"

template <typename TransitionType, typename MessageType>
class State
{
public:
	typedef TransitionType transition_type;
	typedef MessageType message_type;

	virtual ~State() {};

	virtual void enter() = 0;
	virtual void exit() = 0;
	virtual transition_type handle(message_type message) = 0;
};

template <typename IDType, typename MessageType, typename SizeType = size_t>
class MultiState : public State<Transition<IDType, MultiState<IDType, MessageType, SizeType>>, MessageType>
{
public:
	typedef IDType id_type;
	typedef MultiState state_type;
	typedef SizeType size_type;

	typedef struct Node
	{
		id_type id;
		state_type* state;
		size_type number;
	} node_type;

	virtual void enter() {};
	virtual void exit() {};

	virtual bool startDistribution(const std::vector<node_type>& nodes, size_type& cursor)
	{
		++cursor;
		return true;
	}
	virtual bool distribute(const std::vector<node_type>& nodes, size_type cursor = 0)
	{
		return startDistribution(nodes, cursor);
	}
	static void initCollection(std::vector<node_type>& nodes)
	{
		nodes.push_back({ 0, NULL });
	}
	virtual void startCollection(std::vector<node_type>& nodes, size_type& cursor)
	{
		node_type& node = nodes[cursor];
		node.number = 1;
	}
	virtual bool collect(std::vector<node_type>& nodes, size_type cursor = 0)
	{
		if (cursor != nodes.size())
			return false;

		initCollection(nodes);
		startCollection(nodes, cursor);
		return true;
	}

	virtual void backup(std::vector<id_type>& stack) {}
	virtual bool recover(const std::vector<id_type>& stack, size_type level = 0)
	{
		return true;
	}

	virtual bool addState(id_type id, state_type* state, const std::vector<id_type>& ids, size_type level = 0)
	{
		return false;
	}
	virtual state_type* removeState(id_type id, const std::vector<id_type>& ids, size_type level = 0)
	{
		return NULL;
	}
	virtual bool setState(id_type id, const std::vector<id_type>& ids, size_type level = 0)
	{
		return false;
	}
	virtual state_type* getState(id_type id, const std::vector<id_type>& ids, size_type level = 0)
	{
		return NULL;
	}
	virtual state_type* getState(const std::vector<id_type>& ids, size_type level = 0)
	{
		return NULL;
	}
};
