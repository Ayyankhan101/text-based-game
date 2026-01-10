# Wolf Pack Survival Adventure Game
**DSA Lab Final Project - Team A Implementation**

## 📋 Project Overview
Interactive text-based wolf survival game where players control a lone wolf navigating through dangerous wilderness over 30+ days. Features story-driven decisions, resource management, pack dynamics, achievements, and multiple endings using advanced data structures.

**Duration**: 30-day survival progression
**Technology**: C++17 with Qt6 GUI
**Team Size**: 8-10 members
**Submission Deadline**: January 10th, 2026

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

### Story Structure (30-Day Progression)
- **Days 1-5**: Basic Survival (food, shelter, first pack member)
- **Days 6-15**: Pack Building (recruit Luna the Scout, Fenris the Scout, and others)
- **Days 16-25**: Territory & Leadership (establish territory, manage resources)
- **Days 26-30**: Final Challenges (rival packs, legendary alpha encounter)

### Wolf Statistics
- **Health**: 0-100 (death at 0, affected by combat/injuries)
- **Hunger**: 0-100 (death at 100, increases +5 per decision)
- **Energy**: 0-100 (affects success rates and available choices)
- **Reputation**: 0-100 (affects pack recruitment and negotiations)
- **Spirit**: 0-100 (morale system affecting pack loyalty)

### Pack System
- **Pack Members**: Luna (Hunter), Fenris (Scout), Beta (Second-in-Command), Scout, Guard, and others
- **Roles**: Hunter (hunting bonuses), Scout (early warnings), Guard (protection)
- **Loyalty System**: 0-100 per member, affects pack effectiveness
- **Pack Benefits**: Combat bonuses, hunting success, resource sharing

### Inventory System
- **Food Items**: Small Fish, Winter Berries, Fresh Water, Bird Egg, Insects, Frog, Fresh Meat (reduce hunger)
- **Medicine**: Common Mallow, Root Paste, Moss Dressing, Bee Propolis (restore health)
- **Capacity**: Dynamic storage with strategic usage decisions

### Multiple Endings
1. **Survival Victory**: Survive 30 days in the wilderness
2. **Pack Integration**: Successfully lead your pack to prosperity
3. **Legendary Alpha**: Become the dominant pack leader
4. **Tragic Hero**: Sacrifice for pack survival
5. **Diplomatic Alpha**: Unite multiple packs through negotiation

### Achievements System
- **Survival Achievements**: Survive for 10, 20, and 30 days
- **Pack Achievements**: Recruit first pack member, form pack of 5
- **Stat Achievements**: Maintain health above 80, energy mastery
- **Item Achievements**: Collect first item, fill inventory

### Game Modes
- **Terminal Mode**: Text-based interface with ASCII art
- **GUI Mode**: Qt6-based graphical interface with visual stat bars
- **Three Storylines**: Classic (balanced adventure), Survival (extreme difficulty), Pack (leadership focus)

### Difficulty Levels
- **Easy**: Lower hunger increase, fewer events
- **Normal**: Balanced gameplay
- **Hard**: Higher hunger increase, more events

## 🏗️ Technical Architecture

### Build System
```bash
# Build the project
mkdir build
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
text-based-game/
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
inventory.addItem("Small Fish", FOOD, -30, 1);  // Add 1 small fish that reduces hunger by 30
inventory.useItem("Common Mallow");              // Use herbs to heal
inventory.displayInventory();                    // Show all items
```

### Pack Management
```cpp
// Pack member recruitment and management
pack.addMember("Luna", "Hunter", 80);      // Add Luna with 80 loyalty
pack.addMember("Fenris", "Scout", 70);     // Add Fenris with 70 loyalty
int benefit = calculatePackBenefits(pack); // Calculate hunting bonuses
```

## 🧪 Testing & Validation

### Automated Testing
- **Decision Tree Validator**: Verifies complete trees with no missing paths
- **Balance Tester**: Runs simulations for difficulty tuning
- **Integration Tests**: All systems working together

### Manual Testing Checklist
- [x] All 3 storylines functional
- [x] Inventory system functional in both modes
- [x] Pack recruitment and loyalty system working
- [x] Save/load preserves complete game state
- [x] Undo system works for game history
- [x] GUI responsive and user-friendly
- [x] Achievements system tracking progress

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
cd text-based-game
mkdir build && cd build
cmake ..
make

# Run the game
./WolfGame
```

### Playing the Game
1. **Start**: Begin as a lone wolf in winter forest
2. **Survive**: Make decisions to maintain health, manage hunger
3. **Build Pack**: Recruit wolves like Luna and Fenris
4. **Manage Resources**: Use inventory items strategically
5. **Lead**: Guide your pack through challenges
6. **Achieve Ending**: Survive 30 days to reach victory

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
- **Decision Nodes**: 84+ total scenarios across all storylines
- **Story Endings**: 93+ total unique conclusions across all storylines
- **Data Structures**: 6 different types implemented
- **Development Time**: 7 days (one week)
- **Team Size**: 8-10 members

## 🎖️ Bonus Features Implemented
- [x] **Extended Story**: 36+ nodes vs required 20+
- [x] **Rich GUI**: Qt6 interface with animated stat bars
- [x] **Complete Integration**: All systems working together
- [x] **Comprehensive Testing**: Automated validation tools
- [x] **Professional Documentation**: Complete technical docs
- [x] **Achievements System**: Track player progress and accomplishments
- [x] **Auto-Save**: Automatic game saves every 5 decisions
- [x] **Multiple Storylines**: Classic, Survival, and Pack storylines

---

**Team A - DSA Lab Final Project**
**Submission Date**: January 10th, 2026
**Grade Weight**: 30 points total (Report: 15%, Implementation: 85%)
