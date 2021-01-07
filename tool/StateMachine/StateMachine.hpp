/*
* 文件：StateMachine.hpp
* 
* 版本：v1.0
* 作者：许聪
* 邮箱：2592419242@qq.com
* 创建日期：2021年01月07日
* 
* 摘要：
* 1.原型为有限状态机，不过可以自由组合层次状态机。
* 2.支持序列化与反序列化，序列化用于收集所有状态实例，释放实例资源，反序列化用于分发状态节点，构造状态机。
*   序列化和反序列化采用深广度结合方式，先遍历根节点的所有子节点，再遍历一子节点的所有子节点，按照此顺序向下遍历，直到最下层为止，再回到上层，遍历另一子节点的所有子节点。
* 3.支持备份与恢复状态机，按照由上至下的层级顺序，序列化和反序列化有效状态ID。
* 4.提供添加、移除、查找指定状态节点，或者说指定层级之状态实例的方法。
* 5.结合过渡实例和处理消息方法，支持在转换状态之前，动态生成并添加目标状态。
*   此方法仅限于生成同一层级的状态节点，跨节点实现可以由上下层级的状态节点自行设定时机，原则上节点内自治，以降低不同节点之间的耦合性。
*/

#pragma once

#include <utility>
#include <cstddef>
#include <vector>
#include <map>
//#include <unordered_map>
#include <iostream>

#include "Transition.hpp"
#include "State.hpp"

template <typename IDType, typename MessageType, typename SizeType = size_t>
class StateMachine : public MultiState<IDType, MessageType, SizeType>
{
public:
	typedef IDType id_type;
	typedef MultiState<IDType, MessageType, SizeType> state_type;
	typedef typename state_type::transition_type transition_type;
	typedef MessageType message_type;
	typedef SizeType size_type;
	typedef typename state_type::node_type node_type;
	typedef std::map<id_type, state_type*> mapping_type;
	//typedef std::unordered_map<id_type, state_type*> mapping_type;

private:
	bool valid;
	id_type id;
	mapping_type mapping;

public:
	StateMachine() : valid(false) {}

	// 处理输入消息
	virtual transition_type handle(message_type message)
	{
		// 若当前状态节点为无效状态，返回无效过渡实例
		if (!valid)
			return transition_type();

		// 传递消息至当前状态，获取返回的过渡实例
		state_type* state = mapping.find(id)->second;
		transition_type transition = state->handle(message);

		// 若过渡实例为无效或者忽略类型，不处理而返回至上层
		if (transition.invalid() || transition.ignore())
			return transition;

		// 若过渡实例为创建类型，添加指定状态至当前状态节点
		if (transition.createable())
			addState(transition.getID(), transition.getState());

		/* 根据过渡实例转换当前状态 */
		// 若转换状态之后，不能够再次转换，返回过渡实例
		if (!update(transition))
			return transition;
		// 若转换状态之后，仍然存在转换的可能性，则递归至无法转换为止
		return handle(message);
	}

	/*
	 * 分发状态节点，用于构造状态机。
	 *
	 * 参数：
	 *     1.节点向量：用以反序列化状态节点，按照深广度结合方式构造状态树。
	 *     2.游标：用以索引当前状态节点。
	 * 返回值：
	 *     构造失败返回false，构造成功返回true。
	 *
	 * 若游标值大于等于节点数量，则参数有误，可能是节点数量或者顺序错误。
	 * 若添加节点失败，即构造失败，可能向同一状态机添加相同ID的节点。
	 */
	virtual bool distribute(const std::vector<node_type>& nodes, size_type cursor = 0)
	{
		return startDistribution(nodes, cursor);
	}

	/*
	 * 收集状态节点，用于释放资源。
	 *
	 * 参数：
	 *     1.节点向量：用以序列化状态节点，按照深广度结合方式遍历状态树。
	 *     2.游标：用以索引当前状态节点。
	 * 返回值：
	 *     收集失败返回false，收集成功返回true。
	 *
	 * 若游标不等于节点数量，即游标位置有误，收集失败。
	 */
	virtual bool collect(std::vector<node_type>& nodes, size_type cursor = 0)
	{
		if (cursor != nodes.size())
			return false;

		state_type::initCollection(nodes);
		startCollection(nodes, cursor);
		return true;
	}

