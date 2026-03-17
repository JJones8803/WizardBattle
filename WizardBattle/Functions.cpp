#include <iostream>
#include <chrono>
#include <thread>
#include <random>
#include <iterator>
#include <string>

#include "Functions.h"
#include "Classes.h"

 using namespace std;
 using namespace std::this_thread;
 using namespace std::chrono;
 using namespace std::chrono_literals;

void text_print(std::string_view text, int delay) {
    for (char c : text) {
        cout << c;
        cout.flush();
        sleep_for(milliseconds(delay));
    }
    cout << endl;
} 

// Overloads forwarding to the std::string_view implementation
void text_print(const std::string& text, int delay) {
    text_print(std::string_view(text), delay);
}

void text_print(const char* text, int delay) {
    text_print(std::string_view(text), delay);
}

GameStatus BattleEngine::checkWinCondition(const Wizard& wizard1, const Wizard& wizard2) {
    //wizard 2 wins
    if (wizard1.getHp() <= 0) return GameStatus::NPCWins;
    //Wizard 1 wins
    if (wizard2.getHp() <= 0) return GameStatus::P1Wins;

    return GameStatus::IP;
}

void BattleEngine::displayResults(GameStatus status, const Wizard& wizard1, const Wizard& wizard2) {
	switch (status) { //Displays different victory messages based on who won
        case GameStatus::P1Wins:
            std::cout << wizard2.getName() << " has been vanquished. " << wizard1.getName() << " is the victor!\n";
            break;

        case GameStatus::NPCWins:
            std::cout << wizard1.getName() << " has been vanquished. " << wizard2.getName() << " is the victor!\n";
            break;
    }
}

void BattleEngine::displayBattle(int turnCount, string_view turnPlayer, const Wizard& Playerwizard, const Wizard& EnemyWizard) {
    cout << "===== Turn " << turnCount << ", " << turnPlayer << "'s move =====\n"
         << "(You) " << Playerwizard.getName() << " (HP: " << Playerwizard.getHp() << ")\n";

	int spellIndex = 1;
	for (const auto& [spellName, spellInfo] : Playerwizard.getSpells()) {
        cout << "  " << spellIndex++ << ". " << spellName << " (DMG: " << spellInfo.damage << ", Uses left: " << spellInfo.uses << ")\n";
    }
    cout << "\n"
		<< "VS\n" << EnemyWizard.getName() << " (HP: " << EnemyWizard.getHp() << ")" << endl;
} 

void BattleEngine::spellCast(Wizard& caster, Wizard& target, const std::string& spellName) {
	auto& spells = caster.getSpells(); // Get a reference to the caster's spells for easy access
	auto& choice = spells[spellName]; // Get a reference to the chosen spell's info for easy access

    switch (choice.category) {
        case SpellType::Shield:
			caster.setIsShielded(true); // Activate shield status for the caster
			text_print(caster.getName() + " casts " + spellName + " and raises a magical shield!");
            break;

		case SpellType::Buff:
			caster.setIsBuffed(true); // Activate buff status for the caster;
            text_print(caster.getName() + " casts " + spellName + " and focuses their energy!");
            break;

        case SpellType::Attack:
            text_print(caster.getName() + " casts " + spellName + "!");
			executeTurn(caster, target, spellName); // Execute the attack logic for the chosen spell);
            break;
    }
    choice.uses--;
    if (choice.uses == 0){
		text_print(caster.getName() + " has exhausted " + spellName + "!");
		spells.erase(spellName); // Remove the spell from the caster's spell list if it's exhausted
    }
    
}

