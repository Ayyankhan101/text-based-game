#ifndef DECISION_TREE_H
#define DECISION_TREE_H

#include <string>

struct DecisionNode {
    int scenarioID;
    std::string description;
    std::string choiceA_text;
    std::string choiceB_text;
    DecisionNode* left;  // Choice A
    DecisionNode* right; // Choice B
    bool isEnding;
    std::string endingText;
};

class DecisionTree {
private:
    DecisionNode* root;
    DecisionNode* currentNode;
    void deleteTree(DecisionNode* node);  // Helper for destructor
public:
    DecisionTree();
    ~DecisionTree();
    void insertNode(DecisionNode* parent, DecisionNode* child, bool isLeft);
    DecisionNode* getCurrentNode() const;
    void setCurrentNode(DecisionNode* node);
    DecisionNode* getRoot();
    void buildSampleTree();
    DecisionNode* findNodeById(int id);  // Add function to find node by ID
};

#endif