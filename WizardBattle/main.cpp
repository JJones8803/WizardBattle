#include <iostream>
#include <vector>
#include <ctime>
#include <random>
#include <chrono>
#include "Classes.h"
#include "Functions.h"

using namespace std;

int main() {
    // Use a modern RNG seeded from random_device to avoid casting/truncation issues
    std::random_device rd;
    std::mt19937 rng(rd());

    Wizard player, npc;

    //intro text
    text_print("A Bombastic Battle of Wizards and Witches is brewing...", 50);
    cout << endl;

    // 1. INTRO SCREEN
    cout << "========================================" << endl;
    cout << "           WIZARD BATTLE 2.0            " << endl;
    cout << "========================================" << endl;
    cout << "      ()          *** " << endl;
    cout << "     /|\\          /                     " << endl;
    cout << "    / | \\        /                      " << endl;
    cout << "========================================" << endl;

    //2. WIZARD SELECTION
    vector<Wizard> roster = { createFireWizard(), createWaterWizard(), createEarthWizard() };
    int choice = -1;

    while (choice < 1 || choice > static_cast<int>(roster.size())) {
        cout << "\nSelect your caster: " << endl;
        for (size_t i = 0; i < roster.size(); ++i) {
            cout << "  " << (i + 1) << ". " << roster[i].getName() << " (" << roster[i].getType() << " Wizard)" << endl;
        }

        cout << "Choice: ";
        if (!(cin >> choice)) {
            cin.clear();
            cin.ignore(1000, '\n');
            continue;
        }
    }

    player = roster[choice - 1]; // Set the player to the chosen wizard (adjusting for 0-based index)

    // Choose an NPC using the modern RNG to avoid potential overflow in promotions/casts
    std::uniform_int_distribution<int> dist(0, static_cast<int>(roster.size()) - 1);
    npc = roster[dist(rng)];

    text_print("\n" + player.getName() + " VS " + npc.getName() + "!!", 50);
    text_print("FIGHT!!\n");

	//3. GAME STATE INITIALIZATION
    GameStatus status = GameStatus::IP;
	Player currentPlayer = Player::P1;
	int turnCount = 1;

	//4. MAIN GAME LOOP
    while (status == GameStatus::IP) {
        string turnTitle = (currentPlayer == Player::P1) ? "YOUR TURN" : "ENEMY TURN";

        BattleEngine::displayBattle(turnCount, turnTitle, player, npc);

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
            BattleEngine::spellCast(player, npc, selectedSpell);

        }
        else {
            //NPC's turn: Get the NPC's move and execute it
            text_print(npc.getName() + " is choosing a spell...");
            string npcSpell = BattleEngine::getNPCMove(npc);
			BattleEngine::spellCast(npc, player, npcSpell);
        }

        //5. POST-TURN CHECK: Check for win conditions after each turn and update the game status accordingly
        status = BattleEngine::checkWinCondition(player, npc);

        if (status == GameStatus::IP) {
            currentPlayer = (currentPlayer == Player::P1) ? Player::NPC : Player::P1; // Switch turns
            turnCount++;
            cout << "\nPress Enter to continue...";
            cin.ignore(); // Wait for user input before proceeding to the next turn
            cin.get();
        }
    }
	//6. END SCREEN: Display the results of the battle and the end screen based on who won
    BattleEngine::displayEndScreen(status);

    return 0;
}