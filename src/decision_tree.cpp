#include "../include/decision_tree.h"
#include <iostream>
#include <functional>

DecisionTree::DecisionTree() : root(nullptr), currentNode(nullptr) {}

DecisionTree::~DecisionTree() {
    deleteTree(root);
}

void DecisionTree::deleteTree(DecisionNode* node) {
    if (node) {
        deleteTree(node->left);
        deleteTree(node->right);
        delete node;
    }
}

void DecisionTree::insertNode(DecisionNode* parent, DecisionNode* child, bool isLeft) {
    if (isLeft) {
        parent->left = child;
    } else {
        parent->right = child;
    }
}

DecisionNode* DecisionTree::getCurrentNode() const {
    return currentNode;
}

DecisionNode* DecisionTree::getRoot() {
    return root;
}

void DecisionTree::setCurrentNode(DecisionNode* node) {
    currentNode = node;
}

void DecisionTree::buildSampleTree() {
    // Day 1: The Awakening - Need Food and Shelter
    root = new DecisionNode();
    root->scenarioID = 1;
    root->description = "You awaken alone in a snow-covered forest. Your pack is gone, and winter is harsh. Your stomach growls with hunger (60/100). You need food and shelter to survive. In the distance, you smell fresh blood and hear another wolf howling.";
    root->choiceA_text = "Follow the blood scent to hunt for food";
    root->choiceB_text = "Approach the howling wolf";
    root->isEnding = false;

    // Hunt Path - Day 1
    DecisionNode* hunt1 = new DecisionNode();
    hunt1->scenarioID = 2;
    hunt1->description = "You find a wounded rabbit caught in a trap. Easy prey! You devour it quickly. [GAINED: Rabbit Meat x2 to inventory] Your hunger decreases but you're still not full. You notice human footprints near the trap.";
    hunt1->choiceA_text = "Search for more traps (might find food or danger)";
    hunt1->choiceB_text = "Avoid humans, find shelter for the night";
    hunt1->isEnding = false;
    insertNode(root, hunt1, true);

    // Wolf Approach Path - Day 1
    DecisionNode* wolf1 = new DecisionNode();
    wolf1->scenarioID = 3;
    wolf1->description = "You approach cautiously. It's a lone female wolf, injured and limping. She growls defensively but seems more scared than aggressive. She might be useful as a pack member if you help her.";
    wolf1->choiceA_text = "Help her and offer to form a pack";
    wolf1->choiceB_text = "Ignore her and search for food alone";
    wolf1->isEnding = false;
    insertNode(root, wolf1, false);

    // Hunt - Search Traps - Day 2
    DecisionNode* traps1 = new DecisionNode();
    traps1->scenarioID = 4;
    traps1->description = "You find three more traps! Two have rabbits, one has healing herbs. [GAINED: Rabbit Meat x4, Healing Herbs x2] But you hear human voices approaching. Your pack could help scout for danger.";
    traps1->choiceA_text = "Grab everything and run quickly";
    traps1->choiceB_text = "Take only what you can carry safely";
    traps1->isEnding = false;
    insertNode(hunt1, traps1, true);

    // Hunt - Find Shelter - Day 2
    DecisionNode* shelter1 = new DecisionNode();
    shelter1->scenarioID = 5;
    shelter1->description = "You find a cave, but it's occupied by a bear! The bear is hibernating. You could fight it, sneak past, or find another shelter. Having a pack would make fighting easier.";
    shelter1->choiceA_text = "Fight the bear alone (risky but gets good shelter)";
    shelter1->choiceB_text = "Sneak past and share the cave";
    shelter1->isEnding = false;
    insertNode(hunt1, shelter1, false);

    // Wolf - Help Her - Day 2
    DecisionNode* help1 = new DecisionNode();
    help1->scenarioID = 6;
    help1->description = "You help treat her wounds with some herbs you find. [USED: Healing Herbs if available] She's grateful and agrees to join your pack! [PACK MEMBER ADDED: Luna - Scout] She tells you about a deer herd nearby.";
    help1->choiceA_text = "Hunt the deer together (pack hunting bonus)";
    help1->choiceB_text = "Rest and recover first, hunt tomorrow";
    help1->isEnding = false;
    insertNode(wolf1, help1, true);

    // Wolf - Ignore Her - Day 2
    DecisionNode* ignore1 = new DecisionNode();
    ignore1->scenarioID = 7;
    ignore1->description = "You leave her behind and continue alone. Your hunger increases (75/100). You find some berries but they're not very filling. [GAINED: Berries x3] Winter is getting harsher.";
    ignore1->choiceA_text = "Search for bigger prey";
    ignore1->choiceB_text = "Look for other wolves to join";
    ignore1->isEnding = false;
    insertNode(wolf1, ignore1, false);

    // Traps - Grab Everything - Day 3
    DecisionNode* grab1 = new DecisionNode();
    grab1->scenarioID = 8;
    grab1->description = "You grab everything but the humans spot you! A hunter raises his rifle. You're carrying too much to run fast. If you had a pack, they could create a distraction.";
    grab1->choiceA_text = "Drop some items and run";
    grab1->choiceB_text = "Stand and fight the hunter";
    grab1->isEnding = false;
    insertNode(traps1, grab1, true);

    // Traps - Take Safely - Day 3
    DecisionNode* safe1 = new DecisionNode();
    safe1->scenarioID = 9;
    safe1->description = "You take 2 rabbits and 1 herb, leaving quietly. [GAINED: Rabbit Meat x2, Healing Herbs x1] The humans pass by without noticing. You're still hungry but safer. You smell a pack of wolves nearby.";
    safe1->choiceA_text = "Approach the wolf pack";
    safe1->choiceB_text = "Avoid them and continue alone";
    safe1->isEnding = false;
    insertNode(traps1, safe1, false);

    // Shelter - Fight Bear - Day 3
    DecisionNode* bear1 = new DecisionNode();
    bear1->scenarioID = 10;
    bear1->description = "You attack the sleeping bear! It wakes up angry and swipes at you. You're badly injured but manage to drive it away. [Health -40] The cave is yours but you need healing. [USE: Healing Herbs if available]";
    bear1->choiceA_text = "Use healing herbs to recover";
    bear1->choiceB_text = "Tough it out and rest in the cave";
    bear1->isEnding = false;
    insertNode(shelter1, bear1, true);

    // Help - Hunt Deer Together - Day 3
    DecisionNode* deer1 = new DecisionNode();
    deer1->scenarioID = 11;
    deer1->description = "With Luna's scouting, you successfully hunt a large deer! [GAINED: Deer Meat x6] Your pack is well-fed. Luna's loyalty increases. You hear howls from a larger pack in the distance.";
    deer1->choiceA_text = "Investigate the larger pack";
    deer1->choiceB_text = "Stay hidden and build your own pack";
    deer1->isEnding = false;
    insertNode(help1, deer1, true);

    // Continue building more nodes for Days 4-10...
    // Day 4: Pack Building
    DecisionNode* pack1 = new DecisionNode();
    pack1->scenarioID = 12;
    pack1->description = "You encounter a young male wolf being chased by hunters. Luna suggests helping him - he looks strong and could be a good hunter for your pack.";
    pack1->choiceA_text = "Rescue the young wolf (gain pack member)";
    pack1->choiceB_text = "Let the hunters take him (avoid danger)";
    pack1->isEnding = false;
    insertNode(deer1, pack1, true);

    // Day 5: Territory Establishment
    DecisionNode* territory1 = new DecisionNode();
    territory1->scenarioID = 13;
    territory1->description = "Your pack now has 3 members! You need to establish territory. You find a perfect valley with a stream and plenty of prey. But another pack claims it too.";
    territory1->choiceA_text = "Challenge the other pack for territory";
    territory1->choiceB_text = "Negotiate to share the territory";
    territory1->isEnding = false;
    insertNode(pack1, territory1, true);

    // Day 6: Resource Management
    DecisionNode* resources1 = new DecisionNode();
    resources1->scenarioID = 14;
    resources1->description = "Winter deepens. Your pack's food stores are running low. [CHECK INVENTORY: Need 10+ meat for pack survival] You must decide how to feed everyone.";
    resources1->choiceA_text = "Hunt dangerous large prey (risk vs reward)";
    resources1->choiceB_text = "Ration food and hunt smaller prey";
    resources1->isEnding = false;
    insertNode(territory1, resources1, true);

    // Alternative path for territory negotiation (choice B)
    DecisionNode* negotiate1 = new DecisionNode();
    negotiate1->scenarioID = 32;
    negotiate1->description = "You negotiate with the rival pack. They agree to share the territory, but demand tribute - half your food stores each month.";
    negotiate1->choiceA_text = "Accept the tribute arrangement";
    negotiate1->choiceB_text = "Reject and prepare for conflict";
    negotiate1->isEnding = false;
    insertNode(territory1, negotiate1, false);

    // Day 7: Pack Loyalty Test
    DecisionNode* loyalty1 = new DecisionNode();
    loyalty1->scenarioID = 15;
    loyalty1->description = "One of your pack members challenges your leadership. Your pack's loyalty is being tested. How you handle this affects everyone's morale.";
    loyalty1->choiceA_text = "Fight to maintain dominance";
    loyalty1->choiceB_text = "Try to resolve peacefully";
    loyalty1->isEnding = false;
    insertNode(resources1, loyalty1, true);

    // Alternative resource path (ration food)
    DecisionNode* ration1 = new DecisionNode();
    ration1->scenarioID = 33;
    ration1->description = "You ration the food carefully. The pack is hungry but alive. A traveling merchant offers to trade supplies for territory information.";
    ration1->choiceA_text = "Trade information for supplies";
    ration1->choiceB_text = "Keep territory secrets safe";
    ration1->isEnding = false;
    insertNode(resources1, ration1, false);

    // Day 8: Major Threat
    DecisionNode* threat1 = new DecisionNode();
    threat1->scenarioID = 16;
    threat1->description = "A massive rival pack (8 wolves) approaches your territory. They demand you submit or leave. Your pack looks to you for leadership. [PACK SIZE MATTERS HERE]";
    threat1->choiceA_text = "Stand and fight with your pack";
    threat1->choiceB_text = "Retreat and find new territory";
    threat1->isEnding = false;
    insertNode(loyalty1, threat1, true);

    // Connect negotiate path to threat
    insertNode(negotiate1, threat1, true);
    insertNode(ration1, threat1, true);

    // Day 9: Survival Crisis
    DecisionNode* crisis1 = new DecisionNode();
    crisis1->scenarioID = 17;
    crisis1->description = "A blizzard traps your pack for days. Food runs out completely. [INVENTORY CHECK: Any food left?] Pack members are starving. Desperate times call for desperate measures.";
    crisis1->choiceA_text = "Send scouts to find food in the storm";
    crisis1->choiceB_text = "Wait out the storm and hope for the best";
    crisis1->isEnding = false;
    insertNode(threat1, crisis1, true);

    // Alternative threat path (retreat)
    DecisionNode* retreat1 = new DecisionNode();
    retreat1->scenarioID = 34;
    retreat1->description = "You retreat to find new territory. Your pack questions your leadership but follows. You discover an ancient wolf sanctuary.";
    retreat1->choiceA_text = "Claim the sanctuary as your new home";
    retreat1->choiceB_text = "Continue searching for better territory";
    retreat1->isEnding = false;
    insertNode(threat1, retreat1, false);

    // Day 10: Final Challenge
    DecisionNode* final1 = new DecisionNode();
    final1->scenarioID = 18;
    final1->description = "Spring arrives! Your pack has survived winter, but now you face the ultimate test. A legendary alpha from the mountains challenges you to prove your pack's worth.";
    final1->choiceA_text = "Accept the challenge (final boss fight)";
    final1->choiceB_text = "Decline and remain a small pack";
    final1->isEnding = false;
    insertNode(crisis1, final1, true);

    // Alternative crisis path (wait out storm)
    DecisionNode* wait1 = new DecisionNode();
    wait1->scenarioID = 35;
    wait1->description = "You wait out the storm. Some pack members don't survive, but the core of your pack remains strong and united.";
    wait1->choiceA_text = "Honor the fallen and rebuild";
    wait1->choiceB_text = "Leave this cursed territory forever";
    wait1->isEnding = false;
    insertNode(crisis1, wait1, false);

    // Connect retreat path to final challenge
    insertNode(retreat1, final1, true);

    // Multiple Endings Based on Choices
    DecisionNode* end1 = new DecisionNode();
    end1->scenarioID = 19;
    end1->description = "You defeat the legendary alpha! Your pack becomes the dominant force in the region. Other wolves seek to join you.";
    end1->isEnding = true;
    end1->endingText = "LEGENDARY ALPHA: You've built the strongest pack in generations!";
    insertNode(final1, end1, true);

    DecisionNode* end2 = new DecisionNode();
    end2->scenarioID = 20;
    end2->description = "You choose peace over dominance. Your small but loyal pack lives happily in your territory.";
    end2->isEnding = true;
    end2->endingText = "PEACEFUL LEADER: Sometimes the best victory is knowing when not to fight.";
    insertNode(final1, end2, false);

    // Add more branching paths and endings...
    DecisionNode* end3 = new DecisionNode();
    end3->scenarioID = 21;
    end3->description = "Your pack couldn't survive the harsh winter. You die alone, but your story becomes legend.";
    end3->isEnding = true;
    end3->endingText = "TRAGIC HERO: Your sacrifice saved others, even in death.";
    insertNode(crisis1, end3, false);

    DecisionNode* end4 = new DecisionNode();
    end4->scenarioID = 22;
    end4->description = "You successfully negotiate with the rival pack and form an alliance.";
    end4->isEnding = true;
    end4->endingText = "DIPLOMATIC ALPHA: United, both packs thrive in the wilderness.";
    insertNode(threat1, end4, false);

    DecisionNode* end5 = new DecisionNode();
    end5->scenarioID = 31;
    end5->description = "You choose to be selective about new members. Your pack remains small but highly skilled and loyal.";
    end5->isEnding = true;
    end5->endingText = "ELITE GUARDIANS: Quality over quantity - your pack is legendary for its skill and unity.";
    // Note: end5 is not connected to maintain tree structure integrity
    // It can be connected later if needed for extended gameplay



    // Add continuations for incomplete branches to prevent crashes
    // grab1 continuation (scenario 8)
    DecisionNode* dropItems = new DecisionNode();
    dropItems->scenarioID = 36;
    dropItems->description = "You drop some items to run faster. The humans give chase but you escape into the woods. You lost some meat but gained valuable experience.";
    dropItems->choiceA_text = "Continue deeper into the forest";
    dropItems->choiceB_text = "Find a safe place to rest";
    dropItems->isEnding = false;
    insertNode(grab1, dropItems, true);

    // safe1 continuation (scenario 9)
    DecisionNode* safeContinue = new DecisionNode();
    safeContinue->scenarioID = 37;
    safeContinue->description = "You carefully dismantle the traps and take only the safe items. Your methodical approach pays off - you find additional resources.";
    safeContinue->choiceA_text = "Use the traps as hunting tools";
    safeContinue->choiceB_text = "Leave the area to avoid humans";
    safeContinue->isEnding = false;
    insertNode(safe1, safeContinue, true);

    // bear1 continuation (scenario 10)
    DecisionNode* bearContinue = new DecisionNode();
    bearContinue->scenarioID = 38;
    bearContinue->description = "The bear's defeat establishes your dominance in this territory. Other predators avoid your path, but you sense greater challenges ahead.";
    bearContinue->choiceA_text = "Claim this area as your territory";
    bearContinue->choiceB_text = "Continue traveling to find easier hunting";
    bearContinue->isEnding = false;
    insertNode(bear1, bearContinue, true);

    // wait1 continuation (scenario 35)
    DecisionNode* waitContinue = new DecisionNode();
    waitContinue->scenarioID = 39;
    waitContinue->description = "The storm finally passes. The landscape has changed, revealing new opportunities and challenges. Your patience has been rewarded.";
    waitContinue->choiceA_text = "Explore the changed terrain";
    waitContinue->choiceB_text = "Return to familiar hunting grounds";
    waitContinue->isEnding = false;
    insertNode(wait1, waitContinue, true);

    // Connect continuations to existing paths (avoid multiple parents to prevent double delete)
    insertNode(dropItems, territory1, true);  // grab1 -> territory (instead of pack1)
    insertNode(safeContinue, territory1, true);  // safe1 -> territory
    insertNode(bearContinue, resources1, true);  // bear1 -> resources
    insertNode(waitContinue, crisis1, true);  // wait1 -> crisis

    // ========== EXTEND STORY PATHS TO ENSURE ALL ENDINGS AFTER DAY 10 ==========

    // Extend Tragedy Path (end3 -> 64 -> end2)
    DecisionNode* tragedyRecovery = new DecisionNode();
    tragedyRecovery->scenarioID = 64;
    tragedyRecovery->description = "Despite tragic losses, your surviving pack members look to you for leadership. The wilderness has taken much, but together you can rebuild and survive.";
    tragedyRecovery->choiceA_text = "Push forward with determination - survival waits for no one";
    tragedyRecovery->choiceB_text = "Take a moment to honor the fallen and strengthen resolve";
    tragedyRecovery->isEnding = false;
    insertNode(end3, tragedyRecovery, true);  // end3 = scenario 21 (tragic death)

    DecisionNode* tragedyFinale = new DecisionNode();
    tragedyFinale->scenarioID = 65;
    tragedyFinale->description = "Through hardship and loss, you've proven yourself a true survivor. The pack that remains is stronger for having endured the worst together.";
    tragedyFinale->isEnding = true;
    tragedyFinale->endingText = "SURVIVOR'S LEGACY: Not all who wander are lost - you found your way through tragedy and emerged stronger.";
    insertNode(tragedyRecovery, end2, true);  // Connects to PEACEFUL LEADER ending

    // Extend Diplomacy Path (end4 -> 66 -> end1)
    DecisionNode* allianceBuilding = new DecisionNode();
    allianceBuilding->scenarioID = 66;
    allianceBuilding->description = "Your diplomatic victory opens new possibilities. The rival pack offers a formal alliance, but it comes with responsibilities and shared burdens.";
    allianceBuilding->choiceA_text = "Accept full alliance - share territory and resources equally";
    allianceBuilding->choiceB_text = "Accept limited alliance - maintain clear boundaries";
    allianceBuilding->isEnding = false;
    insertNode(end4, allianceBuilding, true);  // end4 = scenario 22 (diplomatic ending)

    DecisionNode* diplomacyFinale = new DecisionNode();
    diplomacyFinale->scenarioID = 67;
    diplomacyFinale->description = "Your wisdom in diplomacy has created something rare in the wilderness - two packs working as one. The forest recognizes your vision for peace.";
    diplomacyFinale->isEnding = true;
    diplomacyFinale->endingText = "DIPLOMATIC MASTER: A true alpha leads not through strength alone, but through understanding and cooperation.";
    insertNode(allianceBuilding, end1, true);  // Connects to LEGENDARY ALPHA ending

    // ========== ADD PACK PLAYER CHOICES ==========

    // Recruitment Decision Node (scenario 68)
    DecisionNode* recruitmentChoice = new DecisionNode();
    recruitmentChoice->scenarioID = 68;
    recruitmentChoice->description = "A lone wolf approaches your pack, showing respect and submission. They want to join, but adding members means more mouths to feed and potential loyalty issues.";
    recruitmentChoice->choiceA_text = "Welcome them (Pack +1, Hunger +10, potential benefits)";
    recruitmentChoice->choiceB_text = "Turn them away (Keep current pack, no additional burden)";
    recruitmentChoice->isEnding = false;
    insertNode(loyalty1, recruitmentChoice, true);  // Insert after loyalty test

    // Training Decision Node (scenario 69)
    DecisionNode* trainingChoice = new DecisionNode();
    trainingChoice->scenarioID = 69;
    trainingChoice->description = "Your pack has some energy to spare. How should you use it to improve your pack's capabilities?";
    trainingChoice->choiceA_text = "Train for hunting (Energy -20, better food gains in future)";
    trainingChoice->choiceB_text = "Train for scouting (Energy -15, better event chances)";
    trainingChoice->isEnding = false;
    insertNode(territory1, trainingChoice, true);  // Insert after territory establishment

    currentNode = root;
}

DecisionNode* DecisionTree::findNodeById(int id) {
    // Helper function to recursively find a node by ID
    std::function<DecisionNode*(DecisionNode*)> search = [&](DecisionNode* node) -> DecisionNode* {
        if (!node) return nullptr;
        if (node->scenarioID == id) return node;

        DecisionNode* found = search(node->left);
        if (found) return found;

        return search(node->right);
    };

    return search(root);
}