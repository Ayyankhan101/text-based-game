#include "../include/action_queue.h"
#include <iostream>

ActionQueue::ActionQueue() {}

void ActionQueue::enqueue(Action a) {
    actions.push(a);
}

void ActionQueue::processNext(Wolf& wolf) {
    if (!actions.empty()) {
        Action a = actions.front();
        actions.pop();
        std::cout << "\033[34m" << a.description << "\033[0m" << std::endl;
        a.execute(wolf);
    }
}

bool ActionQueue::isEmpty() {
    return actions.empty();
}