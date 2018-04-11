#include "Word.h"
//Word class implementation for wordsearch
//ethan riley, 2017

Word::Word(std::string input_string, bool found) {
	letters = input_string;
	length = input_string.length();
	present_in_board = found;
	start_point[0] = -9999;
	start_point[1] = -9999;
}

unsigned int Word::getLength() const {
	return length;
}

unsigned int Word::getOrientation() const {
	return orientation;
}

void Word::setOrientation(unsigned int new_orientation) {
	orientation = new_orientation;
}

void Word::setStartPoint(int x_pos, int y_pos) {
	start_point[0] = x_pos;
	start_point[1] = y_pos;
}

std::string Word::getLetters() const {
	return letters;
}