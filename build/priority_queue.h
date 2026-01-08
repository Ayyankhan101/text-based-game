#ifndef PRIORITY_QUEUE_H
#define PRIORITY_QUEUE_H

#include <string>
#include <vector>
#include <functional>

// Forward declaration
struct Wolf;

struct Event {
    std::string name;
    int priority; // 1-3, lower is more urgent
    std::string description;

    // Use std::function for proper Wolf effect
    std::function<void(Wolf&)> effect;

    bool operator>(const Event& other) const {
        return priority > other.priority;
    }
    bool operator<(const Event& other) const {
        return priority < other.priority;
    }
};

class PriorityQueue {
private:
    std::vector<Event> heap;
    void heapifyUp(int index);
    void heapifyDown(int index);
public:
    void insert(Event e);
    Event extractMin();
    bool isEmpty();
    int size(); // Add size function for event cycling
};

#endif