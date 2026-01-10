#include <iostream>
#include <string>
#include <random>
#include <cctype>
#include <limits>
#include <algorithm>
#include <QtWidgets/QApplication>
#include "../include/wolf.h"
#include "../include/decision_tree.h"
#include "../include/priority_queue.h"
#include "../include/game_stack.h"
#include "../include/inventory.h"
#include "../include/pack.h"
#include "../include/action_queue.h"
#include "../include/game_window.h"
#include "../include/ascii_art.h"
#include "../include/achievements.h"
#include "../include/game_types.h"

// Color codes
#define RESET   "\033[0m"
#define RED     "\033[31m"
#define GREEN   "\033[32m"
#define YELLOW  "\033[33m"
#define BLUE    "\033[34m"
#define CYAN    "\033[36m"
#define BOLD    "\033[1m"

Difficulty difficulty = NORMAL;
Storyline storyline = CLASSIC;
GameMode currentMode = TERMINAL;



// Global variables
float eventChance = 0.3f;
int hungerIncrease = 5;

// Function declarations
void showMenu();
void showSettings();
void applyDifficulty();
void saveGame(const Wolf& wolf, const DecisionTree& tree, const Inventory& inventory, const Pack& pack, int dayCounter, int slot = 1);
void loadGame(Wolf& wolf, DecisionTree& tree, Inventory& inventory, Pack& pack, int& dayCounter, int slot = 1);
int runGuiGame(QApplication& app, Wolf& wolf, DecisionTree& tree, PriorityQueue& events, GameStack& history, ActionQueue& actions, Inventory& inventory, Pack& pack, int& dayCounter, Difficulty& difficulty, Storyline& storyline);
void cleanupFallbackNodes(); // Cleanup fallback nodes to prevent memory leaks - Kept for compatibility

// Fallback node cleanup function
void cleanupFallbackNodes() {
    // No-op since we no longer use fallback nodes - trees are now complete
    // This function is kept for compatibility with existing code
}

