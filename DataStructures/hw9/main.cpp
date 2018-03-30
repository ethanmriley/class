#include <fstream>
#include <string>
#include <iostream>
#include <vector>
#include <cmath>
#include "hashTable.h"
#include <assert.h>

//main file for hash table
//ethan riley, 2017


//reads genome into an std::string for easier manipulation
std::string getFullGenome(std::string input_file_name) {
	std::ifstream input_file;
	input_file.open(input_file_name.c_str());
	std::string genome = "";
	std::string line;
	while (input_file.good()) {
		input_file >> line;
		genome += line;
	}
	return genome;
}

int main() {
	std::string genome_file_name;
	std::string next_command;
	int table_size;
	float occupancy;
	int k;
	hashTable k_mers;
	std::cin >> genome_file_name >> genome_file_name;
	std::cin >> next_command;
	while (next_command != "kmer") {
		if (next_command == "table_size") {
			std::cin >> table_size;
			k_mers.setTableSize(table_size);
		} else if (next_command == "occupancy") {
			std::cin >> occupancy;
			k_mers.setTableOccupancy(occupancy);
		}
		std::cin >> next_command;
	}
	std::cin >> k;


	std::string genome = getFullGenome(genome_file_name);
	k_mers.setGenome(genome);
	k_mers.setTableK(k);
	k_mers.createTable();

	std::cin >> next_command;
	while (next_command != "quit") {
		assert(next_command == "query");
		int mismatches;
		std::string query_sequence;
		std::cin >> mismatches >> query_sequence;
		k_mers.query(query_sequence, mismatches);
		std::cin >> next_command;
	}
}