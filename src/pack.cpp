#include "../include/pack.h"
#include <iostream>

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
    if (size == 0) {
        std::cout << "No pack members." << std::endl;
        return;
    }
    PackMember* current = head;
    std::cout << "Pack Members:" << std::endl;
    while (current) {
        std::cout << "- " << current->name << " (" << current->role << ", Loyalty: " << current->loyalty << ")" << std::endl;
        current = current->next;
    }
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
    PackMember* current = head;
    while (current) {
        // Loyalty decreases if wolf's hunger increases (pack doesn't get fed)
        if (hungerIncrease > 0) {
            current->loyalty -= hungerIncrease; // Higher hunger increase = more loyalty loss
            if (current->loyalty < 0) current->loyalty = 0;

            // If loyalty gets too low, member might leave
            if (current->loyalty <= 20) {
                std::cout << current->name << " has left the pack due to low loyalty!" << std::endl;
                PackMember* next = current->next;
                if (head == current) {
                    head = next;
                    delete current;
                    current = next;
                    size--;
                } else {
                    PackMember* prev = head;
                    while (prev && prev->next != current) prev = prev->next;
                    if (prev) {
                        prev->next = next;
                        delete current;
                        current = next;
                        size--;
                    }
                }
            } else {
                current = current->next;
            }
        } else {
            current = current->next;
        }
    }
}

void Pack::displayPackWithLoyalty() {
    if (size == 0) {
        std::cout << "No pack members." << std::endl;
        return;
    }
    PackMember* current = head;
    std::cout << "Pack Members:" << std::endl;
    while (current) {
        std::string loyaltyColor = "";
        std::string loyaltyStatus = "";
        if (current->loyalty >= 70) {
            loyaltyStatus = " (Loyal)";
        } else if (current->loyalty >= 40) {
            loyaltyStatus = " (Stable)";
        } else {
            loyaltyStatus = " (Unstable)";
        }
        std::cout << "- " << current->name << " (" << current->role << ", Loyalty: " << current->loyalty << loyaltyStatus << ")" << std::endl;
        current = current->next;
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
