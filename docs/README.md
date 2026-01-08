# Wolf Pack Survival Adventure Game
**DSA Lab Final Project - Team A Implementation**

## 📋 Project Overview
Interactive text-based wolf survival game where players control a lone wolf navigating through dangerous wilderness over 10+ days. Features story-driven decisions, resource management, pack dynamics, and multiple endings using advanced data structures.

**Duration**: 10-day survival progression  
**Technology**: C++17 with Qt6 GUI  
**Team Size**: 8-10 members  
**Submission Deadline**: January 9th, 2026

## 🎯 Requirements Compliance

### ✅ Core Features (MUST HAVE)
- [x] **36+ Decision Nodes**: Extended story with complex branching (36+ nodes implemented)
- [x] **22+ Different Endings**: Multiple story conclusions based on choices
- [x] **Binary Decision Tree**: Story progression and narrative branching
- [x] **Priority Queue**: Event management with 3 priority levels
- [x] **Stack**: Undo functionality (5 levels) for decision backtracking
- [x] **Linked Lists**: Dynamic inventory and pack member storage
- [x] **Queue**: Multi-turn action sequences
- [x] **GUI Interface**: Qt6-based user interface
- [x] **Save/Load System**: Persistent game state management

### ✅ Data Structures Implementation
| Feature | Data Structure | Implementation | Status |
|---------|---------------|----------------|---------|
| Story Decisions | Binary Tree | decision_tree.cpp/h | ✅ Complete |
| Threat Management | Min Heap Priority Queue | priority_queue.cpp/h | ✅ Complete |
| Inventory | Singly Linked List | inventory.cpp/h | ✅ Complete |
| Pack Members | Singly Linked List | pack.cpp/h | ✅ Complete |
| Game History (Undo) | Stack | game_stack.cpp/h | ✅ Complete |
| Action Sequences | Queue | action_queue.cpp/h | ✅ Complete |

## 🎮 Game Features

### Story Structure (10-Day Progression)
- **Days 1-2**: Basic Survival (food, shelter, first pack member)
- **Days 3-5**: Pack Building (recruit Luna the Scout, Fenrir the Hunter)
- **Days 6-8**: Territory & Leadership (establish territory, manage resources)
- **Days 9-10**: Final Challenges (rival packs, legendary alpha encounter)

### Wolf Statistics
- **Health**: 0-100 (death at 0, affected by combat/injuries)
- **Hunger**: 0-100 (death at 100, increases +5 per decision)
- **Energy**: 0-100 (affects success rates and available choices)
- **Reputation**: 0-100 (affects pack recruitment and negotiations)
- **Spirit**: 0-100 (morale system affecting pack loyalty)

### Pack System
- **Pack Members**: Luna (Scout), Fenrir (Hunter), and others
- **Roles**: Hunter (hunting bonuses), Scout (early warnings), Guard (protection)
- **Loyalty System**: 0-100 per member, affects pack effectiveness
- **Pack Benefits**: Combat bonuses, hunting success, resource sharing

### Inventory System
- **Food Items**: Rabbit Meat, Deer Meat, Berries (reduce hunger)
- **Medicine**: Healing Herbs (restore health)
- **Tools**: Territory Maps, Wolf Scent Markers (aid recruitment/navigation)
- **Capacity**: Maximum 10 items with strategic usage decisions

### Multiple Endings
1. **Legendary Alpha**: Defeat final boss, become dominant pack leader
2. **Peaceful Leader**: Lead through wisdom rather than strength
3. **Tragic Hero**: Sacrifice for pack survival
4. **Diplomatic Alpha**: Unite multiple packs through negotiation
5. **Wise Leader**: Prosper through intelligence and resource management

## 🏗️ Technical Architecture

### Build System
```bash
# Build the project
cd build
cmake ..
make

# Run executables
./WolfGame        # Main game
./Validator       # Decision tree validation
./BalanceTester   # Game balance testing
```

### File Structure
```
DSA-LAB-PROJECT/
├── src/                    # Source files (.cpp)
│   ├── main.cpp           # Main game loop
│   ├── decision_tree.cpp  # Binary tree implementation
│   ├── priority_queue.cpp # Event management
│   ├── game_stack.cpp     # Undo system
│   ├── inventory.cpp      # Item management
│   ├── pack.cpp           # Pack member management
│   ├── action_queue.cpp   # Action sequences
│   ├── game_window.cpp    # Qt GUI implementation
│   ├── validator.cpp      # Tree validation
│   └── balance_tester.cpp # Balance testing
├── include/               # Header files (.h)
├── build/                 # Build output directory
├── CMakeLists.txt        # Build configuration
└── docs/                 # Documentation files
```

### Dependencies
- **Qt6**: Core, Widgets, Gui modules
- **CMake**: 3.16 or higher
- **C++17**: Compatible compiler
- **Linux/Windows/macOS**: Cross-platform support

