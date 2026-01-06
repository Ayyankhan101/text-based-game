#include "../include/game_window.h"
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMessageBox>
#include <QtGui/QPainter>
#include <QtGui/QPainterPath>
#include <QtGui/QLinearGradient>
#include <QtGui/QGuiApplication>
#include <QtGui/QScreen>
#include <iostream>
#include <fstream>
#include "inventory.h"



// StatBar implementation
StatBar::StatBar(const QString& label, QWidget* parent) : QWidget(parent), currentValue(50), labelText(label) {
    setMinimumSize(100, 25);
    setMaximumHeight(25);

    animation = new QPropertyAnimation(this, "value");
    animation->setDuration(500);
    connect(animation, &QPropertyAnimation::valueChanged, this, [this]() {
        update();
    });
}

void StatBar::setLabel(const QString& label) {
    labelText = label;
    update();
}

int StatBar::value() const {
    return currentValue;
}

void StatBar::setValue(int value) {
    currentValue = value;
    update();
}

void StatBar::animateTo(int value) {
    animation->setStartValue(currentValue);
    animation->setEndValue(value);
    animation->start();
}

void StatBar::paintEvent(QPaintEvent* event) {
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    // Calculate the area for the label and the bar
    int labelHeight = 15;
    QRect labelRect = QRect(0, 0, width(), labelHeight);
    QRect barRect = QRect(0, labelHeight, width(), height() - labelHeight);

    // Draw label
    if (!labelText.isEmpty()) {
        painter.setPen(QColor(255, 255, 255)); // White text
        painter.setFont(QFont("Arial", 9, QFont::Bold));
        painter.drawText(labelRect, Qt::AlignCenter, labelText);
    }

    // Draw background with rounded corners for the bar part
    QPainterPath backgroundPath;
    backgroundPath.addRoundedRect(barRect, 8, 8);
    painter.fillPath(backgroundPath, QColor(105, 105, 105)); // Darker gray for better contrast

    // Calculate fill width based on current value (0-100)
    int fillWidth = (currentValue * barRect.width()) / 100;

    // Create a smaller rectangle for the filled portion to account for padding
    QRect fillRect = barRect.adjusted(3, 3, -3, -3);
    fillRect.setWidth(fillWidth - 6); // Subtract 6 to account for left and right padding

    // Choose color based on value with improved colors
    QColor fillColor;
    if (currentValue > 70) {
        fillColor = QColor(76, 175, 80); // Material Green
    } else if (currentValue > 30) {
        fillColor = QColor(255, 193, 7); // Material Amber
    } else {
        fillColor = QColor(244, 67, 54); // Material Red
    }

    // Add gradient effect to the filled portion
    if (!fillRect.isEmpty()) {
        QLinearGradient gradient(0, barRect.y(), 0, barRect.y() + barRect.height());
        gradient.setColorAt(0.0, fillColor.lighter(120));
        gradient.setColorAt(1.0, fillColor.darker(120));

        QPainterPath fillPath;
        fillPath.addRoundedRect(fillRect, 5, 5);
        painter.fillPath(fillPath, gradient);

        // Add subtle shine effect
        QRectF shineRect = QRectF(fillRect.x(), fillRect.y(), fillRect.width(), fillRect.height()/3);
        QLinearGradient shineGradient(0, fillRect.y(), 0, fillRect.y() + fillRect.height()/3);
        shineGradient.setColorAt(0.0, QColor(255, 255, 255, 80));
        shineGradient.setColorAt(1.0, QColor(255, 255, 255, 0));
        painter.fillRect(shineRect, QBrush(shineGradient));
    }

    // Draw border for the bar part
    painter.setPen(QColor(50, 50, 50)); // Darker border for better contrast
    painter.drawRoundedRect(barRect.adjusted(0, 0, -1, -1), 8, 8);

    // Draw value text in the center of the bar if there's enough space
    if (barRect.width() > 60) { // Only draw text if the bar is wide enough
        painter.setPen(QColor(255, 255, 255)); // White text
        painter.setFont(QFont("Arial", 8, QFont::Bold));
        QString valueText = QString("%1%").arg(currentValue);
        painter.drawText(barRect, Qt::AlignCenter, valueText);
    }

    QWidget::paintEvent(event);
}

GameWindow::GameWindow(Wolf& w, DecisionTree& t, PriorityQueue& e, GameStack& h, ActionQueue& a, Inventory& i, Pack& p, int& d, QWidget* parent)
    : QMainWindow(parent), wolf(w), tree(t), events(e), history(h), actions(a), inventory(i), pack(p), dayCounter(d) {

    qDebug() << "GameWindow constructor start";

    // Temporary fixed size for debugging
    resize(1000, 700);

    qDebug() << "GameWindow constructor: size set";

    // Set up the main layout with menu bar, central widget, and status bar
    setupMenuBar();
    setupStatusBar();
    setupCentralWidget();
    setupSidebar();

    // Initialize display
    updateDisplay();
}

void GameWindow::setupMenuBar() {
    menuBar = new QMenuBar(this);

    // File menu
    QMenu* fileMenu = menuBar->addMenu("File");
    QAction* newGameAction = fileMenu->addAction("New Game");
    newGameAction->setShortcut(QKeySequence("Ctrl+N"));
    connect(newGameAction, &QAction::triggered, this, &GameWindow::onNewGame);

    QAction* saveAction = fileMenu->addAction("Save");
    saveAction->setShortcut(QKeySequence("Ctrl+S"));
    connect(saveAction, &QAction::triggered, this, &GameWindow::onSave);

    QAction* loadAction = fileMenu->addAction("Load");
    loadAction->setShortcut(QKeySequence("Ctrl+L"));
    connect(loadAction, &QAction::triggered, this, &GameWindow::onLoad);

    fileMenu->addSeparator();

    QAction* exitAction = fileMenu->addAction("Exit");
    exitAction->setShortcut(QKeySequence("Ctrl+Q"));
    connect(exitAction, &QAction::triggered, this, &GameWindow::onExit);

    // Game menu
    QMenu* gameMenu = menuBar->addMenu("Game");
    QAction* undoAction = gameMenu->addAction("Undo");
    undoAction->setShortcut(QKeySequence("Ctrl+Z"));
    connect(undoAction, &QAction::triggered, this, &GameWindow::onUndo);

    QAction* inventoryAction = gameMenu->addAction("Inventory");
    inventoryAction->setShortcut(QKeySequence("Ctrl+I"));
    connect(inventoryAction, &QAction::triggered, this, &GameWindow::onInventory);

    QAction* packAction = gameMenu->addAction("Pack");
    packAction->setShortcut(QKeySequence("Ctrl+P"));
    connect(packAction, &QAction::triggered, this, &GameWindow::onPack);

    // Help menu
    QMenu* helpMenu = menuBar->addMenu("Help");
    QAction* aboutAction = helpMenu->addAction("About");
    connect(aboutAction, &QAction::triggered, this, &GameWindow::onAbout);
}

void GameWindow::setupStatusBar() {
    statusBar = new QStatusBar(this);
    // Add to layout later in setupCentralWidget
}

