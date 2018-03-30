#include <vector>
#include <string>

//header file for hash table with linear probing
//ethan riley, 2017

class hashTable {

private:
	std::vector<std::pair<std::string, int> > data;
	std::string genome;
	int hash(std::string key) const;
	void linearProbe(int index) const;
	void insert(int index, std::pair<std::string, int> k_mer);
	void resize();
	std::vector<std::pair<std::string, int> > trawl(std::string genome) const;
	unsigned int k; //k-mer length
	int size; //size of table
	float occupancy;

public:
	hashTable();
	int getTableSize() const;
	void setTableSize(int size);
	void setGenome(std::string genome);
	void createTable();
	void setTableK(int new_k);
	void setTableOccupancy(float new_occupancy);
	void printTable() const;
	void query(std::string sequence, int mismatches) const;


};