	/*
	 * 备份状态机的有效状态
	 * 按照由上至下的层级顺序入栈有效状态，直至遇到无效状态或者最下层节点为止。
	 *
	 * 参数：
	 *     1.状态向量栈：用以序列化有效状态。
	 */
	virtual void backup(std::vector<id_type>& stack)
	{
		if (!valid)
			return;

		stack.push_back(id);
		state_type* state = mapping.find(id)->second;
		state->backup(stack);
	}

	/*
	 * 恢复状态机至指定状态
	 * 按照由上至下的层级顺序恢复状态机，直至遍历状态完成，或者遇到最下层节点为止。
	 *
	 * 参数：
	 *     1.状态向量栈：用以反序列化有效状态。
	 *     2.层级：用以索引当前状态节点。
	 * 返回值：
	 *     恢复失败返回false，恢复成功返回true。
	 *
	 * 若未找到指定ID的节点，则恢复失败，可能是状态向量栈或者初始层级有误，也可能是存在动态节点，而未提前生成节点。
	 */
	virtual bool recover(const std::vector<id_type>& stack, size_type level = 0)
	{
		if (level >= stack.size())
			return true;

		id_type id = stack[level];
		typename mapping_type::iterator iterator = mapping.find(id);
		if (iterator == mapping.end())
			return false;

		this->id = id;
		valid = true;
		return iterator->second->recover(stack, level + 1);
	}

	/*
	 * 添加状态至当前状态节点
	 * 
	 * 参数：
	 *     1.状态ID：用以索引待添加状态。
	 *     2.状态指针：指向已创建的状态实例。
	 * 返回值：
	 *     添加失败返回false，添加成功返回true。
	 * 
	 * 若状态指针为空，或者存在相同状态ID，则添加失败。
	 */
	bool addState(id_type id, state_type* state)
	{
		if (state == NULL)
			return false;

		if (mapping.find(id) != mapping.end())
			return false;
		return mapping.insert(std::make_pair(id, state)).second;
	}

	/*
	 * 添加状态至给定状态节点
	 * 以当前状态节点为根，向下遍历至给定状态节点，并且插入待添加状态。
	 * 
	 * 参数：
	 *     1.状态ID：用以索引待添加状态。
	 *     2.状态指针：指向已创建的状态实例。
	 *     3.状态ID向量：按照从上至下的层级顺序搜索状态节点。
	 *     4.层级：用以索引当前状态节点。
	 * 返回值：
	 *     添加失败返回false，添加成功返回true。
	 * 
	 * 若层级大于状态ID数量，则添加失败，可能是状态ID向量或者初始层级有误。
	 * 若未找到给定状态节点，则添加失败。
	 */
	virtual bool addState(id_type id, state_type* state, const std::vector<id_type>& ids, size_type level = 0)
	{
		if (level > ids.size())
			return false;
		if (level == ids.size())
			return addState(id, state);

		typename mapping_type::iterator iterator = mapping.find(ids[level]);
		if (iterator == mapping.end())
			return false;
		return iterator->second->addState(id, state, ids, level + 1);
	}

	/*
	 * 移除当前状态节点的指定状态
	 * 
	 * 参数：
	 *     1.状态ID：用以索引待移除状态。
	 * 返回值：
	 *     移除失败返回NULL，移除成功返回移除的状态指针。
	 * 
	 * 若当前状态节点有效，并且待移除状态是当前状态，或者未找到待移除状态，则移除失败。
	 */
	state_type* removeState(id_type id)
	{
		if (valid && this->id == id)
			return NULL;

		typename mapping_type::iterator iterator = mapping.find(id);
		if (iterator == mapping.end())
			return NULL;

		state_type* state = iterator->second;
		mapping.erase(iterator);
		return state;
	}

