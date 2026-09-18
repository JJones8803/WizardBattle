#pragma once
#include <iostream>
#include <vector>
#include <string>
#include <functional>

#include "Classes.h"
#include "Functions.h"

struct DialogueLine {
    std::string speaker;
    std::string text;
};

struct Encounter {
    std::string chapterTitle;
    std::vector<DialogueLine> introDialogue;
    std::function<Wizard(const Wizard& player)> generateEnemy;
    std::vector<DialogueLine> victoryDialogue;
    std::function<void(Wizard& player)> onVictoryReward;
};

void runStoryMode(Wizard& player);