#include "Classes.h"
#include <iostream>

Wizard::Wizard(const std::string& name, const std::string& type,
	const std::map<std::string, SpellInfo>& spells,
	const std::vector<std::string>& resistances,
	const std::vector<std::string>& weaknesses,
	int hp)
	: hp(hp),
	  name(name),
	  type(type),
	  spells(spells),
	  resistances(resistances),
	  weaknesses(weaknesses),
	  isShielded(false),
	  isBuffed(false) 
{}

Wizard::~Wizard(){}

int Wizard::getHp() const {
	return hp;
}

std::string Wizard::getName() const {
	return name;
}

std::string Wizard::getType() const {
	return type;
}

// Non-const version allows modification of spells, while const version does not
std::map<std::string, SpellInfo>& Wizard::getSpells() {
	return spells;
}

// Const version for read-only access to spells
const std::map<std::string, SpellInfo>& Wizard::getSpells() const {
	return spells;
}

std::vector<std::string> Wizard::getResistances() const {
	return resistances;
}

std::vector<std::string> Wizard::getWeaknesses() const {
	return weaknesses;
}

bool Wizard::getIsShielded() const {
	return isShielded;
}

bool Wizard::getIsBuffed() const {
	return isBuffed;
}

void Wizard::setHp(int newHp) {
	hp = newHp;
}

void Wizard::setIsShielded(bool shielded) {
	isShielded = shielded;
}

void Wizard::setIsBuffed(bool buffed) {
	isBuffed = buffed;
}
void Wizard::setName(std::string newName){
	name = newName;
}

void Wizard::setType(std::string newType){
	type = newType;
}

void Wizard::setSpells(std::map<std::string, SpellInfo> newSpells){
	spells = newSpells;
}

void Wizard::setResistances(std::vector<std::string> newResistances){
	resistances = newResistances;
}

void Wizard::setWeaknesses(std::vector<std::string> newWeakness) {
	weaknesses = newWeakness;
} 

void Wizard::takeDamage(int rawDamage, const std::string& attackType) {
	int finalDamage = rawDamage;

	for (const std::string& w : weaknesses) {
		if (w == attackType) {
			finalDamage *= 2; // Double damage if the attack type is a weakness
			std::cout << "It hits HARD! "; 
		}
	}

	for (const std::string& r : resistances) {
		if (r == attackType) {
			finalDamage /= 2; // Double damage if the attack type is a weakness
			std::cout << "It hits like a sock...";
		}
	}

	this->hp -= finalDamage; // Subtract the final damage from the wizard's HP
	if (this->hp < 0) {
		this->hp = 0; // Ensure HP doesn't go below 0
	}

	std::cout << name << " took " << finalDamage << " damage!\n";
}

Wizard createFireWizard() {
	Wizard wiz;
	wiz.setName("Pyron, the Fire Wizard");
	wiz.setType("Fire");
	wiz.setSpells({
		{"Fireball", {150, 10, SpellType::Attack}},
		{"Smelter Skleter", {300, 3, SpellType::Attack}},
		{"Primordal Chaos",{450, 1, SpellType::Attack}},
		{"|Fire wall|", {0, 15, SpellType::Shield}},
		{"+Hair Spray of Doom!+", {0, 15, SpellType::Buff}} 
		});
	wiz.setResistances({ "Earth" });
	wiz.setWeaknesses({ "Water" });
	return wiz;
}

Wizard createWaterWizard() {
	Wizard wiz;
	wiz.setName("Tonga, the Water Witch");
	wiz.setType("Water");
	wiz.setSpells({
		{"Water Spout", {150, 10, SpellType::Attack}},
		{"Dreadful Torrent", {300, 3, SpellType::Attack}},
		{"Atlantean Sorrows",{450, 1, SpellType::Attack}},
		{"|Bubble Shield|", {0, 15, SpellType::Shield}},
		{"+The Leviathan's Call!+", {0, 15, SpellType::Buff}}
		});
	wiz.setResistances({ "Fire" });
	wiz.setWeaknesses({ "Earth" });
	return wiz;
}

Wizard createEarthWizard() {
	Wizard wiz;
	wiz.setName("Gandora, the Earth Druid");
	wiz.setType("Earth");
	wiz.setSpells({
		{"Leaf Blade", {150, 10, SpellType::Attack}},
		{"Pot Shot Rock", {300, 3, SpellType::Attack}},
		{"Mother Earth's Last Stand",{450, 1, SpellType::Attack}},
		{"|Diamond Encrusted Barrier|", {0, 15, SpellType::Shield}},
		{"+Mother Earth's Gift of Strength!+", {0, 15, SpellType::Buff}}
		});
	wiz.setResistances({ "Water" });
	wiz.setWeaknesses({ "Fire" });
	return wiz;
}