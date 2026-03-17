#pragma once
#include "Classes.h"

class Wizard; // Forward declaration: "I don't need the whole file, just know this name exists."

void text_print(std::string_view text, int delay=100);
// Overloads to accept common string types (helps with calls using std::string or string literals)
void text_print(const std::string& text, int delay=100);
void text_print(const char* text, int delay=100);

namespace BattleEngine {
	GameStatus checkWinCondition(const Wizard& wizard1, const Wizard& wizard2);
	void displayResults(GameStatus status, const Wizard& wizard1, const Wizard& wizard2);
	void displayBattle(int turnCount, std::string_view turnPlayer, const Wizard& wizard1, const Wizard& wizard2);
	void spellCast(Wizard& caster, Wizard& target, const std::string& spellName);
	void executeTurn(Wizard& attacker, Wizard& defender, const std::string& spellName);
	std::string getPlayerMove(const Wizard& player);
	std::string getNPCMove(const Wizard& npc);
	bool isValidMove(const Wizard& caster, const std::string& spellName);
	std::string getSpellNameByIndex(const Wizard& caster, int index);
	void displayEndScreen(GameStatus status);
}