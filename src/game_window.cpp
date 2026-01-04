#include "../include/game_window.h"
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMessageBox>
#include <QtGui/QPainter>
#include <QtGui/QPainterPath>
#include <QtGui/QLinearGradient>
#include <iostream>
#include <fstream>
#include "../include/inventory.h"

// Forward declaration
int calculatePackBenefits(const Pack& pack);

// StatBar implementation
StatBar::StatBar(QWidget* parent) : QWidget(parent), currentValue(50) {
    setMinimumSize(100, 20);
    setMaximumHeight(20);

    animation = new QPropertyAnimation(this, "value");
    animation->setDuration(500);
    connect(animation, &QPropertyAnimation::valueChanged, this, [this]() {
        update();
    });
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

    // Draw background with rounded corners
    QPainterPath backgroundPath;
    backgroundPath.addRoundedRect(rect(), 8, 8);
    painter.fillPath(backgroundPath, QColor(105, 105, 105)); // Darker gray for better contrast

    // Calculate fill width based on current value (0-100)
    int fillWidth = (currentValue * width()) / 100;

    // Create a smaller rectangle for the filled portion to account for padding
    QRect fillRect = rect().adjusted(3, 3, -3, -3);
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
        QLinearGradient gradient(0, 0, 0, height());
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

    // Draw border
    painter.setPen(QColor(50, 50, 50)); // Darker border for better contrast
    painter.drawRoundedRect(rect().adjusted(0, 0, -1, -1), 8, 8);

    // Draw value text in the center if there's enough space
    if (width() > 60) { // Only draw text if the bar is wide enough
        painter.setPen(QColor(255, 255, 255)); // White text
        painter.setFont(QFont("Arial", 8, QFont::Bold));
        QString valueText = QString("%1%").arg(currentValue);
        painter.drawText(rect(), Qt::AlignCenter, valueText);
    }

    QWidget::paintEvent(event);
}

