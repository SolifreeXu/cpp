/*
* �ļ���State.hpp
* 
* �汾��v1.0
* ���ߣ����
* ���䣺2592419242@qq.com
* �������ڣ�2021��01��07��
* 
* ժҪ��
* 1.����˼�룺����ʤ�ڱ������Է�����֯����������ߡ�״̬������״̬����״̬�γ�״̬����
* 3.��״̬Ϊ״̬��ǰ�ᣬ��״̬���麯��֧��״̬���ĵݹ�ӿڡ�
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
