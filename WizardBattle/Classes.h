#pragma once
#include <string>
#include <string_view>
#include <map>
#include <vector>
#include <unordered_map>

enum class Player{ P1, NPC };
enum class GameStatus{ IP, P1Wins, NPCWins };

//Allows unique names without compromising clarity in the code.
enum class SpellType{ Attack, Buff, Shield };

//OVERHAUL: Affinity allows for smoother damage checks (1.5x & 0.75x respectfully)
enum class Affinity{ Neutral, Advantage, Disadvantage };
struct SpellInfo {
	int damage;
	int uses;
	SpellType category; //The spell's DNA. 
};

//OVERHAUL: Element System class to pair with affinity checks
class ElementSystem {
public:
//returns the affinity relationship between an attacker element and a defender element
//pairs are encoded as "Attacker->Defender"
	static Affinity getAffinity(const std::string& attacker, const std::string& defender){
		static const std::unordered_map<std::string, Affinity> typeMatrix = {
			{"Fire->Earth", Affinity::Advantage},
			{"Fire->Water", Affinity::Disadvantage},

			{"Water->Fire", Affinity::Advantage},
			{"Water->Earth", Affinity::Disadvantage},
			
			{"Earth->Water", Affinity::Advantage},
			{"Earth->Fire", Affinity::Disadvantage}
		};
		std::string key = attacker + "->" + defender;
		auto it = typeMatrix.find(key);
		return (it != typeMatrix.end()) ? it->second : Affinity::Neutral;
	}
	static std::string getWeakerElement(const std::string& element) {
		if (element == "Fire") return "Earth";
		if (element == "Water") return "Fire";
		if (element == "Earth") return "Water";
		return "";
	}
	static std::string getCounterElement(const std::string& element){
		if (element == "Fire") return "Water";
		if (element == "Water") return "Earth";
		if (element == "Earth") return "Fire";
		return "";	
	}
};

class Wizard {
private:
	int hp = 2500;
	int maxHp = 2500;
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
		int hp = 2500, int maxHp = 2500);
	~Wizard();

	int getHp() const;
	int getMaxHP() const;
	std::string getName() const;
	std::string getType() const;
	std::map<std::string, SpellInfo>& getSpells();
	const std::map<std::string, SpellInfo>& getSpells() const;
	std::vector<std::string> getResistances() const;
	std::vector<std::string> getWeaknesses() const;
	bool getIsShielded() const;
	bool getIsBuffed() const;
	void setHp(int newHp);
	void setMaxHp(int newHp);
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

Wizard createMartialArtist();

Wizard createNoboro();

Wizard createWizardByType(const std::string& type);

