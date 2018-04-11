#include "hashTable.h"
#include <cmath>
#include <assert.h>
#include <iostream>

//implementation of hash table with linear probing and FNV hash
//ethan riley, 2017

//populates table with empty values to appropriate size
void hashTable::setTableSize(int size) {
	for (int i = 0; i < size; i++) {
		data.push_back(std::make_pair("empty", -9999));
	}
	this->size = size;
}

void hashTable::setTableK(int new_k) {
	this->k = new_k;
}

void hashTable::setTableOccupancy(float new_occupancy) {
	this->occupancy = new_occupancy;
}

//default constructor
//sets size to 100 and occupancy to .5
hashTable::hashTable() {
	setTableSize(100);
	this->occupancy = .5;
}

//reads through the genome and breaks it up into k-mers of length k 
std::vector<std::pair<std::string, int> > hashTable::trawl(std::string genome) const {
	unsigned int start = 0;
	std::vector<std::pair<std::string, int> > k_mers;
	//while there are valid sequences of length k
	while (start + this->k < genome.length()) {
		unsigned int i = 0;
		std::string sequence = "";
		while (i < this->k) {
			sequence += genome[start + i];
			i++;
		}
		std::pair<std::string, int> k_mer = std::make_pair(sequence, start);
		k_mers.push_back(k_mer);
		start++;
	}
	return k_mers;
}


//hash function, credit to https://stackoverflow.com/questions/98153/whats-the-best-hashing-algorithm-to-use-on-a-stl-string-when-using-hash-map
/* magic numbers from http://www.isthe.com/chongo/tech/comp/fnv/ */

static const unsigned int InitialFNV = 2166136261U;
static const unsigned int FNVMultiple = 16777619;

int hashTable::hash(std::string key) const {
	
	/* Fowler / Noll / Vo (FNV) Hash */
    unsigned int hash = InitialFNV;
    for(unsigned int i = 0; i < key.length(); i++)
    {
        hash = hash ^ (key[i]);       /* xor  the low 8 bits */
        hash = hash * FNVMultiple;  /* multiply by the magic number */
    }
    return hash%(this->size);

}


//uses linear probing to insert the key into the table
void hashTable::insert(int index, std::pair<std::string, int> k_mer) {
	if(data[index].first == "empty") {
		//the hash location works, let's put the k-mer here
		data[index] = k_mer;
	} else {
		//the hash location is filled, so we have to do linear probing to find another
		while(data[index].first != "empty") {
			assert(index < data.size());
			index = (index + 1)%this->size;
		}

		data[index] = k_mer;
	}
}

void hashTable::resize() {
	std::vector<std::pair<std::string, int> > old_data = this->data; //copy entries from old table
	data.clear();                                                    //clear old table   
	setTableSize(size*2);                                            //double table size
	for (unsigned int i = 0; i < old_data.size(); i++) {             //re-insert entries into new table
		int index = hash(old_data[i].first);
		insert(index, old_data[i]);
	}
}


//uses the trawl function to create all k-mers from genome
//then inserts those k-mers into the table
//also handles resizing
void hashTable::createTable() {
	std::vector<std::pair<std::string, int> > k_mers = trawl(this->genome);
	int occupied = 0;
	for (unsigned int i = 0; i < k_mers.size(); i++, occupied++) {
		if (((float)occupied / this->size) > this->occupancy) {
			resize();
		}
		int index = hash(k_mers[i].first);
		insert(index, k_mers[i]);			
	}
}

int hashTable::getTableSize() const {
	return this->size;
}


//helper function for debugging
void hashTable::printTable() const {
	int occupied = 0;
	for (int i = 0; i < this->size; i++) {
		if (data[i].first != "empty") {
			std::cout << data[i].first << " " << data[i].second << std::endl;
			occupied++;
		}
	}
	std::cout << occupied << " of " << data.size() << " used!" << std::endl;
}


//returns the genome locations of a search sequence, to some user-specified degree of accuracy
void hashTable::query(std::string sequence, int mismatches) const {
	std::string seed = "";
	std::vector<int> seed_locations;

	//generate seed of size k to search the table with
	for (unsigned int i = 0; i < this->k; i++) {
		seed += sequence[i];
	}

	//hash the seed to find its location in the table
	unsigned int index = hash(seed);

	//linear probe from hash location to find all matching seeds
	while (data[index].first != "empty") {
		if (data[index].first == seed) {	
			seed_locations.push_back(data[index].second);
		}
		index = (index + 1)%this->size;
	}

	bool found = false;
	std::cout << "Query: " << sequence << std::endl;
	for (unsigned int j = 0; j < seed_locations.size(); j++) {
		int m = 0;
		int k = 0;
		std::string found_sequence = "";
		//while the sequence is valid, read through the genome and add characters onto the seed
		while(m <= mismatches && found_sequence.length() < sequence.length()) {
			char next_char = genome[seed_locations[j] + k];
			if (next_char != sequence[k]) {
				m++;
				if (m > mismatches) {
					break;
				}
			}
			found_sequence += next_char;
			k++;
		}
		//if a sequence matches the query length after checking for mismatches, it is valid
		if (found_sequence.length() == sequence.length()) {
			found = true;
			std::cout << seed_locations[j] << " " << m << " " << found_sequence << std::endl;
		}
	}
	if (not(found)){
		std::cout << "No Match" << std::endl;
	}
}

void hashTable::setGenome(std::string genome) {
	this->genome = genome;
}