int main(int argc, char* argv[]) {
    // Terminal mode
    DecisionTree tree;
        // Tree will be built based on storyline selection
    // Define event functions that take Wolf reference
    auto bearAttack = [](Wolf& w){ w.updateHealth(-20); };
    auto foundBerries = [](Wolf& w){ w.updateHunger(-10); };
    auto severeHunger = [](Wolf& w){ w.updateHealth(-10); };
    auto injury = [](Wolf& w){ w.updateHealth(-15); w.updateEnergy(-10); };
    auto weatherChange = [](Wolf& w){ w.updateEnergy(-5); };
    auto loneWolf = [](Wolf& w){ w.updateSpirit(-10); }; // Feeling lonely without pack
    auto packRecruit = [](Wolf& w){ w.updateSpirit(15); }; // Feeling strong with pack

    PriorityQueue events;
    events.insert({"Bear Attack", 1, "A bear approaches!", bearAttack});
    events.insert({"Found Berries", 3, "You find food and water near a stream!", foundBerries});
    events.insert({"Severe Hunger", 2, "Hunger is worsening.", severeHunger});
    events.insert({"Injury", 2, "You injure your paw.", injury});
    events.insert({"Weather Change", 3, "A storm approaches.", weatherChange});
    events.insert({"Lone Wolf", 3, "You feel lonely without a pack.", loneWolf});
    events.insert({"Pack Recruitment", 2, "You encounter a lone wolf willing to join your pack.", packRecruit});
    GameStack history;
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> dis(0.0, 1.0);

    Inventory inventory;
    Pack pack;
    ActionQueue actions;
    Achievements achievements; // Initialize achievements system

    // Menu loop and game execution - wrapped in outer loop for return to menu functionality
    bool gameRunning = true;
    while (gameRunning) {
        int menuChoice;
        Wolf wolf;
        int dayCounter = 1; // Initialize day counter
        int decisionsSinceAutoSave = 0; // Track decisions for auto-save
        bool loaded = false;
        do {
            showMenu();
            std::cin >> menuChoice;
            switch (menuChoice) {
            case 1: // Start New Game (Terminal)
                        currentMode = TERMINAL;
                        wolf = Wolf(); // Reset
                        applyDifficulty();
                        
                        // Add starting inventory based on difficulty
                        inventory.addStartingSupplies(difficulty);
                        
                        // Build appropriate story tree
                        switch (storyline) {
                            case CLASSIC:
                                tree.buildClassicStory();
                                std::cout << GREEN << "\nStarting CLASSIC storyline - Balanced adventure!\n" << RESET << std::endl;
                                break;
                            case SURVIVAL:
                                tree.buildSurvivalStory();
                                wolf.health = 40;
                                wolf.hunger = 80;
                                wolf.energy = 60;
                                std::cout << YELLOW << "\nStarting SURVIVAL storyline - Extreme difficulty!\n" << RESET << std::endl;
                                break;
                            case PACK:
                                tree.buildPackStory();
                                wolf.reputation = 20;
                                pack.addMember("Beta", "Second-in-Command", 70);
                                pack.addMember("Scout", "Scout", 60);
                                std::cout << BLUE << "\nStarting PACK storyline - Leadership focus!\n" << RESET << std::endl;
                                break;
                        }
                        
                        loaded = false;
                        break;
            case 2: // Load Game (Terminal)
                currentMode = TERMINAL;
                std::cout << "Enter load slot (1-3): ";
                {
                    int inputSlot;
                    std::cin >> inputSlot;
                    if (inputSlot < 1 || inputSlot > 3) inputSlot = 1; // Default to slot 1
                    loadGame(wolf, tree, inventory, pack, dayCounter, inputSlot);
                }
                applyDifficulty();
                loaded = true;
                break;
            case 3: // Start New Game (GUI)
                        currentMode = GUI;
                        wolf = Wolf(); // Reset
                        applyDifficulty();
                        
                        // Add starting inventory based on difficulty
                        inventory.addStartingSupplies(difficulty);
                        
                        // Build appropriate story tree
                        switch (storyline) {
                            case CLASSIC:
                                tree.buildClassicStory();
                                break;
                            case SURVIVAL:
                                tree.buildSurvivalStory();
                                wolf.health = 40;
                                wolf.hunger = 80;
                                wolf.energy = 60;
                                break;
                            case PACK:
                                tree.buildPackStory();
                                wolf.reputation = 20;
                                pack.addMember("Beta", "Second-in-Command", 70);
                                pack.addMember("Scout", "Scout", 60);
                                break;
                        }
                        
                        loaded = false;
                        break;
            case 4: // Load Game (GUI)
                currentMode = GUI;
                std::cout << "Enter load slot (1-3): ";
                {
                    int inputSlot;
                    std::cin >> inputSlot;
                    if (inputSlot < 1 || inputSlot > 3) inputSlot = 1; // Default to slot 1
                    loadGame(wolf, tree, inventory, pack, dayCounter, inputSlot);
                }
                applyDifficulty();
                loaded = true;
                break;
            case 5: // Settings
                int settingChoice;
                do {
                    showSettings();
                    std::cin >> settingChoice;
                    switch (settingChoice) {
                        case 1: {
                            std::cout << "Select Difficulty (1-Easy, 2-Normal, 3-Hard): ";
                            int d;
                            std::cin >> d;
                            if (d >=1 && d <=3) difficulty = static_cast<Difficulty>(d-1);
                            break;
                        }
                        case 2: {
                            std::cout << "Select Storyline (1-Classic, 2-Survival, 3-Pack): ";
                            int s;
                            std::cin >> s;
                            if (s >=1 && s <=3) storyline = static_cast<Storyline>(s-1);
                            break;
                        }
                        case 3:
                            break;
                    }
                } while (settingChoice != 3);
                break;
            case 6:
                return 0;
            default:
                std::cout << RED << "Invalid option." << RESET << std::endl;
        }
    } while (menuChoice != 1 && menuChoice != 2 && menuChoice != 3 && menuChoice != 4);

    if (currentMode == TERMINAL) {

        std::cout << "\n" << CYAN << "Press Enter to begin your journey..." << RESET;
                std::cin.ignore();
                std::cin.get();

        while (wolf.isAlive()) {
            if (!actions.isEmpty()) {
                actions.processNext(wolf);
                std::string healthColor = (wolf.health < 50) ? RED : GREEN;
                std::string hungerColor = (wolf.hunger > 80) ? RED : GREEN;
                std::string energyColor = (wolf.energy < 50) ? RED : GREEN;
                std::string spiritColor = (wolf.spirit < 30) ? RED : (wolf.spirit > 70) ? BLUE : YELLOW;
                std::string repColor = (wolf.reputation < 30) ? RED : (wolf.reputation > 70) ? BLUE : YELLOW;
                std::cout << "Day: " << dayCounter << ", Health: " << healthColor << wolf.health << RESET << ", Hunger: " << hungerColor << wolf.hunger << RESET << ", Energy: " << energyColor << wolf.energy << RESET << ", Spirit: " << spiritColor << wolf.spirit << RESET << ", Reputation: " << repColor << wolf.reputation << RESET << std::endl;
                continue;
            }

            DecisionNode* currentDecisionNode = tree.getCurrentNode();
            // Display ASCII art for the current scenario
            if (!currentDecisionNode) {
                std::cout << RED << "ERROR: No current scenario available!" << RESET << std::endl;
                break;
            }
            std::cout << AsciiArt::getScenarioArt(currentDecisionNode->scenarioID) << std::endl;
            std::cout << BOLD << currentDecisionNode->description << RESET << std::endl;
            if (currentDecisionNode->isEnding) {
                std::cout << GREEN << currentDecisionNode->endingText << RESET << std::endl;
                break;
            }
            std::cout << YELLOW << "A: " << currentDecisionNode->choiceA_text << RESET << std::endl;
            std::cout << YELLOW << "B: " << currentDecisionNode->choiceB_text << RESET << std::endl;

            // Push current state before choice (including day and a copy of inventory)
            history.push({currentDecisionNode, wolf.health, wolf.hunger, wolf.energy, dayCounter, inventory.clone()});

            std::cout << CYAN << "Choose A, B, U (undo), S (save), L (load), I (inventory), V (use item), P (pack), E (achievements), Q (quit): " << RESET;
            char input;
            std::cin >> input;
            if (!std::cin) {
                std::cin.clear();
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                input = ' '; // invalid
            } else {
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // ignore rest of line
            }
            input = std::toupper(input);
            if (input == 'U') {
                if (!history.isEmpty()) {
                    GameState prev = history.pop();
                    wolf.health = prev.health;
                    wolf.hunger = prev.hunger;
                    wolf.energy = prev.energy;
                    dayCounter = prev.day;  // Restore day counter
                    // Restore inventory from the saved copy
                    if (prev.inventory) {
                        inventory.replaceWith(prev.inventory);
                        delete prev.inventory; // Clean up the copied inventory after restoration
                    }
                    tree.setCurrentNode(prev.currentNode);
                    continue;
                } else {
                    std::cout << "No undo available." << std::endl;
                    continue;
                }
            } else if (input == 'A') {
                // Check if the wolf can make this choice
                if (!wolf.canMakeChoice(5)) { // Assume choice A requires 5 energy
                    std::cout << RED << "Cannot make this choice - insufficient health, too hungry, or not enough energy!" << RESET << std::endl;
                    // Pop the pushed state since invalid and clean up inventory
                    GameState invalidState = history.pop();
                    if (invalidState.inventory) {
                        delete invalidState.inventory;
                    }
                    continue;
                }

                // Additional checks based on spirit and reputation
                if (wolf.spirit < 20) {
                    // Very low spirit affects ability to make bold choices
                    std::cout << YELLOW << "😔 Your extremely low spirit makes you hesitant to take risks." << RESET << std::endl;
                }
                if (!currentDecisionNode->left) {
                    std::cout << RED << "ERROR: No left path available in decision tree! This should not happen after tree completion." << RESET << std::endl;
                    // This shouldn't happen anymore since we've completed the trees
                    // Just break out of the game loop
                    break;
                } else {
                    tree.setCurrentNode(currentDecisionNode->left);
                    if (currentDecisionNode->left->scenarioID == 2) {
                        actions.enqueue({"Tracking the deer...", [](Wolf& w){ w.updateEnergy(-10); std::cout << "Spent energy tracking." << std::endl; }});
                        actions.enqueue({"Attacking the deer...", [](Wolf& w){ w.updateHunger(-30); std::cout << "Killed deer! Hunger reduced." << std::endl; }});
                    }
                    // Add items
                    if (currentDecisionNode->left->scenarioID == 4) inventory.addItem("Small Fish", FOOD, -30, 1);
                    // Add reputation changes based on scenario
                    if (currentDecisionNode->left->scenarioID == 12) wolf.updateReputation(20); // Leading with strength
                    if (currentDecisionNode->left->scenarioID == 9) wolf.updateReputation(-10); // Sparing traveler
                }
            } else if (input == 'B') {
                // Check if the wolf can make this choice
                if (!wolf.canMakeChoice(3)) { // Assume choice B requires 3 energy
                    std::cout << RED << "Cannot make this choice - insufficient health, too hungry, or not enough energy!" << RESET << std::endl;
                    // Pop the pushed state since invalid and clean up inventory
                    GameState invalidState = history.pop();
                    if (invalidState.inventory) {
                        delete invalidState.inventory;
                    }
                    continue;
                }

                // Additional checks based on spirit and reputation
                if (wolf.spirit < 20) {
                    // Very low spirit affects ability to make bold choices
                    std::cout << YELLOW << "😔 Your extremely low spirit makes you hesitant to take risks." << RESET << std::endl;
                }
                if (!currentDecisionNode->right) {
                    std::cout << RED << "ERROR: No right path available in decision tree! This should not happen after tree completion." << RESET << std::endl;
                    // This shouldn't happen anymore since we've completed the trees
                    // Just break out of the game loop
                    break;
                } else {
                    tree.setCurrentNode(currentDecisionNode->right);
                    // Add reputation changes based on scenario
                    if (currentDecisionNode->right->scenarioID == 12) wolf.updateReputation(10); // Leading with wisdom
                    if (currentDecisionNode->right->scenarioID == 9) wolf.updateReputation(15); // Embracing shame
                }
            } else if (input == 'S') {
                std::cout << "Enter save slot (1-3): ";
                int saveSlot;
                std::cin >> saveSlot;
                if (saveSlot < 1 || saveSlot > 3) saveSlot = 1; // Default to slot 1
                saveGame(wolf, tree, inventory, pack, dayCounter, saveSlot);
                // Clean up the saved state when popping for save
                GameState savedState = history.pop(); // Pop the state and clean up the inventory copy
                if (savedState.inventory) {
                    delete savedState.inventory;
                }
                continue;
            } else if (input == 'L') {
                std::cout << "Enter load slot (1-3): ";
                int loadSlot;
                std::cin >> loadSlot;
                if (loadSlot < 1 || loadSlot > 3) loadSlot = 1; // Default to slot 1
                loadGame(wolf, tree, inventory, pack, dayCounter, loadSlot);
                continue;
            } else if (input == 'I') {
                inventory.displayInventory();
                std::cout << CYAN << "Press Enter to continue..." << RESET;
                std::cin.ignore();
                std::cin.get();
                continue;
            } else if (input == 'V') {  // V for Use Item (View inventory then select)
                // Show inventory first
                inventory.displayWithNumbers();
                // Ask for item selection
                std::cout << CYAN << "Enter item number to use (0 to cancel): " << RESET;
                int itemNum;
                std::cin >> itemNum;
                if (!std::cin) {
                    std::cin.clear();
                    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                    std::cout << RED << "Invalid input." << RESET << std::endl;
                    continue;
                }
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // ignore rest of line
                if (itemNum == 0) {
                    std::cout << "Cancelled." << std::endl;
                    continue;
                }
                // Use the item by number
                if (inventory.useItemByNumber(itemNum, wolf)) {
                    std::cout << GREEN << "Item used successfully!" << RESET << std::endl;
                } else {
                    std::cout << RED << "Invalid item number or item cannot be used." << RESET << std::endl;
                }
                continue;
            } else if (input == 'P') {
                pack.displayPack();
                continue;
            } else if (input == 'E') {  // E for achievements
                achievements.displayAchievements();
                continue;
            } else if (input == 'Q') {
                cleanupFallbackNodes();
                break;
            } else {
                std::cout << "Invalid choice." << std::endl;
                // Pop the pushed state since invalid
                history.pop();
                continue;
            }

            // Recruitment - Add Luna when first mentioned (scenario 11)
            if (currentDecisionNode->scenarioID == 11) {
                std::cout << GREEN << "🐺 Luna has joined your pack as a Hunter!" << RESET << std::endl;
                pack.addMember("Luna", "Hunter", 80);
                // Luna might bring some supplies
                if (gen() % 2 == 0) { // 50% chance
                    inventory.addItem("Common Mallow", HERB, 10, 1);
                    std::cout << GREEN << "🌿 Luna brought some healing herbs from her previous travels!" << RESET << std::endl;
                }
            }
            // Add young wolf if rescued (scenario 12, choice A)
            if (currentDecisionNode->scenarioID == 12 && input == 'A') {
                std::cout << GREEN << "🐺 Fenris has joined your pack as a Scout!" << RESET << std::endl;
                pack.addMember("Fenris", "Scout", 70);
                // Fenris might bring some supplies
                if (gen() % 3 == 0) { // 33% chance
                    inventory.addItem("Common Mallow", HERB, 10, 1);
                    std::cout << GREEN << "🌿 Fenris found some healing herbs nearby!" << RESET << std::endl;
                }
            }
            // Territory scenario - pack already has members from previous scenarios
            if (currentDecisionNode->scenarioID == 13) {
                std::cout << YELLOW << "🐺 Your pack has " << pack.getSize() << " member(s). Establish your territory!" << RESET << std::endl;
                // Add some healing items when establishing territory
                if (gen() % 3 == 0) { // 33% chance to find healing herbs
                    inventory.addItem("Common Mallow", HERB, 15, 1);
                    std::cout << GREEN << "🌿 While exploring your new territory, you find some healing herbs!" << RESET << std::endl;
                }
            }
            // Add Ally as Guard (scenario 16)
            if (currentDecisionNode->scenarioID == 16) {
                std::cout << GREEN << "🐺 Ally has joined your pack as a Guard!" << RESET << std::endl;
                pack.addMember("Ally", "Guard", 60);
                // Ally might bring some supplies
                if (gen() % 3 == 0) { // 33% chance
                    inventory.addItem("Common Mallow", HERB, 10, 1);
                    std::cout << GREEN << "🌿 Ally brought some healing herbs from his previous pack!" << RESET << std::endl;
                }
            }

            // Pack player choice effects
            if (currentDecisionNode->scenarioID == 68 && input == 'A') {
                // Accepted recruitment
                pack.addMember("Recruit", "Scout", 65);
                wolf.updateHunger(10); // More mouths to feed
                std::cout << GREEN << "🐺 New pack member recruited! Scout added." << RESET << std::endl;
            }
            if (currentDecisionNode->scenarioID == 69) {
                // Training choice
                if (input == 'A') {
                    // Hunting training
                    wolf.energy -= 20;
                    std::cout << GREEN << "🐺 Pack hunting skills improved!" << RESET << std::endl;
                    // Future food gains increased (this would need more complex implementation)
                } else {
                    // Scouting training
                    wolf.energy -= 15;
                    eventChance += 0.1f; // Better event chances
                    std::cout << GREEN << "👁️ Pack scouting skills improved!" << RESET << std::endl;
                }
            }

            // Calculate dynamic hunger increase based on difficulty and pack size
            int currentHungerIncrease = hungerIncrease;

            // If the wolf has a pack, hunger increases slightly faster (more mouths to feed)
            if (pack.getSize() > 0) {
                // Add bounds checking to prevent overflow
                int packSize = std::min(pack.getSize(), 100); // Cap at reasonable value
                currentHungerIncrease = static_cast<int>(currentHungerIncrease * (1.0 + (packSize * 0.1)));
            }

            // Update stats
            wolf.updateHunger(currentHungerIncrease);

            // Update pack loyalty based on hunger increase
            pack.updateLoyalty(currentHungerIncrease);


            // Increment day counter after each decision/action
            dayCounter++;

            // ========== INVENTORY CONSUMPTION SYSTEM ==========
            // Food consumption every 5 days
            if (dayCounter % 5 == 0 && dayCounter > 1) {
                std::cout << CYAN << "\n📅 Day " << dayCounter << " - Time to eat!" << RESET << std::endl;

                if (inventory.useItem("Small Fish", wolf)) {
                    std::cout << GREEN << "  ✓ Ate Small Fish (hunger reduced)" << RESET << std::endl;
                } else if (inventory.useItem("Winter Berries", wolf)) {
                    std::cout << GREEN << "  ✓ Ate Winter Berries (hunger reduced)" << RESET << std::endl;
                } else if (inventory.useItem("Fresh Water", wolf)) {
                    std::cout << GREEN << "  ✓ Drank Fresh Water (hunger reduced)" << RESET << std::endl;
                } else if (inventory.useItem("Bird Egg", wolf)) {
                    std::cout << GREEN << "  ✓ Ate Bird Egg (hunger reduced)" << RESET << std::endl;
                } else if (inventory.useItem("Insects", wolf)) {
                    std::cout << GREEN << "  ✓ Ate Insects (hunger reduced)" << RESET << std::endl;
                } else if (inventory.useItem("Frog", wolf)) {
                    std::cout << GREEN << "  ✓ Ate Frog (hunger reduced)" << RESET << std::endl;
                } else if (inventory.useItem("Fresh Meat", wolf)) {
                    std::cout << GREEN << "  ✓ Ate Fresh Meat (hunger reduced)" << RESET << std::endl;
                } else {
                    std::cout << RED << "  ✗ No food! Your pack goes hungry." << RESET << std::endl;
                    wolf.updateHunger(10);  // Extra hunger penalty
                }
            }

            // Healing item usage when injured - expanded threshold
            if (wolf.health < 60 && dayCounter > 1) {  // Lower threshold to make healing more accessible
                if (wolf.health < 40) {
                    std::cout << RED << "\n💊 Your wolf is critically injured!" << RESET << std::endl;
                } else {
                    std::cout << YELLOW << "\n💊 Your wolf needs healing!" << RESET << std::endl;
                }

                if (inventory.useItem("Common Mallow", wolf)) {
                    std::cout << GREEN << "  ✓ Used Common Mallow (health restored)" << RESET << std::endl;
                } else if (inventory.useItem("Root Paste", wolf)) {
                    std::cout << GREEN << "  ✓ Used Root Paste (health restored)" << RESET << std::endl;
                } else if (inventory.useItem("Moss Dressing", wolf)) {
                    std::cout << GREEN << "  ✓ Used Moss Dressing (health restored)" << RESET << std::endl;
                } else if (inventory.useItem("Bee Propolis", wolf)) {
                    std::cout << GREEN << "  ✓ Used Bee Propolis (health restored)" << RESET << std::endl;
                } else {
                    std::cout << RED << "  ✗ No healing items available!" << RESET << std::endl;
                }
            }

            // Random event check
            if (dis(gen) < eventChance) {
                if (!events.isEmpty()) {
                    Event e = events.extractMin();
                    std::cout << RED << e.description << RESET << std::endl;
                    e.effect(wolf);  // Apply effect directly to wolf
                    // Add items for found berries - use difficulty scaling
                    if (e.name == "Found Berries") {
                        // Add items based on difficulty
                        inventory.addRandomFood(difficulty);
                        if (difficulty == EASY) {
                            inventory.addItem("Fresh Water", WATER, 0, 2); // Extra water on easy
                        } else {
                            inventory.addItem("Fresh Water", WATER, 0, 1);
                        }
                    }
                    // Add pack member for recruitment event
                    if (e.name == "Pack Recruitment" && pack.getSize() < 5) { // Max 5 members
                        // Randomly select a role and name for the new pack member
                        std::string roles[] = {"Hunter", "Scout", "Guard"};
                        std::string names[] = {"Fenris", "Lupin", "Canis", "Wolfe", "Dire"};
                        int roleIdx = gen() % 3;
                        int nameIdx = gen() % 5;
                        int loyalty = 50 + (gen() % 30); // Loyalty between 50-80
                        pack.addMember(names[nameIdx] + std::to_string(gen() % 100), roles[roleIdx], loyalty);
                    }
                    // Reinsert the event back into the queue to cycle events
                    events.insert(e);
                }
            }

            // Apply pack benefits
            int packBenefit = calculatePackBenefits(pack);
            if (packBenefit > 0) {
                // Apply benefits based on pack size and loyalty
                if (wolf.energy < 100) wolf.energy += packBenefit; // Pack helps with energy
                if (wolf.health < 100 && packBenefit > 10) wolf.health += packBenefit / 2; // Pack helps with health if benefit is significant
                if (wolf.spirit < 100) wolf.spirit += packBenefit; // Pack improves spirit/morale
                if (wolf.spirit > 100) wolf.spirit = 100;
                if (wolf.energy > 100) wolf.energy = 100;
                if (wolf.health > 100) wolf.health = 100;
            }

            // Apply spirit-based effects
            if (wolf.spirit < 30) {
                // Low spirit affects decision-making and survival
                std::cout << YELLOW << "😔 Your low spirit affects your motivation and decision-making..." << RESET << std::endl;
                // Slightly reduce energy recovery rate
                if (wolf.energy < 100) wolf.energy -= 1; // Small penalty when spirit is very low
            } else if (wolf.spirit > 70) {
                // High spirit improves survival chances
                std::cout << GREEN << "💪 High spirit boosts your determination and resilience!" << RESET << std::endl;
                // Small bonus to energy or health recovery
                if (wolf.energy < 100) wolf.energy += 1; // Small bonus when spirit is high
            }

            // Apply reputation-based effects
            if (wolf.reputation < 30) {
                // Low reputation makes it harder to recruit pack members
                std::cout << YELLOW << "⚠️  Low reputation makes other wolves wary of joining your pack." << RESET << std::endl;
            } else if (wolf.reputation > 70) {
                // High reputation attracts more pack members and better treatment
                std::cout << GREEN << "🌟 High reputation attracts loyal pack members!" << RESET << std::endl;
                // Higher chance of successful pack recruitment
                if (gen() % 5 == 0 && pack.getSize() < 5) { // 20% chance to attract a new member
                    std::string roles[] = {"Hunter", "Scout", "Guard"};
                    std::string names[] = {"Fenris", "Lupin", "Canis", "Wolfe", "Dire"};
                    int roleIdx = gen() % 3;
                    int nameIdx = gen() % 5;
                    int loyalty = 60 + (gen() % 20); // Loyalty between 60-80
                    std::string newName = names[nameIdx] + std::to_string(gen() % 100);
                    pack.addMember(newName, roles[roleIdx], loyalty);
                    std::cout << GREEN << "🐺 " << newName << " has joined your pack due to your high reputation!" << RESET << std::endl;
                }
            }

            // Auto-save every 5 decisions (use slot 0 for auto-save)
            decisionsSinceAutoSave++;
            if (decisionsSinceAutoSave >= 5) {
                saveGame(wolf, tree, inventory, pack, dayCounter, 0); // Auto-save to slot 0
                decisionsSinceAutoSave = 0; // Reset counter
            }

            std::string healthColor = (wolf.health < 50) ? RED : GREEN;
            std::string hungerColor = (wolf.hunger > 80) ? RED : GREEN;
            std::string energyColor = (wolf.energy < 50) ? RED : GREEN;
            std::string spiritColor = (wolf.spirit < 30) ? RED : (wolf.spirit > 70) ? BLUE : YELLOW;
            std::string repColor = (wolf.reputation < 30) ? RED : (wolf.reputation > 70) ? BLUE : YELLOW;
            std::cout << "Day: " << dayCounter << ", Health: " << healthColor << wolf.health << RESET << ", Hunger: " << hungerColor << wolf.hunger << RESET << ", Energy: " << energyColor << wolf.energy << RESET << ", Spirit: " << spiritColor << wolf.spirit << RESET << ", Reputation: " << repColor << wolf.reputation << RESET << std::endl;

            // Check for achievements
            achievements.checkSurvivalAchievements(dayCounter);
            achievements.checkPackAchievements(pack.getSize());
            achievements.checkStatAchievements(wolf.health, wolf.hunger, wolf.energy);

            // Count items in inventory for achievement check
            int itemCount = 0;
            Item* currentItem = inventory.getHead();
            while (currentItem) {
                itemCount += currentItem->quantity;
                currentItem = currentItem->next;
            }
            achievements.checkItemAchievements(itemCount);

            // Check for game over conditions
            if (!wolf.isAlive()) {
                std::cout << RED << "You died!" << RESET << std::endl;
                break; // Exit the game loop
            }

            // Check for win condition (survive 30 days)
            if (dayCounter >= 30) {
                std::cout << GREEN << "Congratulations! You survived 30 days in the wilderness!" << RESET << std::endl;
                std::cout << "Final pack size: " << pack.getSize() << std::endl;

                // Check for win achievement
                achievements.unlockAchievement("SURVIVE_30");
                achievements.unlockAchievement("NO_DEATH"); // If player survived without dying

                break; // Exit the game loop
            }
        }
    } else if (currentMode == GUI) {
        bool guiRunning = true;
        while (guiRunning) {
            // Initialize Qt application
            QApplication app(argc, argv);

            // Run the GUI game
            int result = runGuiGame(app, wolf, tree, events, history, actions, inventory, pack, dayCounter, difficulty, storyline);

            // If the result indicates return to menu, reset and break
            if (result == -1) {
                std::cout << CYAN << "\nReturning to main menu..." << RESET << std::endl;
                // Reset game state for fresh start
                wolf = Wolf();
                dayCounter = 1;
                pack = Pack();
                inventory = Inventory();
                history.clear();
                actions.clear();
                currentMode = TERMINAL; // Reset to terminal mode for menu
                loaded = false;
                guiRunning = false;
                tree.reset(); // Reset decision tree to prevent memory leak
                cleanupFallbackNodes(); // Clean up any fallback nodes from GUI
            } else {
                // Normal exit from GUI
                return result;
            }
        }
        // GUI game ended - go back to menu by breaking outer loop
        break;
    }

    if (!wolf.isAlive()) {
        std::cout << "You died!" << std::endl;
    }

    // Cleanup fallback nodes before exiting game session
    cleanupFallbackNodes();

    // Continue outer loop to show menu again
    continue;
}

// Cleanup before final exit
cleanupFallbackNodes();
return 0;
}

