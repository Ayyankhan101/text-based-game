#include "../include/game_stack.h"
#include "../include/inventory.h"  // Include to get complete Inventory type definition
#include <stdexcept>

GameStack::GameStack() {
    // Constructor - nothing special needed
}

GameStack::~GameStack() {
    // Destructor - clean up any inventory copies in the stack
    for (auto& state : stack) {
        if (state.inventory) {
            delete state.inventory;
        }
    }
}

void GameStack::push(GameState gs) {
    if (stack.size() >= maxSize) {
        // Remove oldest and clean up its inventory copy
        if (!stack.empty() && stack[0].inventory) {
            delete stack[0].inventory;
        }
        stack.erase(stack.begin()); // Remove oldest
    }
    stack.push_back(gs);
}

GameState GameStack::pop() {
    if (stack.empty()) throw std::runtime_error("Stack is empty");
    GameState gs = stack.back();
    stack.pop_back();
    return gs;
}

bool GameStack::isEmpty() {
    return stack.empty();
}

int GameStack::size() {
    return stack.size();
}

