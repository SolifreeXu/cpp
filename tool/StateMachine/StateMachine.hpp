/*
* �ļ���StateMachine.hpp
* 
* �汾��v1.0
* ���ߣ����
* ���䣺2592419242@qq.com
* �������ڣ�2021��01��07��
* 
* ժҪ��
* 1.ԭ��Ϊ����״̬������������������ϲ��״̬����
* 2.֧�����л��뷴���л������л������ռ�����״̬ʵ�����ͷ�ʵ����Դ�������л����ڷַ�״̬�ڵ㣬����״̬����
*   ���л��ͷ����л��������Ƚ�Ϸ�ʽ���ȱ������ڵ�������ӽڵ㣬�ٱ���һ�ӽڵ�������ӽڵ㣬���մ�˳�����±�����ֱ�����²�Ϊֹ���ٻص��ϲ㣬������һ�ӽڵ�������ӽڵ㡣
* 3.֧�ֱ�����ָ�״̬���������������µĲ㼶˳�����л��ͷ����л���Ч״̬ID��
* 4.�ṩ��ӡ��Ƴ�������ָ��״̬�ڵ㣬����˵ָ���㼶֮״̬ʵ���ķ�����
* 5.��Ϲ���ʵ���ʹ�����Ϣ������֧����ת��״̬֮ǰ����̬���ɲ����Ŀ��״̬��
*   �˷�������������ͬһ�㼶��״̬�ڵ㣬��ڵ�ʵ�ֿ��������²㼶��״̬�ڵ������趨ʱ����ԭ���Ͻڵ������Σ��Խ��Ͳ�ͬ�ڵ�֮�������ԡ�
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

	// ����������Ϣ
	virtual transition_type handle(message_type message)
	{
		// ����ǰ״̬�ڵ�Ϊ��Ч״̬��������Ч����ʵ��
		if (!valid)
			return transition_type();

		// ������Ϣ����ǰ״̬����ȡ���صĹ���ʵ��
		state_type* state = mapping.find(id)->second;
		transition_type transition = state->handle(message);

		// ������ʵ��Ϊ��Ч���ߺ������ͣ���������������ϲ�
		if (transition.invalid() || transition.ignore())
			return transition;

		// ������ʵ��Ϊ�������ͣ����ָ��״̬����ǰ״̬�ڵ�
		if (transition.createable())
			addState(transition.getID(), transition.getState());

		/* ���ݹ���ʵ��ת����ǰ״̬ */
		// ��ת��״̬֮�󣬲��ܹ��ٴ�ת�������ع���ʵ��
		if (!update(transition))
			return transition;
		// ��ת��״̬֮����Ȼ����ת���Ŀ����ԣ���ݹ����޷�ת��Ϊֹ
		return handle(message);
	}

	/*
	 * �ַ�״̬�ڵ㣬���ڹ���״̬����
	 *
	 * ������
	 *     1.�ڵ����������Է����л�״̬�ڵ㣬�������Ƚ�Ϸ�ʽ����״̬����
	 *     2.�α꣺����������ǰ״̬�ڵ㡣
	 * ����ֵ��
	 *     ����ʧ�ܷ���false������ɹ�����true��
	 *
	 * ���α�ֵ���ڵ��ڽڵ���������������󣬿����ǽڵ���������˳�����
	 * ����ӽڵ�ʧ�ܣ�������ʧ�ܣ�������ͬһ״̬�������ͬID�Ľڵ㡣
	 */
	virtual bool distribute(const std::vector<node_type>& nodes, size_type cursor = 0)
	{
		return startDistribution(nodes, cursor);
	}

	/*
	 * �ռ�״̬�ڵ㣬�����ͷ���Դ��
	 *
	 * ������
	 *     1.�ڵ��������������л�״̬�ڵ㣬�������Ƚ�Ϸ�ʽ����״̬����
	 *     2.�α꣺����������ǰ״̬�ڵ㡣
	 * ����ֵ��
	 *     �ռ�ʧ�ܷ���false���ռ��ɹ�����true��
	 *
	 * ���α겻���ڽڵ����������α�λ�������ռ�ʧ�ܡ�
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
	 * ����״̬������Ч״̬
	 * �����������µĲ㼶˳����ջ��Ч״̬��ֱ��������Ч״̬�������²�ڵ�Ϊֹ��
	 *
	 * ������
	 *     1.״̬����ջ���������л���Ч״̬��
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
	 * �ָ�״̬����ָ��״̬
	 * �����������µĲ㼶˳��ָ�״̬����ֱ������״̬��ɣ������������²�ڵ�Ϊֹ��
	 *
	 * ������
	 *     1.״̬����ջ�����Է����л���Ч״̬��
	 *     2.�㼶������������ǰ״̬�ڵ㡣
	 * ����ֵ��
	 *     �ָ�ʧ�ܷ���false���ָ��ɹ�����true��
	 *
	 * ��δ�ҵ�ָ��ID�Ľڵ㣬��ָ�ʧ�ܣ�������״̬����ջ���߳�ʼ�㼶����Ҳ�����Ǵ��ڶ�̬�ڵ㣬��δ��ǰ���ɽڵ㡣
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
	 * ���״̬����ǰ״̬�ڵ�
	 * 
	 * ������
	 *     1.״̬ID���������������״̬��
	 *     2.״ָ̬�룺ָ���Ѵ�����״̬ʵ����
	 * ����ֵ��
	 *     ���ʧ�ܷ���false����ӳɹ�����true��
	 * 
	 * ��״ָ̬��Ϊ�գ����ߴ�����ͬ״̬ID�������ʧ�ܡ�
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
	 * ���״̬������״̬�ڵ�
	 * �Ե�ǰ״̬�ڵ�Ϊ�������±���������״̬�ڵ㣬���Ҳ�������״̬��
	 * 
	 * ������
	 *     1.״̬ID���������������״̬��
	 *     2.״ָ̬�룺ָ���Ѵ�����״̬ʵ����
	 *     3.״̬ID���������մ������µĲ㼶˳������״̬�ڵ㡣
	 *     4.�㼶������������ǰ״̬�ڵ㡣
	 * ����ֵ��
	 *     ���ʧ�ܷ���false����ӳɹ�����true��
	 * 
	 * ���㼶����״̬ID�����������ʧ�ܣ�������״̬ID�������߳�ʼ�㼶����
	 * ��δ�ҵ�����״̬�ڵ㣬�����ʧ�ܡ�
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
	 * �Ƴ���ǰ״̬�ڵ��ָ��״̬
	 * 
	 * ������
	 *     1.״̬ID�������������Ƴ�״̬��
	 * ����ֵ��
	 *     �Ƴ�ʧ�ܷ���NULL���Ƴ��ɹ������Ƴ���״ָ̬�롣
	 * 
	 * ����ǰ״̬�ڵ���Ч�����Ҵ��Ƴ�״̬�ǵ�ǰ״̬������δ�ҵ����Ƴ�״̬�����Ƴ�ʧ�ܡ�
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
	 * �Ƴ�ָ��״̬�ڵ��ָ��״̬
	 * �Ե�ǰ״̬�ڵ�Ϊ�������±���������״̬�ڵ㣬�����Ƴ�Ŀ��״̬��
	 * 
	 * ������
	 *     1.״̬ID�������������Ƴ�״̬��
	 *     2.״̬ID���������մ������µĲ㼶˳������״̬�ڵ㡣
	 *     3.�㼶������������ǰ״̬�ڵ㡣
	 * ����ֵ��
	 *     �Ƴ�ʧ�ܷ���NULL���Ƴ��ɹ������Ƴ���״ָ̬�롣
	 * 
	 * ���㼶����״̬ID���������Ƴ�ʧ�ܣ�������״̬ID�������߳�ʼ�㼶����
	 * ��δ�ҵ�ָ��״̬�ڵ㣬���Ƴ�ʧ�ܡ�
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
	 * ���õ�ǰ״̬�ڵ�ĵ�ǰ״̬
	 * 
	 * ������
	 *     1.״̬ID����������Ŀ��״̬��
	 * ����ֵ��
	 *     ����ʧ�ܷ���false�����óɹ�����true��
	 * 
	 * ����ǰ״̬�ڵ�Ϊ��Ч״̬������δ�ҵ�Ŀ��״̬��������ʧ�ܡ�
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
	 * ����ָ��״̬�ڵ�ĵ�ǰ״̬
	 * �Ե�ǰ״̬�ڵ�Ϊ�������±���������״̬�ڵ㣬�������õ�ǰ״̬ΪĿ��״̬��
	 * 
	 * ������
	 *     1.״̬ID����������Ŀ��״̬��
	 *     2.״̬ID���������մ������µĲ㼶˳������״̬�ڵ㡣
	 *     3.�㼶������������ǰ״̬�ڵ㡣
	 * ����ֵ��
	 *     ����ʧ�ܷ���false�����óɹ�����true��
	 * 
	 * ���㼶����״̬ID������������ʧ�ܣ�������״̬ID�������߳�ʼ�㼶����
	 * ��δ�ҵ�ָ��״̬�ڵ㣬������ʧ�ܡ�
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
	 * ��ȡ��ǰ״̬�ڵ��ָ��״̬
	 * 
	 * ������
	 *     1.״̬ID����������Ŀ��״̬��
	 * ����ֵ��
	 *     ��ȡʧ�ܷ���NULL����ȡ�ɹ�����Ŀ��״ָ̬�롣
	 * 
	 * ����ǰ״̬�ڵ�Ϊ��Ч״̬������δ�ҵ�Ŀ��״̬�����ȡʧ�ܡ�
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
	 * ��ȡָ��״̬�ڵ��ָ��״̬
	 * �Ե�ǰ״̬�ڵ�Ϊ�������±���������״̬�ڵ㣬���һ�ȡĿ��״̬��
	 * 
	 * ������
	 *     1.״̬ID����������Ŀ��״̬��
	 *     2.״̬ID���������մ������µĲ㼶˳������״̬�ڵ㡣
	 *     3.�㼶������������ǰ״̬�ڵ㡣
	 * ����ֵ��
	 *     ��ȡʧ�ܷ���NULL����ȡ�ɹ�����Ŀ��״ָ̬�롣
	 * 
	 * ���㼶����״̬ID���������ȡʧ�ܣ�������״̬ID�������߳�ʼ�㼶����
	 * ��δ�ҵ�ָ��״̬�ڵ㣬���ȡʧ�ܡ�
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
	 * ��ȡ��ǰ״̬�ڵ�ĵ�ǰ״̬
	 * 
	 * ����ֵ��
	 *     ��ȡʧ�ܷ���NULL����ȡ�ɹ�����Ŀ��״ָ̬�롣
	 * 
	 * ����ǰ״̬�ڵ�Ϊ��Ч״̬�����ȡʧ�ܡ�
	 */
	state_type* getState()
	{
		return getState(id);
	}

	/*
	 * ��ȡָ��״̬�ڵ�ĵ�ǰ״̬
	 * �Ե�ǰ״̬�ڵ�Ϊ�������±���������״̬�ڵ㣬���һ�ȡ��ǰ״̬��
	 * 
	 * ������
	 *     1.״̬ID���������մ������µĲ㼶˳������״̬�ڵ㡣
	 *     2.�㼶������������ǰ״̬�ڵ㡣
	 * ����ֵ��
	 *     ��ȡʧ�ܷ���NULL����ȡ�ɹ�����Ŀ��״ָ̬�롣
	 * 
	 * ���㼶����״̬ID���������ȡʧ�ܣ�������״̬ID�������߳�ʼ�㼶����
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
	 * ���ݹ���ʵ��ת��״̬
	 * 
	 * ������
	 *     1.����ʵ��������ָʾ״̬�ڵ��ת��������
	 * ����ֵ��
	 *     ��ת��״̬֮�󣬲��ܹ��ٴ�ת�����򷵻�false��
	 *     ��ת��״̬֮����Ȼ����ת���Ŀ����ԣ��򷵻�true��
	 */
	bool update(transition_type& transition)
	{
		// ���������˳���ǰ״̬
		typename mapping_type::iterator previous = mapping.find(id);
		if (previous != mapping.end())
			previous->second->exit();

		// ������ʵ��Ϊ�����ͣ�˵����ǰ״̬�ڵ��޺���״̬������Ϊ��Ч״̬
		if (transition.empty())
			return valid = false;

		// ��Ŀ��״̬�����ڣ�˵��״̬�ڵ㷢���쳣��������Ŀ��״̬���󣬻���δ��ǰ���Ŀ��״̬
		typename mapping_type::iterator next = mapping.find(transition.getID());
		if (next == mapping.end())
		{
			std::cerr << "bool update(transition_type&): switch to an invalid state." << std::endl;
			// ����ʵ����Ϊ��Ч����
			transition.setType(transition_type::INVALID);
			return false;
		}

		// ����Ŀ��״̬
		next->second->enter();
		// ��ǰ״̬�ڵ�ĵ�ǰ״̬��ΪĿ��״̬
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
	 * �ռ�״̬�ڵ�
	 *
	 * ������
	 *     1.�ڵ��������������л�״̬�ڵ㣬�������Ƚ�Ϸ�ʽ����״̬������ʼ��Ҫ���һ���սڵ㡣
	 *     2.�α꣺����������ǰ״̬�ڵ㣬��ʼ��Ҫָ����ӵĿսڵ㡣
	 *
	 * ���ڽڵ������ĳ�ʼ�������Ե��þ�̬����MultiState::initCollection�������ֶ���ӿսڵ㡣
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
