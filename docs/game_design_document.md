# Wolf Pack Survival Adventure Game - Complete Game Design Document

## 1. Executive Summary

The Wolf Pack Survival Adventure Game is an interactive text-based adventure where players control a lone wolf navigating through a dangerous wilderness. The game features story-driven decisions, resource management, combat encounters, pack dynamics, achievements, and multiple endings. This project demonstrates how data structures power game mechanics, AI decision-making, and narrative branching.

## 2. Game Overview

### 2.1 Game Concept
- **Genre**: Interactive Text-Based Adventure
- **Platform**: Cross-platform C++ application with Qt GUI
- **Target Audience**: Strategy and adventure game enthusiasts
- **Gameplay**: Decision-based survival mechanics with branching narratives

### 2.2 Core Gameplay Loop
1. Player encounters a scenario with choices
2. Player makes a decision based on current stats and inventory
3. Decision affects stats, inventory, and story path
4. Random events may occur
5. Game state is saved and next scenario is presented
6. Pack members and inventory are updated based on choices
7. Achievements are checked and unlocked based on progress

## 3. Game Mechanics

### 3.1 Core Stats System
- **Health (0-100)**: Physical well-being; death at 0
- **Hunger (0-100)**: Sustenance level; death at 100
- **Energy (0-100)**: Available for actions; affects choice availability
- **Reputation (0-100)**: Affects pack interactions and recruitment
- **Spirit (0-100)**: Morale and mental well-being

### 3.2 Decision System
- Binary choice system using Binary Decision Tree
- Each decision affects stats and story path
- Choices have immediate and long-term consequences
- Path-dependent outcomes and multiple endings
- Decision tree has 84+ nodes across all storylines

### 3.3 Inventory System
- Dynamic item storage using Singly Linked List
- Items have types (FOOD, HERB, WATER), effects, and quantities
- Food Items: Small Fish, Winter Berries, Fresh Water, Bird Egg, Insects, Frog, Fresh Meat (reduce hunger)
- Medicine: Common Mallow, Root Paste, Moss Dressing, Bee Propolis (restore health)
- Items consumed automatically every 5 days
- Healing items used when health drops below threshold

### 3.4 Pack Management
- Pack members with roles (Hunter, Scout, Guard) and loyalty
- Pack benefits based on member roles and loyalty
- Loyalty system with decay and member departure
- Pack size affects resource consumption (hunger increases faster with more pack members)
- Pack members: Luna (Hunter), Fenris (Scout), Beta (Second-in-Command), Scout, Guard, and others
- Pack recruitment through story choices and random events

### 3.5 Event System
- Priority-based event queue using Min Heap
- Events have priority levels (1-3, lower is more urgent)
- Critical events (bear attacks) trigger first
- Events affect player stats and game state
- Event chance varies by difficulty level

### 3.6 Achievement System
- **Survival Achievements**: Survive for 10, 20, and 30 days
- **Pack Achievements**: Recruit first pack member, form pack of 5
- **Stat Achievements**: Maintain health above 80, energy mastery
- **Item Achievements**: Collect first item, fill inventory
- Achievements tracked and displayed to players

### 3.7 Day Counter System
- Game progresses over 30 days
- Stats decrease gradually each day
- Food consumption occurs every 5 days
- Random events occur based on event chance
- Win condition: survive 30 days

## 4. Story & Narrative

### 4.1 Setting
The game is set in a mystical wilderness where ancient spirits and the legendary First Pack influence the world. Players control a lone wolf separated from their pack during a harsh winter storm.

### 4.2 Main Objectives
- Survive 30 days in the wilderness
- Maintain critical stats within safe ranges
- Make meaningful decisions that affect story outcomes
- Collect resources and build a pack
- Face various threats and challenges
- Unlock achievements as you progress

### 4.3 Narrative Structure
- Branching narrative with 84+ decision nodes across all storylines
- Multiple story paths based on player choices
- 93+ different endings across all storylines based on decisions and stats
- Consequences that carry forward through the story
- Three distinct storylines: Classic, Survival, Pack

