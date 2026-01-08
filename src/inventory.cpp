#include "../include/inventory.h"
#include <iostream>
#include <string>

// Color codes for terminal output
#define RESET   "\033[0m"
#define RED     "\033[31m"
#define GREEN   "\033[32m"
#define YELLOW  "\033[33m"
#define BLUE    "\033[34m"
#define CYAN    "\033[36m"

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
    std::cout << CYAN << "=== INVENTORY ===" << RESET << std::endl;
    if (!head) {
        std::cout << "Your inventory is empty." << std::endl;
    } else {
        Item* current = head;
        while (current) {
            std::string typeStr;
            if (current->type == FOOD) typeStr = "Food";
            else if (current->type == HERB) typeStr = "Herb";
            else if (current->type == TOOL) typeStr = "Tool";
            else if (current->type == WATER) typeStr = "Water";
            
            std::cout << current->name << " (x" << current->quantity << ") - " 
                      << typeStr << " [Effect: " << current->effect << "]";
            if (current->type == FOOD) std::cout << " - Reduces hunger";
            else if (current->type == HERB) std::cout << " - Restores health";
            else if (current->type == WATER) std::cout << " - Hydrates and energizes";
            std::cout << std::endl;
            current = current->next;
        }
    }
    std::cout << "Total items: " << count << std::endl;
    std::cout << CYAN << "=================" << RESET << std::endl;
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
                // Tools might provide other benefits
                wolf.updateEnergy(current->effect);
            } else if (current->type == WATER) {
                // Water reduces hunger and boosts energy
                wolf.updateHunger(current->effect);
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

void Inventory::clear() {
    // Clear all items safely without causing double delete
    Item* current = head;
    while (current) {
        Item* next = current->next;
        delete current;
        current = next;
    }
    head = nullptr;
    count = 0;
}

void Inventory::displayWithNumbers() {
    std::cout << CYAN << "=== INVENTORY ===" << RESET << std::endl;
    if (!head) {
        std::cout << "Your inventory is empty." << std::endl;
    } else {
        Item* current = head;
        int index = 1;
        while (current) {
            std::string typeStr;
            if (current->type == FOOD) typeStr = "Food";
            else if (current->type == HERB) typeStr = "Herb";
            else if (current->type == TOOL) typeStr = "Tool";
            else if (current->type == WATER) typeStr = "Water";
            
            std::cout << index << ". " << current->name << " (x" << current->quantity << ") - " 
                      << typeStr << " [Effect: " << current->effect << "]";
            if (current->type == FOOD) std::cout << " - Reduces hunger";
            else if (current->type == HERB) std::cout << " - Restores health";
            else if (current->type == WATER) std::cout << " - Hydrates and energizes";
            std::cout << std::endl;
            current = current->next;
            index++;
        }
    }
    std::cout << "Total items: " << count << std::endl;
    std::cout << CYAN << "=================" << RESET << std::endl;
}

bool Inventory::useItemByNumber(int itemNumber, Wolf& wolf) {
    if (itemNumber <= 0 || !head) return false;
    
    Item* current = head;
    Item* prev = nullptr;
    int index = 1;
    
    while (current) {
        if (index == itemNumber) {
            // Apply the item's effect to the wolf based on its type
            if (current->type == FOOD) {
                wolf.updateHunger(current->effect);
            } else if (current->type == HERB) {
                wolf.updateHealth(current->effect);
            } else if (current->type == TOOL) {
                wolf.updateEnergy(current->effect);
            } else if (current->type == WATER) {
                wolf.updateHunger(current->effect);
                wolf.updateEnergy(current->effect);
            }
            
            std::cout << GREEN << "Used " << current->name << "!" << RESET << std::endl;
            
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
        index++;
    }
    return false; // Item not found
}

int Inventory::getCount() const {
    return count;
}

// Helper methods for quantity scaling
int Inventory::getFoodQuantity(Difficulty diff) {
    switch(diff) {
        case EASY: return 3;
        case NORMAL: return 1;
        case HARD: return 1;
    }
    return 1;
}

int Inventory::getHealingQuantity(Difficulty diff) {
    switch(diff) {
        case EASY: return 2;
        case NORMAL: return 1;
        case HARD: return 0;
    }
    return 1;
}

int Inventory::getScavengeCount(Difficulty diff) {
    switch(diff) {
        case EASY: return 3;
        case NORMAL: return 1;
        case HARD: return 1;
    }
    return 1;
}

void Inventory::addStartingSupplies(Difficulty diff) {
    switch(diff) {
        case EASY:
            // Easy mode: Start with basic supplies
            addItem("Small Fish", FOOD, -15, 2);
            addItem("Winter Berries", FOOD, -8, 2);
            addItem("Common Mallow", HERB, 10, 1);
            std::cout << GREEN << "[EASY MODE] You start with basic supplies!" << RESET << std::endl;
            break;
        case NORMAL:
            // Normal mode: No starting items (classic behavior)
            std::cout << YELLOW << "[NORMAL MODE] Good luck, wolf!" << RESET << std::endl;
            break;
        case HARD:
            // Hard mode: Maybe a small bonus item for fairness
            addItem("Insects", FOOD, -5, 2);
            std::cout << RED << "[HARD MODE] Survive by your wits!" << RESET << std::endl;
            break;
    }
}

void Inventory::addRandomFood(Difficulty diff) {
    std::vector<std::pair<std::string, std::pair<ItemType, int>>> foodOptions;
    
    // Food options with (name, (type, effect))
    foodOptions.push_back({"Small Fish", {FOOD, -15}});
    foodOptions.push_back({"Bird Egg", {FOOD, -10}});
    foodOptions.push_back({"Winter Berries", {FOOD, -8}});
    foodOptions.push_back({"Fresh Water", {WATER, -15}});
    foodOptions.push_back({"Insects", {FOOD, -5}});
    foodOptions.push_back({"Frog", {FOOD, -12}});
    
    int count = getFoodQuantity(diff);
    
    // Add random items based on count
    for(int i = 0; i < count; i++) {
        if (!isFull() && !foodOptions.empty()) {
            int idx = rand() % foodOptions.size();
            auto& item = foodOptions[idx];
            addItem(item.first, item.second.first, item.second.second, 1);
        }
    }
}

void Inventory::addRandomHealing(Difficulty diff) {
    std::vector<std::pair<std::string, std::pair<ItemType, int>>> healingOptions;
    
    // Healing options
    healingOptions.push_back({"Common Mallow", {HERB, 10}});
    healingOptions.push_back({"Root Paste", {HERB, 15}});
    healingOptions.push_back({"Moss Dressing", {HERB, 12}});
    healingOptions.push_back({"Bee Propolis", {HERB, 20}});
    
    int count = getHealingQuantity(diff);
    
    for(int i = 0; i < count; i++) {
        if (!isFull() && !healingOptions.empty()) {
            int idx = rand() % healingOptions.size();
            auto& item = healingOptions[idx];
            addItem(item.first, item.second.first, item.second.second, 1);
        }
    }
}

void Inventory::addScavengedItems(Difficulty diff) {
    std::vector<std::pair<std::string, std::pair<ItemType, int>>> scavengeOptions;
    
    // Mixed scavenge options
    scavengeOptions.push_back({"Small Fish", {FOOD, -15}});
    scavengeOptions.push_back({"Winter Berries", {FOOD, -8}});
    scavengeOptions.push_back({"Fresh Water", {WATER, -15}});
    scavengeOptions.push_back({"Common Mallow", {HERB, 10}});
    scavengeOptions.push_back({"Insects", {FOOD, -5}});
    scavengeOptions.push_back({"Bird Egg", {FOOD, -10}});
    scavengeOptions.push_back({"Dry Grass", {TOOL, 10}}); // Energy boost
    
    int count = getScavengeCount(diff);
    
    for(int i = 0; i < count; i++) {
        if (!isFull() && !scavengeOptions.empty()) {
            int idx = rand() % scavengeOptions.size();
            auto& item = scavengeOptions[idx];
            addItem(item.first, item.second.first, item.second.second, 1);
        }
    }
}

void Inventory::addHuntLoot(Difficulty diff, int scenarioId) {
    // Different loot based on scenario
    switch(scenarioId) {
        case 2: // First hunt - wounded rabbit
            addItem("Rabbit Meat", FOOD, -30, (diff == EASY) ? 4 : 2);
            break;
        case 4: // Find traps
            addItem("Rabbit Meat", FOOD, -30, (diff == EASY) ? 6 : 4);
            addItem("Healing Herbs", HERB, 20, (diff == EASY) ? 3 : 2);
            addItem("Common Mallow", HERB, 10, (diff == EASY) ? 2 : 1);
            break;
        case 11: // Hunt deer with Luna
            addItem("Rabbit Meat", FOOD, -30, (diff == EASY) ? 6 : 4);
            addItem("Small Fish", FOOD, -15, (diff == EASY) ? 2 : 1);
            break;
        default:
            // Generic hunt loot
            addRandomFood(diff);
            break;
    }
}