# Wolf Pack Survival Adventure Game - Complete Game Design Document

## 1. Executive Summary

The Wolf Pack Survival Adventure Game is an interactive text-based adventure where players control a lone wolf navigating through a dangerous wilderness. The game features story-driven decisions, resource management, combat encounters, pack dynamics, and multiple endings. This project demonstrates how data structures power game mechanics, AI decision-making, and narrative branching.

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

### 3.3 Inventory System
- Dynamic item storage using Singly Linked List
- Items have types (FOOD, HERB, TOOL), effects, and quantities
- Maximum 10 items capacity
- Items provide stat benefits when used

### 3.4 Pack Management
- Pack members with roles (Hunter, Scout, Guard) and loyalty
- Pack benefits based on member roles and loyalty
- Loyalty system with decay and member departure
- Pack size affects resource consumption

### 3.5 Event System
- Priority-based event queue using Min Heap
- Events have priority levels (1-3, lower is more urgent)
- Critical events (bear attacks) trigger first
- Events affect player stats and game state

## 4. Story & Narrative

### 4.1 Setting
The game is set in a mystical wilderness where ancient spirits and the legendary First Pack influence the world. Players control a lone wolf separated from their pack during a harsh winter storm.

### 4.2 Main Objectives
- Survive 30 days in the wilderness
- Maintain critical stats within safe ranges
- Make meaningful decisions that affect story outcomes
- Collect resources and build a pack
- Face various threats and challenges

### 4.3 Narrative Structure
- Branching narrative with 36+ decision nodes
- Multiple story paths based on player choices
- 22+ different endings based on decisions and stats
- Consequences that carry forward through the story

## 5. Technical Architecture

### 5.1 Data Structures Implementation
- **Binary Decision Tree**: Story branching and navigation
- **Priority Queue (Min Heap)**: Event management and scheduling
- **Singly Linked List**: Inventory and pack member storage
- **Stack**: Game state history for undo functionality
- **Queue**: Action sequences for multi-turn activities

### 5.2 Game Systems Integration
- Decision tree connected to stat management
- Event system integrated with random encounter system
- Inventory system linked to item usage and effects
- Pack system connected to loyalty and benefit calculations
- Save/load system for persistence

## 6. User Interface

### 6.1 Terminal Mode
- Text-based interface with color-coded stat displays
- Choice selection via keyboard input
- Inventory and pack management through commands
- Clear scenario descriptions and choice options

### 6.2 GUI Mode
- Qt-based graphical interface with visual stat bars
- Interactive buttons for choices and actions
- Animated wolf character and environment graphics
- Visual feedback for stat changes and events

## 7. Game Balance

### 7.1 Difficulty Settings
- **Easy**: Lower hunger increase, fewer events
- **Normal**: Balanced challenge (default)
- **Hard**: Higher hunger increase, more frequent events

### 7.2 Win/Lose Conditions
- **Win**: Survive 30 days in the wilderness
- **Lose**: Health reaches 0 or Hunger reaches 100
- **Multiple Endings**: Based on choices, stats, and pack size

## 8. Content Overview

### 8.1 Decision Nodes
- 36+ unique decision scenarios
- Branching paths with meaningful choices
- Consequences that affect game state
- Varied narrative themes and challenges

### 8.2 Endings
- **Spirit Ally**: Join ancient spirits
- **Healer of the Wild**: Become legendary healer
- **Pack Integration**: Join rival pack
- **Cave Wisdom**: Achieve spiritual enlightenment
- **Supply Discovery**: Find resources for survival
- **Pup Rescue**: Gain pack allies through compassion
- **Moon Path**: Discover ancient pack grounds
- **Plus 15+ additional endings** across all storylines

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

### 10.2 Validation Results
- Decision tree has 36+ nodes (exceeds 20+ requirement)
- Game has 22+ endings (exceeds 5+ requirement)
- All paths and endings are reachable
- Difficulty balanced for 10-80% survival rate