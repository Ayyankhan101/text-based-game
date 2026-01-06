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
    // Default to classic story
    buildClassicStory();
}

void DecisionTree::buildClassicStory() {
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

void DecisionTree::buildSurvivalStory() {
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
    fightBear->description = "You attack the bear in a desperate fury! It awakens and mauls you badly (-25 health), but you drive it out into the storm. The cave is yours. You find the bear's food cache: dried fish and roots. [GAINED: Fish x3, Roots x2]";
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
    cabin->description = "You reach the cabin just as the storm hits! Inside, you find supplies: canned food, matches, and a first aid kit. [GAINED: Canned Food x5, First Aid Kit x1]. But the door is damaged and won't close properly.";
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

    currentNode = root;
}

void DecisionTree::buildPackStory() {
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
    findFood->description = "You successfully hunt rabbits alone, bringing back food for 3 days. [GAINED: Rabbit x6] Scar is impressed (+20 loyalty). He asks if you want to recruit more wolves to strengthen the pack.";
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
    splitPack->description = "You split the pack into two hunting groups. Your group successfully takes down a deer! The other group also succeeds. The pack feasts tonight. Your leadership is proven. [GAINED: Deer Meat x8]";
    splitPack->choiceA_text = "Celebrate and strengthen pack bonds";
    splitPack->choiceB_text = "Immediately plan next hunt (ambitious)";
    splitPack->isEnding = false;
    insertNode(acceptLeadership, splitPack, true);

    // Accept Leadership - Keep together
    DecisionNode* keepTogether = new DecisionNode();
    keepTogether->scenarioID = 209;
    keepTogether->description = "Your pack hunts as one unit. The coordination is perfect. You bring down a massive elk! The pack celebrates your cautious but effective leadership. [GAINED: Elk Meat x12]";
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

    currentNode = root;
}