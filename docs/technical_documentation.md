# Wolf Pack Survival Adventure Game - Technical Documentation

## 1. System Architecture

### 1.1 Overview
The Wolf Pack Survival Adventure Game is built using C++17 with Qt6 for the graphical user interface. The system follows a modular architecture with clear separation of concerns between game logic, data structures, and user interface components. The game supports three distinct storylines (Classic, Survival, Pack) and includes an achievements system.

### 1.2 Core Components
- **Game Logic Layer**: Wolf, DecisionTree, PriorityQueue, GameStack, Inventory, Pack, ActionQueue, Achievements
- **User Interface Layer**: GameWindow (Qt-based GUI), terminal interface
- **Data Management Layer**: Save/Load system, validation framework
- **Utility Layer**: Validation tools, balance testers

## 2. Data Structures Implementation

### 2.1 Binary Decision Tree
**File**: `decision_tree.h/cpp`

**Structure**:
```cpp
struct DecisionNode {
    int scenarioID;
    std::string description;
    std::string choiceA_text;
    std::string choiceB_text;
    DecisionNode* left;   // Choice A
    DecisionNode* right;  // Choice B
    bool isEnding;
    std::string endingText;
};
```

**Functionality**:
- `buildSampleTree()`: Constructs the complete decision tree with 84+ nodes across all storylines
- `buildClassicStory()`: Builds the classic storyline tree
- `buildSurvivalStory()`: Builds the survival storyline tree with extreme difficulty
- `buildPackStory()`: Builds the pack storyline tree with leadership focus
- `findNodeById()`: Locates a node by its ID for save/load functionality
- `getCurrentNode()/setCurrentNode()`: Manages current position in the tree
- `reset()`: Deletes tree and resets for new game
- **Story Integration**: Nodes include inventory usage, pack recruitment, and resource management

### 2.2 Priority Queue (Min Heap)
**File**: `priority_queue.h/cpp`

**Structure**:
```cpp
struct Event {
    std::string name;
    int priority; // 1-3, lower is more urgent
    std::string description;
    std::function<void(Wolf&)> effect; // Lambda for stat changes
};
```

**Functionality**:
- `insert(Event e)`: Adds event to the heap, maintaining min-heap property
- `extractMin()`: Returns highest priority event and removes it from heap
- `isEmpty()`: Checks if queue is empty
- `size()`: Returns current number of events in queue

### 2.3 Linked List (Inventory & Pack)
**File**: `inventory.h/cpp` and `pack.h/cpp`

**Inventory Structure**:
```cpp
struct Item {
    std::string name;
    ItemType type; // FOOD, HERB, WATER
    int effect;
    int quantity;
    Item* next;
};
```

**Pack Structure**:
```cpp
struct PackMember {
    std::string name;
    std::string role; // Hunter, Scout, Guard
    int loyalty; // 0-100
    PackMember* next;
};
```

**Functionality**:
- `addItem()`, `useItem()`: Inventory management with story integration
- `useItemByNumber()`: Use items by index number
- `displayWithNumbers()`: Display items with numbers for selection
- `addStartingSupplies()`: Add starting items based on difficulty
- `addRandomFood()`: Add random food items based on difficulty
- `addMember()`, `removeMember()`: Pack management with loyalty system
- `updateLoyalty()`: Loyalty decay and member departure based on hunger/decisions
- `calculatePackBenefits()`: Utility function for pack combat/hunting bonuses

### 2.4 Stack (Game History)
**File**: `game_stack.h/cpp`

**Structure**:
```cpp
struct GameState {
    DecisionNode* currentNode;
    int health, hunger, energy;
    int day;
    Inventory* inventory; // Complete inventory for restoration
};
```

**Functionality**:
- `push(GameState gs)`: Saves current game state with unlimited levels
- `pop()`: Restores previous game state
- `isEmpty()`: Checks if stack is empty
- `clear()`: Clears all game states
- Automatic cleanup of inventory states

### 2.5 Queue (Action Sequences)
**File**: `action_queue.h/cpp`

**Structure**:
```cpp
struct Action {
    std::string description;
    std::function<void(Wolf&)> execute;
};
```

**Functionality**:
- `enqueue(Action a)`: Adds action to queue
- `processNext(Wolf& wolf)`: Executes next action in sequence
- `isEmpty()`: Checks if queue is empty
- `clear()`: Clears all actions

### 2.6 Achievements System
**File**: `achievements.h`

**Structure**:
```cpp
struct Achievement {
    std::string id;
    std::string name;
    std::string description;
    bool unlocked;
    std::string unlockCondition;
};
```

**Functionality**:
- `checkSurvivalAchievements()`: Check for survival-based achievements
- `checkPackAchievements()`: Check for pack-related achievements
- `checkStatAchievements()`: Check for stat-based achievements
- `checkItemAchievements()`: Check for item-related achievements
- `displayAchievements()`: Show all achievements with unlock status
- `unlockAchievement()`: Unlock a specific achievement

## 3. Game Systems Integration

### 3.1 Wolf Class
**File**: `wolf.h`

