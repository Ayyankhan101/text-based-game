#ifndef INVENTORY_H
#define INVENTORY_H

#include <string>
#include <vector>
#include "wolf.h"
#include "game_types.h"

enum ItemType { FOOD, HERB, TOOL, WATER };

struct Item {
    std::string name;
    ItemType type;
    int effect;
    int quantity;
    Item* next;
};

class Inventory {
private:
    Item* head;
    int count;
public:
    Inventory();
    ~Inventory();
    bool addItem(std::string name, ItemType type, int effect, int qty);
    bool useItem(std::string name);
    void displayInventory();
    bool isFull() const;
    Item* getHead() const;
    Inventory* clone() const;
    void replaceWith(Inventory* other);
    bool useItem(std::string name, Wolf& wolf);
    void clear();
    void displayWithNumbers();
    bool useItemByNumber(int itemNumber, Wolf& wolf);
    int getCount() const;
    
    // Difficulty-scaled item distribution methods
    void addStartingSupplies(Difficulty diff); // Add starting items based on difficulty
    void addRandomFood(Difficulty diff);       // Add random food based on difficulty
    void addRandomHealing(Difficulty diff);    // Add random healing items based on difficulty
    void addScavengedItems(Difficulty diff);   // Add mixed scavenged items based on difficulty
    void addHuntLoot(Difficulty diff, int scenarioId); // Add loot from hunting scenarios
    
    // Helper methods
    static int getFoodQuantity(Difficulty diff);     // Get quantity for food drops
    static int getHealingQuantity(Difficulty diff);  // Get quantity for healing drops
    static int getScavengeCount(Difficulty diff);    // Get number of scavenged items
};

#endif