#include <iostream>
#include <string>
#include <random>
#include <cctype>
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

// Color codes
#define RESET   "\033[0m"
#define RED     "\033[31m"
#define GREEN   "\033[32m"
#define YELLOW  "\033[33m"
#define BLUE    "\033[34m"
#define CYAN    "\033[36m"
#define BOLD    "\033[1m"

enum Difficulty { EASY, NORMAL, HARD };
enum Storyline { CLASSIC, SURVIVAL, PACK };
enum GameMode { TERMINAL, GUI };

Difficulty difficulty = NORMAL;
Storyline storyline = CLASSIC;
GameMode currentMode = TERMINAL;



// Function declarations
void showMenu();
void showSettings(); // Default to terminal mode

// Global variables
float eventChance = 0.3f;
int hungerIncrease = 5;

// Function declarations
void showMenu();
void showSettings();
void applyDifficulty();
void saveGame(const Wolf& wolf, const DecisionTree& tree, const Inventory& inventory, const Pack& pack, int dayCounter, int slot = 1);
void loadGame(Wolf& wolf, DecisionTree& tree, Inventory& inventory, Pack& pack, int& dayCounter, int slot = 1);
int runGuiGame(QApplication& app, Wolf& wolf, DecisionTree& tree, PriorityQueue& events, GameStack& history, ActionQueue& actions, Inventory& inventory, Pack& pack, int& dayCounter);