**Core Stats**:
- `health`, `hunger`, `energy`: Primary survival metrics
- `reputation`, `spirit`: Secondary metrics affecting gameplay
- `canMakeChoice()`: Validation function for decision making

**Stat Management**:
- `updateHunger()`, `updateEnergy()`, `updateHealth()`: Stat modification with bounds checking
- `updateReputation()`, `updateSpirit()`: Secondary stat modification
- `isAlive()`: Death condition validation

### 3.2 Game Loop Integration
The main game loop in `main.cpp` integrates all systems:

1. **Menu System**: Handles game mode selection (terminal/GUI) and settings
2. **Story Selection**: Builds appropriate decision tree based on selected storyline
3. **Decision Processing**: Handles player choices and updates game state
4. **Stat Updates**: Applies hunger increase and other stat changes
5. **Event System**: Processes random events with priority
6. **Pack Management**: Updates loyalty and applies benefits
7. **Inventory Management**: Tracks items and enables usage
8. **Day Counter**: Tracks days survived and triggers daily events
9. **Achievement System**: Checks and unlocks achievements based on progress
10. **Auto-Save**: Automatically saves game every 5 decisions
11. **GUI Integration**: Switches to Qt GUI when selected

### 3.3 GUI Integration
**File**: `game_window.cpp`

**Qt Components**:
- `StatBar`: Animated progress bars for health, hunger, energy, spirit
- `QLabel`: Story display with scrolling
- `QPushButton`: Choice buttons with dynamic enabling/disabling
- `QMenuBar/QStatusBar`: Menu and status bar
- `QSplitter`: Layout management
- `QScrollArea`: Scrollable story display
- `QProgressBar`: Visual stat indicators

## 4. Save/Load System

### 4.1 File Format
The save system stores game state in `savegame_X.txt` (where X is the slot number) with the following format:
```
health
hunger
energy
reputation
spirit
dayCounter
currentNodeID
itemCount
[item data repeated itemCount times]
packMemberCount
[pack member data repeated packMemberCount times]
storyline
difficulty
```

### 4.2 Implementation
- `saveGame()`: Serializes all game state to file
- `loadGame()`: Deserializes game state from file and restores position in decision tree
- Support for multiple save slots (1-3)
- Auto-save functionality to slot 0 every 5 decisions
- Automatic cleanup of previous inventory states during load
- Preservation of storyline and difficulty settings

## 5. Validation Framework

### 5.1 Decision Tree Validator
**File**: `validator.cpp`

**Functionality**:
- Validates tree structure and node connectivity
- Counts total nodes and endings across all storylines
- Finds all possible paths through the decision tree
- Verifies all 93+ endings are reachable across all storylines

### 5.2 Balance Tester
**File**: `balance_tester.cpp`

**Functionality**:
- Runs simulation games to test difficulty balance
- Calculates survival rates across different difficulty settings
- Validates that game is neither too easy nor too hard
- Tests all three storylines for balance

## 6. Build System

### 6.1 CMake Configuration
**File**: `CMakeLists.txt`

**Components**:
- Qt6 Core, Widgets, and Gui libraries
- Automatic source file discovery
- Separate executables for main game, validator, and balance tester
- Cross-platform compatibility

### 6.2 Dependencies
- Qt6 development libraries
- C++17 compatible compiler
- CMake 3.16 or higher

## 7. Code Quality & Standards

### 7.1 Memory Management
- Proper cleanup in destructors
- Smart pointer usage where appropriate
- Prevention of memory leaks in game state management
- Automatic cleanup of cloned inventories
- Proper handling of linked list node deletion

### 7.2 Error Handling
- Input validation for user choices
- File I/O error handling in save/load system
- Boundary checks for all stat modifications
- Graceful handling of invalid states
- String length validation in save/load system

## 8. Testing & Validation

### 8.1 Automated Tests
- Decision tree validation (node count, ending count across all storylines)
- Path validation (all paths reachable)
- Balance testing (survival rate validation)
- Integration testing (all systems working together)
- Save/load system validation

### 8.2 Manual Testing
- Playthrough validation for all storylines
- Difficulty setting verification
- GUI functionality testing
- Achievement system validation
- Pack recruitment and loyalty system testing

## 9. Performance Considerations

### 9.1 Memory Usage
- Efficient linked list implementations
- Proper cleanup of game states
- Optimized event queue operations
- Memory-efficient achievement tracking

### 9.2 Runtime Performance
- O(log n) operations for priority queue
- O(1) operations for stack/queue
- Efficient tree traversal algorithms
- Optimized GUI update mechanisms
- Efficient achievement checking algorithms

## 10. Extensibility

### 10.1 Modular Design
- Clear separation of data structures and game logic
- Easy addition of new event types
- Flexible inventory system for new item types
- Scalable decision tree architecture
- Pluggable achievement system

### 10.2 Future Enhancements
- Additional game modes
- More complex AI for pack members
- Enhanced GUI with animations
- Multiplayer functionality
- Additional storylines
- Advanced statistics tracking