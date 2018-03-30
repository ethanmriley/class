#include "pokemon.h"
#include <iostream>

Pokemon::Pokemon(const f_type& facts_) {
	facts = facts_;
	color = facts.find("color")->second[0];
	catch_rate = facts.find("catchRate")->second[0];
	egg_groups = facts.find("eggGroups")->second;
}

std::string Pokemon::getLabel() const {
	std::vector<std::string> label_vec = facts.find("label")->second;
	return label_vec[0];
}

std::vector<std::string> Pokemon::getEggGroups() const {
	return egg_groups;
}

//reads through every egg group of two pokemon to check for shared groups
bool Pokemon::SharesEggGroup(const Pokemon* p) const{
	std::vector<std::string> groups_1 = this->egg_groups;
	std::vector<std::string> groups_2 = p->getEggGroups();
	bool shared = false;
	for (unsigned int i = 0; i < groups_1.size(); i++) {
		for (unsigned int j = 0; j < groups_2.size(); j++) {
			if (groups_1[i] == groups_2[j]) {
				shared = true;
			}
		}
	}
	return shared;
}

//I can identify all List 8 pokemon with their catch rate and color 
NidoranMale::NidoranMale(const f_type& facts_) : Pokemon(facts_), Monster(facts_), Field(facts_) {
	if (this->catch_rate != "235" && this->catch_rate != "120" && this->catch_rate != "45") {
		throw -999; //throw an integer (to be caught by main.cpp) if the facts passed to the constructor don't match it
	}
}

Nidorino::Nidorino(const f_type& facts_) : Pokemon(facts_), NidoranMale(facts_) {
	if (this->catch_rate != "120" && this->catch_rate != "45") {
		throw -999;
	}
}

Nidoking::Nidoking(const f_type& facts_) : Pokemon(facts_), Nidorino(facts_) {
	if (this->catch_rate != "45") {
		throw -999;
	}
}

Seel::Seel(const f_type& facts_) : Pokemon(facts_), Water1(facts_), Field(facts_) {
	if (not((this->catch_rate == "190" && this->color == "White") || (this->catch_rate == "75" && this->color == "White"))) {
		throw -999;
	}
}

Dewgong::Dewgong(const f_type& facts_) : Pokemon(facts_), Seel(facts_) {
	if (not(this->catch_rate == "75" && this->color == "White")) {
		throw -999;
	}
}

Shellos::Shellos(const f_type& facts_) : Pokemon(facts_), Water1(facts_), Amorphous(facts_) {
	if (not((this->catch_rate == "190" && this->color == "Purple") || (this->catch_rate == "75" && this->color == "Purple"))) {
		throw -999;
	}
}

Gastrodon::Gastrodon(const f_type& facts_) : Pokemon(facts_), Shellos(facts_) {
	if (not(this->catch_rate == "75" && this->color == "Purple")) {
		throw -999;
	}
}

Finneon::Finneon(const f_type& facts_) : Pokemon(facts_), Water2(facts_) {
	if (not((this->catch_rate == "190" && this->color == "Blue") || (this->catch_rate == "75" && this->color == "Blue"))) {
		throw -999;
	}
}

Lumineon::Lumineon(const f_type& facts_) : Pokemon(facts_), Finneon(facts_) {
	if (not(this->catch_rate == "75" && this->color == "Blue")) {
		throw -999;
	}
}

Basculin::Basculin(const f_type& facts_) : Pokemon(facts_), Water2(facts_) {
	if (this->catch_rate != "25") {
		throw -999;
	}
}