#include <iostream>

#include "Functions.h"
#include "StoryMode.h"

static void playDialogue(const std::vector<DialogueLine>& dialogue){
    for (const auto& line : dialogue) {
        std::cout << "\n[" << line.speaker << "]\n";
        text_print(line.text, 35);
        std::cout << "(Press Enter to continue...)";
        std::cin.ignore(1000, '\n');
        std::cin.get();
    }
}

/*Remember the structure: 
struct Encounter {
    std::string chapterTitle;
    std::vector<DialogueLine> introDialogue;
    std::function<Wizard(const Wizard& player)> generateEnemy;
    std::vector<DialogueLine> victoryDialogue;
    std::function<void(Wizard& player)> onVictoryReward;
};
*/
static std::vector<Encounter> buildCampaign(){
    std::vector<Encounter> campaign;
    campaign.push_back({
        "Prologue---Element Monastery", //Chapter title---Setting
        {/*Dialogue context: Thing get tense between the MC and his rival (wizard weak to MC). 
        `In order to resolve this, Prof. Garadine, their mentor, sets up a battle for them to resolve their differences...*/
        //Dialogue format: {"[NAME]", "[TEXT]"}; Names so far: Prof. Garadine, MC, Rival, Tertiary Fool
            {"[Rival]", ""},
            {}
        },
    });

    return campaign;
}