	/*
	 * 移除指定状态节点的指定状态
	 * 以当前状态节点为根，向下遍历至给定状态节点，并且移除目标状态。
	 * 
	 * 参数：
	 *     1.状态ID：用以索引待移除状态。
	 *     2.状态ID向量：按照从上至下的层级顺序搜索状态节点。
	 *     3.层级：用以索引当前状态节点。
	 * 返回值：
	 *     移除失败返回NULL，移除成功返回移除的状态指针。
	 * 
	 * 若层级大于状态ID数量，则移除失败，可能是状态ID向量或者初始层级有误。
	 * 若未找到指定状态节点，则移除失败。
	 */
	virtual state_type* removeState(id_type id, const std::vector<id_type>& ids, size_type level = 0)
	{
		if (level > ids.size())
			return NULL;
		if (level == ids.size())
			return removeState(id);

		typename mapping_type::iterator iterator = mapping.find(ids[level]);
		if (iterator == mapping.end())
			return NULL;
		return iterator->second->removeState(id, ids, level + 1);
	}

	/*
	 * 设置当前状态节点的当前状态
	 * 
	 * 参数：
	 *     1.状态ID：用以索引目标状态。
	 * 返回值：
	 *     设置失败返回false，设置成功返回true。
	 * 
	 * 若当前状态节点为有效状态，或者未找到目标状态，则设置失败。
	 */
	bool setState(id_type id)
	{
		if (valid)
			return false;

		typename mapping_type::iterator next = mapping.find(id);
		if (next == mapping.end())
			return false;
		next->second->enter();

		this->id = id;
		return valid = true;
	}

	/*
	 * 设置指定状态节点的当前状态
	 * 以当前状态节点为根，向下遍历至给定状态节点，并且设置当前状态为目标状态。
	 * 
	 * 参数：
	 *     1.状态ID：用以索引目标状态。
	 *     2.状态ID向量：按照从上至下的层级顺序搜索状态节点。
	 *     3.层级：用以索引当前状态节点。
	 * 返回值：
	 *     设置失败返回false，设置成功返回true。
	 * 
	 * 若层级大于状态ID数量，则设置失败，可能是状态ID向量或者初始层级有误。
	 * 若未找到指定状态节点，则设置失败。
	 */
	virtual bool setState(id_type id, const std::vector<id_type>& ids, size_type level = 0)
	{
		if (level > ids.size())
			return false;
		if (level == ids.size())
			return setState(id);

		typename mapping_type::iterator iterator = mapping.find(ids[level]);
		if (iterator == mapping.end())
			return NULL;
		return iterator->second->setState(id, ids, level + 1);
	}

	/*
	 * 获取当前状态节点的指定状态
	 * 
	 * 参数：
	 *     1.状态ID：用以索引目标状态。
	 * 返回值：
	 *     获取失败返回NULL，获取成功返回目标状态指针。
	 * 
	 * 若当前状态节点为无效状态，或者未找到目标状态，则获取失败。
	 */
	state_type* getState(id_type id)
	{
		if (!valid)
			return NULL;

		typename mapping_type::iterator iterator = mapping.find(id);
		if (iterator == mapping.end())
			return NULL;
		return iterator->second;
	}

	/*
	 * 获取指定状态节点的指定状态
	 * 以当前状态节点为根，向下遍历至给定状态节点，并且获取目标状态。
	 * 
	 * 参数：
	 *     1.状态ID：用以索引目标状态。
	 *     2.状态ID向量：按照从上至下的层级顺序搜索状态节点。
	 *     3.层级：用以索引当前状态节点。
	 * 返回值：
	 *     获取失败返回NULL，获取成功返回目标状态指针。
	 * 
	 * 若层级大于状态ID数量，则获取失败，可能是状态ID向量或者初始层级有误。
	 * 若未找到指定状态节点，则获取失败。
	 */
	virtual state_type* getState(id_type id, const std::vector<id_type>& ids, size_type level = 0)
	{
		if (level > ids.size())
			return NULL;
		if (level == ids.size())
			return getState(id);

		typename mapping_type::iterator iterator = mapping.find(ids[level]);
		if (iterator == mapping.end())
			return NULL;
		return iterator->second->getState(id, ids, level + 1);
	}