void showMenu() {
    std::cout << BOLD << CYAN << R"(
╔═══════════════════════════════════════════╗
║         🐺 WOLF SURVIVAL GAME 🐺          ║
╚═══════════════════════════════════════════╝
)" << RESET << std::endl;

    std::cout << "  " << BOLD << GREEN << "1.▶ Start New Game (Terminal)" << RESET << std::endl;
    std::cout << "    2.Load Game (Terminal)" << std::endl;
    std::cout << "  " << BOLD << GREEN << "3.▶ Start New Game (GUI)" << RESET << std::endl;
    std::cout << "    4.Load Game (GUI)" << std::endl;
    std::cout << "  " << YELLOW << "5.⚙️ Settings" << RESET << std::endl;
    std::cout << "  " << RED << "6.❌ Quit" << RESET << std::endl;
    std::cout << std::endl << CYAN << "Choose an option: " << RESET;
}



void showSettings() {
    std::cout << BOLD << YELLOW << R"(
╔═══════════════════════════════════════════╗
║              ⚙️ SETTINGS ⚙️               ║
╚═══════════════════════════════════════════╝
)" << RESET << std::endl;

    std::cout << "  " << GREEN << "1.⚔️ Difficulty: " << RESET;
    switch (difficulty) {
        case EASY: std::cout << GREEN << "[EASY]" << RESET; break;
        case NORMAL: std::cout << YELLOW << "[NORMAL]" << RESET; break;
        case HARD: std::cout << RED << "[HARD]" << RESET; break;
    }
    std::cout << std::endl;

    std::cout << "  " << BLUE << "2.📖 Storyline: " << RESET;
    switch (storyline) {
        case CLASSIC: std::cout << CYAN << "[CLASSIC]" << RESET; break;
        case SURVIVAL: std::cout << GREEN << "[SURVIVAL]" << RESET; break;
        case PACK: std::cout << YELLOW << "[PACK]" << RESET; break;
    }
    std::cout << std::endl;

    std::cout << std::endl << "  " << RED << "3.⬅️ Back" << RESET << std::endl;
    std::cout << std::endl << CYAN << "Choose an option: " << RESET;
}

