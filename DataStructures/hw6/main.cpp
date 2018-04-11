#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include "Grid.h"
#include <list>
#include <sstream>
//program to recursively generate wordsearch grids given input words and dimensions
//ethan riley, 2017

std::string alpha = "abcdefghijklmnopqrstuvwxyz";

//outputs board to ostream
void print(const Grid& grid, std::ofstream& output_file) {
	output_file << "Board:" << std::endl;
	for(unsigned int i = 0; i < grid.board.size(); i++) {
		output_file << "  ";
		for(unsigned int j = 0; j < grid.board[0].size(); j++) {
			output_file << grid.board[i][j];
		}
		output_file << std::endl;
	}
}

//checks for empty spaces in a board
//O(w*h)
bool hasEmptySpaces(const Grid& grid) {
	bool has_empty_spaces = false;
	for (unsigned int i = 0; i < grid.height; i++) {
		for (unsigned int j = 0; j < grid.width; j++) {
			if (grid.board[i][j] == '.') {
				has_empty_spaces = true;
			}
		}
	}
	return has_empty_spaces;
}

//checks if a grid is present in a vector already
bool presentInVector(const std::vector<Grid>& search_list, const Grid& grid) {
	bool present_in_vector = false;
	for (unsigned int i = 0; i < search_list.size(); i++) {
		if(search_list[i] == grid) {
			present_in_vector = true;
		}
	}
	return present_in_vector;
}

//creates all the permutations of a list of words on a w*h board
void recursivePermutate(std::vector<Word> word_list, Grid& grid, std::vector<Grid>& valid_grids, unsigned int index) {
	
	//at the last word, add the board to a list of valid grids
	if (index == word_list.size()) {
		valid_grids.push_back(grid);
		return;
		}	

	Word next_word = word_list[index];

	//iterate through all x, y, and orientations
	for (int i = 0; i < 8; i++) {

		for (int j = 0; j < grid.height; j++) {

			for (int k = 0; k < grid.width; k++) {

				Grid new_grid = grid.copy(); //each recursive call generates a new board				
				next_word.setOrientation(i);
				next_word.setStartPoint(k, j);

				if (new_grid.add(next_word)) {
					recursivePermutate(word_list, new_grid, valid_grids, (index+1)); //continue to the next word
				}
			}
		}
	}
}

//searches a grid for a word recursively
void recursiveSearch(const Word& word, const Grid& grid, int x_pos, int y_pos, unsigned int orientation, int counter, bool& word_found) {
	std::string letters = word.getLetters();
	std::vector<std::vector<char> > board = grid.board;	

	//if all letters are correct, the word is present
	if (counter == letters.length()) {
		word_found = true;
		return;
	}
	if (x_pos < 0 || y_pos < 0 || x_pos >= grid.width || y_pos >= grid.height) {
		//kills the recursive branch if we've gone out of bounds
		return;
	}
	if (orientation == 0) { //eight branches, one for each orientation
		if (board[y_pos][x_pos] == letters[counter]) { //only continue along the path if the board matches the letter
			recursiveSearch(word, grid, x_pos, y_pos + 1, orientation, (counter + 1), word_found);
		}

	} else if (orientation == 1) {
		if (board[y_pos][x_pos] == letters[counter]) {
			recursiveSearch(word, grid, x_pos + 1, y_pos + 1, orientation, (counter + 1), word_found);
		}

	} else if (orientation == 2) {
		if (board[y_pos][x_pos] == letters[counter]) {
			recursiveSearch(word, grid, x_pos + 1, y_pos, orientation, (counter + 1), word_found);
		}

	} else if (orientation == 3) {
		if (board[y_pos][x_pos] == letters[counter]) {
			recursiveSearch(word, grid, x_pos + 1, y_pos - 1, orientation, (counter + 1), word_found);
		}

	} else if (orientation == 4) {
		if (board[y_pos][x_pos] == letters[counter]) {
			recursiveSearch(word, grid, x_pos, y_pos - 1, orientation, (counter + 1), word_found);
		}

	} else if (orientation == 5) {
		if (board[y_pos][x_pos] == letters[counter]) {
			recursiveSearch(word, grid, x_pos - 1, y_pos - 1, orientation, (counter + 1), word_found);
		}

	} else if (orientation == 6) {
		if (board[y_pos][x_pos] == letters[counter]) {
			recursiveSearch(word, grid, x_pos - 1, y_pos, orientation, (counter + 1), word_found);
		}

	} else if (orientation == 7) {
		if (board[y_pos][x_pos] == letters[counter]) {
			recursiveSearch(word, grid, x_pos - 1, y_pos + 1, orientation, (counter + 1), word_found);
		}
	}
}

//helper function to find words in a grid
//checks one word in every location and permutation
//very similar to recursivePermutate()
void recursiveCheckGrid(Word next_word, Grid grid, bool& grid_is_incorrect) {
	
	for (int i = 0; i < 8; i++) {

		for (int j = 0; j < grid.height; j++) {

			for (int k = 0; k < grid.width; k++) {

				next_word.setOrientation(i);
				next_word.setStartPoint(k, j);

				bool word_found = false;
				recursiveSearch(next_word, grid, next_word.start_point[0], next_word.start_point[1], next_word.getOrientation(), 0, word_found);
				if (word_found) {
					grid_is_incorrect = word_found;
				}
			}
		}	
	}
}

