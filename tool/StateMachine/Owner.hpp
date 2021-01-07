#pragma once

#include <cstddef>
#include <vector>

#include "StateMachine.hpp"

enum StateType
{
    TYPE_A,
    TYPE_A1,
    TYPE_A2,
    TYPE_B,
    TYPE_B1,
    TYPE_B2,
    TYPE_C
};

typedef StateMachine<size_t, size_t> state_machine_type;
typedef state_machine_type::state_type state_type;
typedef state_machine_type::transition_type transition_type;

class Owner
{
    state_machine_type stateMachine;
    StateType stage;

public:
    Owner()
    {
        distributeStateMachine();
    }

    ~Owner()
    {
        collectStateMachine();
    }

    state_machine_type& getStateMachine()
    {
        return stateMachine;
    }

    void setStage(StateType stage)
    {
        this->stage = stage;
    }

    StateType getStage()
    {
        return stage;
    }

private:
    void distributeStateMachine();
    void collectStateMachine();
};

class A : public state_machine_type
{
    Owner *const owner;
public:
    A(Owner* owner) : owner(owner) {}
    virtual void enter();
    virtual void exit();
    virtual transition_type handle(message_type message);
};

class B : public state_machine_type
{
    Owner* const owner;
public:
    B(Owner* owner) : owner(owner) {}
    virtual void enter();
    virtual void exit();
    virtual transition_type handle(message_type message);
};

class C : public state_type
{
    Owner* const owner;
public:
    C(Owner* owner) : owner(owner) {}
    virtual void enter();
    virtual void exit();
    virtual transition_type handle(message_type message);
};

class A1 : public state_type
{
    Owner* const owner;
public:
    A1(Owner* owner) : owner(owner) {}
    virtual void enter();
    virtual void exit();
    virtual transition_type handle(message_type message);
};

class A2 : public state_type
{
    Owner* const owner;
public:
    A2(Owner* owner) : owner(owner) {}
    virtual void enter();
    virtual void exit();
    virtual transition_type handle(message_type message);
};

class B1 : public state_type
{
    Owner* const owner;
public:
    B1(Owner* owner) : owner(owner) {}
    virtual void enter();
    virtual void exit();
    virtual transition_type handle(message_type message);
};

class B2 : public state_type
{
    Owner* const owner;
public:
    B2(Owner* owner) : owner(owner) {}
    virtual void enter();
    virtual void exit();
    virtual transition_type handle(message_type message);
};
