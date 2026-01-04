#ifndef ACHIEVEMENTS_H
#define ACHIEVEMENTS_H

#include <string>
#include <vector>
#include <set>
#include <iostream>

struct Achievement {
    std::string id;
    std::string name;
    std::string description;
    bool unlocked;
    std::string unlockCondition;
    
    Achievement(std::string _id, std::string _name, std::string _desc, std::string _condition) 
        : id(_id), name(_name), description(_desc), unlocked(false), unlockCondition(_condition) {}
};

class Achievements {
private:
    std::vector<Achievement> achievements;
    std::set<std::string> unlockedAchievements;
    
public:
    Achievements();
    
    void initializeAchievements();
    void checkAchievements();
    void unlockAchievement(const std::string& id);
    void displayAchievements() const;
    bool isAchievementUnlocked(const std::string& id) const;
    
    // Specific achievement checks
    void checkSurvivalAchievements(int daysSurvived);
    void checkPackAchievements(int packSize);
    void checkStatAchievements(int health, int hunger, int energy);
    void checkEndingAchievements(bool reachedEnding);
    void checkItemAchievements(int itemsCollected);
    
    int getUnlockedCount() const;
    int getTotalCount() const;
};

inline Achievements::Achievements() {
    initializeAchievements();
}

inline void Achievements::initializeAchievements() {
    // Survival achievements
    achievements.emplace_back("SURVIVE_10", "Ten Day Survivor", "Survive for 10 days", "Reach day 10");
    achievements.emplace_back("SURVIVE_20", "Twenty Day Survivor", "Survive for 20 days", "Reach day 20");
    achievements.emplace_back("SURVIVE_30", "Wilderness Master", "Survive for 30 days", "Reach day 30");
    
    // Pack achievements
    achievements.emplace_back("FIRST_PACK", "First Companion", "Recruit your first pack member", "Have pack size >= 1");
    achievements.emplace_back("PACK_OF_FIVE", "Pack Leader", "Form a pack of 5 members", "Have pack size >= 5");
    achievements.emplace_back("LOYAL_PACK", "Loyal Followers", "Maintain pack loyalty above 70", "Keep average loyalty > 70");
    
    // Stat achievements
    achievements.emplace_back("HEALTHY_WOLF", "Healthy Wolf", "Keep health above 80", "Maintain health > 80");
    achievements.emplace_back("HUNGRY_HUNTER", "Hungry Hunter", "Collect 10 food items", "Collect 10 food items");
    achievements.emplace_back("ENERGY_MASTER", "Energy Master", "Never let energy drop below 20", "Never have energy < 20");
    
    // Ending achievements
    achievements.emplace_back("SPIRIT_ALLY", "Spirit Ally", "Reach the spirit ally ending", "Reach spirit ally ending");
    achievements.emplace_back("HEALER", "Healer of the Wild", "Reach the healer ending", "Reach healer ending");
    achievements.emplace_back("PACK_INTEGRATION", "Pack Integration", "Join a rival pack", "Reach pack integration ending");
    
    // Miscellaneous achievements
    achievements.emplace_back("FIRST_ITEM", "First Item", "Collect your first item", "Collect any item");
    achievements.emplace_back("MAX_ITEMS", "Collector", "Fill your inventory", "Have 10 items in inventory");
    achievements.emplace_back("NO_DEATH", "Perfect Survival", "Complete game without dying", "Win without dying");
}

inline void Achievements::checkAchievements() {
    // This would be called periodically to check for unlocked achievements
    // Implementation would depend on game state
}

inline void Achievements::unlockAchievement(const std::string& id) {
    for (auto& achievement : achievements) {
        if (achievement.id == id && !achievement.unlocked) {
            achievement.unlocked = true;
            unlockedAchievements.insert(id);
            std::cout << "\033[33mAchievement Unlocked: " << achievement.name << "\033[0m" << std::endl;
            std::cout << achievement.description << std::endl << std::endl;
            return;
        }
    }
}

inline void Achievements::displayAchievements() const {
    std::cout << "\033[36m=== ACHIEVEMENTS ===\033[0m" << std::endl;
    int unlockedCount = 0;
    for (const auto& achievement : achievements) {
        std::string status = achievement.unlocked ? "\033[32m[UNLOCKED]\033[0m" : "\033[31m[LOCKED]\033[0m";
        std::string color = achievement.unlocked ? "\033[32m" : "\033[37m";
        std::cout << status << " " << color << achievement.name << "\033[0m" << std::endl;
        if (achievement.unlocked) {
            unlockedCount++;
        }
    }
    std::cout << "\nUnlocked: " << unlockedCount << "/" << achievements.size() << std::endl;
}

inline bool Achievements::isAchievementUnlocked(const std::string& id) const {
    return unlockedAchievements.find(id) != unlockedAchievements.end();
}

inline void Achievements::checkSurvivalAchievements(int daysSurvived) {
    if (daysSurvived >= 10 && !isAchievementUnlocked("SURVIVE_10")) {
        unlockAchievement("SURVIVE_10");
    }
    if (daysSurvived >= 20 && !isAchievementUnlocked("SURVIVE_20")) {
        unlockAchievement("SURVIVE_20");
    }
    if (daysSurvived >= 30 && !isAchievementUnlocked("SURVIVE_30")) {
        unlockAchievement("SURVIVE_30");
    }
}

inline void Achievements::checkPackAchievements(int packSize) {
    if (packSize >= 1 && !isAchievementUnlocked("FIRST_PACK")) {
        unlockAchievement("FIRST_PACK");
    }
    if (packSize >= 5 && !isAchievementUnlocked("PACK_OF_FIVE")) {
        unlockAchievement("PACK_OF_FIVE");
    }
}

inline void Achievements::checkStatAchievements(int health, int hunger, int energy) {
    if (health > 80 && !isAchievementUnlocked("HEALTHY_WOLF")) {
        unlockAchievement("HEALTHY_WOLF");
    }
    if (energy >= 20 && !isAchievementUnlocked("ENERGY_MASTER")) {
        unlockAchievement("ENERGY_MASTER");
    }
}

inline void Achievements::checkEndingAchievements(bool reachedEnding) {
    // This would be called when an ending is reached
    // Specific implementation would depend on which ending
}

inline void Achievements::checkItemAchievements(int itemsCollected) {
    if (itemsCollected >= 1 && !isAchievementUnlocked("FIRST_ITEM")) {
        unlockAchievement("FIRST_ITEM");
    }
    if (itemsCollected >= 10 && !isAchievementUnlocked("MAX_ITEMS")) {
        unlockAchievement("MAX_ITEMS");
    }
}

inline int Achievements::getUnlockedCount() const {
    return unlockedAchievements.size();
}

inline int Achievements::getTotalCount() const {
    return achievements.size();
}

#endif