void applyDifficulty() {
    switch (difficulty) {
        case EASY:
            // Easy mode adjustments
            hungerIncrease = 3;
            eventChance = 0.2f;
            break;
        case NORMAL:
            // Normal mode (default)
            hungerIncrease = 5;
            eventChance = 0.3f;
            break;
        case HARD:
            // Hard mode adjustments
            hungerIncrease = 8;
            eventChance = 0.5f;
            break;
    }
}

#include <fstream>

void saveGame(const Wolf& wolf, const DecisionTree& tree, const Inventory& inventory, const Pack& pack, int dayCounter, int slot) {
    std::string filename = "savegame_" + std::to_string(slot) + ".txt";
    std::ofstream saveFile(filename);
    if (saveFile.is_open()) {
        try {
            // Save wolf stats
            saveFile << wolf.health << "\n";
            saveFile << wolf.hunger << "\n";
            saveFile << wolf.energy << "\n";
            saveFile << wolf.reputation << "\n";
            saveFile << wolf.spirit << "\n";
            saveFile << dayCounter << "\n";

            // Save current node ID (we'll need a function to get node ID)
            // For now, we'll save the scenario ID of the current node
            if (tree.getCurrentNode()) {
                saveFile << tree.getCurrentNode()->scenarioID << "\n";
            } else {
                saveFile << "1\n"; // Default to start node if null
            }

            // Save inventory
            Item* currentItem = inventory.getHead();
            int itemCount = 0;
            // First count items
            Item* temp = currentItem;
            while (temp) {
                itemCount++;
                temp = temp->next;
            }
            saveFile << itemCount << "\n";

            // Then save each item
            currentItem = inventory.getHead();
            while (currentItem) {
                saveFile << currentItem->name << "\n";
                saveFile << static_cast<int>(currentItem->type) << "\n";
                saveFile << currentItem->effect << "\n";
                saveFile << currentItem->quantity << "\n";
                currentItem = currentItem->next;
            }

            // Save pack
            PackMember* currentMember = pack.getHead();
            int memberCount = 0;
            // First count members
            PackMember* tempMember = currentMember;
            while (tempMember) {
                memberCount++;
                tempMember = tempMember->next;
            }
            saveFile << memberCount << "\n";

            // Then save each pack member
            currentMember = pack.getHead();
            while (currentMember) {
                saveFile << currentMember->name << "\n";
                saveFile << currentMember->role << "\n";
                saveFile << currentMember->loyalty << "\n";
                currentMember = currentMember->next;
            }

            // Save storyline and difficulty settings
            saveFile << static_cast<int>(storyline) << "\n";
            saveFile << static_cast<int>(difficulty) << "\n";

            saveFile.close();
            std::cout << "Game saved successfully!" << std::endl;
        } catch (const std::exception& e) {
            std::cout << "Error during save: " << e.what() << std::endl;
            if (saveFile.is_open()) {
                saveFile.close();
            }
        }
    } else {
        std::cout << "Error: Could not open save file for writing." << std::endl;
    }
}

