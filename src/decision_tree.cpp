#include "../include/decision_tree.h"
#include <iostream>
#include <functional>
#include <queue>
#include <set>

DecisionTree::DecisionTree() : root(nullptr), currentNode(nullptr) {}

DecisionTree::~DecisionTree() {
    deleteTree(root, deletedNodes);
    deletedNodes.clear();
}

void DecisionTree::reset() {
    deleteTree(root, deletedNodes);
    root = nullptr;
    currentNode = nullptr;
    deletedNodes.clear();
}

void DecisionTree::deleteTree(DecisionNode* node, std::unordered_set<DecisionNode*>& deleted) {
    if (!node || deleted.count(node) > 0) return;
    deleted.insert(node);
    // Recursively delete children first
    if (node->left && deleted.count(node->left) == 0) {
        deleteTree(node->left, deleted);
    }
    if (node->right && deleted.count(node->right) == 0) {
        deleteTree(node->right, deleted);
    }
    // Finally delete the current node
    delete node;
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
    // Default to classic story
    buildClassicStory();
}

void DecisionTree::buildClassicStory() {
    // Reset any existing tree before building new one
    reset();
    
    // Day 1: The Awakening - Need Food and Shelter
    root = new DecisionNode();
    root->scenarioID = 1;
    root->description = "You awaken alone in a snow-covered forest. Your pack is gone, and winter is harsh. Your throat is parched and stomach growls with hunger (60/100). You need food, water, and shelter to survive. In the distance, you smell fresh blood and hear another wolf howling.";
    root->choiceA_text = "Follow the blood scent to hunt for food";
    root->choiceB_text = "Approach the howling wolf";
    root->isEnding = false;

    // Hunt Path - Day 1
    DecisionNode* hunt1 = new DecisionNode();
    hunt1->scenarioID = 2;
    hunt1->description = "You find a wounded rabbit caught in a trap. Easy prey! You devour it quickly. [GAINED: Small Fish x2, Fresh Water x1] Your hunger and thirst decrease but you're still not full. You notice human footprints near the trap.";
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
    traps1->description = "You find three more traps! Two have fish, one has healing herbs. [GAINED: Small Fish x3, Winter Berries x2, Common Mallow x1] But you hear human voices approaching. Your pack could help scout for danger.";
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
    help1->description = "You help treat her wounds with some herbs you find. [USED: Common Mallow if available] She's grateful and agrees to join your pack! [PACK MEMBER ADDED: Luna - Scout] She tells you about a deer herd nearby.";
    help1->choiceA_text = "Hunt the deer together (pack hunting bonus)";
    help1->choiceB_text = "Rest and recover first, hunt tomorrow";
    help1->isEnding = false;
    insertNode(wolf1, help1, true);

    // Wolf - Ignore Her - Day 2
    DecisionNode* ignore1 = new DecisionNode();
    ignore1->scenarioID = 7;
    ignore1->description = "You leave her behind and continue alone. Your hunger increases (75/100). You find some winter berries near a stream. [GAINED: Winter Berries x3, Fresh Water x1] Winter is getting harsher and you need more substantial food.";
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
    safe1->description = "You take 2 small fish and 1 herb, leaving quietly. [GAINED: Small Fish x2, Common Mallow x1] The humans pass by without noticing. You're still hungry but safer. You smell a pack of wolves nearby.";
    safe1->choiceA_text = "Approach the wolf pack";
    safe1->choiceB_text = "Avoid them and continue alone";
    safe1->isEnding = false;
    insertNode(traps1, safe1, false);

    // Shelter - Fight Bear - Day 3
    DecisionNode* bear1 = new DecisionNode();
    bear1->scenarioID = 10;
    bear1->description = "You attack the sleeping bear! It wakes up angry and swipes at you. You're badly injured but manage to drive it away. [Health -40] The cave is yours but you need healing. [USE: Common Mallow if available]";
    bear1->choiceA_text = "Use healing herbs to recover";
    bear1->choiceB_text = "Tough it out and rest in the cave";
    bear1->isEnding = false;
    insertNode(shelter1, bear1, true);

    // Help - Hunt Deer Together - Day 3
    DecisionNode* deer1 = new DecisionNode();
    deer1->scenarioID = 11;
    deer1->description = "You meet Luna, a skilled lone hunter. She offers to join your pack! Together, you successfully hunt a large deer! [GAINED: Fresh Meat x8, Fresh Water x2] Your pack is well-fed. Luna's loyalty increases. You hear howls from a larger pack in the distance.";
    deer1->choiceA_text = "Investigate the larger pack";
    deer1->choiceB_text = "Stay hidden and build your own pack";
    deer1->isEnding = false;
    insertNode(help1, deer1, true);

    // Continue building more nodes for Days 4-10...
    // Day 4: Pack Building
    DecisionNode* pack1 = new DecisionNode();
    pack1->scenarioID = 12;
    pack1->description = "With Luna in your pack, you encounter a young male wolf being chased by hunters. He looks strong and could be a good hunter for your pack.";
    pack1->choiceA_text = "Rescue the young wolf (gain pack member: Fenris as Scout)";
    pack1->choiceB_text = "Let the hunters take him (avoid danger)";
    pack1->isEnding = false;
    insertNode(deer1, pack1, true);

    // Day 5: Territory Establishment
    DecisionNode* territory1 = new DecisionNode();
    territory1->scenarioID = 13;
    territory1->description = "You have found Luna, a skilled hunter who joined your pack! Now you need to establish territory. You find a perfect valley with a stream and plenty of prey. But another pack claims it too.";
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
    end3->description = "Your pack struggles to survive the harsh winter. Some members perish, but the core remains. Through hardship, your remaining pack grows stronger and more resilient.";
    end3->isEnding = false;  // Changed from true - continue to recovery path
    end3->endingText = "";
    insertNode(crisis1, end3, false);

    DecisionNode* end4 = new DecisionNode();
    end4->scenarioID = 22;
    end4->description = "You successfully negotiate with the rival pack. They propose an alliance rather than conflict.";
    end4->choiceA_text = "Accept full alliance - share territory and resources equally";
    end4->choiceB_text = "Accept limited alliance - maintain clear boundaries";
    end4->isEnding = false;
    insertNode(threat1, end4, false);  // Alternative to crisis path

    // Diplomacy finale - unique ending
    DecisionNode* diplomacyFinale = new DecisionNode();
    diplomacyFinale->scenarioID = 67;
    diplomacyFinale->description = "Your wisdom in diplomacy has created something rare in the wilderness - two packs working as one. The forest recognizes your vision for peace, and your combined force becomes unmatched.";
    diplomacyFinale->isEnding = true;
    diplomacyFinale->endingText = "DIPLOMATIC MASTER: A true alpha leads not through strength alone, but through understanding and cooperation.";
    insertNode(end4, diplomacyFinale, true);  // True unique ending from end4

    DecisionNode* end5 = new DecisionNode();
    end5->scenarioID = 31;
    end5->description = "You choose to be selective about new members. Your pack remains small but highly skilled and loyal. Through careful management and selective recruitment, your elite pack becomes legendary.";
    end5->isEnding = true;
    end5->endingText = "ELITE GUARDIANS: Quality over quantity - your pack is legendary for its skill and unity.";
    insertNode(loyalty1, end5, false);  // Connect as alternative to recruitment path



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

    // NEW: Scavenging Scenario - Difficulty-scaled loot
    DecisionNode* scavenge1 = new DecisionNode();
    scavenge1->scenarioID = 40;
    scavenge1->description = "You discover an abandoned hunter's camp! The fire is still warm - they left recently. You carefully search through their supplies and trash. [SCAVENGED: Mixed supplies based on difficulty] You hear wolves howl in the distance.";
    scavenge1->choiceA_text = "Quickly gather what you can and leave";
    scavenge1->choiceB_text = "Take your time to find everything (risk humans return)";
    scavenge1->isEnding = false;
    insertNode(bearContinue, scavenge1, true);

    // Scavenge - Quick gather
    DecisionNode* quickGather = new DecisionNode();
    quickGather->scenarioID = 41;
    quickGather->description = "You grab the easiest items and flee before the humans return. [GAINED: Small Fish x2, Fresh Water x1, Common Mallow x1] You escape just as you hear voices!";
    quickGather->choiceA_text = "Hide and observe the humans";
    quickGather->choiceB_text = "Keep moving to find safer ground";
    quickGather->isEnding = false;
    insertNode(scavenge1, quickGather, true);

    // Scavenge - Take time
    DecisionNode* takeTime = new DecisionNode();
    takeTime->scenarioID = 42;
    takeTime->description = "You thoroughly search the camp. You find more supplies but the humans return! A hunter spots you and raises his rifle. [GAINED: Small Fish x4, Winter Berries x2, Common Mallow x2, Fresh Water x2]";
    takeTime->choiceA_text = "Drop some items and run";
    takeTime->choiceB_text = "Stand your ground and growl";
    takeTime->isEnding = false;
    insertNode(scavenge1, takeTime, false);

    // Connect scavenge paths to main story
    insertNode(quickGather, territory1, true);
    insertNode(takeTime, resources1, true);

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

    // Add missing connections for other scenarios to prevent null pointers
    if (!dropItems->left) {
        DecisionNode* dropItemsEndA = new DecisionNode();
        dropItemsEndA->scenarioID = 361;
        dropItemsEndA->description = "You continue deeper into the forest. The trees grow thicker and wildlife more abundant. You find a suitable resting spot for the night.";
        dropItemsEndA->isEnding = false;
        dropItemsEndA->choiceA_text = "Set up camp for the night";
        dropItemsEndA->choiceB_text = "Keep moving to find better shelter";
        insertNode(dropItems, dropItemsEndA, true);
    }

    if (!dropItems->right) {
        DecisionNode* dropItemsEndB = new DecisionNode();
        dropItemsEndB->scenarioID = 362;
        dropItemsEndB->description = "You find a safe place to rest in a sheltered grove. The night passes peacefully and you wake refreshed.";
        dropItemsEndB->isEnding = false;
        dropItemsEndB->choiceA_text = "Continue on your journey";
        dropItemsEndB->choiceB_text = "Explore the surrounding area";
        insertNode(dropItems, dropItemsEndB, false);
    }

    if (!safeContinue->left) {
        DecisionNode* safeContinueEndA = new DecisionNode();
        safeContinueEndA->scenarioID = 371;
        safeContinueEndA->description = "You use your tools to improve hunting. Your efficiency increases significantly.";
        safeContinueEndA->isEnding = false;
        safeContinueEndA->choiceA_text = "Hunt for larger prey";
        safeContinueEndA->choiceB_text = "Set traps for smaller animals";
        insertNode(safeContinue, safeContinueEndA, true);
    }

    if (!safeContinue->right) {
        DecisionNode* safeContinueEndB = new DecisionNode();
        safeContinueEndB->scenarioID = 372;
        safeContinueEndB->description = "You focus on gathering more supplies. Your resource collection improves.";
        safeContinueEndB->isEnding = false;
        safeContinueEndB->choiceA_text = "Search for medicinal herbs";
        safeContinueEndB->choiceB_text = "Look for materials to craft tools";
        insertNode(safeContinue, safeContinueEndB, false);
    }

    // ========== EXTEND STORY PATHS TO ENSURE ALL ENDINGS AFTER DAY 10 ==========

    // Extend Tragedy Path (end3 -> 64 -> end2)
    DecisionNode* tragedyRecovery = new DecisionNode();
    tragedyRecovery->scenarioID = 64;
    tragedyRecovery->description = "Despite tragic losses, your surviving pack members look to you for leadership. The wilderness has taken much, but together you can rebuild and survive.";
    tragedyRecovery->choiceA_text = "Push forward with determination - survival waits for no one";
    tragedyRecovery->choiceB_text = "Take a moment to honor the fallen and strengthen resolve";
    tragedyRecovery->isEnding = false;
    insertNode(end3, tragedyRecovery, true);  // end3 = scenario 21 (tragedy path)

    // TRAGIC HERO ending - separate branch from crisis
    DecisionNode* tragicHeroEnd = new DecisionNode();
    tragicHeroEnd->scenarioID = 70;  // Changed from 68 to avoid duplicate with recruitmentChoice
    tragicHeroEnd->description = "The harsh winter proved too much. Your pack fell apart, and you died alone in the frozen wilderness. But your story of courage and determination becomes legend among wolves for generations.";
    tragicHeroEnd->isEnding = true;
    tragicHeroEnd->endingText = "TRAGIC HERO: Your sacrifice saved others, even in death. Your legend lives on.";
    insertNode(crisis1, tragicHeroEnd, false);  // Alternative to end3 path

    DecisionNode* tragedyFinale = new DecisionNode();
    tragedyFinale->scenarioID = 65;
    tragedyFinale->description = "Through hardship and loss, you've proven yourself a true survivor. The pack that remains is stronger for having endured the worst together. Your legacy of resilience inspires future generations.";
    tragedyFinale->isEnding = true;
    tragedyFinale->endingText = "SURVIVOR'S LEGACY: Not all who wander are lost - you found your way through tragedy and emerged stronger.";
    insertNode(tragedyRecovery, tragedyFinale, true);  // True unique ending

    // Extend Diplomacy Path (end4 -> 66 -> end1)
    DecisionNode* allianceBuilding = new DecisionNode();
    allianceBuilding->scenarioID = 66;
    allianceBuilding->description = "Your diplomatic victory opens new possibilities. The rival pack offers a formal alliance, but it comes with responsibilities and shared burdens.";
    allianceBuilding->choiceA_text = "Accept full alliance - share territory and resources equally";
    allianceBuilding->choiceB_text = "Accept limited alliance - maintain clear boundaries";
    allianceBuilding->isEnding = false;
    insertNode(end4, allianceBuilding, true);  // end4 = scenario 22 (diplomatic ending)
    
    // ========== ADD PACK PLAYER CHOICES ==========

    // Recruitment Decision Node (scenario 68)
    DecisionNode* recruitmentChoice = new DecisionNode();
    recruitmentChoice->scenarioID = 68;
    recruitmentChoice->description = "A lone wolf approaches your pack, showing respect and submission. They want to join, but adding members means more mouths to feed and potential loyalty issues.";
    recruitmentChoice->choiceA_text = "Welcome them (Pack +1, Hunger +10, potential benefits)";
    recruitmentChoice->choiceB_text = "Turn them away (Keep current pack, no additional burden)";
    recruitmentChoice->isEnding = false;
    insertNode(loyalty1, recruitmentChoice, true);  // Insert after loyalty test

    // Recruitment Choice Children - Lead to unique endings instead of cycling back
    DecisionNode* recruitMember = new DecisionNode();
    recruitMember->scenarioID = 71;
    recruitMember->description = "Your pack grows stronger with the new member. Together, you face the challenges of winter and emerge victorious.";
    recruitMember->isEnding = true;
    recruitMember->endingText = "EXPANDED PACK: Your willingness to welcome others has created a powerful pack!";
    insertNode(recruitmentChoice, recruitMember, true);

    DecisionNode* rejectRecruit = new DecisionNode();
    rejectRecruit->scenarioID = 72;
    rejectRecruit->description = "You maintain your pack's discipline and focus. While smaller, your pack remains tight-knit and effective.";
    rejectRecruit->isEnding = true;
    rejectRecruit->endingText = "ELITE SQUAD: Sometimes quality matters more than quantity. Your focused pack thrives!";
    insertNode(recruitmentChoice, rejectRecruit, false);

    // Training Decision Node (scenario 69)
    DecisionNode* trainingChoice = new DecisionNode();
    trainingChoice->scenarioID = 69;
    trainingChoice->description = "Your pack has some energy to spare. How should you use it to improve your pack's capabilities?";
    trainingChoice->choiceA_text = "Train for hunting (Energy -20, better food gains in future)";
    trainingChoice->choiceB_text = "Train for scouting (Energy -15, better event chances)";
    trainingChoice->isEnding = false;
    insertNode(territory1, trainingChoice, true);  // Insert after territory establishment

    // Training Choice Children - Lead to unique endings instead of cycling back
    DecisionNode* huntTraining = new DecisionNode();
    huntTraining->scenarioID = 73;
    huntTraining->description = "Your pack's enhanced hunting skills prove invaluable. You secure abundant food throughout the harsh winter.";
    huntTraining->isEnding = true;
    huntTraining->endingText = "MASTER HUNTERS: Your pack becomes legendary for its hunting prowess!";
    insertNode(trainingChoice, huntTraining, true);

    DecisionNode* scoutTraining = new DecisionNode();
    scoutTraining->scenarioID = 74;
    scoutTraining->description = "Your scouts become remarkably skilled at spotting opportunities and dangers. Your pack thrives on superior intelligence.";
    scoutTraining->isEnding = true;
    scoutTraining->endingText = "ELITE SCOUTS: Your pack's awareness and adaptability become legendary!";
    insertNode(trainingChoice, scoutTraining, false);

    // ========== COMPLETE THE TREE BY ADDING PROPER ENDINGS FOR NULL POINTERS ==========
    // Add proper endings to prevent fallback scenarios for any remaining null pointers

    // Add endings for scenario 12 (rescue young wolf path) if not already connected
    if (!pack1->left) {
        DecisionNode* pack1EndA = new DecisionNode();
        pack1EndA->scenarioID = 75;
        pack1EndA->description = "You successfully rescue the young wolf. Together, you face the challenges of the wilderness. Your pack grows stronger with each passing day.";
        pack1EndA->isEnding = false;  // Not an ending, continue story
        pack1EndA->choiceA_text = "Continue building your pack";
        pack1EndA->choiceB_text = "Focus on territory expansion";
        insertNode(pack1, pack1EndA, true);
    }

    if (!pack1->right) {
        DecisionNode* pack1EndB = new DecisionNode();
        pack1EndB->scenarioID = 76;
        pack1EndB->description = "You choose not to risk helping the young wolf. The hunters take him, but you've preserved your pack's safety for now.";
        pack1EndB->isEnding = false;  // Not an ending, continue story
        pack1EndB->choiceA_text = "Establish territory elsewhere";
        pack1EndB->choiceB_text = "Search for other pack members";
        insertNode(pack1, pack1EndB, false);
    }

    // Add endings for other potential null pointers to avoid fallback scenarios
    if (!territory1->left) {
        DecisionNode* territoryEndA = new DecisionNode();
        territoryEndA->scenarioID = 77;
        territoryEndA->description = "You successfully challenge the rival pack for territory. After a fierce battle, you emerge victorious and establish your pack's dominance.";
        territoryEndA->isEnding = false;  // Not an ending, continue story
        territoryEndA->choiceA_text = "Consolidate your victory";
        territoryEndA->choiceB_text = "Expand your territory further";
        insertNode(territory1, territoryEndA, true);
    }

    if (!territory1->right) {
        DecisionNode* territoryEndB = new DecisionNode();
        territoryEndB->scenarioID = 78;
        territoryEndB->description = "You choose to negotiate with the rival pack. Through diplomacy, you reach an agreement that benefits both packs.";
        territoryEndB->isEnding = false;  // Not an ending, continue story
        territoryEndB->choiceA_text = "Form a permanent alliance";
        territoryEndB->choiceB_text = "Agree to temporary cooperation";
        insertNode(territory1, territoryEndB, false);
    }

    // Add missing connections for other nodes that might have null pointers
    if (!hunt1->left) {
        DecisionNode* hunt1EndA = new DecisionNode();
        hunt1EndA->scenarioID = 4;
        hunt1EndA->description = "You search for more traps and find them. [GAINED: Small Fish x3, Winter Berries x2, Common Mallow x1] But you hear human voices approaching.";
        hunt1EndA->isEnding = false;
        hunt1EndA->choiceA_text = "Grab everything and run quickly";
        hunt1EndA->choiceB_text = "Take only what you can carry safely";
        insertNode(hunt1, hunt1EndA, true);
    }

    if (!hunt1->right) {
        DecisionNode* hunt1EndB = new DecisionNode();
        hunt1EndB->scenarioID = 5;
        hunt1EndB->description = "You find a cave, but it's occupied by a bear! The bear is hibernating. You could fight it, sneak past, or find another shelter.";
        hunt1EndB->isEnding = false;
        hunt1EndB->choiceA_text = "Fight the bear alone (risky but gets good shelter)";
        hunt1EndB->choiceB_text = "Sneak past and share the cave";
        insertNode(hunt1, hunt1EndB, false);
    }

    if (!wolf1->left) {
        DecisionNode* wolf1EndA = new DecisionNode();
        wolf1EndA->scenarioID = 6;
        wolf1EndA->description = "You help treat her wounds with some herbs you find. She's grateful and agrees to join your pack! [PACK MEMBER ADDED: Luna - Scout] She tells you about a deer herd nearby.";
        wolf1EndA->isEnding = false;
        wolf1EndA->choiceA_text = "Hunt the deer together (pack hunting bonus)";
        wolf1EndA->choiceB_text = "Rest and recover first, hunt tomorrow";
        insertNode(wolf1, wolf1EndA, true);
    }

    if (!wolf1->right) {
        DecisionNode* wolf1EndB = new DecisionNode();
        wolf1EndB->scenarioID = 7;
        wolf1EndB->description = "You leave her behind and continue alone. Your hunger increases (75/100). You find some winter berries near a stream.";
        wolf1EndB->isEnding = false;
        wolf1EndB->choiceA_text = "Search for bigger prey";
        wolf1EndB->choiceB_text = "Look for other wolves to join";
        insertNode(wolf1, wolf1EndB, false);
    }

    // Add connections for other nodes that still have null pointers
    if (!shelter1->left) {
        DecisionNode* shelter1EndA = new DecisionNode();
        shelter1EndA->scenarioID = 8;
        shelter1EndA->description = "You fight the bear alone. It's a fierce battle, but your determination sees you through. You drive the bear away and claim the cave.";
        shelter1EndA->isEnding = false;
        shelter1EndA->choiceA_text = "Use the cave as your base";
        shelter1EndA->choiceB_text = "Continue searching for better shelter";
        insertNode(shelter1, shelter1EndA, true);
    }

    if (!shelter1->right) {
        DecisionNode* shelter1EndB = new DecisionNode();
        shelter1EndB->scenarioID = 9;
        shelter1EndB->description = "You sneak past the bear and share the cave. The bear stays in the back corner, and you make yourself comfortable near the entrance. It's not ideal, but it works.";
        shelter1EndB->isEnding = false;
        shelter1EndB->choiceA_text = "Approach the wolf pack nearby";
        shelter1EndB->choiceB_text = "Avoid them and continue alone";
        insertNode(shelter1, shelter1EndB, false);
    }

    if (!bear1->left) {
        DecisionNode* bear1EndA = new DecisionNode();
        bear1EndA->scenarioID = 10;
        bear1EndA->description = "You use healing herbs to recover. The Common Mallow helps restore your health. The cave is now yours.";
        bear1EndA->isEnding = false;
        bear1EndA->choiceA_text = "Claim this area as your territory";
        bear1EndA->choiceB_text = "Continue traveling to find easier hunting";
        insertNode(bear1, bear1EndA, true);
    }

    if (!bear1->right) {
        DecisionNode* bear1EndB = new DecisionNode();
        bear1EndB->scenarioID = 11;
        bear1EndB->description = "You tough it out and rest in the cave. Your health slowly recovers. By morning, you're stronger. You venture out and find a frozen lake with fish visible under the ice.";
        bear1EndB->isEnding = false;
        bear1EndB->choiceA_text = "Break the ice to fish (risky but rewarding)";
        bear1EndB->choiceB_text = "Move on to find easier food";
        insertNode(bear1, bear1EndB, false);
    }

    if (!deer1->left) {
        DecisionNode* deer1EndA = new DecisionNode();
        deer1EndA->scenarioID = 12;
        deer1EndA->description = "You investigate the larger pack. They seem interested in forming an alliance rather than fighting.";
        deer1EndA->isEnding = false;
        deer1EndA->choiceA_text = "Approach with caution";
        deer1EndA->choiceB_text = "Keep your distance";
        insertNode(deer1, deer1EndA, true);
    }

    if (!deer1->right) {
        DecisionNode* deer1EndB = new DecisionNode();
        deer1EndB->scenarioID = 13;
        deer1EndB->description = "You stay hidden and build your own pack. You encounter a young male wolf being chased by hunters.";
        deer1EndB->isEnding = false;
        deer1EndB->choiceA_text = "Rescue the young wolf (gain pack member: Fenris as Scout)";
        deer1EndB->choiceB_text = "Let the hunters take him (avoid danger)";
        insertNode(deer1, deer1EndB, false);
    }

    if (!resources1->left) {
        DecisionNode* resources1EndA = new DecisionNode();
        resources1EndA->scenarioID = 14;
        resources1EndA->description = "You hunt dangerous large prey. It's a risky but rewarding choice. You successfully bring down a large deer!";
        resources1EndA->isEnding = false;
        resources1EndA->choiceA_text = "Share the meat with the pack";
        resources1EndA->choiceB_text = "Keep some for yourself";
        insertNode(resources1, resources1EndA, true);
    }

    if (!resources1->right) {
        DecisionNode* resources1EndB = new DecisionNode();
        resources1EndB->scenarioID = 33;
        resources1EndB->description = "You ration the food carefully. The pack is hungry but alive. A traveling merchant offers to trade supplies for territory information.";
        resources1EndB->isEnding = false;
        resources1EndB->choiceA_text = "Trade information for supplies";
        resources1EndB->choiceB_text = "Keep territory secrets safe";
        insertNode(resources1, resources1EndB, false);
    }

    if (!loyalty1->left) {
        DecisionNode* loyalty1EndA = new DecisionNode();
        loyalty1EndA->scenarioID = 15;
        loyalty1EndA->description = "You fight to maintain dominance. Your strength proves you're worthy of leadership.";
        loyalty1EndA->isEnding = false;
        loyalty1EndA->choiceA_text = "Lead with strength";
        loyalty1EndA->choiceB_text = "Lead with wisdom";
        insertNode(loyalty1, loyalty1EndA, true);
    }

    if (!loyalty1->right) {
        DecisionNode* loyalty1EndB = new DecisionNode();
        loyalty1EndB->scenarioID = 31;
        loyalty1EndB->description = "You try to resolve peacefully. Your wisdom earns respect from the pack.";
        loyalty1EndB->isEnding = true;
        loyalty1EndB->endingText = "ELITE GUARDIANS: Quality over quantity - your pack is legendary for its skill and unity.";
        insertNode(loyalty1, loyalty1EndB, false);
    }

    if (!threat1->left) {
        DecisionNode* threat1EndA = new DecisionNode();
        threat1EndA->scenarioID = 16;
        threat1EndA->description = "You stand and fight with your pack. It's a fierce battle, but your pack's loyalty and strength see you through.";
        threat1EndA->isEnding = false;
        threat1EndA->choiceA_text = "Accept victory and consolidate power";
        threat1EndA->choiceB_text = "Pursue fleeing enemies";
        insertNode(threat1, threat1EndA, true);
    }

    if (!threat1->right) {
        DecisionNode* threat1EndB = new DecisionNode();
        threat1EndB->scenarioID = 34;
        threat1EndB->description = "You retreat and find new territory. Your pack questions your leadership but follows. You discover an ancient wolf sanctuary.";
        threat1EndB->isEnding = false;
        threat1EndB->choiceA_text = "Claim the sanctuary as your new home";
        threat1EndB->choiceB_text = "Continue searching for better territory";
        insertNode(threat1, threat1EndB, false);
    }

    if (!crisis1->left) {
        DecisionNode* crisis1EndA = new DecisionNode();
        crisis1EndA->scenarioID = 17;
        crisis1EndA->description = "You send scouts to find food in the storm. Some return with supplies, others don't survive.";
        crisis1EndA->isEnding = false;
        crisis1EndA->choiceA_text = "Accept the challenge (final boss fight)";
        crisis1EndA->choiceB_text = "Decline and remain a small pack";
        insertNode(crisis1, crisis1EndA, true);
    }

    if (!crisis1->right) {
        DecisionNode* crisis1EndB = new DecisionNode();
        crisis1EndB->scenarioID = 35;
        crisis1EndB->description = "You wait out the storm and hope for the best. Some pack members don't survive, but the core remains strong.";
        crisis1EndB->isEnding = false;
        crisis1EndB->choiceA_text = "Honor the fallen and rebuild";
        crisis1EndB->choiceB_text = "Leave this cursed territory forever";
        insertNode(crisis1, crisis1EndB, false);
    }

    // Add connections for remaining nodes that still have null pointers
    // These are nodes that were created but not properly connected in the original tree

    // Add connections for help1 (scenario 6)
    if (!help1->left) {
        DecisionNode* help1EndA = new DecisionNode();
        help1EndA->scenarioID = 6;
        help1EndA->description = "You hunt the deer together successfully. Your pack's teamwork proves effective!";
        help1EndA->isEnding = false;
        help1EndA->choiceA_text = "Investigate the larger pack";
        help1EndA->choiceB_text = "Stay hidden and build your own pack";
        insertNode(help1, help1EndA, true);
    }

    if (!help1->right) {
        DecisionNode* help1EndB = new DecisionNode();
        help1EndB->scenarioID = 6;
        help1EndB->description = "You rest and recover first, then hunt tomorrow. Your patience pays off.";
        help1EndB->isEnding = false;
        help1EndB->choiceA_text = "Hunt successfully tomorrow";
        help1EndB->choiceB_text = "Focus on other priorities";
        insertNode(help1, help1EndB, false);
    }

    // Add connections for ignore1 (scenario 7)
    if (!ignore1->left) {
        DecisionNode* ignore1EndA = new DecisionNode();
        ignore1EndA->scenarioID = 7;
        ignore1EndA->description = "You search for bigger prey. Your hunger drives you to take greater risks.";
        ignore1EndA->isEnding = false;
        ignore1EndA->choiceA_text = "Hunt dangerous large prey";
        ignore1EndA->choiceB_text = "Look for safer options";
        insertNode(ignore1, ignore1EndA, true);
    }

    if (!ignore1->right) {
        DecisionNode* ignore1EndB = new DecisionNode();
        ignore1EndB->scenarioID = 7;
        ignore1EndB->description = "You look for other wolves to join. The pack mentality is strong in the wilderness.";
        ignore1EndB->isEnding = false;
        ignore1EndB->choiceA_text = "Approach a lone wolf";
        ignore1EndB->choiceB_text = "Search for a pack to join";
        insertNode(ignore1, ignore1EndB, false);
    }

    // Add connections for grab1 (scenario 8)
    if (!grab1->right) {
        DecisionNode* grab1EndB = new DecisionNode();
        grab1EndB->scenarioID = 8;
        grab1EndB->description = "You stand and fight the hunter. It's a dangerous choice but you must protect your pack.";
        grab1EndB->isEnding = false;
        grab1EndB->choiceA_text = "Fight aggressively";
        grab1EndB->choiceB_text = "Use defensive tactics";
        insertNode(grab1, grab1EndB, false);
    }

    // Add connections for safe1 (scenario 9)
    if (!safe1->left) {
        DecisionNode* safe1EndA = new DecisionNode();
        safe1EndA->scenarioID = 9;
        safe1EndA->description = "You approach the wolf pack. They seem cautious but not hostile.";
        safe1EndA->isEnding = false;
        safe1EndA->choiceA_text = "Show submission";
        safe1EndA->choiceB_text = "Show strength";
        insertNode(safe1, safe1EndA, true);
    }

    if (!safe1->right) {
        DecisionNode* safe1EndB = new DecisionNode();
        safe1EndB->scenarioID = 9;
        safe1EndB->description = "You avoid them and continue alone. Independence has its own rewards and challenges.";
        safe1EndB->isEnding = false;
        safe1EndB->choiceA_text = "Continue alone";
        safe1EndB->choiceB_text = "Reconsider joining them";
        insertNode(safe1, safe1EndB, false);
    }

    // Add connections for bear1 (scenario 10) - already handled above

    // Add connections for bear1->right (scenario 11)
    if (!bear1->right) {
        DecisionNode* bear1EndB = new DecisionNode();
        bear1EndB->scenarioID = 11;
        bear1EndB->description = "You tough it out and rest in the cave. Your health slowly recovers.";
        bear1EndB->isEnding = false;
        bear1EndB->choiceA_text = "Break the ice to fish";
        bear1EndB->choiceB_text = "Move on to find easier food";
        insertNode(bear1, bear1EndB, false);
    }

    // Add connections for deer1 (scenario 11) - this is the second deer1
    if (!deer1->left) {
        DecisionNode* deer1EndA = new DecisionNode();
        deer1EndA->scenarioID = 11;
        deer1EndA->description = "You investigate the larger pack. They seem interested in forming an alliance.";
        deer1EndA->isEnding = false;
        deer1EndA->choiceA_text = "Approach with caution";
        deer1EndA->choiceB_text = "Keep your distance";
        insertNode(deer1, deer1EndA, true);
    }

    if (!deer1->right) {
        DecisionNode* deer1EndB = new DecisionNode();
        deer1EndB->scenarioID = 11;
        deer1EndB->description = "You stay hidden and build your own pack. You encounter a young wolf.";
        deer1EndB->isEnding = false;
        deer1EndB->choiceA_text = "Rescue the young wolf";
        deer1EndB->choiceB_text = "Let the hunters take him";
        insertNode(deer1, deer1EndB, false);
    }

    // Add connections for territory1 (scenario 13)
    if (!territory1->left) {
        DecisionNode* territory1EndA = new DecisionNode();
        territory1EndA->scenarioID = 13;
        territory1EndA->description = "You challenge the other pack for territory. A fierce battle ensues.";
        territory1EndA->isEnding = false;
        territory1EndA->choiceA_text = "Fight aggressively";
        territory1EndA->choiceB_text = "Use strategy and cunning";
        insertNode(territory1, territory1EndA, true);
    }

    if (!territory1->right) {
        DecisionNode* territory1EndB = new DecisionNode();
        territory1EndB->scenarioID = 13;
        territory1EndB->description = "You negotiate to share the territory. Diplomacy can be as effective as force.";
        territory1EndB->isEnding = false;
        territory1EndB->choiceA_text = "Accept the terms";
        territory1EndB->choiceB_text = "Negotiate better conditions";
        insertNode(territory1, territory1EndB, false);
    }

    // Add connections for bearContinue (scenario 38)
    if (!bearContinue->right) {
        DecisionNode* bearContinueEndB = new DecisionNode();
        bearContinueEndB->scenarioID = 38;
        bearContinueEndB->description = "You continue traveling to find easier hunting. The journey is long but rewarding.";
        bearContinueEndB->isEnding = false;
        bearContinueEndB->choiceA_text = "Settle in a new area";
        bearContinueEndB->choiceB_text = "Keep exploring";
        insertNode(bearContinue, bearContinueEndB, false);
    }

    // Add connections for other nodes that might have been missed
    if (!scavenge1->left) {
        DecisionNode* scavenge1EndA = new DecisionNode();
        scavenge1EndA->scenarioID = 40;
        scavenge1EndA->description = "You quickly gather what you can and leave. The supplies are valuable.";
        scavenge1EndA->isEnding = false;
        scavenge1EndA->choiceA_text = "Hide and observe the humans";
        scavenge1EndA->choiceB_text = "Keep moving to find safer ground";
        insertNode(scavenge1, scavenge1EndA, true);
    }

    if (!scavenge1->right) {
        DecisionNode* scavenge1EndB = new DecisionNode();
        scavenge1EndB->scenarioID = 40;
        scavenge1EndB->description = "You take your time to find everything. Your thoroughness pays off with extra supplies.";
        scavenge1EndB->isEnding = false;
        scavenge1EndB->choiceA_text = "Drop some items and run";
        scavenge1EndB->choiceB_text = "Stand your ground and growl";
        insertNode(scavenge1, scavenge1EndB, false);
    }

    // Final check: Convert any remaining null pointers to proper endings
    // This ensures the tree is completely connected with no null pointers
    std::queue<DecisionNode*> q;
    std::set<DecisionNode*> visited;
    q.push(root);
    visited.insert(root);

    while (!q.empty()) {
        DecisionNode* current = q.front();
        q.pop();

        if (!current->isEnding) {
            // If left is null, create an ending
            if (!current->left) {
                DecisionNode* ending = new DecisionNode();
                ending->scenarioID = 999; // Use high number to avoid conflicts
                ending->description = "Your journey comes to an end. The wilderness has tested you in ways you never imagined. Your choices have shaped your destiny.";
                ending->choiceA_text = "Reflect on your journey";
                ending->choiceB_text = "Accept your fate";
                ending->isEnding = true;
                ending->endingText = "JOURNEY'S END: Your adventure in the wilderness has concluded.";
                current->left = ending;
            } else if (visited.find(current->left) == visited.end()) {
                visited.insert(current->left);
                q.push(current->left);
            }

            // If right is null, create an ending
            if (!current->right) {
                DecisionNode* ending = new DecisionNode();
                ending->scenarioID = 1000; // Use high number to avoid conflicts
                ending->description = "Your path leads to its conclusion. The choices you've made have brought you to this moment.";
                ending->choiceA_text = "Embrace the outcome";
                ending->choiceB_text = "Learn from the experience";
                ending->isEnding = true;
                ending->endingText = "STORY'S END: Your tale in the wild has reached its conclusion.";
                current->right = ending;
            } else if (visited.find(current->right) == visited.end()) {
                visited.insert(current->right);
                q.push(current->right);
            }
        }
    }

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

void DecisionTree::buildSurvivalStory() {
    // Reset any existing tree before building new one
    reset();
    
    // Survival mode - harsher, resource-focused story
    root = new DecisionNode();
    root->scenarioID = 101;
    root->description = "SURVIVAL MODE: You wake up injured and disoriented in a frozen wasteland. A blizzard approaches from the north. Your health is critically low (40/100) and hunger gnaws at you (80/100). You must find shelter immediately or perish.";
    root->choiceA_text = "Search for a cave before the storm hits (risky but necessary)";
    root->choiceB_text = "Try to outrun the storm and find lower ground";
    root->isEnding = false;

    // Path A: Cave search
    DecisionNode* cave1 = new DecisionNode();
    cave1->scenarioID = 102;
    cave1->description = "You find a small cave, but it's occupied by a hibernating bear. The storm is nearly upon you. Fighting the bear could be fatal in your weakened state, but you need shelter NOW.";
    cave1->choiceA_text = "Fight the bear despite your injuries (-30 health risk)";
    cave1->choiceB_text = "Risk the storm and search for another shelter";
    cave1->isEnding = false;
    insertNode(root, cave1, true);

    // Path B: Outrun storm
    DecisionNode* run1 = new DecisionNode();
    run1->scenarioID = 103;
    run1->description = "You run desperately through the snow. Your energy depletes rapidly (-15 energy). You spot what looks like an abandoned hunter's cabin in the distance, but the storm is catching up.";
    run1->choiceA_text = "Sprint to the cabin (risk energy depletion)";
    run1->choiceB_text = "Dig into a snowdrift for temporary shelter";
    run1->isEnding = false;
    insertNode(root, run1, false);

    // Cave - Fight bear
    DecisionNode* fightBear = new DecisionNode();
    fightBear->scenarioID = 104;
    fightBear->description = "You attack the bear in a desperate fury! It awakens and mauls you badly (-25 health), but you drive it out into the storm. The cave is yours. You find the bear's food cache: dried fish and herbs. [GAINED: Small Fish x4, Common Mallow x1]";
    fightBear->choiceA_text = "Eat immediately to recover";
    fightBear->choiceB_text = "Save food and rest to recover health naturally";
    fightBear->isEnding = false;
    insertNode(cave1, fightBear, true);

    // Cave - Search more
    DecisionNode* searchMore = new DecisionNode();
    searchMore->scenarioID = 105;
    searchMore->description = "The storm hits before you find shelter. You're caught in the blizzard (-20 health, -10 energy). Through the whiteout, you see a fallen tree with a hollow underneath.";
    searchMore->choiceA_text = "Take shelter in the hollow";
    searchMore->choiceB_text = "Keep moving, you might freeze if you stop";
    searchMore->isEnding = false;
    insertNode(cave1, searchMore, false);

    // Run - Sprint to cabin
    DecisionNode* cabin = new DecisionNode();
    cabin->scenarioID = 106;
    cabin->description = "You reach the cabin just as the storm hits! Inside, you find supplies left by hunters: smoked fish and medicinal plants. [GAINED: Small Fish x4, Common Mallow x2, Fresh Water x2]. But the door is damaged and won't close properly.";
    cabin->choiceA_text = "Use energy to repair the door (-15 energy)";
    cabin->choiceB_text = "Block it with furniture and hope for the best";
    cabin->isEnding = false;
    insertNode(run1, cabin, true);

    // Run - Snowdrift shelter
    DecisionNode* snowdrift = new DecisionNode();
    snowdrift->scenarioID = 107;
    snowdrift->description = "You dig frantically into the snowdrift. It provides basic shelter from the wind (-5 health). The storm rages for hours. When it finally passes, you're alive but severely weakened. You spot wolf tracks heading east.";
    snowdrift->choiceA_text = "Follow the tracks (might lead to a pack)";
    snowdrift->choiceB_text = "Head west toward what might be human settlement";
    snowdrift->isEnding = false;
    insertNode(run1, snowdrift, false);

    // Fight Bear - Eat immediately
    DecisionNode* eatNow = new DecisionNode();
    eatNow->scenarioID = 108;
    eatNow->description = "You devour the fish ravenously. Your hunger decreases significantly. The storm passes. At dawn, you hear howling - a pack is nearby. They might accept you or see you as a threat.";
    eatNow->choiceA_text = "Approach the pack (risky but need allies)";
    eatNow->choiceB_text = "Avoid them and continue alone";
    eatNow->isEnding = false;
    insertNode(fightBear, eatNow, true);

    // Fight Bear - Save food
    DecisionNode* saveFood = new DecisionNode();
    saveFood->scenarioID = 109;
    saveFood->description = "You rest to heal naturally. Your body slowly recovers. By morning, you're stronger. You venture out and find a frozen lake with fish visible under the ice.";
    saveFood->choiceA_text = "Break the ice to fish (risky but rewarding)";
    saveFood->choiceB_text = "Move on to find easier food";
    saveFood->isEnding = false;
    insertNode(fightBear, saveFood, false);

    // Search More - Take shelter in hollow
    DecisionNode* hollow = new DecisionNode();
    hollow->scenarioID = 110;
    hollow->description = "The hollow tree saves your life. You survive the blizzard barely. At dawn, you're weak but alive. You see smoke rising in the distance - humans or perhaps another wolf's den?";
    hollow->choiceA_text = "Investigate the smoke";
    hollow->choiceB_text = "Stay hidden and hunt nearby";
    hollow->isEnding = false;
    insertNode(searchMore, hollow, true);

    // Search More - Keep moving
    DecisionNode* keepMoving = new DecisionNode();
    keepMoving->scenarioID = 111;
    keepMoving->description = "You push through the storm with sheer willpower. You collapse from exhaustion but wake up in a protected valley. A deer herd grazes nearby - easy prey in your desperate state.";
    keepMoving->choiceA_text = "Hunt the deer while you can";
    keepMoving->choiceB_text = "Rest first, you're too weak";
    keepMoving->isEnding = false;
    insertNode(searchMore, keepMoving, false);

    // Cabin - Repair door
    DecisionNode* repairDoor = new DecisionNode();
    repairDoor->scenarioID = 112;
    repairDoor->description = "You spend energy repairing the door properly. The cabin is secure. You survive the storm in comfort. Days later, you're recovered and strong. Time to venture out and establish territory.";
    repairDoor->choiceA_text = "Scout for permanent den site";
    repairDoor->choiceB_text = "Stay in cabin and stockpile food";
    repairDoor->isEnding = false;
    insertNode(cabin, repairDoor, true);

    // Cabin - Block with furniture
    DecisionNode* blockDoor = new DecisionNode();
    blockDoor->scenarioID = 113;
    blockDoor->description = "The furniture blocks most of the wind, but the cabin is freezing. You burn furniture for warmth. You survive but the cabin is now unusable. You must move on.";
    blockDoor->choiceA_text = "Head north to higher ground";
    blockDoor->choiceB_text = "Head south to lower valleys";
    blockDoor->isEnding = false;
    insertNode(cabin, blockDoor, false);

    // Snowdrift - Follow tracks
    DecisionNode* followTracks = new DecisionNode();
    followTracks->scenarioID = 114;
    followTracks->description = "You follow the tracks and find a pack of 4 wolves. They're cautious but not hostile. Their alpha approaches you carefully. This is your chance to join a pack and survive together.";
    followTracks->choiceA_text = "Show submission and ask to join";
    followTracks->choiceB_text = "Show strength and offer alliance";
    followTracks->isEnding = false;
    insertNode(snowdrift, followTracks, true);

    // Snowdrift - Head west
    DecisionNode* headWest = new DecisionNode();
    headWest->scenarioID = 115;
    headWest->description = "You head toward human territory. You find a campsite with food scraps and supplies. But humans are dangerous. You hear vehicles approaching.";
    headWest->choiceA_text = "Grab food quickly and run";
    headWest->choiceB_text = "Hide and observe the humans";
    headWest->isEnding = false;
    insertNode(snowdrift, headWest, false);

    // Add endings for survival paths
    DecisionNode* survivalEnd1 = new DecisionNode();
    survivalEnd1->scenarioID = 120;
    survivalEnd1->description = "Against impossible odds, you survived the brutal wilderness. Your resourcefulness and determination kept you alive through the harshest conditions.";
    survivalEnd1->isEnding = true;
    survivalEnd1->endingText = "SURVIVOR'S TRIUMPH: You conquered the wilderness through pure survival instinct! Days survived: " + std::to_string(30);
    insertNode(repairDoor, survivalEnd1, true);

    DecisionNode* survivalEnd2 = new DecisionNode();
    survivalEnd2->scenarioID = 121;
    survivalEnd2->description = "You joined the pack and together survived the winter. Strength in numbers proved to be the key to survival.";
    survivalEnd2->isEnding = true;
    survivalEnd2->endingText = "PACK SURVIVOR: Together, you overcame the wilderness!";
    insertNode(followTracks, survivalEnd2, true);

    DecisionNode* survivalEnd3 = new DecisionNode();
    survivalEnd3->scenarioID = 122;
    survivalEnd3->description = "You learned to thrive in the harsh wilderness alone. Every challenge made you stronger.";
    survivalEnd3->isEnding = true;
    survivalEnd3->endingText = "LONE SURVIVOR: The wilderness respects your strength!";
    insertNode(eatNow, survivalEnd3, false);

    DecisionNode* survivalEnd4 = new DecisionNode();
    survivalEnd4->scenarioID = 123;
    survivalEnd4->description = "You found a balance between caution and boldness. Your wisdom kept you alive.";
    survivalEnd4->isEnding = true;
    survivalEnd4->endingText = "WISE SURVIVOR: Intelligence triumphed over brute force!";
    insertNode(saveFood, survivalEnd4, true);

    DecisionNode* survivalEnd5 = new DecisionNode();
    survivalEnd5->scenarioID = 124;
    survivalEnd5->description = "The wilderness tested you beyond measure, but you persevered. You are forever changed.";
    survivalEnd5->isEnding = true;
    survivalEnd5->endingText = "HARDENED SURVIVOR: What doesn't kill you makes you stronger!";
    insertNode(hollow, survivalEnd5, true);

    // ========== COMPLETE THE SURVIVAL TREE BY ADDING PROPER ENDINGS FOR NULL POINTERS ==========
    // Add proper endings to prevent fallback scenarios for any remaining null pointers

    // Add endings for any null pointers in the survival story
    if (!cave1->left) {
        DecisionNode* caveEndA = new DecisionNode();
        caveEndA->scenarioID = 125;
        caveEndA->description = "You fight the bear despite your injuries. It's a fierce battle, but your determination sees you through. You drive the bear away and claim the cave.";
        caveEndA->isEnding = true;
        caveEndA->endingText = "VICTORIOUS BATTLE: Your courage and determination secured shelter against all odds.";
        cave1->left = caveEndA;
    }

    if (!cave1->right) {
        DecisionNode* caveEndB = new DecisionNode();
        caveEndB->scenarioID = 126;
        caveEndB->description = "You search desperately for another shelter, but the storm catches you. You find a hollow tree just in time to survive the blizzard.";
        caveEndB->isEnding = true;
        caveEndB->endingText = "NARROW ESCAPE: Quick thinking saved you from the deadly storm.";
        cave1->right = caveEndB;
    }

    if (!run1->left) {
        DecisionNode* runEndA = new DecisionNode();
        runEndA->scenarioID = 127;
        runEndA->description = "You sprint to the cabin despite your energy depletion. You make it inside just as the storm hits, saving yourself from freezing.";
        runEndA->isEnding = true;
        runEndA->endingText = "TIMELY ARRIVAL: Your speed and timing saved you from the storm.";
        run1->left = runEndA;
    }

    if (!run1->right) {
        DecisionNode* runEndB = new DecisionNode();
        runEndB->scenarioID = 128;
        runEndB->description = "You dig into a snowdrift for shelter. It's not ideal, but it protects you from the worst of the storm. You survive to see another day.";
        runEndB->isEnding = true;
        runEndB->endingText = "RESOURCEFUL SURVIVAL: Making do with what you have can be enough to survive.";
        run1->right = runEndB;
    }

    // Final check: Convert any remaining null pointers to proper endings
    // This ensures the tree is completely connected with no null pointers
    std::queue<DecisionNode*> q;
    std::set<DecisionNode*> visited;
    q.push(root);
    visited.insert(root);

    while (!q.empty()) {
        DecisionNode* current = q.front();
        q.pop();

        if (!current->isEnding) {
            // If left is null, create an ending
            if (!current->left) {
                DecisionNode* ending = new DecisionNode();
                ending->scenarioID = 999; // Use high number to avoid conflicts
                ending->description = "Your journey comes to an end. The wilderness has tested you in ways you never imagined. Your choices have shaped your destiny.";
                ending->choiceA_text = "Reflect on your journey";
                ending->choiceB_text = "Accept your fate";
                ending->isEnding = true;
                ending->endingText = "JOURNEY'S END: Your adventure in the wilderness has concluded.";
                current->left = ending;
            } else if (visited.find(current->left) == visited.end()) {
                visited.insert(current->left);
                q.push(current->left);
            }

            // If right is null, create an ending
            if (!current->right) {
                DecisionNode* ending = new DecisionNode();
                ending->scenarioID = 1000; // Use high number to avoid conflicts
                ending->description = "Your path leads to its conclusion. The choices you've made have brought you to this moment.";
                ending->choiceA_text = "Embrace the outcome";
                ending->choiceB_text = "Learn from the experience";
                ending->isEnding = true;
                ending->endingText = "STORY'S END: Your tale in the wild has reached its conclusion.";
                current->right = ending;
            } else if (visited.find(current->right) == visited.end()) {
                visited.insert(current->right);
                q.push(current->right);
            }
        }
    }

    currentNode = root;
}

void DecisionTree::buildPackStory() {
    // Reset any existing tree before building new one
    reset();
    
    // Pack mode - focus on social dynamics and leadership
    root = new DecisionNode();
    root->scenarioID = 201;
    root->description = "PACK MODE: You are the omega of a struggling pack. The alpha is old and weak. Food is scarce, and tensions run high. Three pack members challenge the alpha's leadership tonight. You must choose a side.";
    root->choiceA_text = "Support the alpha (loyalty but risk losing)";
    root->choiceB_text = "Join the challengers (opportunity but betrayal)";
    root->isEnding = false;

    // Path A: Support alpha
    DecisionNode* supportAlpha = new DecisionNode();
    supportAlpha->scenarioID = 202;
    supportAlpha->description = "You stand beside the alpha. The challengers are impressed by your loyalty (+20 reputation). The alpha wins but is mortally wounded. With his dying breath, he names YOU as the new alpha. The pack looks to you for leadership.";
    supportAlpha->choiceA_text = "Accept leadership and prove yourself (hunt immediately)";
    supportAlpha->choiceB_text = "Suggest the beta take over (humble approach)";
    supportAlpha->isEnding = false;
    insertNode(root, supportAlpha, true);

    // Path B: Join challengers
    DecisionNode* joinChallengers = new DecisionNode();
    joinChallengers->scenarioID = 203;
    joinChallengers->description = "You join the challengers. Together, you overpower the alpha. The victory is swift. The lead challenger, Scar, becomes the new alpha. He respects your choice but doesn't fully trust you yet. (Loyalty: 50)";
    joinChallengers->choiceA_text = "Prove your worth by finding food for the pack";
    joinChallengers->choiceB_text = "Offer to scout for new territory";
    joinChallengers->isEnding = false;
    insertNode(root, joinChallengers, false);

    // Support Alpha - Accept leadership
    DecisionNode* acceptLeadership = new DecisionNode();
    acceptLeadership->scenarioID = 204;
    acceptLeadership->description = "You lead the pack on a hunt immediately to prove yourself. You track a deer herd. Your pack of 5 wolves looks to you. The beta suggests splitting up for better coverage.";
    acceptLeadership->choiceA_text = "Split the pack (efficient but risky)";
    acceptLeadership->choiceB_text = "Keep pack together (safer but harder hunt)";
    acceptLeadership->isEnding = false;
    insertNode(supportAlpha, acceptLeadership, true);

    // Support Alpha - Suggest beta
    DecisionNode* suggestBeta = new DecisionNode();
    suggestBeta->scenarioID = 205;
    suggestBeta->description = "The beta gratefully accepts leadership. You remain as second-in-command with high respect (+30 reputation). The pack's morale improves. However, a rival pack has been encroaching on your territory.";
    suggestBeta->choiceA_text = "Suggest aggressive defense of territory";
    suggestBeta->choiceB_text = "Suggest negotiation and sharing territory";
    suggestBeta->isEnding = false;
    insertNode(supportAlpha, suggestBeta, false);

    // Join Challengers - Find food
    DecisionNode* findFood = new DecisionNode();
    findFood->scenarioID = 206;
    findFood->description = "You successfully hunt along the stream, catching small fish and finding winter berries. [GAINED: Small Fish x4, Winter Berries x2, Fresh Water x1] Scar is impressed (+20 loyalty). He asks if you want to recruit more wolves to strengthen the pack.";
    findFood->choiceA_text = "Yes, bigger pack means more power";
    findFood->choiceB_text = "No, keep the pack small and agile";
    findFood->isEnding = false;
    insertNode(joinChallengers, findFood, true);

    // Join Challengers - Scout territory  
    DecisionNode* scoutTerritory = new DecisionNode();
    scoutTerritory->scenarioID = 207;
    scoutTerritory->description = "You discover rich hunting grounds to the north, but they're controlled by a powerful pack of 8 wolves. You also find a hidden valley to the east - smaller but unclaimed.";
    scoutTerritory->choiceA_text = "Report the northern grounds (challenge the rival pack)";
    scoutTerritory->choiceB_text = "Report the eastern valley (safe but limited)";
    scoutTerritory->isEnding = false;
    insertNode(joinChallengers, scoutTerritory, false);

    // Accept Leadership - Split pack
    DecisionNode* splitPack = new DecisionNode();
    splitPack->scenarioID = 208;
    splitPack->description = "You split the pack into two hunting groups. Your group successfully takes down a deer near the stream! The other group also succeeds with fish from the ice. The pack feasts tonight. Your leadership is proven. [GAINED: Fresh Meat x8, Small Fish x4]";
    splitPack->choiceA_text = "Celebrate and strengthen pack bonds";
    splitPack->choiceB_text = "Immediately plan next hunt (ambitious)";
    splitPack->isEnding = false;
    insertNode(acceptLeadership, splitPack, true);

    // Accept Leadership - Keep together
    DecisionNode* keepTogether = new DecisionNode();
    keepTogether->scenarioID = 209;
    keepTogether->description = "Your pack hunts as one unit. The coordination is perfect. You bring down a massive elk near the frozen lake! The pack celebrates your cautious but effective leadership. [GAINED: Fresh Meat x12, Small Fish x3]";
    keepTogether->choiceA_text = "Share meat equally with all";
    keepTogether->choiceB_text = "Reward the best hunters more";
    keepTogether->isEnding = false;
    insertNode(acceptLeadership, keepTogether, false);

    // Suggest Beta - Aggressive defense
    DecisionNode* aggressiveDefense = new DecisionNode();
    aggressiveDefense->scenarioID = 210;
    aggressiveDefense->description = "The beta leads an aggressive patrol. You encounter the rival pack at the border. A fight breaks out! Your pack wins but suffers injuries. The rivals retreat.";
    aggressiveDefense->choiceA_text = "Pursue them to show dominance";
    aggressiveDefense->choiceB_text = "Let them go and treat wounded";
    aggressiveDefense->isEnding = false;
    insertNode(suggestBeta, aggressiveDefense, true);

    // Suggest Beta - Negotiation
    DecisionNode* negotiate = new DecisionNode();
    negotiate->scenarioID = 211;
    negotiate->description = "You arrange a meeting with the rival alpha. They agree to share territory if you help them hunt large prey together occasionally. An alliance forms.";
    negotiate->choiceA_text = "Accept the alliance";
    negotiate->choiceB_text = "Reject, demand they leave entirely";
    negotiate->isEnding = false;
    insertNode(suggestBeta, negotiate, false);

    // Find Food - Recruit more
    DecisionNode* recruitMore = new DecisionNode();
    recruitMore->scenarioID = 212;
    recruitMore->description = "You recruit 3 lone wolves. Your pack grows to 8 strong! [PACK SIZE: 8] But food consumption increases dramatically. You need a large hunting ground.";
    recruitMore->choiceA_text = "Challenge rival pack for their territory";
    recruitMore->choiceB_text = "Search for unclaimed land further away";
    recruitMore->isEnding = false;
    insertNode(findFood, recruitMore, true);

    // Find Food - Keep small
    DecisionNode* keepSmall = new DecisionNode();
    keepSmall->scenarioID = 213;
    keepSmall->description = "Your small pack of 5 remains agile and efficient. You control a modest but sustainable territory. Life is good, though not glorious.";
    keepSmall->choiceA_text = "Be content with this life";
    keepSmall->choiceB_text = "Seek to expand slowly over time";
    keepSmall->isEnding = false;
    insertNode(findFood, keepSmall, false);

    // Scout Territory - Northern grounds
    DecisionNode* northernGrounds = new DecisionNode();
    northernGrounds->scenarioID = 214;
    northernGrounds->description = "Scar decides to challenge the northern pack for their rich territory. Your pack of 5 faces their pack of 8. It's a massive battle! Your pack fights bravely.";
    northernGrounds->choiceA_text = "Fight to the death";
    northernGrounds->choiceB_text = "Retreat strategically";
    northernGrounds->isEnding = false;
    insertNode(scoutTerritory, northernGrounds, true);

    // Scout Territory - Eastern valley
    DecisionNode* easternValley = new DecisionNode();
    easternValley->scenarioID = 215;
    easternValley->description = "Your pack claims the peaceful eastern valley. It's smaller but safe. You build a stable, peaceful territory. Life is calm and sustainable.";
    easternValley->choiceA_text = "Settle here permanently";
    easternValley->choiceB_text = "Use as base to expand later";
    easternValley->isEnding = false;
    insertNode(scoutTerritory, easternValley, false);

    // Pack endings
    DecisionNode* packEnd1 = new DecisionNode();
    packEnd1->scenarioID = 220;
    packEnd1->description = "Your pack has grown to become the most powerful in the region. Your leadership is legendary.";
    packEnd1->isEnding = true;
    packEnd1->endingText = "LEGENDARY ALPHA: Your pack dominates through strength and unity!";
    insertNode(recruitMore, packEnd1, true);

    DecisionNode* packEnd2 = new DecisionNode();
    packEnd2->scenarioID = 221;
    packEnd2->description = "Your diplomatic skills created a peaceful alliance. Two packs live in harmony.";
    packEnd2->isEnding = true;
    packEnd2->endingText = "WISE DIPLOMAT: Peace through understanding!";
    insertNode(negotiate, packEnd2, true);

    DecisionNode* packEnd3 = new DecisionNode();
    packEnd3->scenarioID = 222;
    packEnd3->description = "Your small but loyal pack thrives. Quality over quantity proved wise.";
    packEnd3->isEnding = true;
    packEnd3->endingText = "ELITE PACK: Small numbers, maximum efficiency!";
    insertNode(keepSmall, packEnd3, true);

    DecisionNode* packEnd4 = new DecisionNode();
    packEnd4->scenarioID = 223;
    packEnd4->description = "Through bold action, you proved yourself a decisive leader. The pack respects strength.";
    packEnd4->isEnding = true;
    packEnd4->endingText = "BOLD LEADER: Fortune favors the brave!";
    insertNode(splitPack, packEnd4, true);

    DecisionNode* packEnd5 = new DecisionNode();
    packEnd5->scenarioID = 224;
    packEnd5->description = "Your careful, considerate leadership built a strong foundation. The pack thrives under your wisdom.";
    packEnd5->isEnding = true;
    packEnd5->endingText = "THOUGHTFUL ALPHA: Wisdom and unity prevail!";
    insertNode(keepTogether, packEnd5, true);

    // ========== COMPLETE THE PACK TREE BY ADDING PROPER ENDINGS FOR NULL POINTERS ==========
    // Add proper endings to prevent fallback scenarios for any remaining null pointers

    // Add endings for any null pointers in the pack story
    if (!supportAlpha->left) {
        DecisionNode* alphaEndA = new DecisionNode();
        alphaEndA->scenarioID = 225;
        alphaEndA->description = "You accept leadership and immediately set out to prove yourself. Your decisive actions earn the respect of the pack.";
        alphaEndA->isEnding = true;
        alphaEndA->endingText = "NEW ALPHA: Your decisive leadership earned the pack's respect.";
        supportAlpha->left = alphaEndA;
    }

    if (!supportAlpha->right) {
        DecisionNode* alphaEndB = new DecisionNode();
        alphaEndB->scenarioID = 226;
        alphaEndB->description = "You humbly suggest the beta take over. The pack appreciates your wisdom and humility.";
        alphaEndB->isEnding = true;
        alphaEndB->endingText = "WISDOM FIRST: Sometimes stepping aside shows true leadership.";
        supportAlpha->right = alphaEndB;
    }

    if (!joinChallengers->left) {
        DecisionNode* challenderEndA = new DecisionNode();
        challenderEndA->scenarioID = 227;
        challenderEndA->description = "You successfully find food for the pack. Your contribution strengthens your position within the group.";
        challenderEndA->isEnding = true;
        challenderEndA->endingText = "VALUABLE CONTRIBUTOR: Your efforts benefited the whole pack.";
        joinChallengers->left = challenderEndA;
    }

    if (!joinChallengers->right) {
        DecisionNode* challenderEndB = new DecisionNode();
        challenderEndB->scenarioID = 228;
        challenderEndB->description = "You successfully scout for new territory. Your reconnaissance skills prove valuable to the pack.";
        challenderEndB->isEnding = true;
        challenderEndB->endingText = "EXPLORER: Your scouting opened new possibilities for the pack.";
        joinChallengers->right = challenderEndB;
    }

    // Final check: Convert any remaining null pointers to proper endings
    // This ensures the tree is completely connected with no null pointers
    std::queue<DecisionNode*> q;
    std::set<DecisionNode*> visited;
    q.push(root);
    visited.insert(root);

    while (!q.empty()) {
        DecisionNode* current = q.front();
        q.pop();

        if (!current->isEnding) {
            // If left is null, create an ending
            if (!current->left) {
                DecisionNode* ending = new DecisionNode();
                ending->scenarioID = 999; // Use high number to avoid conflicts
                ending->description = "Your journey comes to an end. The wilderness has tested you in ways you never imagined. Your choices have shaped your destiny.";
                ending->choiceA_text = "Reflect on your journey";
                ending->choiceB_text = "Accept your fate";
                ending->isEnding = true;
                ending->endingText = "JOURNEY'S END: Your adventure in the wilderness has concluded.";
                current->left = ending;
            } else if (visited.find(current->left) == visited.end()) {
                visited.insert(current->left);
                q.push(current->left);
            }

            // If right is null, create an ending
            if (!current->right) {
                DecisionNode* ending = new DecisionNode();
                ending->scenarioID = 1000; // Use high number to verify conflicts
                ending->description = "Your path leads to its conclusion. The choices you've made have brought you to this moment.";
                ending->choiceA_text = "Embrace the outcome";
                ending->choiceB_text = "Learn from the experience";
                ending->isEnding = true;
                ending->endingText = "STORY'S END: Your tale in the wild has reached its conclusion.";
                current->right = ending;
            } else if (visited.find(current->right) == visited.end()) {
                visited.insert(current->right);
                q.push(current->right);
            }
        }
    }

    currentNode = root;
}