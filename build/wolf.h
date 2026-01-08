#ifndef WOLF_H
#define WOLF_H

#include <string>

struct Wolf {
    int health = 100; // 0-100
    int hunger = 0;   // 0-100
    int energy = 100; // 0-100
    int reputation = 0; // 0-100
    int spirit = 50;   // 0-100, wolf spirit/morale

    bool isAlive() const {
        return health > 0 && hunger < 100;
    }

    void updateHunger(int amount) {
        int oldHunger = hunger;
        hunger += amount;
        if (hunger < 0) hunger = 0;
        if (hunger > 100) hunger = 100;
        // Only penalize health when hunger INCREASES past 80 (not when eating)
        if (oldHunger <= 80 && hunger > 80) {
            health -= 5; // Hunger affects health when it gets too high
        }
        if (health < 0) health = 0;
    }

    void updateEnergy(int amount) {
        energy += amount;
        if (energy < 0) energy = 0;
        if (energy > 100) energy = 100;
    }

    void updateHealth(int amount) {
        health += amount;
        if (health < 0) health = 0;
        if (health > 100) health = 100;
    }

    void updateReputation(int amount) {
        reputation += amount;
        if (reputation < 0) reputation = 0;
        if (reputation > 100) reputation = 100;
    }

    void updateSpirit(int amount) {
        spirit += amount;
        if (spirit < 0) spirit = 0;
        if (spirit > 100) spirit = 100;
    }

    // Validation functions
    bool canMakeChoice(int energyReq = 0) const {
        return health > 0 && hunger < 100 && energy >= energyReq;
    }

    // Pack benefit functions
    int getPackBenefit() const { // Calculate benefits from pack members
        // This function will be called from elsewhere where Pack is available
        // For now, we'll return a placeholder - actual implementation will be in main game logic
        return 0;
    }
};

#endif