void GameWindow::setupCentralWidget() {
    qDebug() << "setupCentralWidget start";

    // Dark theme styling
    setStyleSheet(R"(
        QWidget {
            background: #1a1a2e;
            color: #e0e0e0;
        }
        QLabel {
            background: transparent;
            color: #e0e0e0;
        }
        QPushButton {
            background: #16213e;
            color: #e0e0e0;
            border: 2px solid #0f3460;
            border-radius: 8px;
            padding: 8px 16px;
            font-weight: bold;
        }
        QPushButton:hover {
            background: #0f3460;
        }
        QPushButton:pressed {
            background: #e94560;
        }
        QTextEdit {
            background: #16213e;
            color: #e0e0e0;
            border: 2px solid #0f3460;
            border-radius: 8px;
            padding: 10px;
        }
    )");

    qDebug() << "setupCentralWidget: stylesheet set";

    // Create central widget
    QWidget* centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);
    qDebug() << "setupCentralWidget: central widget created";

    // Create main layout for central widget
    QVBoxLayout* mainLayout = new QVBoxLayout(centralWidget);
    qDebug() << "setupCentralWidget: layout created";

    // Create title with wolf ASCII art
    QLabel* titleLabel = new QLabel(centralWidget);
    titleLabel->setText(
        "🌲🌲🌲 WOLF PACK SURVIVAL ADVENTURE 🌲🌲🌲\n"
        "           /\\   /\\   \n"
        "          (  . .)  \n"
        "           )   (   \n"
        "          (  v  )  \n"
        "         ^^  ^  ^^"
    );
    titleLabel->setAlignment(Qt::AlignCenter);
    titleLabel->setStyleSheet("font-size: 16px; font-weight: bold; color: #f39c12; padding: 10px;");
    mainLayout->addWidget(titleLabel);

    // Create enhanced stats display with day counter
    QHBoxLayout* topInfoLayout = new QHBoxLayout();

    // Day counter with moon phases
    dayLabel = new QLabel(centralWidget);
    updateDayDisplay();
    dayLabel->setStyleSheet("font-size: 18px; font-weight: bold; color: #f1c40f; padding: 10px; border: 2px solid #f39c12; border-radius: 8px;");
    topInfoLayout->addWidget(dayLabel);

    // Pack size indicator
    packLabel = new QLabel(centralWidget);
    updatePackDisplay();
    packLabel->setStyleSheet("font-size: 16px; font-weight: bold; color: #e74c3c; padding: 10px; border: 2px solid #c0392b; border-radius: 8px;");
    topInfoLayout->addWidget(packLabel);

    mainLayout->addLayout(topInfoLayout);

    // Create stat bars with enhanced styling
    QHBoxLayout* barsLayout = new QHBoxLayout();

    // Health bar with heart icon
    healthBar = new StatBar("❤️ Health", centralWidget);
    healthBar->setMinimumSize(150, 40);
    barsLayout->addWidget(healthBar);

    // Hunger bar with food icon
    hungerBar = new StatBar("🍖 Hunger", centralWidget);
    hungerBar->setMinimumSize(150, 40);
    barsLayout->addWidget(hungerBar);

    // Energy bar with lightning icon
    energyBar = new StatBar("⚡ Energy", centralWidget);
    energyBar->setMinimumSize(150, 40);
    barsLayout->addWidget(energyBar);

    // Spirit bar with star icon
    spiritBar = new StatBar("⭐ Spirit", centralWidget);
    spiritBar->setMinimumSize(150, 40);
    barsLayout->addWidget(spiritBar);

    mainLayout->addLayout(barsLayout);

    // Create main content area with splitter for story and choices
    mainSplitter = new QSplitter(Qt::Horizontal, centralWidget);

    // Left side: Story area with wolf graphic
    QWidget* storyWidget = new QWidget(centralWidget);
    QVBoxLayout* storyLayout = new QVBoxLayout(storyWidget);

    // Animated wolf graphic area
    wolfGraphic = new QLabel(centralWidget);
    wolfGraphic->setMinimumSize(200, 200);
    wolfGraphic->setMaximumSize(200, 200);
    wolfGraphic->setStyleSheet(R"(
        background: qlineargradient(x1:0, y1:0, x2:1, y2:1,
            stop:0 #27ae60, stop:0.5 #2ecc71, stop:1 #27ae60);
        border: 3px solid #1e8449;
        border-radius: 15px;
        color: white;
        font-weight: bold;
        font-size: 24px;
    )");
    wolfGraphic->setAlignment(Qt::AlignCenter);
    updateWolfGraphic(1); // Initialize with starting graphic

    storyLayout->addWidget(wolfGraphic);

    // Simple story text
    storyLabel = new QLabel(centralWidget);
    storyLabel->setWordWrap(true);
    storyLabel->setText("Welcome to the game");
    storyLayout->addWidget(storyLabel, 1);

    // Choice buttons
    QHBoxLayout* choiceLayout = new QHBoxLayout();

    buttonA = new QPushButton("Choice A", centralWidget);
    buttonA->setStyleSheet(R"(
        QPushButton {
            background: qlineargradient(x1:0, y1:0, x2:0, y2:1,
                stop:0 #e74c3c, stop:1 #c0392b);
            border: 2px solid #a93226;
            border-radius: 10px;
            color: white;
            font-weight: bold;
            font-size: 14px;
            padding: 12px 20px;
            min-height: 40px;
        }
        QPushButton:hover {
            background: qlineargradient(x1:0, y1:0, x2:0, y2:1,
                stop:0 #ec7063, stop:1 #e74c3c);
        }
        QPushButton:pressed {
            background: qlineargradient(x1:0, y1:0, x2:0, y2:1,
                stop:0 #c0392b, stop:1 #a93226);
        }
    )");
    connect(buttonA, &QPushButton::clicked, this, &GameWindow::onChoiceA);

    buttonB = new QPushButton("Choice B", centralWidget);
    buttonB->setStyleSheet(R"(
        QPushButton {
            background: qlineargradient(x1:0, y1:0, x2:0, y2:1,
                stop:0 #3498db, stop:1 #2980b9);
            border: 2px solid #2471a3;
            border-radius: 10px;
            color: white;
            font-weight: bold;
            font-size: 14px;
            padding: 12px 20px;
            min-height: 40px;
        }
        QPushButton:hover {
            background: qlineargradient(x1:0, y1:0, x2:0, y2:1,
                stop:0 #5dade2, stop:1 #3498db);
        }
        QPushButton:pressed {
            background: qlineargradient(x1:0, y1:0, x2:0, y2:1,
                stop:0 #2980b9, stop:1 #2471a3);
        }
    )");
    connect(buttonB, &QPushButton::clicked, this, &GameWindow::onChoiceB);

    choiceLayout->addWidget(buttonA);
    choiceLayout->addWidget(buttonB);
    storyLayout->addLayout(choiceLayout);

    // Add story widget to splitter
    mainSplitter->addWidget(storyWidget);

    // Right side: Control buttons
    QWidget* controlWidget = new QWidget(centralWidget);
    QVBoxLayout* controlLayout = new QVBoxLayout(controlWidget);

    // Control buttons group
    QGroupBox* controlsGroup = new QGroupBox("Game Controls", centralWidget);
    QVBoxLayout* controlsLayout = new QVBoxLayout(controlsGroup);

    undoButton = new QPushButton("↶ Undo", centralWidget);
    saveButton = new QPushButton("💾 Save", centralWidget);
    loadButton = new QPushButton("📁 Load", centralWidget);
    inventoryButton = new QPushButton("🎒 Inventory", centralWidget);
    packButton = new QPushButton("🐺 Pack", centralWidget);

    // Style control buttons
    QString controlButtonStyle =
        "QPushButton {"
            "background: qlineargradient(x1:0, y1:0, x2:0, y2:1,"
                "stop:0 #95a5a6, stop:1 #7f8c8d);"
            "border: 2px solid #6c7b7d;"
            "border-radius: 8px;"
            "color: white;"
            "font-weight: bold;"
            "padding: 10px 15px;"
            "min-height: 40px;"
        "}"
        "QPushButton:hover {"
            "background: qlineargradient(x1:0, y1:0, x2:0, y2:1,"
                "stop:0 #bdc3c7, stop:1 #95a5a6);"
        "}";

    undoButton->setStyleSheet(controlButtonStyle);
    saveButton->setStyleSheet(controlButtonStyle);
    loadButton->setStyleSheet(controlButtonStyle);
    inventoryButton->setStyleSheet(controlButtonStyle);
    packButton->setStyleSheet(controlButtonStyle);

    connect(undoButton, &QPushButton::clicked, this, &GameWindow::onUndo);
    connect(saveButton, &QPushButton::clicked, this, &GameWindow::onSave);
    connect(loadButton, &QPushButton::clicked, this, &GameWindow::onLoad);
    connect(inventoryButton, &QPushButton::clicked, this, &GameWindow::onInventory);
    connect(packButton, &QPushButton::clicked, this, &GameWindow::onPack);

    controlsLayout->addWidget(undoButton);
    controlsLayout->addWidget(saveButton);
    controlsLayout->addWidget(loadButton);
    controlsLayout->addWidget(inventoryButton);
    controlsLayout->addWidget(packButton);
    controlsLayout->addStretch(); // Add stretch to push buttons to the top

    controlLayout->addWidget(controlsGroup);
    controlLayout->addStretch(); // Add stretch to fill space

    mainSplitter->addWidget(controlWidget);
    mainSplitter->setStretchFactor(0, 3); // Story area gets more space
    mainSplitter->setStretchFactor(1, 1); // Controls area gets less space

    mainLayout->addWidget(mainSplitter);

    // Set the status bar
    setStatusBar(statusBar);
}

void GameWindow::setupSidebar() {
    qDebug() << "setupSidebar: sidebar feature disabled in simplified GUI mode";
    // Note: Sidebar functionality was disabled during debugging to prevent crashes.
    // If needed in the future, the complete implementation is available in git history.
    // The sidebar would provide inventory and pack management in a dock widget.
}

void GameWindow::onChoiceA() {
    qDebug() << "onChoiceA called";
    processChoice(true);
}

void GameWindow::updateInventoryDisplay() {
    if (inventoryList) {
        inventoryList->clear();

        Item* current = inventory.getHead();
        if (current) {
            while (current) {
                QString itemText = QString("%1 (x%2)").arg(QString::fromStdString(current->name)).arg(current->quantity);
                inventoryList->addItem(itemText);
                current = current->next;
            }
        } else {
            inventoryList->addItem("No items in inventory");
        }
    }
}

void GameWindow::updatePackDisplaySidebar() {
    if (packTree) {
        packTree->clear();

        PackMember* current = pack.getHead();
        if (current) {
            while (current) {
                QTreeWidgetItem* item = new QTreeWidgetItem();
                item->setText(0, QString::fromStdString(current->name));
                item->setText(1, QString::fromStdString(current->role));
                item->setText(2, QString::number(current->loyalty));

                // Color code loyalty
                if (current->loyalty >= 70) {
                    item->setForeground(2, QBrush(QColor(0, 255, 0))); // Green for loyal
                } else if (current->loyalty >= 40) {
                    item->setForeground(2, QBrush(QColor(255, 255, 0))); // Yellow for stable
                } else {
                    item->setForeground(2, QBrush(QColor(255, 0, 0))); // Red for unstable
                }

                packTree->addTopLevelItem(item);
                current = current->next;
            }
        } else {
            QTreeWidgetItem* item = new QTreeWidgetItem();
            item->setText(0, "No pack members");
            item->setText(1, "");
            item->setText(2, "");
            packTree->addTopLevelItem(item);
        }
    }
}

void GameWindow::updateGameInfo() {
    if (gameInfoLabel) {
        QString infoText = QString(
            "Day: %1\n"
            "Health: %2\n"
            "Hunger: %3\n"
            "Energy: %4\n"
            "Spirit: %5\n"
            "Reputation: %6\n"
            "Pack Size: %7\n"
            "\n"
            "Game Status:\n"
            "%8"
        ).arg(dayCounter)
         .arg(wolf.health)
         .arg(wolf.hunger)
         .arg(wolf.energy)
         .arg(wolf.spirit)
         .arg(wolf.reputation)
         .arg(pack.getSize())
         .arg(wolf.isAlive() ? "Alive" : "Dead");

        gameInfoLabel->setText(infoText);
    }
}

void GameWindow::onNewGame() {
    // Reset game state
    wolf = Wolf();
    dayCounter = 1;
    inventory = Inventory();
    pack = Pack();

    // Rebuild decision tree
    tree.buildSampleTree();

    // Update displays
    updateDisplay();
    updateStats();
    updateInventoryDisplay();
    updatePackDisplaySidebar();
    updateGameInfo();

    if (statusBar) {
        statusBar->showMessage("New game started!", 2000);
    }
}



void GameWindow::onAbout() {
    QMessageBox::about(this, "About",
                       "Wolf Pack Survival Adventure\n\n"
                       "A text-based survival game with Qt GUI\n\n"
                       "Version 1.0\n"
                       "© 2026 Text-Based Game Project");
}

void GameWindow::onExit() {
    QMessageBox::StandardButton reply =
        QMessageBox::question(this, "Exit Game",
                              "Are you sure you want to exit the game?",
                              QMessageBox::Yes | QMessageBox::No);

    if (reply == QMessageBox::Yes) {
        QApplication::quit();
    }
}

void GameWindow::onChoiceB() {
    processChoice(false);
}

void GameWindow::onUndo() {
    if (!history.isEmpty()) {
        GameState prev = history.pop();
        wolf.health = prev.health;
        wolf.hunger = prev.hunger;
        wolf.energy = prev.energy;
        dayCounter = prev.day;  // Restore day counter
        // Restore inventory from the saved copy
        if (prev.inventory) {
            inventory.replaceWith(prev.inventory);
            delete prev.inventory; // Clean up the copied inventory after restoration
        }
        tree.setCurrentNode(prev.currentNode);
        updateDisplay();  // Update the display after undo
        updateStats();    // Update the stats display
        // Update button states after undo
        updateDisplay();  // Update button states based on current wolf state
    } else {
        QMessageBox::information(this, "Undo", "No actions to undo.");
    }
}

void GameWindow::onSave() {
    // Create a dialog to select save slot
    QDialog saveDialog(this);
    saveDialog.setWindowTitle("Save Game");
    saveDialog.setModal(true);
    saveDialog.resize(300, 150);

    QVBoxLayout* layout = new QVBoxLayout(&saveDialog);

    QLabel* label = new QLabel("Select save slot:", &saveDialog);
    layout->addWidget(label);

    QComboBox* slotSelector = new QComboBox(&saveDialog);
    slotSelector->addItem("Slot 1");
    slotSelector->addItem("Slot 2");
    slotSelector->addItem("Slot 3");
    layout->addWidget(slotSelector);

    QHBoxLayout* buttonLayout = new QHBoxLayout();
    QPushButton* saveButton = new QPushButton("Save", &saveDialog);
    QPushButton* cancelButton = new QPushButton("Cancel", &saveDialog);
    buttonLayout->addWidget(saveButton);
    buttonLayout->addWidget(cancelButton);
    layout->addLayout(buttonLayout);

    // Connect cancel button
    connect(cancelButton, &QPushButton::clicked, &saveDialog, &QDialog::reject);

    // Connect save button
    // Note: Lambda captures by reference are safe here because button clicks are synchronous
    // The dialog and slotSelector exist for the duration of the user's interaction
    connect(saveButton, &QPushButton::clicked, this, [this, &saveDialog, slotSelector]() {
        int slot = slotSelector->currentIndex() + 1;
        std::string filename = "savegame_" + std::to_string(slot) + ".txt";
        std::ofstream saveFile(filename);
    if (saveFile.is_open()) {
        // Save wolf stats
        saveFile << wolf.health << "\n";
        saveFile << wolf.hunger << "\n";
        saveFile << wolf.energy << "\n";
        saveFile << wolf.reputation << "\n";
        saveFile << wolf.spirit << "\n";
        saveFile << dayCounter << "\n";

        // Save current node ID
        if (tree.getCurrentNode()) {
            saveFile << tree.getCurrentNode()->scenarioID << "\n";
        } else {
            saveFile << "1\n"; // Default to start node if null
        }

        // Save inventory
        Item* currentItem = inventory.getHead();
        int itemCount = 0;
        // First count items
        Item* temp = currentItem;
        while (temp) {
            itemCount++;
            temp = temp->next;
        }
        saveFile << itemCount << "\n";

        // Then save each item
        currentItem = inventory.getHead();
        while (currentItem) {
            saveFile << currentItem->name << "\n";
            saveFile << currentItem->type << "\n";
            saveFile << currentItem->effect << "\n";
            saveFile << currentItem->quantity << "\n";
            currentItem = currentItem->next;
        }

        // Save pack
        PackMember* currentMember = pack.getHead();
        int memberCount = 0;
        // First count members
        PackMember* tempMember = currentMember;
        while (tempMember) {
            memberCount++;
            tempMember = tempMember->next;
        }
        saveFile << memberCount << "\n";

        // Then save each pack member
        currentMember = pack.getHead();
        while (currentMember) {
            saveFile << currentMember->name << "\n";
            saveFile << currentMember->role << "\n";
            saveFile << currentMember->loyalty << "\n";
            currentMember = currentMember->next;
        }

        saveFile.close();
        QMessageBox::information(this, "Save", "Game saved successfully!");
        saveDialog.accept();
    } else {
        QMessageBox::critical(this, "Save Error", "Could not open save file.");
        saveDialog.reject();
    }
});
    saveDialog.exec();
}

void GameWindow::onLoad() {
    // Create a dialog to select load slot
    QDialog loadDialog(this);
    loadDialog.setWindowTitle("Load Game");
    loadDialog.setModal(true);
    loadDialog.resize(300, 150);

    QVBoxLayout* layout = new QVBoxLayout(&loadDialog);

    QLabel* label = new QLabel("Select load slot:", &loadDialog);
    layout->addWidget(label);

    QComboBox* slotSelector = new QComboBox(&loadDialog);
    slotSelector->addItem("Slot 1");
    slotSelector->addItem("Slot 2");
    slotSelector->addItem("Slot 3");
    layout->addWidget(slotSelector);

    QHBoxLayout* buttonLayout = new QHBoxLayout();
    QPushButton* loadButton = new QPushButton("Load", &loadDialog);
    QPushButton* cancelButton = new QPushButton("Cancel", &loadDialog);
    buttonLayout->addWidget(loadButton);
    buttonLayout->addWidget(cancelButton);
    layout->addLayout(buttonLayout);

    // Connect cancel button
    connect(cancelButton, &QPushButton::clicked, &loadDialog, &QDialog::reject);

    // Connect load button
    // Note: Lambda captures by reference are safe here because button clicks are synchronous
    connect(loadButton, &QPushButton::clicked, this, [this, &loadDialog, slotSelector]() {
        int slot = slotSelector->currentIndex() + 1;
        std::string filename = "savegame_" + std::to_string(slot) + ".txt";
        std::ifstream loadFile(filename);
    if (loadFile.is_open()) {
        // Load wolf stats
        loadFile >> wolf.health;
        loadFile >> wolf.hunger;
        loadFile >> wolf.energy;
        loadFile >> wolf.reputation;
        loadFile >> wolf.spirit;
        loadFile >> dayCounter;

        // Load current node ID
        int nodeId;
        loadFile >> nodeId;

        // Find the node in the tree by ID
        tree.buildSampleTree(); // Rebuild tree to ensure it's populated
        DecisionNode* foundNode = tree.findNodeById(nodeId);
        if (foundNode) {
            tree.setCurrentNode(foundNode);
        } else {
            // If node not found, go to root
            tree.setCurrentNode(tree.getRoot());
        }

        // Clear current inventory
        Item* current = inventory.getHead();
        while (current) {
            Item* next = current->next;
            delete current;
            current = next;
        }
        inventory = Inventory(); // Reset inventory

        // Load inventory
        int itemCount;
        loadFile >> itemCount;
        for (int i = 0; i < itemCount; i++) {
            std::string name;
            ItemType type;
            int effect, quantity;

            loadFile >> name;
            int typeInt;
            loadFile >> typeInt;
            type = static_cast<ItemType>(typeInt);
            loadFile >> effect;
            loadFile >> quantity;

            inventory.addItem(name, type, effect, quantity);
        }

        // Clear current pack
        PackMember* currentMember = pack.getHead();
        while (currentMember) {
            PackMember* next = currentMember->next;
            delete currentMember;
            currentMember = next;
        }
        pack = Pack(); // Reset pack

        // Load pack
        int memberCount;
        loadFile >> memberCount;
        for (int i = 0; i < memberCount; i++) {
            std::string name, role;
            int loyalty;

            loadFile >> name >> role >> loyalty;

            pack.addMember(name, role, loyalty);
        }

        loadFile.close();

        // Update display after loading
        updateDisplay();
        updateStats();

        QMessageBox::information(this, "Load", "Game loaded successfully!");
        loadDialog.accept();
    } else {
        QMessageBox::critical(this, "Load Error", "Could not open save file.");
        loadDialog.reject();
    }
});
    loadDialog.exec();
}

void GameWindow::onInventory() {
    // Create a dialog to display inventory with options to use items
    QDialog inventoryDialog(this);
    inventoryDialog.setWindowTitle("Inventory");
    inventoryDialog.setModal(true);
    inventoryDialog.resize(400, 300);

    QVBoxLayout* layout = new QVBoxLayout(&inventoryDialog);

    // Display inventory contents
    QTextEdit* inventoryDisplay = new QTextEdit(&inventoryDialog);
    inventoryDisplay->setReadOnly(true);

    QString inventoryText = "Inventory:\n\n";

    Item* current = inventory.getHead();
    if (current) {
        int itemCount = 0;
        while (current) {
            inventoryText += QString("- %1 (x%2)\n").arg(QString::fromStdString(current->name)).arg(current->quantity);
            current = current->next;
            itemCount++;
        }
        inventoryText += QString("\nTotal items: %1").arg(itemCount);
    } else {
        inventoryText += "No items in inventory.";
    }

    inventoryDisplay->setText(inventoryText);
    layout->addWidget(inventoryDisplay);

    // Add "Use Item" button if there are items
    if (inventory.getHead()) {
        QHBoxLayout* buttonLayout = new QHBoxLayout();

        QPushButton* useButton = new QPushButton("Use Item", &inventoryDialog);
        QPushButton* closeButton = new QPushButton("Close", &inventoryDialog);

        buttonLayout->addWidget(useButton);
        buttonLayout->addWidget(closeButton);
        layout->addLayout(buttonLayout);

        // Connect close button
        connect(closeButton, &QPushButton::clicked, &inventoryDialog, &QDialog::accept);

        // Connect use button
        connect(useButton, &QPushButton::clicked, this, [this, &inventoryDialog]() {
            // Create a dialog to select an item to use
            QDialog useDialog(this);
            useDialog.setWindowTitle("Use Item");
            useDialog.setModal(true);
            useDialog.resize(300, 200);

            QVBoxLayout* useLayout = new QVBoxLayout(&useDialog);

            QLabel* label = new QLabel("Select an item to use:", &useDialog);
            useLayout->addWidget(label);

            QComboBox* itemSelector = new QComboBox(&useDialog);
            Item* current = inventory.getHead();
            while (current) {
                itemSelector->addItem(QString::fromStdString(current->name + " (x" + std::to_string(current->quantity) + ")"));
                current = current->next;
            }
            useLayout->addWidget(itemSelector);

            QHBoxLayout* buttonLayout = new QHBoxLayout();
            QPushButton* useSelectedButton = new QPushButton("Use", &useDialog);
            QPushButton* cancelSelectedButton = new QPushButton("Cancel", &useDialog);
            buttonLayout->addWidget(useSelectedButton);
            buttonLayout->addWidget(cancelSelectedButton);
            useLayout->addLayout(buttonLayout);

            // Connect cancel button
            connect(cancelSelectedButton, &QPushButton::clicked, &useDialog, &QDialog::reject);

            // Connect use button
            connect(useSelectedButton, &QPushButton::clicked, this, [this, &useDialog, &inventoryDialog, itemSelector]() {
                int index = itemSelector->currentIndex();
                if (index >= 0) {
                    // Find the selected item in the inventory
                    Item* current = inventory.getHead();
                    for (int i = 0; i < index && current; i++) {
                        current = current->next;
                    }

                    if (current) {
                        // Use the proper useItem method that handles effects and removal
                        std::string itemName = current->name;
                        bool success = inventory.useItem(itemName, wolf);
                        
                        if (success) {
                            QMessageBox::information(this, "Item Used", 
                                QString("Used %1 successfully!").arg(QString::fromStdString(itemName)));
                            updateStats(); // Update the display
                        } else {
                            QMessageBox::warning(this, "Error", "Could not use item.");
                        }

                        useDialog.accept();
                        inventoryDialog.accept(); // Close inventory dialog too
                    }
                }
            });

            useDialog.exec();
        });
    } else {
        QPushButton* closeButton = new QPushButton("Close", &inventoryDialog);
        layout->addWidget(closeButton);
        connect(closeButton, &QPushButton::clicked, &inventoryDialog, &QDialog::accept);
    }

    inventoryDialog.exec();
}

void GameWindow::onPack() {
    // Create a dialog to display pack with management options
    QDialog packDialog(this);
    packDialog.setWindowTitle("Pack Management");
    packDialog.setModal(true);
    packDialog.resize(450, 350);

    QVBoxLayout* layout = new QVBoxLayout(&packDialog);

    // Display pack contents
    QTextEdit* packDisplay = new QTextEdit(&packDialog);
    packDisplay->setReadOnly(true);

    QString packText = "Pack Members:\n\n";

    PackMember* current = pack.getHead();
    if (current) {
        int memberCount = 0;
        while (current) {
            QString loyaltyStatus = "";
            QString loyaltyColor = "";
            if (current->loyalty >= 70) {
                loyaltyStatus = " (Loyal)";
                loyaltyColor = "<font color='green'>";
            } else if (current->loyalty >= 40) {
                loyaltyStatus = " (Stable)";
                loyaltyColor = "<font color='orange'>";
            } else {
                loyaltyStatus = " (Unstable)";
                loyaltyColor = "<font color='red'>";
            }

            packText += QString("%1%2 (%3, Loyalty: %4%5)</font><br>")
                           .arg(loyaltyColor)
                           .arg(QString::fromStdString(current->name))
                           .arg(QString::fromStdString(current->role))
                           .arg(current->loyalty)
                           .arg(loyaltyStatus);
            current = current->next;
            memberCount++;
        }
        packText += QString("<br>Total members: %1").arg(memberCount);
    } else {
        packText += "No pack members.";
    }

    packDisplay->setHtml(packText);
    layout->addWidget(packDisplay);

    // Add "Manage Pack" button if there are members
    if (pack.getHead()) {
        QHBoxLayout* buttonLayout = new QHBoxLayout();

        QPushButton* manageButton = new QPushButton("Manage Pack", &packDialog);
        QPushButton* closeButton = new QPushButton("Close", &packDialog);

        buttonLayout->addWidget(manageButton);
        buttonLayout->addWidget(closeButton);
        layout->addLayout(buttonLayout);

        // Connect close button
        connect(closeButton, &QPushButton::clicked, &packDialog, &QDialog::accept);

        // Connect manage button
        connect(manageButton, &QPushButton::clicked, this, [this, &packDialog]() {
            // Create a dialog to manage pack members
            QDialog manageDialog(this);
            manageDialog.setWindowTitle("Manage Pack");
            manageDialog.setModal(true);
            manageDialog.resize(300, 200);

            QVBoxLayout* manageLayout = new QVBoxLayout(&manageDialog);

            QLabel* label = new QLabel("Select a pack member to manage:", &manageDialog);
            manageLayout->addWidget(label);

            QComboBox* memberSelector = new QComboBox(&manageDialog);
            PackMember* current = pack.getHead();
            while (current) {
                memberSelector->addItem(QString::fromStdString(current->name + " (" + current->role + ", Loyalty: " + std::to_string(current->loyalty) + ")"));
                current = current->next;
            }
            manageLayout->addWidget(memberSelector);

            QHBoxLayout* buttonLayout = new QHBoxLayout();
            QPushButton* removeButton = new QPushButton("Remove Member", &manageDialog);
            QPushButton* cancelButton = new QPushButton("Cancel", &manageDialog);
            buttonLayout->addWidget(removeButton);
            buttonLayout->addWidget(cancelButton);
            manageLayout->addLayout(buttonLayout);

            // Connect cancel button
            connect(cancelButton, &QPushButton::clicked, &manageDialog, &QDialog::reject);

            // Connect remove button
            connect(removeButton, &QPushButton::clicked, this, [this, &manageDialog, memberSelector]() {
                int index = memberSelector->currentIndex();
                if (index >= 0) {
                    // Find the selected member in the pack
                    PackMember* current = pack.getHead();
                    for (int i = 0; i < index && current; i++) {
                        current = current->next;
                    }

                    if (current) {
                        // Confirm removal
                        QMessageBox::StandardButton reply =
                            QMessageBox::question(this, "Remove Pack Member",
                                QString("Are you sure you want to remove %1 from your pack?").arg(QString::fromStdString(current->name)),
                                QMessageBox::Yes | QMessageBox::No);

                        if (reply == QMessageBox::Yes) {
                            // Remove the member from the pack
                            pack.removeMember(current->name);

                            QMessageBox::information(this, "Pack Member Removed",
                                QString("Removed %1 from your pack.").arg(QString::fromStdString(current->name)));

                            manageDialog.accept();
                        }
                    }
                }
            });

            manageDialog.exec();
        });
    } else {
        QPushButton* closeButton = new QPushButton("Close", &packDialog);
        layout->addWidget(closeButton);
        connect(closeButton, &QPushButton::clicked, &packDialog, &QDialog::accept);
    }

    packDialog.exec();
}

void GameWindow::updateStats() {
    // Apply pack benefits
    int packBenefit = calculatePackBenefits(pack);
    if (packBenefit > 0) {
        // Apply benefits based on pack size and loyalty
        if (wolf.energy < 100) wolf.energy += packBenefit; // Pack helps with energy
        if (wolf.health < 100 && packBenefit > 10) wolf.health += packBenefit / 2; // Pack helps with health if benefit is significant
        if (wolf.spirit < 100) wolf.spirit += packBenefit; // Pack improves spirit/morale
        if (wolf.spirit > 100) wolf.spirit = 100;
        if (wolf.energy > 100) wolf.energy = 100;
        if (wolf.health > 100) wolf.health = 100;
    }

    // Animate stat bars to new values
    healthBar->animateTo(wolf.health);
    hungerBar->animateTo(100 - wolf.hunger); // Invert for hunger (lower is better)
    energyBar->animateTo(wolf.energy);
    spiritBar->animateTo(wolf.spirit);
    
    // Update displays
    updateDayDisplay();
    updatePackDisplay();
    updateWolfGraphic(tree.getCurrentNode() ? tree.getCurrentNode()->scenarioID : 1);
    
    // Update window title with current status
    QString status;
    if (wolf.health < 30) status = "💀 Critical";
    else if (wolf.hunger > 80) status = "🍖 Starving";
    else if (pack.getSize() > 2) status = "👑 Alpha";
    else status = "🐺 Surviving";
    
    setWindowTitle(QString("🐺 Wolf Pack Survival - %1 - Day %2").arg(status).arg(dayCounter));

    // Check for game over conditions
    if (!wolf.isAlive()) {
        // Disable choice buttons
        buttonA->setEnabled(true);
        buttonB->setEnabled(true);
        buttonA->setText("🔄 New Game");
        buttonB->setText("❌ Exit Game");
        
        // Disconnect old signals
        buttonA->disconnect();
        buttonB->disconnect();
        
        // Connect to new actions
        connect(buttonA, &QPushButton::clicked, this, &GameWindow::onNewGame);
        connect(buttonB, &QPushButton::clicked, this, [this]() {
            cleanupFallbackNodes();
            qApp->quit();
        });
        
        QString deathSummary = QString("💀 Game Over - Your wolf has died!\n\n"
                                      "📊 Final Statistics:\n"
                                      "Days Survived: %1\n"
                                      "Final Health: %2\n"
                                      "Final Hunger: %3\n"
                                      "Final Energy: %4\n"
                                      "Pack Size: %5")
                                 .arg(dayCounter)
                                 .arg(wolf.health)
                                 .arg(wolf.hunger)
                                 .arg(wolf.energy)
                                 .arg(pack.getSize());
        
        QMessageBox::critical(this, "Game Over", deathSummary);
        
        if (statusBar) {
            statusBar->showMessage("Game Over! Choose 'New Game' or 'Exit Game'", 0);
        }
        
        return; // Don't continue processing
    }

    // Check for win condition (survive 30 days)
    if (dayCounter >= 30) {
        // Disable choice buttons
        buttonA->setEnabled(true);
        buttonB->setEnabled(true);
        buttonA->setText("🔄 New Game");
        buttonB->setText("❌ Exit Game");
        
        // Disconnect old signals
        buttonA->disconnect();
        buttonB->disconnect();
        
        // Connect to new actions
        connect(buttonA, &QPushButton::clicked, this, &GameWindow::onNewGame);
        connect(buttonB, &QPushButton::clicked, this, [this]() {
            cleanupFallbackNodes();
            qApp->quit();
        });
        
        QString winSummary = QString("🏆 Victory! You survived 30 days!\n\n"
                                    "📊 Final Statistics:\n"
                                    "Days Survived: %1\n"
                                    "Final Health: %2\n"
                                    "Final Pack Size: %3\n"
                                    "Final Reputation: %4\n\n"
                                    "Congratulations on your survival!")
                               .arg(dayCounter)
                               .arg(wolf.health)
                               .arg(pack.getSize())
                               .arg(wolf.reputation);
        
        QMessageBox::information(this, "Victory!", winSummary);
        
        if (statusBar) {
            statusBar->showMessage("Victory! Choose 'New Game' or 'Exit Game'", 0);
        }
        
        return; // Don't continue processing
    }

    // Check for events
    checkEvents();
}

void GameWindow::updateDisplay() {
    qDebug() << "updateDisplay start";
    DecisionNode* current = tree.getCurrentNode();
    qDebug() << "updateDisplay: got current node: " << current;
    qDebug() << "updateDisplay: before if current";
    if (!current) {
        qDebug() << "updateDisplay: current is null, skipping update";
        return;
    }
    qDebug() << "updateDisplay: current is valid";
    qDebug() << "updateDisplay: current->isEnding: " << current->isEnding;
    qDebug() << "updateDisplay: description length: " << current->description.length();
    qDebug() << "updateDisplay: storyLabel pointer: " << (void*)storyLabel;
    qDebug() << "updateDisplay: buttonA pointer: " << (void*)buttonA;
    qDebug() << "updateDisplay: buttonB pointer: " << (void*)buttonB;
    qDebug() << "updateDisplay: before setText";
    storyLabel->setText(QString::fromStdString(current->description));
    qDebug() << "updateDisplay: after setText";

    if (current->isEnding) {
        qDebug() << "updateDisplay: ending detected";
        buttonA->setEnabled(true);
        buttonB->setEnabled(true);
        buttonA->setText("🔄 New Game");
        buttonB->setText("❌ Exit Game");
        
        // Disconnect old signals to prevent conflicts
        buttonA->disconnect();
        buttonB->disconnect();
        
        // Connect to new ending actions
        connect(buttonA, &QPushButton::clicked, this, &GameWindow::onNewGame);
        connect(buttonB, &QPushButton::clicked, this, [this]() {
            cleanupFallbackNodes();
            qApp->quit();
        });
        
        storyLabel->setText(QString::fromStdString(current->description));

        // Update wolf graphic based on scenario
        updateWolfGraphic(current->scenarioID);

        // Unlock appropriate achievement based on ending type
        if (current->scenarioID == 1000) { // Explorer path
            bool hasGoodStats = (wolf.health >= 50 && wolf.spirit >= 50 && pack.getSize() >= 2);
            if (hasGoodStats) {
                qDebug() << "ACHIEVEMENT UNLOCKED: Resilient Explorer - Found hope in unexpected conclusions!";
            } else {
                qDebug() << "ACHIEVEMENT UNLOCKED: Stoic Explorer - Faced the unknown with quiet determination!";
            }
        } else if (current->scenarioID == 1001) { // Strategist path
            bool hasGoodStats = (wolf.health >= 50 && wolf.spirit >= 50 && pack.getSize() >= 2);
            if (hasGoodStats) {
                qDebug() << "ACHIEVEMENT UNLOCKED: Wise Strategist - Found wisdom in thoughtful reflection!";
            } else {
                qDebug() << "ACHIEVEMENT UNLOCKED: Accepting Spirit - Found peace through quiet acceptance!";
            }
        }

        // Show ending message with clear conclusion
        QString endingTitle = "🎯 Journey Complete";
        
        // Build detailed ending summary
        QString endingSummary = QString::fromStdString(current->endingText);
        endingSummary += QString("\n\n📊 Final Statistics:\n"
                                "Days Survived: %1\n"
                                "Health: %2\n"
                                "Hunger: %3\n"
                                "Energy: %4\n"
                                "Spirit: %5\n"
                                "Reputation: %6\n"
                                "Pack Size: %7")
                           .arg(dayCounter)
                           .arg(wolf.health)
                           .arg(wolf.hunger)
                           .arg(wolf.energy)
                           .arg(wolf.spirit)
                           .arg(wolf.reputation)
                           .arg(pack.getSize());
        
        QMessageBox::information(this, endingTitle, endingSummary);
        
        // Update status bar
        if (statusBar) {
            statusBar->showMessage("Game Complete! Choose 'New Game' or 'Exit Game'", 0);
        }
    } else {
        bool canChooseA = wolf.canMakeChoice(5);
        bool canChooseB = wolf.canMakeChoice(3);

        buttonA->setText(QString::fromStdString("A: " + current->choiceA_text));
        buttonB->setText(QString::fromStdString("B: " + current->choiceB_text));

        buttonA->setEnabled(canChooseA);
        buttonB->setEnabled(canChooseB);

        // Update wolf graphic based on scenario
        updateWolfGraphic(current->scenarioID);
    }
}

void GameWindow::processChoice(bool isA) {
    qDebug() << "processChoice start";
    DecisionNode* current = tree.getCurrentNode();
    qDebug() << "current node:" << current;
    if (!current) {
        QMessageBox::warning(this, "Error", "No current scenario available.");
        return;
    }

    // Check if this is already an ending node
    if (current->isEnding) {
        return; // Don't allow choices on ending nodes
    }

    // Check if the wolf can make this choice
    int energyReq = isA ? 5 : 3; // Choice A requires 5 energy, Choice B requires 3
    if (!wolf.canMakeChoice(energyReq)) {
        QString errorMsg = "Cannot make this choice - insufficient health, too hungry, or not enough energy!";
        QMessageBox::warning(this, "Invalid Choice", errorMsg);
        return; // Don't allow the choice
    }

    // Push current state before choice (including day and a copy of inventory)
    history.push({current, wolf.health, wolf.hunger, wolf.energy, dayCounter, inventory.clone()});

    DecisionNode* nextNode = nullptr;
    if (isA) {
        nextNode = current->left;
        qDebug() << "chose A, next:" << nextNode;
    } else {
        nextNode = current->right;
        qDebug() << "chose B, next:" << nextNode;
    }

    // Safety check for null next node
    if (!nextNode) {
        qDebug() << "creating fallback node";
        // Create a conclusion node for unexpected paths
        nextNode = new DecisionNode();
        nextNode->scenarioID = 999;
        nextNode->description = "Your story reaches its conclusion through unforeseen circumstances. The wilderness has presented challenges that bring your journey to an end.";
        nextNode->choiceA_text = "Reflect on your journey's discoveries";
        nextNode->choiceB_text = "Accept the wisdom of this outcome";
        nextNode->isEnding = false; // Not an ending - allow final choices

        // Determine ending tone based on game state
        bool hasGoodStats = (wolf.health >= 50 && wolf.spirit >= 50 && pack.getSize() >= 2);

        // Create conclusion paths with state-based endings
        DecisionNode* exploreNode = new DecisionNode();
        exploreNode->scenarioID = 1000;
        exploreNode->description = "🎯 FINAL OUTCOME\n\nYour journey concludes with a spirit of exploration.";
        exploreNode->isEnding = true;

        DecisionNode* restNode = new DecisionNode();
        restNode->scenarioID = 1001;
        restNode->description = "🎯 FINAL OUTCOME\n\nYour journey concludes with thoughtful reflection.";
        restNode->isEnding = true;

        // Set ending messages based on game state
        if (hasGoodStats) {
            // Positive endings for good stats
            exploreNode->endingText = "PATH OF DISCOVERY: Despite the unexpected conclusion to your journey, your spirit of exploration and the strength of your pack ensure your legacy will inspire future generations of wolves. You faced the unknown with courage and adaptability.\n\nDays Survived: " + std::to_string(dayCounter) + "\nFinal Pack Size: " + std::to_string(pack.getSize()) + "\n\n🏆 Achievement Unlocked: Resilient Explorer";

            restNode->endingText = "PATH OF WISDOM: Through thoughtful reflection, you found peace and understanding in the face of uncertainty. Your wisdom and the loyalty of your pack created a foundation that will endure beyond this journey.\n\nDays Survived: " + std::to_string(dayCounter) + "\nFinal Pack Size: " + std::to_string(pack.getSize()) + "\n\n🏆 Achievement Unlocked: Wise Strategist";
        } else {
            // Tragic endings for poor stats
            exploreNode->endingText = "BITTERSWEET CONCLUSION: The wilderness proved unforgiving, and your journey ended sooner than hoped. Yet your spirit of exploration endured to the end, facing the unknown with determination despite the hardships.\n\nDays Survived: " + std::to_string(dayCounter) + "\nFinal Pack Size: " + std::to_string(pack.getSize()) + "\n\n🏆 Achievement Unlocked: Stoic Explorer";

            restNode->endingText = "QUIET ACCEPTANCE: In the face of overwhelming challenges, you found a measure of peace through acceptance. Though the journey was difficult, your thoughtful nature brought some solace in reflection.\n\nDays Survived: " + std::to_string(dayCounter) + "\nFinal Pack Size: " + std::to_string(pack.getSize()) + "\n\n🏆 Achievement Unlocked: Accepting Spirit";
        }

        nextNode->left = exploreNode;
        nextNode->right = restNode;

        // Track fallback nodes for proper cleanup
        fallbackNodes.push_back(nextNode);
    }

    // Apply stat changes for the choice
    wolf.energy -= energyReq;
    wolf.hunger += 5;  // Hunger increases over time
    wolf.health -= 2;  // Health decreases from exertion

    // Ensure stat bounds
    wolf.energy = qMax(0, qMin(100, wolf.energy));
    wolf.hunger = qMax(0, qMin(100, wolf.hunger));
    wolf.health = qMax(0, qMin(100, wolf.health));
    wolf.spirit = qMax(0, qMin(100, wolf.spirit));

    // Check for death conditions
    if (!wolf.isAlive()) {
        updateStats(); // This will handle the death screen properly
        return;
    }

    // Set the new current node
    tree.setCurrentNode(nextNode);
    dayCounter++;

    // Check for day limit
    if (dayCounter >= 30) {
        updateStats(); // This will handle the victory screen properly
        return;
    }

    // Check basic survival achievements
    checkBasicAchievements();

    // Update display
    updateDisplay();
    updateStats();

    // Update sidebar information after making a choice
    updateInventoryDisplay();
    updatePackDisplaySidebar();
    updateGameInfo();
}

void GameWindow::checkEvents() {
    // Check for random events
    if (dis(gen) < 0.1) { // 10% chance of event (less frequent)
        if (!events.isEmpty()) {
            Event e = events.extractMin();
            std::string msg = e.name + ": " + e.description;

            // Instead of a popup, update the story label with event info
            QString currentText = storyLabel->text();
            storyLabel->setText(currentText + "\n\n[EVENT] " + QString::fromStdString(msg));

            // Apply the event effect to the wolf
            e.effect(wolf);

            // Add pack member for recruitment event
            if (e.name == "Pack Recruitment" && pack.getSize() < 5) { // Max 5 members
                // Randomly select a role and name for the new pack member
                std::string roles[] = {"Hunter", "Scout", "Guard"};
                std::string names[] = {"Fenris", "Lupin", "Canis", "Wolfe", "Dire"};
                int roleIdx = gen() % 3;
                int nameIdx = gen() % 5;
                int loyalty = 50 + (gen() % 30); // Loyalty between 50-80
                pack.addMember(names[nameIdx] + std::to_string(gen() % 100), roles[roleIdx], loyalty);

                // Update the story label to indicate pack recruitment
                QString recruitmentMsg = QString("\n\n[RECRUITMENT] %1 has joined your pack as a %2!")
                                           .arg(QString::fromStdString(names[nameIdx] + std::to_string(gen() % 100)))
                                           .arg(QString::fromStdString(roles[roleIdx]));
                storyLabel->setText(currentText + recruitmentMsg);
            }

            // Put the event back in the queue with reset priority
            events.insert(e);
        }
    }

    // Update pack loyalty based on hunger increase (simulated)
    // In GUI mode, we'll update loyalty periodically
    static int hungerIncreaseCounter = 0;
    hungerIncreaseCounter++;
    if (hungerIncreaseCounter >= 3) { // Update every few game cycles
        pack.updateLoyalty(5); // Simulate hunger increase
        hungerIncreaseCounter = 0;
    }
}

void GameWindow::updateWolfGraphic(int scenarioId) {
    QString wolfArt;
    QString bgColor;
    
    // Different wolf graphics based on scenario/health/pack size
    if (wolf.health < 30) {
        // Injured wolf
        wolfArt = "🩸🐺\nInjured\nWolf";
        bgColor = "qlineargradient(x1:0, y1:0, x2:1, y2:1, stop:0 #e74c3c, stop:1 #c0392b)";
    } else if (pack.getSize() > 2) {
        // Pack leader
        wolfArt = "👑🐺\nAlpha\nWolf";
        bgColor = "qlineargradient(x1:0, y1:0, x2:1, y2:1, stop:0 #f39c12, stop:1 #e67e22)";
    } else if (wolf.hunger > 70) {
        // Hungry wolf
        wolfArt = "😋🐺\nHungry\nWolf";
        bgColor = "qlineargradient(x1:0, y1:0, x2:1, y2:1, stop:0 #e67e22, stop:1 #d35400)";
    } else if (wolf.energy < 30) {
        // Tired wolf
        wolfArt = "😴🐺\nTired\nWolf";
        bgColor = "qlineargradient(x1:0, y1:0, x2:1, y2:1, stop:0 #95a5a6, stop:1 #7f8c8d)";
    } else {
        // Healthy wolf
        wolfArt = "🐺\nHealthy\nWolf";
        bgColor = "qlineargradient(x1:0, y1:0, x2:1, y2:1, stop:0 #27ae60, stop:1 #2ecc71)";
    }
    
    wolfGraphic->setText(wolfArt);
    wolfGraphic->setStyleSheet(QString(R"(
        background: %1;
        border: 3px solid #1e8449;
        border-radius: 15px;
        color: white;
        font-weight: bold;
        font-size: 20px;
    )").arg(bgColor));
}

void GameWindow::updateDayDisplay() {
    QString moonPhase;
    switch (dayCounter % 8) {
        case 0: moonPhase = "🌑"; break; // New moon
        case 1: moonPhase = "🌒"; break; // Waxing crescent
        case 2: moonPhase = "🌓"; break; // First quarter
        case 3: moonPhase = "🌔"; break; // Waxing gibbous
        case 4: moonPhase = "🌕"; break; // Full moon
        case 5: moonPhase = "🌖"; break; // Waning gibbous
        case 6: moonPhase = "🌗"; break; // Last quarter
        case 7: moonPhase = "🌘"; break; // Waning crescent
    }
    
    dayLabel->setText(QString("%1 Day %2").arg(moonPhase).arg(dayCounter));
}

void GameWindow::updatePackDisplay() {
    QString packText = QString("🐺 Pack: %1 members").arg(pack.getSize());
    if (pack.getSize() > 0) {
        packText += "\n";
        PackMember* current = pack.getHead();
        int count = 0;
        while (current && count < 3) { // Show first 3 members
            QString roleIcon;
            if (current->role == "Hunter") roleIcon = "🏹";
            else if (current->role == "Scout") roleIcon = "👁️";
            else if (current->role == "Guard") roleIcon = "🛡️";
            
            packText += QString("%1%2 ").arg(roleIcon).arg(QString::fromStdString(current->name.substr(0, 6)));
            current = current->next;
            count++;
        }
        if (pack.getSize() > 3) {
            packText += "...";
        }
    }
    packLabel->setText(packText);
}

void GameWindow::checkBasicAchievements() {
    // Achievement checks based on TeamA.txt objectives
    // Note: Static variables persist across games, but achievements can only be unlocked once per session
    // For a full reset between games, call resetAchievements()

    // Survival milestones
    if (dayCounter >= 7) {
        qDebug() << "ACHIEVEMENT UNLOCKED: Week Survivor - Survived 7 days!";
    }
    if (dayCounter >= 14) {
        qDebug() << "ACHIEVEMENT UNLOCKED: Fortnight Fighter - Survived 14 days!";
    }
    if (dayCounter >= 21) {
        qDebug() << "ACHIEVEMENT UNLOCKED: Month Maker - Survived 21 days!";
    }

    // Stat mastery objectives (only unlock if not already achieved in this session)
    static bool healthAchieved = false, energyAchieved = false, spiritAchieved = false;
    static bool pack3Achieved = false, pack5Achieved = false, repAchieved = false;

    if (wolf.health >= 90 && !healthAchieved) {
        healthAchieved = true;
        qDebug() << "ACHIEVEMENT UNLOCKED: Health Master - Maintained 90+ health!";
    }
    if (wolf.energy >= 90 && !energyAchieved) {
        energyAchieved = true;
        qDebug() << "ACHIEVEMENT UNLOCKED: Energy Expert - Maintained 90+ energy!";
    }
    if (wolf.spirit >= 90 && !spiritAchieved) {
        spiritAchieved = true;
        qDebug() << "ACHIEVEMENT UNLOCKED: Spirit Sage - Reached 90+ spirit!";
    }

    // Pack building objectives
    if (pack.getSize() >= 3 && !pack3Achieved) {
        pack3Achieved = true;
        qDebug() << "ACHIEVEMENT UNLOCKED: Pack Builder - Grew pack to 3+ members!";
    }
    if (pack.getSize() >= 5 && !pack5Achieved) {
        pack5Achieved = true;
        qDebug() << "ACHIEVEMENT UNLOCKED: Pack Leader - Built pack of 5+ wolves!";
    }

    // Reputation objectives
    if (wolf.reputation >= 80 && !repAchieved) {
        repAchieved = true;
        qDebug() << "ACHIEVEMENT UNLOCKED: Reputation King - Earned 80+ reputation!";
    }

    // Advanced survival objectives (only check once)
    static bool survivor10Achieved = false, survivor15Achieved = false;
    static bool alphaAchieved = false;

    if (dayCounter >= 10 && wolf.health >= 70 && wolf.hunger <= 30 && !survivor10Achieved) {
        survivor10Achieved = true;
        qDebug() << "ACHIEVEMENT UNLOCKED: Elite Survivor - 10 days with high health and low hunger!";
    }
    if (dayCounter >= 15 && wolf.energy >= 70 && wolf.spirit >= 70 && !survivor15Achieved) {
        survivor15Achieved = true;
        qDebug() << "ACHIEVEMENT UNLOCKED: Master Survivor - 15 days with high energy and spirit!";
    }

    // Special combination objectives
    if (pack.getSize() >= 4 && wolf.reputation >= 70 && wolf.spirit >= 70 && !alphaAchieved) {
        alphaAchieved = true;
        qDebug() << "ACHIEVEMENT UNLOCKED: Alpha Leader - Strong pack, high reputation, and spirit!";
    }

    // Fallback ending achievements (tracked separately since they're one-time per session)
    static bool explorerAchieved = false, strategistAchieved = false;
    static bool stoicExplorerAchieved = false, acceptingSpiritAchieved = false;

    // These will be set when fallback endings are triggered
}

void GameWindow::cleanupFallbackNodes() {
    // Safely cleanup fallback nodes to prevent memory leaks and double frees
    // These nodes are created dynamically when the tree has null pointers
    for (auto node : fallbackNodes) {
        if (node) {
            // Only delete if this node is not part of the main tree
            // Fallback nodes (scenarioID >= 999) are always safe to delete
            if (node->scenarioID >= 999) {
                // Delete child nodes first (exploreNode, restNode)
                if (node->left && node->left->scenarioID >= 999) {
                    delete node->left;
                }
                if (node->right && node->right->scenarioID >= 999) {
                    delete node->right;
                }
                delete node;
            }
        }
    }
    fallbackNodes.clear();
    qDebug() << "cleanupFallbackNodes: cleaned up fallback nodes";
}