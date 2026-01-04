#include <iostream>
#include <random>
#include <vector>
#include <iomanip>
#include "../include/wolf.h"
#include "../include/decision_tree.h"
#include "../include/priority_queue.h"
#include "../include/game_stack.h"
#include "../include/inventory.h"
#include "../include/pack.h"
#include "../include/action_queue.h"

// Forward declaration for the function defined later
int calculatePackBenefits(const Pack& pack);

class BalanceTester {
private:
    std::vector<float> survivalRates;
    std::vector<int> avgDaysSurvived;
    std::vector<int> avgPackSize;
    
public:
    void runBalanceTest() {
        std::cout << "==================================" << std::endl;
        std::cout << "Balance Testing - Running 100 simulations..." << std::endl;
        std::cout << "==================================" << std::endl;
        
        int totalSurvivals = 0;
        int totalSimulations = 100;
        
        for (int i = 0; i < totalSimulations; i++) {
            bool survived = runSingleSimulation();
            if (survived) {
                totalSurvivals++;
            }
            
            if ((i + 1) % 20 == 0) {
                std::cout << "Completed " << (i + 1) << " simulations..." << std::endl;
            }
        }
        
        float survivalRate = (float)totalSurvivals / totalSimulations * 100;
        std::cout << std::endl;
        std::cout << "Balance Test Results:" << std::endl;
        std::cout << "Total simulations: " << totalSimulations << std::endl;
        std::cout << "Survivals: " << totalSurvivals << std::endl;
        std::cout << "Survival Rate: " << std::fixed << std::setprecision(2) << survivalRate << "%" << std::endl;
        
        if (survivalRate >= 10 && survivalRate <= 80) {
            std::cout << "✓ Difficulty is balanced (survival rate between 10-80%)" << std::endl;
        } else if (survivalRate > 80) {
            std::cout << "⚠ Difficulty may be too easy (high survival rate)" << std::endl;
        } else {
            std::cout << "⚠ Difficulty may be too hard (low survival rate)" << std::endl;
        }
        
        std::cout << "==================================" << std::endl;
    }
    
private:
    bool runSingleSimulation() {
        // Initialize game state for simulation
        Wolf wolf;
        DecisionTree tree;
        tree.buildSampleTree();
        PriorityQueue events;
        GameStack history;
        Inventory inventory;
        Pack pack;
        ActionQueue actions;
        int dayCounter = 1;
        
        // Set up basic events for simulation
        auto bearAttack = [](Wolf& w){ w.updateHealth(-20); };
        auto foundBerries = [](Wolf& w){ w.updateHunger(-10); };
        auto severeHunger = [](Wolf& w){ w.updateHealth(-10); };
        auto injury = [](Wolf& w){ w.updateHealth(-15); w.updateEnergy(-10); };
        auto weatherChange = [](Wolf& w){ w.updateEnergy(-5); };
        
        events.insert({"Bear Attack", 1, "A bear approaches!", bearAttack});
        events.insert({"Found Berries", 3, "You find edible berries nearby.", foundBerries});
        events.insert({"Severe Hunger", 2, "Hunger is worsening.", severeHunger});
        events.insert({"Injury", 2, "You injure your paw.", injury});
        events.insert({"Weather Change", 3, "A storm approaches.", weatherChange});
        
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_real_distribution<> dis(0.0, 1.0);
        std::uniform_int_distribution<> choiceDis(0, 1); // Random choice A or B
        
        // Simulate up to 30 days
        for (dayCounter = 1; dayCounter <= 30; dayCounter++) {
            // Check if wolf is still alive
            if (!wolf.isAlive()) {
                return false; // Died before reaching 30 days
            }
            
            // Randomly choose A or B (simplified simulation)
            DecisionNode* current = tree.getCurrentNode();
            if (current && !current->isEnding) {
                // Make a random choice if possible
                if (wolf.canMakeChoice(3)) { // Assume minimum energy requirement
                    bool chooseA = choiceDis(gen) == 0;
                    
                    if (chooseA && current->left) {
                        tree.setCurrentNode(current->left);
                        
                        // Add some items based on scenario
                        if (current->left->scenarioID == 4) inventory.addItem("Rabbit Meat", FOOD, -30, 1);
                        if (current->left->scenarioID == 6) inventory.addItem("Berries", FOOD, -10, 1);
                    } else if (!chooseA && current->right) {
                        tree.setCurrentNode(current->right);
                    }
                }
            }
            
            // Update stats (hunger increases)
            wolf.updateHunger(5); // Normal hunger increase
            
            // Update pack loyalty
            pack.updateLoyalty(5);
            
            // Apply pack benefits
            int packBenefit = calculatePackBenefits(pack);
            if (packBenefit > 0) {
                if (wolf.energy < 100) wolf.energy += packBenefit;
                if (wolf.health < 100 && packBenefit > 10) wolf.health += packBenefit / 2;
                if (wolf.spirit < 100) wolf.spirit += packBenefit;
                if (wolf.spirit > 100) wolf.spirit = 100;
                if (wolf.energy > 100) wolf.energy = 100;
                if (wolf.health > 100) wolf.health = 100;
            }
            
            // Random event check
            if (dis(gen) < 0.3) { // 30% event chance
                if (!events.isEmpty()) {
                    Event e = events.extractMin();
                    e.effect(wolf);
                    
                    // Add item for found berries
                    if (e.name == "Found Berries") inventory.addItem("Berries", FOOD, -10, 1);
                    
                    // Reinsert the event back into the queue to cycle events
                    events.insert(e);
                }
            }
        }
        
        // Check if survived 30 days
        return dayCounter >= 30 && wolf.isAlive();
    }
    
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
};

int main() {
    BalanceTester tester;
    tester.runBalanceTest();
    return 0;
}