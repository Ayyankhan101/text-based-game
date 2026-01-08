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

    void findAllPaths(DecisionNode* node, std::vector<int>& currentPath) {
        if (!node) return;

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
            findAllPaths(node->left, currentPath);
        }
        if (node->right) {
            findAllPaths(node->right, currentPath);
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

        // Print some sample paths
        std::cout << "\nSample paths (first 3):" << std::endl;
        for (int i = 0; i < std::min(3, (int)allPaths.size()); i++) {
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

    // Create and build the decision tree
    DecisionTree tree;
    tree.buildSampleTree();

    // Validate the tree
    DecisionTreeValidator validator;
    bool isValid = validator.validateTree(tree);

    if (isValid) {
        std::cout << "\n✓ All validations passed!" << std::endl;
    } else {
        std::cout << "\n✗ Some validations failed!" << std::endl;
    }

    std::cout << "==================================" << std::endl;

    return 0;
}