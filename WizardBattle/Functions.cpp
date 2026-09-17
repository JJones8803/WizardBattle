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

void text_print(std::string_view text, int delay) { //delay = 100 until told otherwise
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
            int outgoingDamage = choice.damage;

            //Resolve attacker's buff here before sending damage to defender
            if (caster.getIsBuffed()){
                outgoingDamage *= 2;
                caster.setIsBuffed(false);
                text_print(caster.getName() + "'s magic is empowered by their focus...");
            }
            //pass the defense/Affinity calculations off to the target
            target.takeDamage(outgoingDamage, caster.getType());
            break;
        }
    choice.uses--;
    if (choice.uses == 0){
	    text_print(caster.getName() + " has exhausted " + spellName + "!");
	    spells.erase(spellName); // Remove the spell from the caster's spell list if it's exhausted
    }    
}

std::string BattleEngine::getNPCMove(const Wizard& npc, const Wizard& opponent) {
    const auto& spells = npc.getSpells();
    
    std::string bestSpell = "";
    int highestScore = -1000;

    // Calculate current health percentage
    double hpRatio = static_cast<double>(npc.getHp()) / static_cast<double>(npc.getMaxHP());

    for (const auto& entry : spells) {
        const std::string& name = entry.first;
        const SpellInfo& spell = entry.second;

        // Never consider depleted spells
        if (spell.uses <= 0) {
            continue;
        }

        int score = 0;

        switch (spell.category) {
            case SpellType::Shield:
                // If already shielded, do not waste a turn shielding again
                if (npc.getIsShielded()) {
                    score = -500;
                } 
                // Critical danger: Prioritize heavily
                else if (hpRatio < 0.35) {
                    score = 250; 
                } 
                // Moderate damage taken: Reasonable defensive option
                else if (hpRatio < 0.65) {
                    score = 80;
                } 
                // High HP: Low priority to shield
                else {
                    score = 10;
                }
                break;

            case SpellType::Buff:
                // Don't re-buff if already empowered
                if (npc.getIsBuffed()) {
                    score = -500;
                }
                // Don't waste time buffing if about to die
                else if (hpRatio < 0.25) {
                    score = 5;
                }
                // High health is the prime time to set up big combos
                else if (hpRatio > 0.70) {
                    score = 120;
                } 
                else {
                    score = 40;
                }
                break;

            case SpellType::Attack:
                // Base attack value is tied to raw damage potential
                score = spell.damage;

                // 1. If currently buffed, prioritize your biggest nuke to maximize multiplier
                if (npc.getIsBuffed()) {
                    score *= 2; 
                }

                // 2. Kill shot check: If this attack will defeat the opponent right now, TAKE IT!
                if (spell.damage >= opponent.getHp()) {
                    score += 500; 
                }

                // 3. Low HP Desperation: If low on health and unshielded, prioritize faster/heavier hits
                if (hpRatio < 0.40 && !npc.getIsShielded()) {
                    score += 40;
                }
                break;
        }

        // Track the highest-scoring action
        if (score > highestScore) {
            highestScore = score;
            bestSpell = name;
        }
    }

    // Fallback: If somehow all scoring failed, return the first spell with uses > 0
    if (bestSpell.empty()) {
        for (const auto& entry : spells) {
            if (entry.second.uses > 0) return entry.first;
        }
    }

    return bestSpell;
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

GameStatus BattleEngine::runBattle(Wizard& player, Wizard& enemy){
	//GAME STATE INITIALIZATION
    GameStatus status = GameStatus::IP;
	Player currentPlayer = Player::P1;
	int turnCount = 1;

	//MAIN GAME LOOP
    while (status == GameStatus::IP) {
        string turnTitle = (currentPlayer == Player::P1) ? "YOUR TURN" : "ENEMY TURN";

        BattleEngine::displayBattle(turnCount, turnTitle, player, enemy);

        if (currentPlayer == Player::P1) {
            // Player's turn: Get the player's move and execute it
            bool validMove = false;
            string selectedSpell = "";

            while (!validMove) {
                int spellIdx = -1;
                cout << "Select a spell: ";
                if (!(cin >> spellIdx)) {
                    cin.clear();
                    cin.ignore(1000, '\n');
                    continue;
                }

                selectedSpell = BattleEngine::getSpellNameByIndex(player, spellIdx);

                if (BattleEngine::isValidMove(player, selectedSpell)) {
                    validMove = true;
                }
                else {
                    text_print("Invalid spell, choose another.");
                }
            }
            BattleEngine::spellCast(player, enemy, selectedSpell);

        }
        else {
            //NPC's turn: Get the NPC's move and execute it
            text_print(enemy.getName() + " is choosing a spell...");
            string npcSpell = BattleEngine::getNPCMove(enemy, player);
			BattleEngine::spellCast(enemy, player, npcSpell);
        }

        //POST-TURN CHECK: Check for win conditions after each turn and update the game status accordingly
        status = BattleEngine::checkWinCondition(player, enemy);

        if (status == GameStatus::IP) {
            currentPlayer = (currentPlayer == Player::P1) ? Player::NPC : Player::P1; // Switch turns
            turnCount++;
            cout << "\nPress Enter to continue...";
            cin.ignore(); // Wait for user input before proceeding to the next turn
            cin.get();
        }
    }
    return status;
}

//Story mode implementation
//ADD: dialouge function that does the text_print function after every "."
void runStoryMode(Wizard& player){
    vector<Wizard> storyCampaign ={
        
    };
}