int main(int argc, char* argv[]) {
    // Terminal mode
    DecisionTree tree;
    tree.buildSampleTree();

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
    events.insert({"Found Berries", 3, "You find edible berries nearby.", foundBerries});
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

    // Menu loop
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
                switch (storyline) {
                    case SURVIVAL: wolf.health = 80; break;
                    case PACK: wolf.reputation = 20; break;
                    case CLASSIC: break;
                }
                loaded = false;
                break;
            case 2: // Load Game (Terminal)
                currentMode = TERMINAL;
                std::cout << "Enter load slot (1-3): ";
                {
                    int slot;
                    std::cin >> slot;
                    if (slot < 1 || slot > 3) slot = 1; // Default to slot 1
                    loadGame(wolf, tree, inventory, pack, dayCounter, slot);
                }
                applyDifficulty();
                loaded = true;
                break;
            case 3: // Start New Game (GUI)
                currentMode = GUI;
                wolf = Wolf(); // Reset
                applyDifficulty();
                switch (storyline) {
                    case SURVIVAL: wolf.health = 80; break;
                    case PACK: wolf.reputation = 20; break;
                    case CLASSIC: break;
                }
                loaded = false;
                break;
            case 4: // Load Game (GUI)
                currentMode = GUI;
                std::cout << "Enter load slot (1-3): ";
                {
                    int slot;
                    std::cin >> slot;
                    if (slot < 1 || slot > 3) slot = 1; // Default to slot 1
                    loadGame(wolf, tree, inventory, pack, dayCounter, slot);
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

            DecisionNode* current = tree.getCurrentNode();
            // Display ASCII art for the current scenario
            if (!current) {
                std::cout << RED << "ERROR: No current scenario available!" << RESET << std::endl;
                break;
            }
            std::cout << AsciiArt::getScenarioArt(current->scenarioID) << std::endl;
            std::cout << BOLD << current->description << RESET << std::endl;
            if (current->isEnding) {
                std::cout << GREEN << current->endingText << RESET << std::endl;
                break;
            }
            std::cout << YELLOW << "A: " << current->choiceA_text << RESET << std::endl;
            std::cout << YELLOW << "B: " << current->choiceB_text << RESET << std::endl;

            // Push current state before choice (including day and a copy of inventory)
            history.push({current, wolf.health, wolf.hunger, wolf.energy, dayCounter, inventory.clone()});

            std::cout << CYAN << "Choose A, B, U (undo), S (save), L (load), I (inventory), P (pack), E (achievements), Q (quit): " << RESET;
            char input;
            std::cin >> input;
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
                if (!current->left) {
                    std::cout << RED << "ERROR: Invalid choice - no next scenario!" << RESET << std::endl;
                    continue;
                }
                tree.setCurrentNode(current->left);
                if (current->left->scenarioID == 2) {
                    actions.enqueue({"Tracking the deer...", [](Wolf& w){ w.updateEnergy(-10); std::cout << "Spent energy tracking." << std::endl; }});
                    actions.enqueue({"Attacking the deer...", [](Wolf& w){ w.updateHunger(-30); std::cout << "Killed deer! Hunger reduced." << std::endl; }});
                }
                // Add items
                if (current->left->scenarioID == 4) inventory.addItem("Rabbit Meat", FOOD, -30, 1);
                // Removed: Scenario 6 is about helping injured wolf, not finding berries
                // Add reputation changes based on scenario
                if (current->left->scenarioID == 12) wolf.updateReputation(20); // Leading with strength
                if (current->left->scenarioID == 9) wolf.updateReputation(-10); // Sparing traveler
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
                tree.setCurrentNode(current->right);
                // Add reputation changes based on scenario
                if (current->right && current->right->scenarioID == 12) wolf.updateReputation(10); // Leading with wisdom
                if (current->right && current->right->scenarioID == 9) wolf.updateReputation(15); // Embracing shame
            } else if (input == 'S') {
                std::cout << "Enter save slot (1-3): ";
                int slot;
                std::cin >> slot;
                if (slot < 1 || slot > 3) slot = 1; // Default to slot 1
                saveGame(wolf, tree, inventory, pack, dayCounter, slot);
                // Clean up the saved state when popping for save
                GameState savedState = history.pop(); // Pop the state and clean up the inventory copy
                if (savedState.inventory) {
                    delete savedState.inventory;
                }
                continue;
            } else if (input == 'L') {
                std::cout << "Enter load slot (1-3): ";
                int slot;
                std::cin >> slot;
                if (slot < 1 || slot > 3) slot = 1; // Default to slot 1
                loadGame(wolf, tree, inventory, pack, dayCounter, slot);
                continue;
            } else if (input == 'I') {
                inventory.displayInventory();
                // Add option to use an item
                std::cout << "Would you like to use an item? (y/n): ";
                char useChoice;
                std::cin >> useChoice;
                if (std::tolower(useChoice) == 'y') {
                    std::cout << "Enter item name to use: ";
                    std::string itemName;
                    std::cin >> itemName;
                    if (inventory.useItem(itemName, wolf)) {
                        std::cout << "Used " << itemName << ". Applied effect to your stats." << std::endl;
                    } else {
                        std::cout << "Item not found or not in inventory." << std::endl;
                    }
                }
                continue;
            } else if (input == 'P') {
                pack.displayPack();
                continue;
            } else if (input == 'E') {  // E for achievements
                achievements.displayAchievements();
                continue;
            } else if (input == 'Q') {
                break;
            } else {
                std::cout << "Invalid choice." << std::endl;
                // Pop the pushed state since invalid
                history.pop();
                continue;
            }

            // Recruitment
            if (current->scenarioID == 13) pack.addMember("Luna", "Hunter", 80);
            if (current->scenarioID == 16) pack.addMember("Ally", "Guard", 60);

            // Pack player choice effects
            if (current->scenarioID == 68 && input == 'A') {
                // Accepted recruitment
                pack.addMember("Recruit", "Scout", 65);
                wolf.updateHunger(10); // More mouths to feed
                std::cout << GREEN << "🐺 New pack member recruited! Scout added." << RESET << std::endl;
            }
            if (current->scenarioID == 69) {
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
                currentHungerIncrease = static_cast<int>(currentHungerIncrease * (1.0 + (pack.getSize() * 0.1)));
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

                if (inventory.useItem("Rabbit Meat", wolf)) {
                    std::cout << GREEN << "  ✓ Ate Rabbit Meat (hunger reduced)" << RESET << std::endl;
                } else if (inventory.useItem("Berries", wolf)) {
                    std::cout << GREEN << "  ✓ Ate Berries (hunger reduced)" << RESET << std::endl;
                } else {
                    std::cout << RED << "  ✗ No food! Your pack goes hungry." << RESET << std::endl;
                    wolf.updateHunger(10);  // Extra hunger penalty
                }
            }

            // Healing item usage when injured
            if (wolf.health < 40 && dayCounter > 1) {
                std::cout << YELLOW << "\n💊 Your wolf is injured!" << RESET << std::endl;

                if (inventory.useItem("Healing Herbs", wolf)) {
                    std::cout << GREEN << "  ✓ Used Healing Herbs (+30 health)" << RESET << std::endl;
                    wolf.updateHealth(30);
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
                    // Add item for found berries
                    if (e.name == "Found Berries") inventory.addItem("Berries", FOOD, -10, 1);
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
        // Initialize Qt application
        QApplication app(argc, argv);

        // Run the GUI game
        int result = runGuiGame(app, wolf, tree, events, history, actions, inventory, pack, dayCounter);

        return result;
    }

    if (!wolf.isAlive()) {
        std::cout << "You died!" << std::endl;
    }

    return 0;
}

void showMenu() {
    std::cout << BOLD << CYAN << R"(
╔═══════════════════════════════════════════╗
║         🐺 WOLF SURVIVAL GAME 🐺          ║
╚═══════════════════════════════════════════╝
)" << RESET << std::endl;

    std::cout << "  " << BOLD << GREEN << "▶ Start New Game (Terminal)" << RESET << std::endl;
    std::cout << "    Load Game (Terminal)" << std::endl;
    std::cout << "  " << BOLD << GREEN << "▶ Start New Game (GUI)" << RESET << std::endl;
    std::cout << "    Load Game (GUI)" << std::endl;
    std::cout << "  " << YELLOW << "⚙️ Settings" << RESET << std::endl;
    std::cout << "  " << RED << "❌ Quit" << RESET << std::endl;
    std::cout << std::endl << CYAN << "Choose an option: " << RESET;
}



