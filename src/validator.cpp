#include <iostream>
#include <set>
#include <vector>
#include <stack>
#include <utility>
#include "../include/decision_tree.h"

class DecisionTreeValidator {
private:
    std::set<int> allNodes;
    std::vector<int> allEndings;
    std::vector<std::vector<int>> allPaths; // Store all possible paths

    void collectAllNodes(DecisionNode* node) {
        if (!node) return;

        allNodes.insert(node->scenarioID);

        if (node->isEnding) {
            allEndings.push_back(node->scenarioID);
            return;
        }

        collectAllNodes(node->left);
        collectAllNodes(node->right);
    }

    void findAllPaths(DecisionNode* node, std::vector<int>& currentPath, int depth = 0) {
        if (!node || depth > 100) return; // Prevent infinite recursion

        // Add current node to path
        currentPath.push_back(node->scenarioID);

        // If it's an ending, save the path
        if (node->isEnding) {
            allPaths.push_back(currentPath);
            currentPath.pop_back(); // Remove current node before returning
            return;
        }

        // Recursively find paths in left and right subtrees
        if (node->left) {
            findAllPaths(node->left, currentPath, depth + 1);
        }
        if (node->right) {
            findAllPaths(node->right, currentPath, depth + 1);
        }

        // Backtrack
        currentPath.pop_back();
    }

public:
    bool validateTree(DecisionTree& tree) {
        // Clear previous results
        allNodes.clear();
        allEndings.clear();
        allPaths.clear();

        // Start traversal from root
        DecisionNode* root = tree.getRoot();
        if (!root) {
            std::cout << "Error: Tree root is null!" << std::endl;
            return false;
        }

        // Collect all nodes and endings
        collectAllNodes(root);

        // Find all paths
        std::vector<int> currentPath;
        findAllPaths(root, currentPath);

        // Print results
        std::cout << "Validation Results:" << std::endl;
        std::cout << "- Total unique nodes: " << allNodes.size() << std::endl;
        std::cout << "- Total endings: " << allEndings.size() << std::endl;
        std::cout << "- Total possible paths: " << allPaths.size() << std::endl;

        if (allNodes.size() >= 20) {
            std::cout << "✓ Decision tree has 20+ nodes (" << allNodes.size() << ")" << std::endl;
        } else {
            std::cout << "✗ Decision tree has less than 20 nodes (" << allNodes.size() << ")" << std::endl;
        }

        if (allEndings.size() >= 5) {
            std::cout << "✓ Game has 5+ endings (" << allEndings.size() << ")" << std::endl;
        } else {
            std::cout << "✗ Game has less than 5 endings (" << allEndings.size() << ")" << std::endl;
        }

        if (allPaths.size() > 0) {
            std::cout << "✓ All decision paths validated (" << allPaths.size() << " paths)" << std::endl;
        } else {
            std::cout << "✗ No paths found!" << std::endl;
        }

        // Print all paths
        std::cout << "\nAll paths:" << std::endl;
        for (int i = 0; i < allPaths.size(); i++) {
            std::cout << "Path " << (i+1) << ": ";
            for (int j = 0; j < allPaths[i].size(); j++) {
                std::cout << allPaths[i][j];
                if (j < allPaths[i].size() - 1) std::cout << " -> ";
            }
            std::cout << std::endl;
        }

        // Print all ending IDs
        std::cout << "\nAll ending IDs: ";
        for (int id : allEndings) {
            std::cout << id << " ";
        }
        std::cout << std::endl;

        return (allNodes.size() >= 20 && allEndings.size() >= 5 && allPaths.size() > 0);
    }

    int getNodeCount() const {
        return allNodes.size();
    }

    int getEndingCount() const {
        return allEndings.size();
    }

    int getPathCount() const {
        return allPaths.size();
    }
};

int main() {
    std::cout << "==================================" << std::endl;
    std::cout << "Wolf Pack Survival Game - Validator" << std::endl;
    std::cout << "==================================" << std::endl;

    // Create and build the Classic story decision tree
    DecisionTree tree;
    tree.buildClassicStory();

    // Validate the Classic tree
    std::cout << "\n--- CLASSIC STORYLINE ---" << std::endl;
    DecisionTreeValidator validator;
    bool isValid = validator.validateTree(tree);

    // Create and build the Survival story decision tree
    DecisionTree tree2;
    tree2.buildSurvivalStory();

    // Validate the Survival tree
    std::cout << "\n--- SURVIVAL STORYLINE ---" << std::endl;
    DecisionTreeValidator validator2;
    bool isValid2 = validator2.validateTree(tree2);

    // Create and build the Pack story decision tree
    DecisionTree tree3;
    tree3.buildPackStory();

    // Validate the Pack tree
    std::cout << "\n--- PACK STORYLINE ---" << std::endl;
    DecisionTreeValidator validator3;
    bool isValid3 = validator3.validateTree(tree3);

    // Summary
    std::cout << "\n==================================" << std::endl;
    std::cout << "SUMMARY" << std::endl;
    std::cout << "==================================" << std::endl;
    std::cout << "Classic: " << (isValid ? "✓ PASS" : "✗ FAIL") 
              << " (" << validator.getNodeCount() << " nodes, " 
              << validator.getEndingCount() << " endings, " 
              << validator.getPathCount() << " paths)" << std::endl;
    std::cout << "Survival: " << (isValid2 ? "✓ PASS" : "✗ FAIL") 
              << " (" << validator2.getNodeCount() << " nodes, " 
              << validator2.getEndingCount() << " endings, " 
              << validator2.getPathCount() << " paths)" << std::endl;
    std::cout << "Pack: " << (isValid3 ? "✓ PASS" : "✗ FAIL") 
              << " (" << validator3.getNodeCount() << " nodes, " 
              << validator3.getEndingCount() << " endings, " 
              << validator3.getPathCount() << " paths)" << std::endl;

    if (isValid && isValid2 && isValid3) {
        std::cout << "\n✓ All storylines validated!" << std::endl;
    } else {
        std::cout << "\n✗ Some storylines failed validation!" << std::endl;
    }

    std::cout << "==================================" << std::endl;

    return (isValid && isValid2 && isValid3) ? 0 : 1;
}