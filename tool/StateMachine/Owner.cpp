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

    // 生成状态节点
    nodes.reserve(5);
    nodes.push_back(node_type{ 0, NULL, 2 });
    nodes.push_back(node_type{ TYPE_A, new A(this), 2 });
    nodes.push_back(node_type{ TYPE_A1, new A1(this), 1 });
    nodes.push_back(node_type{ TYPE_A2, new A2(this), 1 });
    nodes.push_back(node_type{ TYPE_C, new C(this), 1 });

    // 分发状态节点，构造状态机
    stateMachine.distribute(nodes);
    // 添加指定状态至当前状态节点
    stateMachine.addState(TYPE_B, new B(this));
    // 添加指定状态至指定状态节点
    std::vector<state_machine_type::id_type> ids(1, TYPE_B);
    stateMachine.addState(TYPE_B1, new B1(this), ids);
    //stateMachine.addState(TYPE_B2, new B2(this), ids);

    // 设置当前状态
    stateMachine.setState(TYPE_A);
}

void Owner::collectStateMachine()
{
    // 收集状态节点
    std::vector<state_machine_type::node_type> nodes;
    stateMachine.collect(nodes);

    // 释放状态实例
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
    // 对于内层状态，若转换之后，无可转换状态，即过渡实例为空类型，修改过渡对象为转换类型，并且指定目标状态B
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
    // 若未达到转换条件，返回忽略状态的过渡实例
    if (message < TYPE_C)
        return transition_type(transition_type::IGNORE);
    // 若达到转换条件，但由于无后续状态，返回空状态的过渡实例
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
    // 若未达到转换条件，返回忽略状态的过渡实例
    if (message < TYPE_B1)
        return transition_type(transition_type::IGNORE);
    // 若达到转换条件，返回创建状态的过渡实例，创建并指示状态节点转换至后续状态
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
