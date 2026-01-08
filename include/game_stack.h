#ifndef GAME_STACK_H
#define GAME_STACK_H

#include <vector>

struct DecisionNode; // Forward
struct Item; // Forward declaration for inventory
class Inventory; // Forward declaration for inventory

struct GameState {
    DecisionNode* currentNode;
    int health, hunger, energy;
    int day;           // Add day field
    Inventory* inventory; // Store a copy of the inventory for proper restoration
};

class GameStack {
private:
    std::vector<GameState> stack;
    const int maxSize = 5;
public:
    GameStack();  // Add constructor
    ~GameStack(); // Add destructor
    void push(GameState gs);
    GameState pop();
    bool isEmpty();
    int size();
    void clear(); // Clear all states from the stack
};

#endif