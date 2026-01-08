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
        DecisionNode* getRoot();
        void setCurrentNode(DecisionNode* node);
        void buildSampleTree();
        void buildClassicStory();
        void buildSurvivalStory();
        void buildPackStory();
        DecisionNode* findNodeById(int id);
};

#endif