GameWindow::GameWindow(Wolf& w, DecisionTree& t, PriorityQueue& e, GameStack& h, ActionQueue& a, Inventory& i, Pack& p, int& d, QWidget* parent)
    : QWidget(parent), wolf(w), tree(t), events(e), history(h), actions(a), inventory(i), pack(p), dayCounter(d),
      rd(), gen(rd()), dis(0.0, 1.0) {
    
    setWindowTitle("🐺 Wolf Pack Survival Adventure");
    setMinimumSize(1000, 700);
    
    // Create main layout
    QVBoxLayout* mainLayout = new QVBoxLayout(this);

    // Enhanced styling with wolf theme
    setStyleSheet(R"(
        QWidget {
            background: qlineargradient(x1:0, y1:0, x2:0, y2:1,
                stop:0 #2c3e50, stop:0.5 #34495e, stop:1 #2c3e50);
            color: #ecf0f1;
            font-family: 'Segoe UI', Arial, sans-serif;
        }
        QLabel {
            color: #ecf0f1;
            font-weight: bold;
        }
        QPushButton {
            background: qlineargradient(x1:0, y1:0, x2:0, y2:1,
                stop:0 #3498db, stop:1 #2980b9);
            border: 2px solid #2980b9;
            border-radius: 8px;
            color: white;
            font-weight: bold;
            padding: 8px 16px;
            min-height: 30px;
        }
        QPushButton:hover {
            background: qlineargradient(x1:0, y1:0, x2:0, y2:1,
                stop:0 #5dade2, stop:1 #3498db);
        }
        QPushButton:pressed {
            background: qlineargradient(x1:0, y1:0, x2:0, y2:1,
                stop:0 #2980b9, stop:1 #21618c);
        }
        QTextEdit {
            background-color: rgba(44, 62, 80, 0.8);
            border: 2px solid #34495e;
            border-radius: 10px;
            color: #ecf0f1;
            font-size: 14px;
            padding: 10px;
        }
    )");

    // Create title with wolf ASCII art
    QLabel* titleLabel = new QLabel(this);
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
    dayLabel = new QLabel(this);
    updateDayDisplay();
    dayLabel->setStyleSheet("font-size: 18px; font-weight: bold; color: #f1c40f; padding: 10px; border: 2px solid #f39c12; border-radius: 8px;");
    topInfoLayout->addWidget(dayLabel);
    
    // Pack size indicator
    packLabel = new QLabel(this);
    updatePackDisplay();
    packLabel->setStyleSheet("font-size: 16px; font-weight: bold; color: #e74c3c; padding: 10px; border: 2px solid #c0392b; border-radius: 8px;");
    topInfoLayout->addWidget(packLabel);
    
    mainLayout->addLayout(topInfoLayout);
    
    // Create stat bars with enhanced styling
    QHBoxLayout* barsLayout = new QHBoxLayout();
    
    // Health bar with heart icon
    QVBoxLayout* healthLayout = new QVBoxLayout();
    QLabel* healthLabel = new QLabel("❤️ Health", this);
    healthLabel->setStyleSheet("color: #e74c3c; font-weight: bold;");
    healthBar = new StatBar(this);
    healthLayout->addWidget(healthLabel);
    healthLayout->addWidget(healthBar);
    barsLayout->addLayout(healthLayout);
    
    // Hunger bar with food icon
    QVBoxLayout* hungerLayout = new QVBoxLayout();
    QLabel* hungerLabel = new QLabel("🍖 Hunger", this);
    hungerLabel->setStyleSheet("color: #e67e22; font-weight: bold;");
    hungerBar = new StatBar(this);
    hungerLayout->addWidget(hungerLabel);
    hungerLayout->addWidget(hungerBar);
    barsLayout->addLayout(hungerLayout);
    
    // Energy bar with lightning icon
    QVBoxLayout* energyLayout = new QVBoxLayout();
    QLabel* energyLabel = new QLabel("⚡ Energy", this);
    energyLabel->setStyleSheet("color: #f1c40f; font-weight: bold;");
    energyBar = new StatBar(this);
    energyLayout->addWidget(energyLabel);
    energyLayout->addWidget(energyBar);
    barsLayout->addLayout(energyLayout);
    
    // Spirit bar with star icon
    QVBoxLayout* spiritLayout = new QVBoxLayout();
    QLabel* spiritLabel = new QLabel("⭐ Spirit", this);
    spiritLabel->setStyleSheet("color: #9b59b6; font-weight: bold;");
    spiritBar = new StatBar(this);
    spiritLayout->addWidget(spiritLabel);
    spiritLayout->addWidget(spiritBar);
    barsLayout->addLayout(spiritLayout);
    
    mainLayout->addLayout(barsLayout);
    
    // Create enhanced story display area with wolf graphic
    QHBoxLayout* storyLayout = new QHBoxLayout();
    
    // Animated wolf graphic area
    wolfGraphic = new QLabel(this);
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
    
    // Story text area
    storyLabel = new QTextEdit(this);
    storyLabel->setReadOnly(true);
    storyLabel->setMinimumHeight(200);
    storyLabel->setStyleSheet(R"(
        QTextEdit {
            background: qlineargradient(x1:0, y1:0, x2:0, y2:1,
                stop:0 rgba(52, 73, 94, 0.9), stop:1 rgba(44, 62, 80, 0.9));
            border: 2px solid #34495e;
            border-radius: 10px;
            color: #ecf0f1;
            font-size: 14px;
            padding: 15px;
            line-height: 1.4;
        }
    )");
    
    storyLayout->addWidget(wolfGraphic);
    storyLayout->addWidget(storyLabel, 2); // Give story area more space
    mainLayout->addLayout(storyLayout);
    
    // Enhanced choice buttons
    QHBoxLayout* choiceLayout = new QHBoxLayout();
    
    buttonA = new QPushButton("Choice A", this);
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
    
    buttonB = new QPushButton("Choice B", this);
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
    mainLayout->addLayout(choiceLayout);
    
    // Enhanced control buttons
    QHBoxLayout* controlLayout = new QHBoxLayout();
    
    undoButton = new QPushButton("↶ Undo", this);
    saveButton = new QPushButton("💾 Save", this);
    loadButton = new QPushButton("📁 Load", this);
    inventoryButton = new QPushButton("🎒 Inventory", this);
    packButton = new QPushButton("🐺 Pack", this);
    
    // Style control buttons
    QString controlButtonStyle = 
        "QPushButton {"
            "background: qlineargradient(x1:0, y1:0, x2:0, y2:1,"
                "stop:0 #95a5a6, stop:1 #7f8c8d);"
            "border: 2px solid #6c7b7d;"
            "border-radius: 8px;"
            "color: white;"
            "font-weight: bold;"
            "padding: 8px 12px;"
            "min-width: 80px;"
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
    
    controlLayout->addWidget(undoButton);
    controlLayout->addWidget(saveButton);
    controlLayout->addWidget(loadButton);
    controlLayout->addWidget(inventoryButton);
    controlLayout->addWidget(packButton);
    mainLayout->addLayout(controlLayout);
    
    // Initialize display
    updateDisplay();
}

void GameWindow::onChoiceA() {
    processChoice(true);
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
        QMessageBox::critical(this, "Game Over", "Your wolf has died! Game Over.");
        QTimer::singleShot(0, this, &QWidget::close); // Close the window after showing the message
    }

    // Check for win condition (survive 30 days)
    if (dayCounter >= 30) {
        QString winMessage = QString("Congratulations! You survived 30 days in the wilderness!\nFinal pack size: %1").arg(pack.getSize());
        QMessageBox::information(this, "Victory!", winMessage);
        QTimer::singleShot(0, this, &QWidget::close); // Close the window after showing the message
    }

    // Check for events
    checkEvents();
}

void GameWindow::updateDisplay() {
    DecisionNode* current = tree.getCurrentNode();
    if (current) {
        storyLabel->setText(QString::fromStdString(current->description));

        // Update wolf graphic based on scenario
        updateWolfGraphic(current->scenarioID);

        if (current->isEnding) {
            buttonA->setEnabled(false);
            buttonB->setEnabled(false);
            buttonA->setText("Choice A");
            buttonB->setText("Choice B");
            storyLabel->setHtml(storyLabel->toHtml() + "\n\n" + QString::fromStdString(current->endingText));
        } else {
            // Check if the wolf can make each choice
            bool canChooseA = wolf.canMakeChoice(5); // Assume choice A requires 5 energy
            bool canChooseB = wolf.canMakeChoice(3); // Assume choice B requires 3 energy

            buttonA->setText(QString::fromStdString("A: " + current->choiceA_text));
            buttonB->setText(QString::fromStdString("B: " + current->choiceB_text));

            buttonA->setEnabled(canChooseA);
            buttonB->setEnabled(canChooseB);

            // Style disabled buttons differently
            if (canChooseA) {
                buttonA->setStyleSheet("QPushButton { background-color: #4CAF50; color: white; padding: 8px; border: none; border-radius: 5px; font-size: 14px; }"
                                      "QPushButton:hover { background-color: #45a049; }"
                                      "QPushButton:pressed { background-color: #3d8b40; }");
            } else {
                buttonA->setStyleSheet("QPushButton { background-color: #cccccc; color: #666666; padding: 8px; border: none; border-radius: 5px; font-size: 14px; }");
            }

            if (canChooseB) {
                buttonB->setStyleSheet("QPushButton { background-color: #2196F3; color: white; padding: 8px; border: none; border-radius: 5px; font-size: 14px; }"
                                      "QPushButton:hover { background-color: #1e88e5; }"
                                      "QPushButton:pressed { background-color: #1976d2; }");
            } else {
                buttonB->setStyleSheet("QPushButton { background-color: #cccccc; color: #666666; padding: 8px; border: none; border-radius: 5px; font-size: 14px; }");
            }
        }
    }
}

void GameWindow::processChoice(bool isA) {
    DecisionNode* current = tree.getCurrentNode();
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
    } else {
        nextNode = current->right;
    }

    // Safety check for null next node
    if (!nextNode) {
        // Create a continuation node instead of ending
        nextNode = new DecisionNode();
        nextNode->scenarioID = 999;
        nextNode->description = "Your journey takes an unexpected turn. The wilderness presents new challenges and opportunities ahead.";
        nextNode->choiceA_text = "Continue exploring the wilderness";
        nextNode->choiceB_text = "Rest and plan your next move";
        nextNode->isEnding = false; // Not an ending - allow continuation
        
        // Create simple continuation paths
        DecisionNode* exploreNode = new DecisionNode();
        exploreNode->scenarioID = 1000;
        exploreNode->description = "Your exploration leads to new adventures. The story continues...";
        exploreNode->isEnding = true;
        exploreNode->endingText = "EXPLORER: You chose to keep exploring the unknown wilderness.";
        
        DecisionNode* restNode = new DecisionNode();
        restNode->scenarioID = 1001;
        restNode->description = "You rest and gather strength for future challenges. The story continues...";
        restNode->isEnding = true;
        restNode->endingText = "STRATEGIST: You chose wisdom and preparation over hasty action.";
        
        nextNode->left = exploreNode;
        nextNode->right = restNode;
        
        // Connect it to current node
        if (isA) {
            current->left = nextNode;
        } else {
            current->right = nextNode;
        }
    }

    // Proceed with the choice
    tree.setCurrentNode(nextNode);
    
    // Apply stat changes
    wolf.updateHunger(5); // Hunger increases with each decision
    wolf.updateEnergy(-energyReq); // Energy decreases based on choice
    
    // Check for pack member additions based on story content
    QString description = QString::fromStdString(nextNode->description);
    if (description.contains("PACK MEMBER ADDED: Luna - Scout")) {
        pack.addMember("Luna", "Scout", 85);
    }
    if (description.contains("Fenrir") && description.contains("Hunter")) {
        pack.addMember("Fenrir", "Hunter", 90);
    }
    if (description.contains("gain pack member") || description.contains("Rescue the young wolf")) {
        pack.addMember("Fenrir", "Hunter", 80);
    }
    if (description.contains("Your pack now has 3 members")) {
        // Ensure we have at least 3 members
        if (pack.getSize() < 3) {
            pack.addMember("Luna", "Scout", 85);
            pack.addMember("Fenrir", "Hunter", 90);
            pack.addMember("Ash", "Guard", 75);
        }
    }
    
    // Check for inventory additions
    if (description.contains("GAINED: Rabbit Meat")) {
        inventory.addItem("Rabbit Meat", FOOD, 20, 2);
    }
    if (description.contains("GAINED: Deer Meat")) {
        inventory.addItem("Deer Meat", FOOD, 40, 6);
    }
    if (description.contains("GAINED: Territory Map")) {
        inventory.addItem("Territory Map", TOOL, 0, 1);
    }
    if (description.contains("GAINED: Wolf Scent Marker")) {
        inventory.addItem("Wolf Scent Marker", TOOL, 0, 1);
    }
    if (description.contains("GAINED: Healing Herbs")) {
        inventory.addItem("Healing Herbs", HERB, 30, 2);
    }
    
    // Increment day counter after each decision/action
    dayCounter++;

    // Add reputation changes based on scenario
    if (isA) {
        if (nextNode->scenarioID == 12) wolf.updateReputation(20); // Leading with strength
        if (nextNode->scenarioID == 9) wolf.updateReputation(-10); // Sparing traveler
    } else {
        if (nextNode->scenarioID == 12) wolf.updateReputation(10); // Leading with wisdom
        if (nextNode->scenarioID == 9) wolf.updateReputation(15); // Embracing shame
    }
    
    // Update display and stats
    updateDisplay();
    updateStats();
}

void GameWindow::checkEvents() {
    // Check for random events
    if (dis(gen) < 0.1) { // 10% chance of event (less frequent)
        if (!events.isEmpty()) {
            Event e = events.extractMin();
            std::string msg = e.name + ": " + e.description;

            // Instead of a popup, update the story label with event info
            QString currentText = storyLabel->toPlainText();
            storyLabel->setHtml(currentText + "\n\n[EVENT] " + QString::fromStdString(msg));

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
                storyLabel->setHtml(currentText + recruitmentMsg);
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