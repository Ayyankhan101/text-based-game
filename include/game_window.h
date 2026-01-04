#ifndef GAME_WINDOW_H
#define GAME_WINDOW_H

#include <QtWidgets/QWidget>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QMessageBox>
#include <QtWidgets/QProgressBar>
#include <fstream>
#include <QtWidgets/QTextEdit>
#include <QtWidgets/QDialog>
#include <QtWidgets/QComboBox>
#include <QtCore/QPropertyAnimation>
#include <QtGui/QPainter>
#include <QtCore/QTimer>
#include <random>
#include "../include/wolf.h"
#include "../include/decision_tree.h"
#include "../include/priority_queue.h"
#include "../include/game_stack.h"
#include "../include/action_queue.h"
#include "../include/inventory.h"
#include "../include/pack.h"

class StatBar : public QWidget {
    Q_OBJECT
    Q_PROPERTY(int value READ value WRITE setValue)

public:
    StatBar(QWidget* parent = nullptr);
    int value() const;
    void setValue(int value);
    void animateTo(int value);
protected:
    void paintEvent(QPaintEvent* event) override;
private:
    int currentValue;
    QPropertyAnimation* animation;
};

class GameWindow : public QWidget {
    Q_OBJECT

public:
    GameWindow(Wolf& w, DecisionTree& t, PriorityQueue& e, GameStack& h, ActionQueue& a, Inventory& i, Pack& p, int& d, QWidget* parent = nullptr);

private slots:
    void onChoiceA();
    void onChoiceB();
    void onUndo();
    void onSave();
    void onLoad();
    void onInventory();
    void onPack();
    void updateStats();

private:
    Wolf& wolf;
    DecisionTree& tree;
    PriorityQueue& events;
    GameStack& history;
    ActionQueue& actions;
    Inventory& inventory;
    Pack& pack;
    int& dayCounter; // Add reference to day counter

    QTextEdit* storyLabel;
    QLabel* statsLabel;
    QLabel* dayLabel;
    QLabel* packLabel;
    QPushButton* buttonA;
    QPushButton* buttonB;
    QPushButton* undoButton;
    QPushButton* saveButton;
    QPushButton* loadButton;
    QPushButton* inventoryButton;
    QPushButton* packButton;
    StatBar* healthBar;
    StatBar* hungerBar;
    StatBar* energyBar;
    StatBar* spiritBar;
    QLabel* wolfGraphic;

    std::random_device rd;
    std::mt19937 gen{rd()};
    std::uniform_real_distribution<> dis{0.0, 1.0};

    void updateDisplay();
    void processChoice(bool isA);
    void checkEvents();
    void updateWolfGraphic(int scenarioId);
    void updateDayDisplay();
    void updatePackDisplay();
};

#endif