void showSettings() {
    std::cout << BOLD << YELLOW << R"(
╔═══════════════════════════════════════════╗
║              ⚙️ SETTINGS ⚙️               ║
╚═══════════════════════════════════════════╝
)" << RESET << std::endl;

    std::cout << "  " << GREEN << "⚔️ Difficulty: " << RESET;
    switch (difficulty) {
        case EASY: std::cout << GREEN << "[EASY]" << RESET; break;
        case NORMAL: std::cout << YELLOW << "[NORMAL]" << RESET; break;
        case HARD: std::cout << RED << "[HARD]" << RESET; break;
    }
    std::cout << std::endl;

    std::cout << "  " << BLUE << "📖 Storyline: " << RESET;
    switch (storyline) {
        case CLASSIC: std::cout << CYAN << "[CLASSIC]" << RESET; break;
        case SURVIVAL: std::cout << GREEN << "[SURVIVAL]" << RESET; break;
        case PACK: std::cout << YELLOW << "[PACK]" << RESET; break;
    }
    std::cout << std::endl;

    std::cout << std::endl << "  " << RED << "⬅️ Back" << RESET << std::endl;
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

        saveFile.close();
        std::cout << "Game saved successfully!" << std::endl;
    } else {
        std::cout << "Error: Could not open save file." << std::endl;
    }
}

void loadGame(Wolf& wolf, DecisionTree& tree, Inventory& inventory, Pack& pack, int& dayCounter, int slot) {
    std::string filename = "savegame_" + std::to_string(slot) + ".txt";
    std::ifstream loadFile(filename);
    if (loadFile.is_open()) {
        // Load wolf stats
        loadFile >> wolf.health;
        loadFile >> wolf.hunger;
        loadFile >> wolf.energy;
        loadFile >> wolf.reputation;
        loadFile >> wolf.spirit;
        loadFile >> dayCounter;

        // Load current node ID
        int nodeId;
        loadFile >> nodeId;

        // Find the node in the tree by ID
        tree.buildSampleTree(); // Rebuild tree to ensure it's populated
        DecisionNode* foundNode = tree.findNodeById(nodeId);
        if (foundNode) {
            tree.setCurrentNode(foundNode);
        } else {
            // If node not found, go to root
            tree.setCurrentNode(tree.getRoot());
        }

        // Clear current inventory using safe clear method
        inventory.clear();

        // Load inventory
        int itemCount;
        loadFile >> itemCount;
        for (int i = 0; i < itemCount; i++) {
            std::string name;
            int typeInt;
            int effect, quantity;

            loadFile >> name;
            loadFile >> typeInt;
            loadFile >> effect;
            loadFile >> quantity;

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
        loadFile >> memberCount;
        for (int i = 0; i < memberCount; i++) {
            std::string name, role;
            int loyalty;

            loadFile >> name >> role >> loyalty;

            pack.addMember(name, role, loyalty);
        }

        loadFile.close();
        std::cout << "Game loaded successfully!" << std::endl;
    } else {
        std::cout << "Error: Could not open save file." << std::endl;
    }
}

int runGuiGame(QApplication& app, Wolf& wolf, DecisionTree& tree, PriorityQueue& events, GameStack& history, ActionQueue& actions, Inventory& inventory, Pack& pack, int& dayCounter) {
    qDebug() << "runGuiGame: starting";
    tree.buildSampleTree();
    qDebug() << "runGuiGame: tree built";
    GameWindow gameWindow(wolf, tree, events, history, actions, inventory, pack, dayCounter);

    qDebug() << "runGuiGame: window created";

    gameWindow.show();

    qDebug() << "runGuiGame: window shown";

    return app.exec();
}