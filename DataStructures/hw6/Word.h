#include <vector>
#include <string>
//Word class implementation for wordsearch
//ethan riley, 2017

class Word {
	
	private:
		unsigned int length;
		unsigned int orientation;
		std::string letters;

	public:
		bool present_in_board;
		Word(std::string input_string, bool found);
		int start_point[2];
		unsigned int getLength() const;
		unsigned int getOrientation() const;
		std::string getLetters() const;
		int* getStartPoint();
		void setOrientation(unsigned int new_orientation);
		void setStartPoint(int x_pos, int y_pos);
};