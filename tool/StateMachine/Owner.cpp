#include "test.hpp"

#include <iostream>

void printOwner(Owner* owner)
{
    std::cout << "stage: " << owner->getStage() << std::endl;
}

void Owner::distributeStateMachine()
{
    typedef state_machine_type::node_type node_type;
    std::vector<node_type> nodes(0);
    //nodes.reserve(8);
    //nodes.push_back(node_type{ 0, NULL, 3 });
    //nodes.push_back(node_type{ TYPE_A, new A(this), 2 });
    //nodes.push_back(node_type{ TYPE_A1, new A1(this), 1 });
    //nodes.push_back(node_type{ TYPE_A2, new A2(this), 1 });
    //nodes.push_back(node_type{ TYPE_B, new B(this), 2 });
    //nodes.push_back(node_type{ TYPE_B1, new B1(this), 1 });
    //nodes.push_back(node_type{ TYPE_B2, new B2(this), 1 });
    //nodes.push_back(node_type{ TYPE_C, new C(this), 1 });

    // ����״̬�ڵ�
    nodes.reserve(5);
    nodes.push_back(node_type{ 0, NULL, 2 });
    nodes.push_back(node_type{ TYPE_A, new A(this), 2 });
    nodes.push_back(node_type{ TYPE_A1, new A1(this), 1 });
    nodes.push_back(node_type{ TYPE_A2, new A2(this), 1 });
    nodes.push_back(node_type{ TYPE_C, new C(this), 1 });

    // �ַ�״̬�ڵ㣬����״̬��
    stateMachine.distribute(nodes);
    // ���ָ��״̬����ǰ״̬�ڵ�
    stateMachine.addState(TYPE_B, new B(this));
    // ���ָ��״̬��ָ��״̬�ڵ�
    std::vector<state_machine_type::id_type> ids(1, TYPE_B);
    stateMachine.addState(TYPE_B1, new B1(this), ids);
    //stateMachine.addState(TYPE_B2, new B2(this), ids);

    // ���õ�ǰ״̬
    stateMachine.setState(TYPE_A);
}

void Owner::collectStateMachine()
{
    // �ռ�״̬�ڵ�
    std::vector<state_machine_type::node_type> nodes;
    stateMachine.collect(nodes);

    // �ͷ�״̬ʵ��
    for (state_machine_type::size_type index = 0; index < nodes.size(); ++index)
        delete nodes[index].state;
}

void A::enter()
{
    std::cout << "void A::enter()" << std::endl;
    owner->setStage(TYPE_A);
    printOwner(owner);
    setState(TYPE_A1);
}

void A::exit()
{
    std::cout << "void A::exit()" << std::endl;
}

transition_type A::handle(message_type message)
{
    transition_type transition = state_machine_type::handle(message);
    // �����ڲ�״̬����ת��֮���޿�ת��״̬��������ʵ��Ϊ�����ͣ��޸Ĺ��ɶ���Ϊת�����ͣ�����ָ��Ŀ��״̬B
    if (transition.empty())
        transition.set(transition_type::CONVERTIBLE, TYPE_B);
    return transition;
}

void B::enter()
{
    std::cout << "void B::enter()" << std::endl;
    owner->setStage(TYPE_B);
    printOwner(owner);
    setState(TYPE_B1);
}

void B::exit()
{
    std::cout << "void B::exit()" << std::endl;
}

transition_type B::handle(message_type message)
{
    transition_type transition = state_machine_type::handle(message);
    if (transition.empty())
        transition.set(transition_type::CONVERTIBLE, TYPE_C);
    return transition;
}

void C::enter()
{
    std::cout << "void C::enter()" << std::endl;
    owner->setStage(TYPE_C);
    printOwner(owner);
}

void C::exit()
{
    std::cout << "void C::exit()" << std::endl;
}

transition_type C::handle(message_type message)
{
    // ��δ�ﵽת�����������غ���״̬�Ĺ���ʵ��
    if (message < TYPE_C)
        return transition_type(transition_type::IGNORE);
    // ���ﵽת���������������޺���״̬�����ؿ�״̬�Ĺ���ʵ��
    return transition_type(transition_type::EMPTY);
}

void A1::enter()
{
    std::cout << "void A1::enter()" << std::endl;
    owner->setStage(TYPE_A1);
    printOwner(owner);
}

void A1::exit()
{
    std::cout << "void A1::exit()" << std::endl;
}

transition_type A1::handle(message_type message)
{
    if (message < TYPE_A1)
        return transition_type(transition_type::IGNORE);
    return transition_type(transition_type::CONVERTIBLE, TYPE_A2);
}

void A2::enter()
{
    std::cout << "void A2::enter()" << std::endl;
    owner->setStage(TYPE_A2);
    printOwner(owner);
}

void A2::exit()
{
    std::cout << "void A2::exit()" << std::endl;
}

transition_type A2::handle(message_type message)
{
    if (message < TYPE_A2)
        return transition_type(transition_type::IGNORE);
    return transition_type(transition_type::EMPTY);
}

void B1::enter()
{
    std::cout << "void B1::enter()" << std::endl;
    owner->setStage(TYPE_B1);
    printOwner(owner);
}

void B1::exit()
{
    std::cout << "void B1::exit()" << std::endl;
}

transition_type B1::handle(message_type message)
{
    // ��δ�ﵽת�����������غ���״̬�Ĺ���ʵ��
    if (message < TYPE_B1)
        return transition_type(transition_type::IGNORE);
    // ���ﵽת�����������ش���״̬�Ĺ���ʵ����������ָʾ״̬�ڵ�ת��������״̬
    return transition_type(transition_type::CREATEABLE, TYPE_B2, new B2(owner));
}

void B2::enter()
{
    std::cout << "void B2::enter()" << std::endl;
    owner->setStage(TYPE_B2);
    printOwner(owner);
}

void B2::exit()
{
    std::cout << "void B2::exit()" << std::endl;
}

transition_type B2::handle(message_type message)
{
    if (message < TYPE_B2)
        return transition_type(transition_type::IGNORE);
    return transition_type(transition_type::EMPTY);
}