## 🎯 Data Structure Usage Examples

### Binary Decision Tree
```cpp
// Story progression through tree nodes
DecisionNode* currentNode = tree.getCurrentNode();
if (playerChoice == 'A') {
    currentNode = currentNode->left;  // Choice A path
} else {
    currentNode = currentNode->right; // Choice B path
}
```

### Priority Queue Events
```cpp
// Critical events processed first
Event bearAttack = {"Bear Attack", 1, "A bear threatens your pack!"};
Event weatherChange = {"Storm", 3, "Weather is changing..."};
eventQueue.insert(bearAttack);  // Priority 1 - processed first
eventQueue.insert(weatherChange); // Priority 3 - processed later
```

### Inventory Management
```cpp
// Dynamic item storage
inventory.addItem("Rabbit Meat", FOOD, 20, 2);  // Add 2 rabbit meat
inventory.useItem("Healing Herbs");              // Use herbs to heal
inventory.displayInventory();                    // Show all items
```

### Pack Management
```cpp
// Pack member recruitment and management
pack.addMember("Luna", "Scout", 85);      // Add Luna with 85 loyalty
pack.addMember("Fenrir", "Hunter", 90);   // Add Fenrir with 90 loyalty
int benefit = calculatePackBenefits(pack); // Calculate hunting bonuses
```

## 🧪 Testing & Validation

### Automated Testing
- **Decision Tree Validator**: Verifies 84 total nodes and 93 total endings across all storylines (40+ nodes, 61+ endings Classic; 22+ nodes, 16+ endings each for Survival and Pack)
- **Balance Tester**: Runs 100+ simulations for difficulty tuning
- **Integration Tests**: All systems working together

### Manual Testing Checklist
- [ ] All 5+ endings reachable
- [ ] Inventory system functional in story
- [ ] Pack recruitment and loyalty system working
- [ ] Save/load preserves complete game state
- [ ] Undo system works for 5 levels
- [ ] GUI responsive and user-friendly

## 📚 Documentation

### Required Deliverables
1. **Source Code**: All .cpp/.h files with build instructions
2. **LaTeX Report**: 10-15 pages covering design and implementation
3. **Demo Video**: 8-10 minutes showing gameplay features
4. **Game Design Document**: Complete story map and mechanics

### Report Structure (LaTeX)
- Title page and game overview
- Data structure implementation details
- Story flowchart and decision tree diagram
- Testing results and game balance analysis
- AI tools usage and development process
- Conclusion and learning outcomes

## 🚀 Getting Started

### Prerequisites
```bash
# Ubuntu/Debian
sudo apt-get install qt6-base-dev cmake build-essential

# Fedora/RHEL
sudo dnf install qt6-qtbase-devel cmake gcc-c++

# macOS
brew install qt6 cmake
```

### Building and Running
```bash
# Clone and build
git clone <repository-url>
cd DSA-LAB-PROJECT
mkdir build && cd build
cmake ..
make

# Run the game
./WolfGame
```

### Playing the Game
1. **Start**: Begin as a lone wolf in winter forest
2. **Survive**: Make decisions to maintain health, manage hunger
3. **Build Pack**: Recruit wolves like Luna and Fenrir
4. **Manage Resources**: Use inventory items strategically
5. **Lead**: Guide your pack through challenges
6. **Achieve Ending**: Reach one of 5+ different conclusions

## 🏆 Educational Objectives

### Data Structure Learning
- **Practical Application**: See CS concepts in real gaming context
- **Performance Understanding**: Experience efficiency of different structures
- **Integration Complexity**: Multiple structures working seamlessly together
- **Problem Solving**: Apply data structures to solve game design challenges

### Game Design Principles
- **Narrative Branching**: How choices create meaningful consequences
- **Resource Management**: Strategic decision-making under constraints
- **Character Development**: Building emotional investment in pack members
- **Replayability**: Multiple paths encourage exploration of strategies

## 📊 Project Statistics
- **Lines of Code**: 2000+ (C++)
- **Decision Nodes**: 84+ total scenarios (40+ Classic, 22+ Survival, 22+ Pack)
- **Story Endings**: 93+ total unique conclusions (61+ Classic, 16+ Survival, 16+ Pack)
- **Data Structures**: 6 different types implemented
- **Development Time**: 7 days (one week)
- **Team Size**: 8-10 members

## 🎖️ Bonus Features Implemented
- [x] **Extended Story**: 36+ nodes vs required 20+
- [x] **Rich GUI**: Qt6 interface with animated stat bars
- [x] **Complete Integration**: All systems working together
- [x] **Comprehensive Testing**: Automated validation tools
- [x] **Professional Documentation**: Complete technical docs

---

**Team A - DSA Lab Final Project**  
**Submission Date**: January 9th, 2026  
**Grade Weight**: 30 points total (Report: 15%, Implementation: 85%)
