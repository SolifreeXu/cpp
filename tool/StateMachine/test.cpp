#include "Owner.hpp"

int main()
{
    Owner owner;
    auto& stateMachine = owner.getStateMachine();
    //for (size_t index = 0, condition = TYPE_C; index <= condition; ++index)
    //    stateMachine.handle(index);

    stateMachine.handle(TYPE_B);
    std::vector<state_machine_type::id_type> stack;
    stateMachine.backup(stack);
    stateMachine.handle(TYPE_B2);
    stateMachine.recover(stack);
    stateMachine.handle(TYPE_C);
    return 0;
}
