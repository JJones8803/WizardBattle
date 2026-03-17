#pragma once
#include <string>
#include <string_view>
#include <map>
#include <vector>

enum class Player{P1, NPC};
enum class GameStatus{IP, P1Wins, NPCWins};

//Allows unique names without compromising clarity in the code.
enum class SpellType { Attack, Buff, Shield };
struct SpellInfo {
	int damage;
	int uses;
	SpellType category; //The spell's DNA. 
};

class Wizard {
private:
	int hp = 2500;
	std::string name, type;
	std::map<std::string, SpellInfo> spells;
	std::vector<std::string> resistances;
	std::vector<std::string> weaknesses;
	bool isShielded = false;
	bool isBuffed = false;

public:
	Wizard() = default; // <-- Add this line to explicitly declare the default constructor
	
	// Parameterized constructor to create specific wizard types
	Wizard(const std::string& name, const std::string& type,
		const std::map<std::string, SpellInfo>& spells,
		const std::vector<std::string>& resistances,
		const std::vector<std::string>& weaknesses,
		int hp = 2500);
	~Wizard();

	int getHp() const;
	std::string getName() const;
	std::string getType() const;
	std::map<std::string, SpellInfo>& getSpells();
	const std::map<std::string, SpellInfo>& getSpells() const;
	std::vector<std::string> getResistances() const;
	std::vector<std::string> getWeaknesses() const;
	bool getIsShielded() const;
	bool getIsBuffed() const;
	void setHp(int newHp);
	void setName(std::string newName);
	void setType(std::string newType);
	void setSpells(std::map<std::string, SpellInfo> newSpells);
	void setResistances(std::vector<std::string> newResistances);
	void setWeaknesses(std::vector<std::string> newWeakness);
	void setIsShielded(bool shielded);
	void setIsBuffed(bool buffed);
	void takeDamage(int rawDamage, const std::string& attackType);
};

Wizard createFireWizard();

Wizard createWaterWizard();

Wizard createEarthWizard();