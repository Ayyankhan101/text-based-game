#ifndef ASCII_ART_H
#define ASCII_ART_H

#include <string>

class AsciiArt {
public:
    static std::string getWolfArt();
    static std::string getForestArt();
    static std::string getBearArt();
    static std::string getPackArt();
    static std::string getVictoryArt();
    static std::string getDefeatArt();
    static std::string getTreeArt();
    static std::string getDeerArt();
    static std::string getBerryArt();
    static std::string getScenarioArt(int scenarioId);
};

inline std::string AsciiArt::getWolfArt() {
    return R"(
  / \__
 (    @\___
 /         O
/   (_____/ 
\_____/   U
)";
}

inline std::string AsciiArt::getForestArt() {
    return R"(
    /\
   /  \
  /    \
 /      \
/________\
    ||    
    ||
)";
}

inline std::string AsciiArt::getBearArt() {
    return R"(
     (\__/)
     (•ㅅ•)
     /   ล
)";
}

inline std::string AsciiArt::getPackArt() {
    return R"(
  / \__     / \__
 (    @\___(    @\___
 /         O         O
/   (_____/   (_____/ 
\_____/       \_____/
)";
}

inline std::string AsciiArt::getVictoryArt() {
    return R"(
  ___   ___   _  _____  ___   _   _  _____ 
 / _ \ | _ \ | |/ / _ \| _ \ /_\ | \| |_   _|
| (_) ||   / | ' <|  _/|   // _ \| .` | | |  
 \__\_\|_|_\ |_|\_\_|  |_|_/_/ \_\_|\_| |_|  
)";
}

inline std::string AsciiArt::getDefeatArt() {
    return R"(
  ___ ___ ___  ___ _  _ _____ ___ 
 | _ \ __| _ \/ __| \| |_   _| _ \
 |   / _||   / (__| .` | | | |   /
 |_|_\___|_|_\\___|_|\_| |_| |_|_\
)";
}

inline std::string AsciiArt::getTreeArt() {
    return R"(
    /\
   /  \
  /    \
 /      \
/________\
    ||    
    ||
)";
}

inline std::string AsciiArt::getDeerArt() {
    return R"(
  /)_/)
  \ / \\
   \   )
   //|
)";
}

inline std::string AsciiArt::getBerryArt() {
    return R"(
  o o o
 o o o o
  o o o
)";
}

inline std::string AsciiArt::getScenarioArt(int scenarioId) {
    switch(scenarioId) {
        case 1:
        case 2:
        case 3:
            return getForestArt();
        case 4:
        case 6:
            return getBerryArt();
        case 5:
        case 10:
        case 11:
            return getBearArt();
        case 12:
        case 13:
            return getPackArt();
        case 16:
            return getVictoryArt();
        case 17:
            return getWolfArt();
        default:
            return getWolfArt();
    }
}

#endif