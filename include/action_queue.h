#ifndef ACTION_QUEUE_H
#define ACTION_QUEUE_H

#include <queue>
#include <string>
#include <functional>

struct Wolf; // Forward declaration

struct Action {
    std::string description;
    std::function<void(Wolf&)> execute;  // Function to execute on Wolf object
};

class ActionQueue {
private:
    std::queue<Action> actions;
public:
    ActionQueue();
    void enqueue(Action a);
    void processNext(Wolf& wolf);
    bool isEmpty();
    void clear(); // Clear all pending actions
};

#endif