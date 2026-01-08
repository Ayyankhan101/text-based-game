#include "../include/pack.h"
#include <iostream>
#include <string>

// Color codes for terminal output
#define RESET   "\033[0m"
#define RED     "\033[31m"
#define GREEN   "\033[32m"
#define YELLOW  "\033[33m"
#define BLUE    "\033[34m"
#define CYAN    "\033[36m"

Pack::Pack() : head(nullptr), size(0) {}

Pack::~Pack() {
    PackMember* current = head;
    while (current) {
        PackMember* next = current->next;
        delete current;
        current = next;
    }
}

void Pack::addMember(std::string name, std::string role, int loyalty) {
    if (size >= 5) {
        std::cout << "Pack is full (max 5 members)." << std::endl;
        return;
    }
    PackMember* newMember = new PackMember{name, role, loyalty, nullptr};
    if (!head) {
        head = newMember;
    } else {
        PackMember* current = head;
        while (current->next) current = current->next;
        current->next = newMember;
    }
    size++;
    std::cout << "Recruited " << name << " as " << role << "." << std::endl;
}

void Pack::displayPack() {
    std::cout << CYAN << "=== YOUR PACK ===" << RESET << std::endl;
    if (size == 0) {
        std::cout << "You have no pack members yet. Recruit wolves to build your pack!" << std::endl;
    } else {
        std::cout << "Pack Size: " << size << " member(s)" << std::endl;
        std::cout << "--------------------" << std::endl;
        PackMember* current = head;
        while (current) {
            std::cout << "• " << current->name << " - " << current->role;
            if (current->loyalty >= 70) {
                std::cout << GREEN << " [Loyal: " << current->loyalty << "]" << RESET;
            } else if (current->loyalty >= 40) {
                std::cout << YELLOW << " [Loyalty: " << current->loyalty << "]" << RESET;
            } else {
                std::cout << RED << " [Unstable: " << current->loyalty << "]" << RESET;
            }
            std::cout << std::endl;
            current = current->next;
        }
        std::cout << "--------------------" << std::endl;
        // Pack benefits summary
        int benefit = calculatePackBenefits(*this);
        if (benefit > 0) {
            std::cout << GREEN << "Pack Bonus: +" << benefit << " to stats" << RESET << std::endl;
        }
    }
    std::cout << CYAN << "==================" << RESET << std::endl;
}

int Pack::getSize() const {
    return size;
}

PackMember* Pack::getHead() const {
    return head;
}

bool Pack::removeMember(std::string name) {
    PackMember* current = head;
    PackMember* prev = nullptr;
    while (current) {
        if (current->name == name) {
            if (prev) prev->next = current->next;
            else head = current->next;
            delete current;
            size--;
            return true;
        }
        prev = current;
        current = current->next;
    }
    return false;
}

void Pack::updateLoyalty(int hungerIncrease) {
    if (hungerIncrease <= 0) return; // Nothing to do if no hunger increase

    PackMember* current = head;
    PackMember* prev = nullptr;

    while (current) {
        // Loyalty decreases if wolf's hunger increases (pack doesn't get fed)
        current->loyalty -= hungerIncrease; // Higher hunger increase = more loyalty loss
        if (current->loyalty < 0) current->loyalty = 0;

        // If loyalty gets too low, member might leave
        if (current->loyalty <= 10) { // Increased threshold from 20 to 10 to make pack more stable
            std::cout << current->name << " has left the pack due to low loyalty!" << std::endl;

            PackMember* nodeToDelete = current;
            PackMember* next = current->next;

            if (prev) {
                // Middle or end of list
                prev->next = next;
            } else {
                // Beginning of list
                head = next;
            }

            delete nodeToDelete;
            size--;

            current = next; // Move to next node
        } else {
            // Move to next node only if we didn't delete the current one
            prev = current;
            current = current->next;
        }
    }
}

// Function to calculate pack benefits
int calculatePackBenefits(const Pack& pack) {
    int totalBenefit = 0;
    PackMember* current = pack.getHead();
    while (current) {
        // Different roles provide different benefits
        if (current->role == "Hunter") {
            // Hunters improve hunting success and provide food
            totalBenefit += (current->loyalty / 20); // Higher loyalty = more benefit
        } else if (current->role == "Scout") {
            // Scouts provide early warnings and better navigation
            totalBenefit += (current->loyalty / 25);
        } else if (current->role == "Guard") {
            // Guards provide protection
            totalBenefit += (current->loyalty / 30);
        }
        current = current->next;
    }
    return totalBenefit;
}
