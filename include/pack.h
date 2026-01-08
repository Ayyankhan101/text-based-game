#ifndef PACK_H
#define PACK_H

#include <string>

struct PackMember {
    std::string name;
    std::string role; // Hunter, Scout, Guard
    int loyalty; // 0-100
    PackMember* next;
};

class Pack {
private:
    PackMember* head;
    int size;
public:
    Pack();
    ~Pack();
    void addMember(std::string name, std::string role, int loyalty);
    void displayPack();
    int getSize() const;
    bool removeMember(std::string name); // If loyalty low
    PackMember* getHead() const; // Add method to access pack members
    void updateLoyalty(int hungerIncrease); // Update loyalty based on wolf's hunger
};

// Utility function to calculate pack benefits
int calculatePackBenefits(const Pack& pack);

#endif