void BattleEngine::executeTurn(Wizard& attacker, Wizard& defender, const std::string& spellName) {
	auto& spells = attacker.getSpells(); // Get a reference to the attacker's spells for easy access
    int baseDamage = spells[spellName].damage;

    //Buff Check
    if (attacker.getIsBuffed()){
        baseDamage *= 2;
		attacker.setIsBuffed(false); // Reset buff status after applying the buff
        std::cout << attacker.getName() << "'s magic is empowered!\n";
    }

	//Shield Check
    if (defender.getIsShielded()) {
		baseDamage /= 2;
		defender.setIsShielded(false); // Reset shield status after applying the shield
        std::cout << defender.getName() << "'s defense is active!\n";
    }
	
    defender.takeDamage(baseDamage, attacker.getType()); // Apply damage to the defender, considering their type for weaknesses/resistances

}
//!!!!!Create basic decision tree at later date. For now, the NPC will just pick a random spell from their spell list to cast each turn.
std::string BattleEngine::getNPCMove(const Wizard& npc) {
	auto& spells = npc.getSpells(); //gets a reference to the NPC's spells for easy access
    if (spells.empty()) { return ""; }// Safety check: if the NPC has no spells, return an empty string (though ideally, this should never happen if wizards are properly initialized)

    if (npc.getHp() < 600 && !npc.getIsShielded()) {
        if ((rand() % 100 < 45)) {
            for (const auto& [spellName, spellInfo] : spells) {
                if (spellInfo.category == SpellType::Shield && spellInfo.uses > 0) return spellName; // Prioritize casting a shield spell if HP is low and not already shielded
			}
        }
    }
	//Basic default spell cast logic below: If the NPC is not in a critical HP state or already shielded, it will randomly select a spell from its spell list to cast.
	//1. Set up the Randomizer (the modern C++ way using <random> library):
	static std::random_device rd; // Seed for random number generator
	static std::mt19937 gen(rd()); // Mersenne Twister random number generator
	int maxIndex = static_cast<int>(spells.size()) - 1; // safe cast because spells is not empty here
	std::uniform_int_distribution<int> dis(0, maxIndex); // Distribution to select a random spell index based on the number of spells available

    //2. Pick a random index from the spell list
	int randomIndex = dis(gen); // randomly select an index for the spell to cast

    //3. Move an iterator to that position 
	auto it = spells.begin();
	std::advance(it, static_cast<std::ptrdiff_t>(randomIndex)); // Move the iterator to the randomly selected index

	//4. Return the name of the spell (the key of the map) corresponding to the randomly selected index
	return it->first; 
}

std::string BattleEngine::getPlayerMove(const Wizard& player) {
    int input;
    cin >> input;
    return BattleEngine::getSpellNameByIndex(player, input);
}


bool BattleEngine::isValidMove(const Wizard& caster, const std::string& spellName) {
	const auto& spells = caster.getSpells();

    if (spellName.empty()) {
        text_print("Please enter the correct input!", 30);
		return false;
    }
    if (spells.count(spellName) == 0){
        text_print("You don't know that one!");
        return false;
    }
    if (spells.at(spellName).uses <= 0) {
        text_print("That spell is exhausted!");
		return false;
    }
    return true;
}

std::string BattleEngine::getSpellNameByIndex(const Wizard& caster, int index) {
    const auto& spells = caster.getSpells();

	if (index <= 0) {
		return ""; // Return empty string for invalid index (non-positive input)
	}

	size_t target = static_cast<size_t>(index - 1); // Convert to 0-based index for map access

    if (target >= spells.size()){
		return ""; // Return empty string for invalid index
	}

	auto it = spells.begin();
	std::advance(it, static_cast<std::ptrdiff_t>(target)); // Move iterator to the desired index
	return it->first; // Return the spell name (key) for the map at the specified index
}

void BattleEngine::displayEndScreen(GameStatus status) {
    std::cout << "\n\n========================================" << std::endl;

    if (status == GameStatus::P1Wins) {
        std::cout << "          CONGRATULATIONS!              " << std::endl;
        std::cout << "       YOU ARE THE ARCHMAGE!            " << std::endl;
        std::cout << "               (^_^)                     " << std::endl;
        std::cout << "              /|   |\\                   " << std::endl;
    }
    else if (status == GameStatus::NPCWins) {
        std::cout << "             GAME OVER                  " << std::endl;
        std::cout << "       THE DARKNESS PREVAILS...         " << std::endl;
        std::cout << "               (X_X)                     " << std::endl;
    }
    std::cout << "========================================" << std::endl;
    std::cout << "          Thank you for playing!        " << std::endl;
    std::cout << "========================================\n" << std::endl;
}