//this function tries to fill in incomplete grids with full words
//massively decreases runtime in certain circumstances
void recursiveFillWords(std::vector<Grid>& initial_grids, std::vector<Word>& word_list, std::vector<Grid>& intermediate_grids) {
	std::vector<Grid> valid_permutations;
	for (unsigned int i = 0; i < initial_grids.size(); i++) {
		if (hasEmptySpaces(initial_grids[i])) {
			//if a grid has empty spots, try to permutate words across it again
			recursivePermutate(word_list, initial_grids[i], valid_permutations, 0);
		} else {
			intermediate_grids.push_back(initial_grids[i]);
		}
	}
	//adds all grids not already in intermediate_grids to intermediate_grids (i had some issues with duplicates)
	for (unsigned int j = 0; j < valid_permutations.size(); j++) {
		if (not(presentInVector(intermediate_grids, valid_permutations[j]))) {
			intermediate_grids.push_back(valid_permutations[j]);
		}
	}
}

//helper function for searching grids
bool verifyGrid(const std::vector<Word>& missing_word_list, const Grid& grid) {
	bool grid_is_incorrect = false;
	for (unsigned int j = 0; j < missing_word_list.size(); j++) {
		recursiveCheckGrid(missing_word_list[j], grid, grid_is_incorrect );
	}
	return (not(grid_is_incorrect));
}

//little function that checks if any single letters are in the forbidden words
//massively reduces runtime in certain circumstances
bool letterInList(char letter, std::vector<Word> word_list) {
	bool found = false;
	for (unsigned int i = 0; i < word_list.size(); i++) {
		if (word_list[i].getLetters().size() == 1) {
			char compare_char;
			std::stringstream ss;
			ss << word_list[i].getLetters(); //i have to convert the string to a char
			ss >> compare_char;
			if (letter == compare_char) {
				found = true;
			}
		}
	}
	return found;
}

//very slow (O(26^n)) function that recursively fills in blank spaces in a grid
void fillEmptySpaces(std::vector<Grid>& valid_grids, Grid grid, std::vector<Word> missing_word_list) {

	bool empty_space = false;
	for (unsigned int j = 0; j < grid.height; j++) {
		for (unsigned int k = 0; k < grid.width; k++) {
			if (grid.board[j][k] == '.') {
				empty_space = true;
				for (unsigned int i = 0; i < 26; i++) {
					if (not(letterInList(alpha[i], missing_word_list))) { //if a letter is forbidden, i can skip it and save a lot of time
						grid.board[j][k] = alpha[i];
						fillEmptySpaces(valid_grids, grid, missing_word_list); //pass the filled grid to a recursive call; this fills in all remaining empty spaces
					}
				}
			}
		}
	}
	if (not(empty_space) && (verifyGrid(missing_word_list, grid)) && not(presentInVector(valid_grids, grid))) {
		valid_grids.push_back(grid);
	}
}

int main(int argc, char* argv[]) {
	std::vector<Word> found_word_list;
	std::vector<Word> missing_word_list;
	std::string input_file_name = argv[1];
	std::string output_file_name = argv[2];
	std::string solutions = argv[3];
	std::ifstream input_file;
	std::ofstream output_file;
	input_file.open(input_file_name.c_str());
	output_file.open(output_file_name.c_str());
	unsigned int width, height;
	input_file >> width;
	input_file >> height;
	Grid grid(height, width);
	bool found = true;
	std::string input_word;

	//parses input into word lists
	while (input_file >> input_word) {
		if (input_word == "+") { found = true; }
		else if (input_word == "-") {found  = false; }
		input_file >> input_word;
		Word new_word(input_word, found);
		if (found) {
			found_word_list.push_back(new_word);
		}
		else {
			missing_word_list.push_back(new_word);
		}
	}

	//permutate all possible boards 
	std::vector<Grid> possible_grids;
	std::vector<Grid> intermediate_grids;
	if (found_word_list.size() == 0) { //if no words are specified to exist, i have to generate all boards /without/ certain words
		fillEmptySpaces(possible_grids, grid, missing_word_list);
	} else {
		recursivePermutate(found_word_list, grid, possible_grids, 0);
	}

	for (unsigned int i  = 0; i < possible_grids.size(); i++) {

		bool grid_is_correct = verifyGrid(missing_word_list, possible_grids[i]);

		if (grid_is_correct) { //remove incorrect grids; the recursive check function is /very/ fast, so removing grids (which could spawn a massive number of recursive calls) here helps
			intermediate_grids.push_back(possible_grids[i]);
		}
	}


	std::vector<Grid> final_grids;
	recursiveFillWords(intermediate_grids, found_word_list, final_grids);

	std::vector<Grid> correct_grids;
	for (unsigned int k = 0; k < final_grids.size(); k++) {
		bool correct = verifyGrid(missing_word_list, final_grids[k]);
		if (not(hasEmptySpaces(final_grids[k]))) {
			correct_grids.push_back(final_grids[k]);
		} else if (correct) {
			fillEmptySpaces(correct_grids, final_grids[k], missing_word_list);
		}
	}

	std::vector<Grid> solution_grids;
	for (unsigned int j = 0; j < correct_grids.size(); j++) {
		if (not(presentInVector(solution_grids, correct_grids[j]))) {
			solution_grids.push_back(correct_grids[j]);
		}
	}

	bool all_solutions;

	if (solutions == "one_solution") {
		all_solutions = false;
	} else {
		all_solutions = true;
	}

	int list_size  = solution_grids.size();
	if (list_size == 0) {
		output_file << "No solutions found" << std::endl;
	} else if (all_solutions) {
		output_file << list_size << " solution(s)" << std::endl;		
		for (unsigned int n = 0; n < solution_grids.size(); n++) {
			print(solution_grids[n], output_file);
		}
	} else {
		print(solution_grids[0], output_file);
	}

	return 0;
}