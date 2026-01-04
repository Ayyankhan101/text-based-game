#!/bin/bash
# Wolf Pack Survival Game - Testing Framework

echo "=================================="
echo "Wolf Pack Survival Game - Testing"
echo "=================================="

# Test 1: Build the game
echo "Test 1: Building the game..."
cd /home/ayyan/DSA-LAB-PROJECT/build
make clean
make

if [ $? -eq 0 ]; then
    echo "✓ Build successful"
else
    echo "✗ Build failed"
    exit 1
fi

# Test 2: Check if executable exists
echo "Test 2: Checking executable..."
if [ -f "WolfGame" ]; then
    echo "✓ Executable exists"
else
    echo "✗ Executable missing"
    exit 1
fi

# Test 3: Check all required files exist
echo "Test 3: Checking required files..."
required_files=(
    "../src/main.cpp"
    "../src/decision_tree.cpp"
    "../src/priority_queue.cpp"
    "../src/game_stack.cpp"
    "../src/inventory.cpp"
    "../src/pack.cpp"
    "../src/action_queue.cpp"
    "../src/game_window.cpp"
    "../include/wolf.h"
    "../include/decision_tree.h"
    "../include/priority_queue.h"
    "../include/game_stack.h"
    "../include/inventory.h"
    "../include/pack.h"
    "../include/action_queue.h"
    "../include/game_window.h"
    "../scenarios.txt"
)

missing_files=0
for file in "${required_files[@]}"; do
    if [ -f "$file" ]; then
        echo "  ✓ $file exists"
    else
        echo "  ✗ $file missing"
        missing_files=$((missing_files + 1))
    fi
done

if [ $missing_files -eq 0 ]; then
    echo "✓ All required files exist"
else
    echo "✗ $missing_files required files missing"
fi

# Test 4: Check decision tree has 20+ nodes
echo "Test 4: Checking decision tree nodes..."
node_count=$(grep -o "scenarioID.*=" ../src/decision_tree.cpp | wc -l)
echo "  Found $node_count decision nodes"
if [ $node_count -ge 20 ]; then
    echo "✓ Decision tree has 20+ nodes ($node_count)"
else
    echo "✗ Decision tree has less than 20 nodes ($node_count)"
fi

# Test 5: Check for 5+ endings
echo "Test 5: Checking for endings..."
ending_count=$(grep -o "isEnding.*= true" ../src/decision_tree.cpp | wc -l)
echo "  Found $ending_count endings"
if [ $ending_count -ge 5 ]; then
    echo "✓ Game has 5+ endings ($ending_count)"
else
    echo "✗ Game has less than 5 endings ($ending_count)"
fi

# Test 6: Check data structures implementation
echo "Test 6: Checking data structures..."
ds_checks=(
    "Binary Decision Tree" 
    "Priority Queue" 
    "Linked List (Inventory)" 
    "Linked List (Pack)" 
    "Stack (Game History)" 
    "Queue (Action Sequences)"
)

for ds in "${ds_checks[@]}"; do
    case $ds in
        "Binary Decision Tree")
            if grep -q "DecisionNode" ../include/decision_tree.h; then
                echo "  ✓ $ds implemented"
            else
                echo "  ✗ $ds missing"
            fi
            ;;
        "Priority Queue")
            if grep -q "PriorityQueue" ../include/priority_queue.h; then
                echo "  ✓ $ds implemented"
            else
                echo "  ✗ $ds missing"
            fi
            ;;
        "Linked List (Inventory)")
            if grep -q "Item.*next" ../include/inventory.h; then
                echo "  ✓ $ds implemented"
            else
                echo "  ✗ $ds missing"
            fi
            ;;
        "Linked List (Pack)")
            if grep -q "PackMember.*next" ../include/pack.h; then
                echo "  ✓ $ds implemented"
            else
                echo "  ✗ $ds missing"
            fi
            ;;
        "Stack (Game History)")
            if grep -q "GameStack" ../include/game_stack.h; then
                echo "  ✓ $ds implemented"
            else
                echo "  ✗ $ds missing"
            fi
            ;;
        "Queue (Action Sequences)")
            if grep -q "ActionQueue" ../include/action_queue.h; then
                echo "  ✓ $ds implemented"
            else
                echo "  ✗ $ds missing"
            fi
            ;;
    esac
done

echo "=================================="
echo "Testing completed!"
echo "=================================="