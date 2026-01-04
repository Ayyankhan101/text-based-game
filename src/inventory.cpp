#include "../include/inventory.h"
#include <iostream>

Inventory::Inventory() : head(nullptr), count(0) {}

Inventory::~Inventory() {
    Item* current = head;
    while (current) {
        Item* next = current->next;
        delete current;
        current = next;
    }
}

bool Inventory::addItem(std::string name, ItemType type, int effect, int qty) {
    if (count >= 10) return false;
    Item* newItem = new Item{name, type, effect, qty, nullptr};
    if (!head) {
        head = newItem;
    } else {
        Item* current = head;
        while (current->next) current = current->next;
        current->next = newItem;
    }
    count++;
    return true;
}

bool Inventory::useItem(std::string name) {
    Item* current = head;
    Item* prev = nullptr;
    while (current) {
        if (current->name == name) {
            // Apply effect (assume wolf is passed, but for now just remove)
            if (current->quantity > 1) {
                current->quantity--;
            } else {
                if (prev) prev->next = current->next;
                else head = current->next;
                delete current;
                count--;
            }
            return true;
        }
        prev = current;
        current = current->next;
    }
    return false;
}

void Inventory::displayInventory() {
    Item* current = head;
    while (current) {
        std::cout << current->name << " (x" << current->quantity << ")" << std::endl;
        current = current->next;
    }
}

bool Inventory::isFull() const {
    return count >= 10;
}

Item* Inventory::getHead() const {
    return head;
}

Inventory* Inventory::clone() const {
    Inventory* newInventory = new Inventory();
    Item* current = head;
    while (current) {
        newInventory->addItem(current->name, current->type, current->effect, current->quantity);
        current = current->next;
    }
    return newInventory;
}

void Inventory::replaceWith(Inventory* other) {
    // Clear current inventory
    Item* current = head;
    while (current) {
        Item* next = current->next;
        delete current;
        current = next;
    }
    head = nullptr;
    count = 0;

    // Copy items from other inventory
    if (other && other->head) {
        Item* otherCurrent = other->head;
        while (otherCurrent) {
            addItem(otherCurrent->name, otherCurrent->type, otherCurrent->effect, otherCurrent->quantity);
            otherCurrent = otherCurrent->next;
        }
    }
}

bool Inventory::useItem(std::string name, Wolf& wolf) {
    Item* current = head;
    Item* prev = nullptr;
    while (current) {
        if (current->name == name) {
            // Apply the item's effect to the wolf based on its type
            if (current->type == FOOD) {
                wolf.updateHunger(current->effect);
            } else if (current->type == HERB) {
                wolf.updateHealth(current->effect);
            } else if (current->type == TOOL) {
                // Tools might provide other benefits (e.g., energy boost, reputation)
                // For now, we'll add a small energy boost
                wolf.updateEnergy(current->effect);
            }

            // Reduce quantity or remove item
            if (current->quantity > 1) {
                current->quantity--;
            } else {
                // Remove the item from the list
                if (prev) {
                    prev->next = current->next;
                } else {
                    head = current->next;
                }
                delete current;
                count--;
            }
            return true;
        }
        prev = current;
        current = current->next;
    }
    return false; // Item not found
}