	/*
	 * 获取当前状态节点的当前状态
	 * 
	 * 返回值：
	 *     获取失败返回NULL，获取成功返回目标状态指针。
	 * 
	 * 若当前状态节点为无效状态，则获取失败。
	 */
	state_type* getState()
	{
		return getState(id);
	}

	/*
	 * 获取指定状态节点的当前状态
	 * 以当前状态节点为根，向下遍历至给定状态节点，并且获取当前状态。
	 * 
	 * 参数：
	 *     1.状态ID向量：按照从上至下的层级顺序搜索状态节点。
	 *     2.层级：用以索引当前状态节点。
	 * 返回值：
	 *     获取失败返回NULL，获取成功返回目标状态指针。
	 * 
	 * 若层级大于状态ID数量，则获取失败，可能是状态ID向量或者初始层级有误。
	 */
	virtual state_type* getState(const std::vector<id_type>& ids, size_type level = 0)
	{
		if (level > ids.size())
			return NULL;
		if (level == ids.size())
			return getState();

		typename mapping_type::iterator iterator = mapping.find(ids[level]);
		if (iterator == mapping.end())
			return NULL;
		return iterator->second->getState(ids, level + 1);
	}

private:
	/*
	 * 根据过渡实例转换状态
	 * 
	 * 参数：
	 *     1.过渡实例：用以指示状态节点的转换操作。
	 * 返回值：
	 *     若转换状态之后，不能够再次转换，则返回false。
	 *     若转换状态之后，仍然存在转换的可能性，则返回true。
	 */
	bool update(transition_type& transition)
	{
		// 若存在则退出当前状态
		typename mapping_type::iterator previous = mapping.find(id);
		if (previous != mapping.end())
			previous->second->exit();

		// 若过渡实例为空类型，说明当前状态节点无后续状态，则设为无效状态
		if (transition.empty())
			return valid = false;

		// 若目标状态不存在，说明状态节点发生异常，可能是目标状态错误，或者未提前添加目标状态
		typename mapping_type::iterator next = mapping.find(transition.getID());
		if (next == mapping.end())
		{
			std::cerr << "bool update(transition_type&): switch to an invalid state." << std::endl;
			// 过渡实例设为无效类型
			transition.setType(transition_type::INVALID);
			return false;
		}

		// 进入目标状态
		next->second->enter();
		// 当前状态节点的当前状态设为目标状态
		id = transition.getID();
		return true;
	}

	virtual bool startDistribution(const std::vector<node_type>& nodes, size_type& cursor)
	{
		if (cursor >= nodes.size())
			return false;

		const node_type& node = nodes[cursor];
		if (cursor++ + node.number >= nodes.size())
			return false;

		for (size_type index = 0; index < node.number; ++index)
		{
			const node_type& node = nodes[cursor];
			if (!addState(node.id, node.state))
				return false;

			if (!node.state->startDistribution(nodes, cursor))
				return false;
		}
		return true;
	}

	/*
	 * 收集状态节点
	 *
	 * 参数：
	 *     1.节点向量：用以序列化状态节点，按照深广度结合方式遍历状态树，初始需要添加一个空节点。
	 *     2.游标：用以索引当前状态节点，初始需要指向添加的空节点。
	 *
	 * 对于节点向量的初始化，可以调用静态函数MultiState::initCollection，无需手动添加空节点。
	 */
	virtual void startCollection(std::vector<node_type>& nodes, size_type& cursor)
	{
		node_type& node = nodes[cursor];
		node.number = mapping.size();
		for (typename mapping_type::iterator iterator = mapping.begin(); iterator != mapping.end(); ++iterator)
		{
			nodes.push_back(node_type{ iterator->first, iterator->second });
			iterator->second->startCollection(nodes, ++cursor);
		}
	}
};