void loadGame(Wolf& wolf, DecisionTree& tree, Inventory& inventory, Pack& pack, int& dayCounter, int slot) {
    std::string filename = "savegame_" + std::to_string(slot) + ".txt";
    std::ifstream loadFile(filename);
    if (loadFile.is_open()) {
        try {
            // Load wolf stats with validation
            if (!(loadFile >> wolf.health)) {
                std::cout << "Error: Invalid health value in save file." << std::endl;
                loadFile.close();
                return;
            }
            if (!(loadFile >> wolf.hunger)) {
                std::cout << "Error: Invalid hunger value in save file." << std::endl;
                loadFile.close();
                return;
            }
            if (!(loadFile >> wolf.energy)) {
                std::cout << "Error: Invalid energy value in save file." << std::endl;
                loadFile.close();
                return;
            }
            if (!(loadFile >> wolf.reputation)) {
                std::cout << "Error: Invalid reputation value in save file." << std::endl;
                loadFile.close();
                return;
            }
            if (!(loadFile >> wolf.spirit)) {
                std::cout << "Error: Invalid spirit value in save file." << std::endl;
                loadFile.close();
                return;
            }
            if (!(loadFile >> dayCounter)) {
                std::cout << "Error: Invalid day counter value in save file." << std::endl;
                loadFile.close();
                return;
            }

            // Validate loaded values
            if (wolf.health < 0 || wolf.health > 100 ||
                wolf.hunger < 0 || wolf.hunger > 100 ||
                wolf.energy < 0 || wolf.energy > 100 ||
                wolf.reputation < 0 || wolf.reputation > 100 ||
                wolf.spirit < 0 || wolf.spirit > 100 ||
                dayCounter < 1) {
                std::cout << "Error: Invalid values in save file." << std::endl;
                loadFile.close();
                return;
            }

            // Load current node ID
            int nodeId;
            if (!(loadFile >> nodeId)) {
                std::cout << "Error: Invalid node ID in save file." << std::endl;
                loadFile.close();
                return;
            }

            // Validate node ID range
            if (nodeId < 0 || nodeId > 1000) {
                std::cout << "Error: Node ID out of valid range in save file." << std::endl;
                loadFile.close();
                return;
            }

            // Build the correct story tree based on storyline (will be set from save file later)
            // First build Classic as default, then we'll navigate to the correct node
            tree.buildClassicStory();
            DecisionNode* foundNode = tree.findNodeById(nodeId);
            if (foundNode) {
                tree.setCurrentNode(foundNode);
            } else {
                // If node not found, go to root
                std::cout << "Warning: Node ID " << nodeId << " not found in tree. Loading from start." << std::endl;
                tree.setCurrentNode(tree.getRoot());
            }

            // Clear current inventory using safe clear method
            inventory.clear();

            // Load inventory
            int itemCount;
            if (!(loadFile >> itemCount)) {
                std::cout << "Error: Invalid item count in save file." << std::endl;
                loadFile.close();
                return;
            }

            if (itemCount < 0 || itemCount > 100) { // Reasonable upper limit
                std::cout << "Error: Invalid item count " << itemCount << " in save file." << std::endl;
                loadFile.close();
                return;
            }

            for (int i = 0; i < itemCount; i++) {
                std::string name;
                int typeInt;
                int effect, quantity;

                if (!(loadFile >> name)) {
                    std::cout << "Error: Invalid item name in save file." << std::endl;
                    break;  // Stop loading items but continue with other data
                }

                // Validate string length to prevent buffer overflow
                if (name.length() > 100) {
                    std::cout << "Error: Item name too long in save file." << std::endl;
                    break;
                }

                if (!(loadFile >> typeInt)) {
                    std::cout << "Error: Invalid item type in save file." << std::endl;
                    break;
                }
                if (!(loadFile >> effect)) {
                    std::cout << "Error: Invalid item effect in save file." << std::endl;
                    break;
                }
                if (!(loadFile >> quantity)) {
                    std::cout << "Error: Invalid item quantity in save file." << std::endl;
                    break;
                }

                // Validate quantity
                if (quantity <= 0 || quantity > 1000) { // Add upper bound check
                    std::cout << "Warning: Invalid item quantity " << quantity << ". Skipping item." << std::endl;
                    continue;
                }

                // Validate the enum value before casting
                if (typeInt < 0 || typeInt > 3) {  // Assuming 0, 1, 2, 3 are valid enum values for FOOD, HERB, TOOL, WATER
                    std::cerr << "Warning: Invalid ItemType value " << typeInt << " in save file. Skipping item." << std::endl;
                    continue;  // Skip this invalid item
                }

                ItemType type = static_cast<ItemType>(typeInt);
                inventory.addItem(name, type, effect, quantity);
            }

            // Clear current pack
            PackMember* currentMember = pack.getHead();
            while (currentMember) {
                PackMember* next = currentMember->next;
                delete currentMember;
                currentMember = next;
            }
            pack = Pack(); // Reset pack

            // Load pack
            int memberCount;
            if (!(loadFile >> memberCount)) {
                std::cout << "Error: Invalid pack member count in save file." << std::endl;
                loadFile.close();
                return;
            }

            if (memberCount < 0 || memberCount > 20) { // Reasonable upper limit
                std::cout << "Error: Invalid pack member count " << memberCount << " in save file." << std::endl;
                loadFile.close();
                return;
            }

            for (int i = 0; i < memberCount; i++) {
                std::string name, role;
                int loyalty;

                if (!(loadFile >> name)) {
                    std::cout << "Error: Invalid pack member name in save file." << std::endl;
                    break;
                }
                if (name.length() > 100) { // Validate string length
                    std::cout << "Error: Pack member name too long in save file." << std::endl;
                    break;
                }
                if (!(loadFile >> role)) {
                    std::cout << "Error: Invalid pack member role in save file." << std::endl;
                    break;
                }
                if (role.length() > 50) { // Validate string length
                    std::cout << "Error: Pack member role too long in save file." << std::endl;
                    break;
                }
                if (!(loadFile >> loyalty)) {
                    std::cout << "Error: Invalid pack member loyalty in save file." << std::endl;
                    break;
                }

                // Validate loyalty
                if (loyalty < 0 || loyalty > 100) {
                    std::cout << "Warning: Invalid loyalty value " << loyalty << ". Using 50 as default." << std::endl;
                    loyalty = 50;
                }

                pack.addMember(name, role, loyalty);
            }

            // Load storyline and difficulty (new format includes these at end of file)
            int loadedStoryline = -1;
            int loadedDifficulty = -1;

            // Try to read storyline - if this fails, it's an old save file
            if (loadFile >> loadedStoryline) {
                // Try to read difficulty
                if (loadFile >> loadedDifficulty) {
                    // Valid new format - set the global variables
                    if (loadedStoryline >= 0 && loadedStoryline <= 2) {
                        storyline = static_cast<Storyline>(loadedStoryline);
                    } else {
                        std::cout << "Warning: Invalid storyline value in save file. Using default." << std::endl;
                        storyline = CLASSIC;
                    }

                    if (loadedDifficulty >= 0 && loadedDifficulty <= 2) {
                        difficulty = static_cast<Difficulty>(loadedDifficulty);
                    } else {
                        std::cout << "Warning: Invalid difficulty value in save file. Using default." << std::endl;
                        difficulty = NORMAL;
                    }
                } else {
                    // Old save file format (no difficulty) - default to Classic/Easy
                    std::cout << "Warning: Old save file format detected. Defaulting to Classic/NORMAL mode." << std::endl;
                    storyline = CLASSIC;
                    difficulty = NORMAL;
                    // Reposition file stream for potential further reads
                    loadFile.clear();
                }
            } else {
                // Very old format - reset to beginning
                loadFile.clear();
                loadFile.seekg(0, std::ios::beg);
                std::cout << "Warning: Very old save file format. Defaulting to Classic/NORMAL mode." << std::endl;
                storyline = CLASSIC;
                difficulty = NORMAL;
            }

            loadFile.close();
            std::cout << "Game loaded successfully!" << std::endl;
        } catch (const std::exception& e) {
            std::cout << "Error during load: " << e.what() << std::endl;
            if (loadFile.is_open()) {
                loadFile.close();
            }
        }
    } else {
        std::cout << "Error: Could not open save file for reading." << std::endl;
    }
}

