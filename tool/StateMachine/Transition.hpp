/*
* �ļ���Transition.hpp
* 
* �汾��v1.0
* ���ߣ����
* ���䣺2592419242@qq.com
* �������ڣ�2021��01��07��
* 
* ժҪ��
* 1.�����ǳ��������������ڼ���Ϣ���ݺ�ָʾ״̬ת��������
* 2.���ɴ�����Ч�����ԡ�������ת�������������͡�
*   ��Чָ״̬ʵ����Ч�����Ա�ʾδ�ﵽת���������������ڴ�����ת��ΪĿ��״̬��ת����Ϊת����Ŀ��״̬������ζ���޺���״̬��Ҳ������ʾ���״̬����������
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