### 4.4 Storylines
- **Classic**: Balanced adventure with moderate difficulty
- **Survival**: Extreme difficulty with lower starting stats
- **Pack**: Leadership focus with reputation mechanics

## 5. Technical Architecture

### 5.1 Data Structures Implementation
- **Binary Decision Tree**: Story branching and navigation
- **Priority Queue (Min Heap)**: Event management and scheduling
- **Singly Linked List**: Inventory and pack member storage
- **Stack**: Game state history for undo functionality
- **Queue**: Action sequences for multi-turn activities
- **Hash Set**: Achievement tracking and duplicate prevention

### 5.2 Game Systems Integration
- Decision tree connected to stat management
- Event system integrated with random encounter system
- Inventory system linked to item usage and effects
- Pack system connected to loyalty and benefit calculations
- Save/load system for persistence
- Achievement system tracking player progress
- Day counter system managing game progression

## 6. User Interface

### 6.1 Terminal Mode
- Text-based interface with color-coded stat displays
- Choice selection via keyboard input (A/B/U/S/L/I/V/P/E/Q)
- Inventory and pack management through commands
- Clear scenario descriptions and choice options
- ASCII art for visual representation of scenarios
- Achievement display with unlock notifications

### 6.2 GUI Mode
- Qt-based graphical interface with visual stat bars
- Interactive buttons for choices and actions
- Animated wolf character and environment graphics
- Visual feedback for stat changes and events
- Menu system with save/load and settings
- Achievement panel showing unlocked achievements

## 7. Game Balance

### 7.1 Difficulty Settings
- **Easy**: Lower hunger increase (3 per decision), fewer events (20% chance)
- **Normal**: Balanced challenge (5 per decision, 30% chance) (default)
- **Hard**: Higher hunger increase (8 per decision), more frequent events (50% chance)

### 7.2 Win/Lose Conditions
- **Win**: Survive 30 days in the wilderness
- **Lose**: Health reaches 0 or Hunger reaches 100
- **Multiple Endings**: Based on choices, stats, and pack size
- **Achievements**: Unlock based on various milestones

## 8. Content Overview

### 8.1 Decision Nodes
- 84+ unique decision scenarios across all storylines
- Branching paths with meaningful choices
- Consequences that affect game state
- Varied narrative themes and challenges

### 8.2 Endings
- **Survival Victory**: Survive 30 days in the wilderness
- **Pack Integration**: Successfully lead your pack to prosperity
- **Legendary Alpha**: Become the dominant pack leader
- **Tragic Hero**: Sacrifice for pack survival
- **Diplomatic Alpha**: Unite multiple packs through negotiation
- **Plus 88+ additional endings** across all storylines

### 8.3 Achievements
- **Survival Achievements**: Survive for 10, 20, and 30 days
- **Pack Achievements**: Recruit first pack member, form pack of 5
- **Stat Achievements**: Maintain health above 80, energy mastery
- **Item Achievements**: Collect first item, fill inventory
- **Special Achievements**: Perfect survival without dying

## 9. Technical Requirements

### 9.1 System Requirements
- **OS**: Linux, Windows, or macOS
- **Memory**: 512MB RAM minimum
- **Storage**: 50MB available space
- **Dependencies**: Qt6 libraries

### 9.2 Development Requirements
- **Language**: C++17
- **Build System**: CMake
- **GUI Framework**: Qt6
- **Data Structures**: Custom implementations

## 10. Quality Assurance

### 10.1 Testing Framework
- Automated validation of decision tree structure
- Balance testing through simulation
- Path validation to ensure all endings are reachable
- Integration testing of all game systems
- Save/load functionality testing

### 10.2 Validation Results
- Decision tree has 84+ nodes across all storylines (exceeds 20+ requirement)
- Game has 93+ endings across all storylines (exceeds 5+ requirement)
- All paths and endings are reachable
- Difficulty balanced for 10-80% survival rate
- All three storylines fully implemented and tested
- Achievement system properly tracking and displaying progress