int runGuiGame(QApplication& app, Wolf& wolf, DecisionTree& tree, PriorityQueue& events, GameStack& history, ActionQueue& actions, Inventory& inventory, Pack& pack, int& dayCounter, Difficulty& difficulty, Storyline& storyline) {
    qDebug() << "runGuiGame: starting";
    tree.buildSampleTree();
    qDebug() << "runGuiGame: tree built";
    GameWindow gameWindow(wolf, tree, events, history, actions, inventory, pack, dayCounter, difficulty, storyline, nullptr);

    qDebug() << "runGuiGame: window created";

    gameWindow.show();

    qDebug() << "runGuiGame: window shown";

    // Flag to track if user wants to return to menu
    bool wantsToReturnToMenu = false;

    // Connect the gameClosed signal to set the flag and close the window
    QObject::connect(&gameWindow, &GameWindow::gameClosed, [&wantsToReturnToMenu]() {
        wantsToReturnToMenu = true;
    });

    // Connect the returnToMenu signal to set the flag
    QObject::connect(&gameWindow, &GameWindow::returnToMenu, [&wantsToReturnToMenu]() {
        wantsToReturnToMenu = true;
    });

    // Run the event loop
    int result = app.exec();

    // Clean up the game window
    gameWindow.cleanupFallbackNodes();

    // Return special value if user wants to return to menu
    if (wantsToReturnToMenu) {
        return -1;  // Special return code indicating return to menu
    }

    return result;
}