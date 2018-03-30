#include "Grid.h"
#include <sstream>
#include <string>
#include <fstream>
//Grid class implementation for wordsearch
//ethan riley, 2017

Grid::Grid(int input_height, int input_width) {
	//populate the board with . chars to the appropriate dimensions
	std::vector<char> row;
	height = input_height;
	width = input_width;
	for(int i = 0; i < width; i++) {
		row.push_back('.');
	}

	for(int j = 0; j < height; j++) {
		board.push_back(row);
	}
}

//helper function to determine end bounds, given word length and orientation
int* findBounds(unsigned int word_orientation, unsigned int word_length, int start_x, int start_y) {
	int end_x;
	int end_y;
	int* end_bounds = new int[2];

	//i have to manually switch depending on orientation
	if(word_orientation==  0) {
		end_y = start_y + word_length - 1;
		end_x = start_x;
	} else if(word_orientation== 1) {
		end_y = start_y + word_length - 1;
		end_x = start_x + word_length - 1;
	} else if(word_orientation== 2) {
		end_y = start_y;
		end_x = start_x + word_length - 1;
	} else if(word_orientation== 3) {
		end_y = start_y - word_length + 1;
		end_x = start_x + word_length - 1;
	} else if(word_orientation== 4) {
		end_y = start_y - word_length + 1;
		end_x = start_x;
	} else if(word_orientation== 5) {
		end_y = start_y - word_length + 1;
		end_x = start_x - word_length + 1;
	} else if(word_orientation== 6) {
		end_y = start_y;
		end_x = start_x - word_length + 1;
	} else if(word_orientation== 7) {
		end_y = start_y + word_length - 1;
		end_x = start_x - word_length + 1;
	} else {
		std::cerr << "weird orientation error with orientation " << word_orientation << std::endl;
		return NULL;
	}
	end_bounds[0] = end_x;
	end_bounds[1] = end_y;

	return end_bounds;
}

//big function to add words to a grid, complete with validity checking
bool Grid::add(const Word& word) {
	unsigned int word_orientation = word.getOrientation();
	unsigned int word_length = word.getLength();
	int start_x;
	int start_y;
	int* end_bounds;
	start_x = word.start_point[0];
	start_y = word.start_point[1];

	std::string word_letters = word.getLetters();

	int end_x, end_y;
	end_bounds = findBounds(word_orientation, word_length, start_x, start_y);
	end_x = end_bounds[0];
	end_y = end_bounds[1];
	if(end_x >= width || end_y >= height || end_y < 0 || end_x < 0) { //if the addition would go out of bounds, discard it
		return false;
	}
	else{
		//we know the addition fits
		//now we check for conflicts with other words
		int i = start_y;
		int j = start_x;
		unsigned int k = 0;
		if(word_orientation== 0){

			for(; k < word_length; i++, k++) {
				if(board[i][j] != word_letters[k] && board[i][j] != '.') { //if at any point the letter being added conflicts with an existing one, die
					return false;
				}				
			}

			//reset values so that i can iterate again and replace the characters
			i = start_y;
			j = start_x;
			k = 0;

			for(; k < word_length; i++, k++) {
					board[i][j] = word_letters[k];
			}
		} else if(word_orientation== 1) { 

			for(; k < word_length; i++, j++, k++) {
				if(board[i][j] != word_letters[k] && board[i][j] != '.') {
					return false;
				}				
			}


			i = start_y;
			j = start_x;
			k = 0;
			
			for(; k < word_length; i++, j++, k++) {
					board[i][j] = word_letters[k];
			}

		} else if(word_orientation== 2) {

			for(; k < word_length; j++, k++) {
				if(board[i][j] != word_letters[k] && board[i][j] != '.') {
					return false;
				}				
			}


			i = start_y;
			j = start_x;
			k = 0;
			
			for(; k < word_length; j++, k++) {
					board[i][j] = word_letters[k];
			}

		} else if(word_orientation== 3) {

			for(; k < word_length; i--, j++, k++) {
				if(board[i][j] != word_letters[k] && board[i][j] != '.') {
					return false;
				}				
			}


			i = start_y;
			j = start_x;
			k = 0;
			
			for(; k < word_length; i--, j++, k++) {
					board[i][j] = word_letters[k];
			}

		} else if(word_orientation== 4) {

			for(; k < word_length; i--, k++) {
				if(board[i][j] != word_letters[k] && board[i][j] != '.') {
					return false;
				}				
			}


			i = start_y;
			j = start_x;
			k = 0;
			
			for(; k < word_length; i--, k++) {
					board[i][j] = word_letters[k];
			}

		} else if(word_orientation== 5) {

			for(; k < word_length; i--, j--, k++) {
				if(board[i][j] != word_letters[k] && board[i][j] != '.') {
					return false;
				}				
			}


			i = start_y;
			j = start_x;
			k = 0;
			
			for(; k < word_length; i--, j--, k++) {
					board[i][j] = word_letters[k];
			}

		} else if(word_orientation== 6) {
			for(; k < word_length; j--, k++) {
				if(board[i][j] != word_letters[k] && board[i][j] != '.') {
					return false;
				}				
			}


			i = start_y;
			j = start_x;
			k = 0;
			
			for(; k < word_length; j--, k++) {
					board[i][j] = word_letters[k];
			}

		} else if(word_orientation== 7) {

			for(; k < word_length; i++, j--, k++) {
				if(board[i][j] != word_letters[k] && board[i][j] != '.') {
					return false;
				}				
			}

			i = start_y;
			j = start_x;
			k = 0;
			
			for(; k < word_length; i++, j--, k++) {
					board[i][j] = word_letters[k];
			}

		} else {
			std::cerr << "weird stuff in grid if/else" << std::endl;
			return false;
		}
		return true;
	}
}

//copy constructor
Grid Grid::copy() const {
	Grid new_grid(height, width);
	for (int i  = 0; i < height; i++) {
		for (int j = 0; j < width; j++) {
			new_grid.board[i][j] = board[i][j];
		}
	}
	return new_grid;
}

//equality operator for use in duplicate checking
bool operator==(const Grid& grid1, const Grid& grid2) {
	if (grid1.height == grid2.height && grid1.width == grid2.width) {
		for (unsigned int i = 0; i < grid1.height; i++) {
			for (unsigned int j = 0; j < grid1.width; j++) {
				if (grid1.board[i][j] != grid2.board[i][j]) {
					return false;
				}
			}
		}
		return true;
	} else {
		return false;
	}
}

//useful helper function for printing a board to std::cout
void Grid::display() const{
	std::cout << "Board:" << std::endl;
	for(unsigned int i = 0; i < board.size(); i++) {
		std::cout << "  ";
		for(unsigned int j = 0; j < board[0].size(); j++) {
			std::cout << board[i][j];
		}
		std::cout << std::endl;
	}
}