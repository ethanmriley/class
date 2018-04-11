#include <iostream>
#include <vector>
#include "Word.h"
//Grid class header for wordsearch
//ethan riley, 2017

class Grid {
	public:
		Grid(int input_height, int input_width);
		bool add(const Word& word);
		void print() const;
		Grid copy() const;
		std::vector<std::vector<char> > board;
		int height;
		int width;
		void display() const;
		
};

bool operator==(const Grid& grid1, const Grid& grid2);