/*
* 文件：Transition.hpp
* 
* 版本：v1.0
* 作者：许聪
* 邮箱：2592419242@qq.com
* 创建日期：2021年01月07日
* 
* 摘要：
* 1.过渡是抽象事物，负责过渡期间信息传递和指示状态转换操作。
* 2.过渡存在无效、忽略、创建、转换、空五种类型。
*   无效指状态实例无效，忽略表示未达到转换条件，创建用于创建并转换为目标状态，转换仅为转换至目标状态，空意味着无后续状态，也可以提示外层状态后续操作。
*/

#pragma once

#include <cstddef>

template <typename IDType, typename StateType>
class Transition
{
public:
	typedef IDType id_type;
	typedef StateType state_type;
	enum Type { INVALID, IGNORE, CREATEABLE, CONVERTIBLE, EMPTY };

private:
	Type type;
	id_type id;
	state_type* state;

public:
	Transition(Type type = INVALID, id_type id = 0, state_type* state = NULL)
		: type(type), id(id), state(state) {}

	void set(Type type = INVALID, id_type id = 0, state_type* state = NULL)
	{
		this->type = type;
		this->id = id;
		this->state = state;
	}

	void setType(Type type)
	{
		this->type = type;
	}

	Type getType()
	{
		return type;
	}

	void setID(id_type id)
	{
		this->id = id;
	}

	id_type getID()
	{
		return id;
	}

	void setState(state_type* state)
	{
		this->state = state;
	}

	state_type* getState()
	{
		return state;
	}

	bool invalid()
	{
		return type == INVALID;
	}

	bool ignore()
	{
		return type == IGNORE;
	}

	bool createable()
	{
		return type == CREATEABLE;
	}

	bool empty()
	{
		return type == EMPTY;
	}
};
