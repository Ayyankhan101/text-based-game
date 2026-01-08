#include <iostream>
#include <queue>
#include "include/decision_tree.h"

int main() {
    DecisionTree tree;
    tree.buildClassicStory();
    
    std::queue<DecisionNode*> q;
    DecisionNode* root = tree.getRoot();
    if (!root) {
        std::cout << "ERROR: Root is null!" << std::endl;
        return 1;
    }
    
    q.push(root);
    int nodeCount = 0;
    int endingCount = 0;
    int nullLeftCount = 0;
    int nullRightCount = 0;
    
    while (!q.empty()) {
        DecisionNode* current = q.front();
        q.pop();
        
        nodeCount++;
        
        if (current->isEnding) {
            endingCount++;
        } else {
            if (current->left == nullptr) {
                nullLeftCount++;
                std::cout << "WARNING: Node " << current->scenarioID << " has null left child" << std::endl;
            } else {
                q.push(current->left);
            }
            
            if (current->right == nullptr) {
                nullRightCount++;
                std::cout << "WARNING: Node " << current->scenarioID << " has null right child" << std::endl;
            } else {
                q.push(current->right);
            }
        }
    }
    
    std::cout << "Tree Analysis:" << std::endl;
    std::cout << "Total nodes: " << nodeCount << std::endl;
    std::cout << "Ending nodes: " << endingCount << std::endl;
    std::cout << "Non-ending nodes with null left: " << nullLeftCount << std::endl;
    std::cout << "Non-ending nodes with null right: " << nullRightCount << std::endl;
    
    if (nullLeftCount == 0 && nullRightCount == 0) {
        std::cout << "SUCCESS: Tree is complete with no null pointers!" << std::endl;
    } else {
        std::cout << "ISSUE: Tree has " << (nullLeftCount + nullRightCount) << " null pointers" << std::endl;
    }
    
    return 0;
}