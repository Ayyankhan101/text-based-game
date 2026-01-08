#ifndef GAME_WINDOW_H
#define GAME_WINDOW_H

#include <QtWidgets/QMainWindow>
#include <QtWidgets/QWidget>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QMessageBox>
#include <QtWidgets/QProgressBar>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QDockWidget>
#include <QtWidgets/QSplitter>
#include <QtWidgets/QTabWidget>
#include <QtWidgets/QListWidget>
#include <QtWidgets/QTreeWidget>
#include <QtWidgets/QTreeWidgetItem>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QFrame>
#include <QtWidgets/QApplication>
#include "game_types.h"
#include <fstream>
#include <QtWidgets/QTextEdit>
#include <QtWidgets/QDialog>
#include <QtWidgets/QComboBox>
#include <QtGui/QCloseEvent>
#include <QtWidgets/QScrollArea>
#include <QtGui/QPainter>
#include <QtCore/QTimer>
#include <random>
#include <unordered_set>
#include "wolf.h"
#include "decision_tree.h"
#include "priority_queue.h"
#include "game_stack.h"
#include "action_queue.h"
#include "inventory.h"
#include "pack.h"

class StatBar : public QWidget {
    Q_OBJECT
    Q_PROPERTY(int value READ value WRITE setValue)

public:
    StatBar(const QString& label, QWidget* parent = nullptr);
    int value() const;
    void setValue(int value);
    void setLabel(const QString& label);
    protected:
    void paintEvent(QPaintEvent* event) override;
private:
    int currentValue;
    QString labelText;
};

class GameWindow : public QMainWindow {
    Q_OBJECT

public:
    GameWindow(Wolf& w, DecisionTree& t, PriorityQueue& e, GameStack& h, ActionQueue& a, Inventory& i, Pack& p, int& d, Difficulty& diff, Storyline& story, QWidget* parent = nullptr);
    void cleanupFallbackNodes();  // Made public for proper cleanup

 signals:
    void gameClosed();     // Signal emitted when game window is closed
    void returnToMenu();   // Signal emitted when user wants to return to main menu

private slots:
    void onChoiceA();
    void onChoiceB();
    void onUndo();
    void onSave();
    void onLoad();
    void onInventory();
    void onPack();
    void updateStats();
    void onNewGame();
    void onAbout();
    void onExit();
    void onReturnToMenu();  // Add slot for returning to menu

private:
    void checkBasicAchievements();
    void closeEvent(QCloseEvent* event) override;  // Override to handle window close
    Wolf& wolf;
    DecisionTree& tree;
    PriorityQueue& events;
    GameStack& history;
    ActionQueue& actions;
    Inventory& inventory;
    Pack& pack;
    int& dayCounter; // Add reference to day counter
    Difficulty& difficulty;
    Storyline& storyline;
    float eventChance;
    int hungerIncrease;

    // Main layout components
    QMenuBar* menuBar;
    QStatusBar* statusBar;
    QSplitter* mainSplitter;
    QScrollArea* storyScrollArea;
    QLabel* storyLabel;
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
    QPushButton* menuButton;  // Button to return to main menu
    StatBar* healthBar;
    StatBar* hungerBar;
    StatBar* energyBar;
    StatBar* spiritBar;
    QLabel* wolfGraphic;

    std::random_device rd;
    std::mt19937 gen{rd()};
    std::uniform_real_distribution<> dis{0.0, 1.0};

    // Track fallback nodes for proper memory management
    std::vector<DecisionNode*> fallbackNodes;
    std::unordered_set<DecisionNode*> deletedNodes;

    // Achievement flags (moved from static to instance variables)
    bool healthAchieved = false, energyAchieved = false, spiritAchieved = false;
    bool pack3Achieved = false, pack5Achieved = false, repAchieved = false;
    bool survivor10Achieved = false, survivor15Achieved = false;
    bool alphaAchieved = false;
    bool explorerAchieved = false, strategistAchieved = false;
    bool stoicExplorerAchieved = false, acceptingSpiritAchieved = false;

    void setupMenuBar();
    void setupStatusBar();
    void setupCentralWidget();
    void updateDisplay();
    void processChoice(bool isA);
    void checkEvents();
    void updateWolfGraphic(int scenarioId);
    void updateDayDisplay();
    void updatePackDisplay();
    void updateInventoryDisplay();
    void updateGameInfo();
};

#endif