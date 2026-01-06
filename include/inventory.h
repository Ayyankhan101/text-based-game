#ifndef INVENTORY_H
#define INVENTORY_H

#include <string>
#include "wolf.h"

enum ItemType { FOOD, HERB, TOOL };

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
    Inventory* clone() const; // Add method to create a deep copy of the inventory
    void replaceWith(Inventory* other); // Add method to replace contents with another inventory
    bool useItem(std::string name, Wolf& wolf); // Add method to use item and apply effect to wolf
    void clear(); // Clear all items from inventory
};

#endif