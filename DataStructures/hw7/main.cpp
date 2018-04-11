// -----------------------------------------------------------------
// HOMEWORK 7 WORD FREQUENCY MAPS
//
// You may use all of, some of, or none of the provided code below.
// You may edit it as you like (provided you follow the homework
// instructions).
// -----------------------------------------------------------------

#include <iostream>
#include <fstream>
#include <string>
#include <list>
#include <vector>
#include <map>
#include <cstdlib>
#include <assert.h>
#include <random>



// ASSIGNMENT: FILL IN YOUR OWN MAP STRUCTURE
typedef  std::map<std::string, std::map<std::string, int> >  MY_MAP; // for window size of 2, this will do
typedef  std::map<std::string, int> ROOT_MAP; //base map of strings and ints

//iterates through a root map, finding the most common word
std::string findMostCommonWord(MY_MAP& data, std::string word) {
	std::string most_used = "";
	int highest_count = 0;

	for (ROOT_MAP::iterator itr = data[word].begin(); itr != data[word].end(); itr++) {
		
		if (itr->second > highest_count) {
			highest_count = itr->second;
			most_used = itr->first;
		} else if (itr->second == highest_count) {
			if (itr->first < most_used) {
				most_used = itr->first;
			}
		}
	
	}

	return most_used;

}

//similar to findMostCommonWord, but this finds the sub-map with the most chains of words
std::string findGreatestBranch(std::map<std::string, MY_MAP>& big_data, std::string word) {
	int greatest_branch_count = 0;
	std::string greatest_branch = "";

	for (MY_MAP::iterator itr1 = big_data[word].begin(); itr1 != big_data[word].end(); itr1++) {

		int branch_count = 0;

		for (ROOT_MAP::iterator itr2 = big_data[word][itr1->first].begin(); itr2 != big_data[word][itr1->first].end(); itr2++) {

			branch_count += itr2->second;
		}

		if (branch_count > greatest_branch_count) {
			greatest_branch = itr1->first;
			greatest_branch_count = branch_count;
		} else if (branch_count == greatest_branch_count && itr1->first < greatest_branch) {
			greatest_branch = itr1->first;
		}

	}

	return greatest_branch;
}

//creates a proportional vector for random selection
void generateChoiceVectors(ROOT_MAP& word_freqs, std::vector<std::string>& choice_vec) {
	for (ROOT_MAP::iterator itr = word_freqs.begin(); itr != word_freqs.end(); itr++) {
		for (int i =0; i < itr->second; i++) {
			choice_vec.push_back(itr->first);
		}
	}
}

//generates a markov chain of the most common words, given a beginning word and a map of window 2
void generateCommonChain(MY_MAP& data, std::vector<std::string>& word_vec, int num_words, int index) {
	//assume word_vec is populated with the beginning word
	if(index == num_words) {
		return;
	}

	std::string next_word = findMostCommonWord(data, word_vec[index]);
	word_vec.push_back(next_word);
	generateCommonChain(data, word_vec, num_words, (index + 1));
}

//generates a random markov chain for window 3
void generateBigRandomChain(std::map<std::string, MY_MAP> big_data, std::vector<std::string>& word_vec, unsigned int num_words, int index, unsigned int starting_length) {
	if (word_vec.size() - starting_length == num_words) {
		return;
	}
	std::vector<std::string> choices;
	//create the choices, proportionally filled with strings
	generateChoiceVectors(big_data[word_vec[index]][word_vec[index+1]], choices);
	unsigned int num_possibilites = choices.size();
	assert(num_possibilites >= 0);
	//get a random number between 1 and 10
	float rand_num = abs(rand() % 10);
	//use that number to create a random index in the correct range
	unsigned int rand_index;
	if (rand_num == 0) {
		rand_index = 0;
	} else {
		rand_index = (int) (rand_num / (10.0/(num_possibilites-1)));
	}
	assert(rand_index >= 0);
	assert(rand_index < num_possibilites);
	word_vec.push_back(choices[rand_index]);
	//continue recursively, shifting the index along to the recently added word
	generateBigRandomChain(big_data, word_vec, num_words, index + 1, starting_length);

}

//simpler function to randomly generate a markov chain for window 2
void generateRandomChain(MY_MAP& data, std::vector<std::string>& word_vec, unsigned int num_words, unsigned int index) {
	if (index == num_words) {
		return;
	}
	std::vector<std::string> choices;
	generateChoiceVectors(data[word_vec[index]], choices);
	int num_possibilites = choices.size();
	int rand_num = rand() % 10;
	int rand_index = (int) (rand_num / (10.0/num_possibilites));
	word_vec.push_back(choices[rand_index]);
	generateRandomChain(data, word_vec, num_words, index + 1);
}

//similarly to generateCommonChain, this creates a markov chain of common words
//this one uses word chains, though, rather than searching for the most common single word
//for use with window = 3
void generateBigCommonChain(std::map<std::string, MY_MAP>& big_data, std::vector<std::string>& word_vec, unsigned int num_words, int index, unsigned int starting_length) {
	if (word_vec.size() - starting_length == num_words) {
		return;
	}
	if (index == 0 && word_vec.size() == 1) {
		//we have been given one root word to follow
		word_vec.push_back(findGreatestBranch(big_data, word_vec[0]));
	}
	assert(word_vec.size() >= 2); //we should have a baseline of 2 words to search from
	std::string next_word = findMostCommonWord(big_data[word_vec[index]], word_vec[index + 1]);
	word_vec.push_back(next_word);
	generateBigCommonChain(big_data, word_vec, num_words, index + 1, starting_length);
}

// Custom helper function that reads the input stream looking for
// double quotes (a special case delimiter needed below), and white
// space.  Contiguous blocks of alphabetic characters are lowercased &
// packed into the word.
bool ReadNextWord(std::istream &istr, std::string &word) {
  char c;
  word.clear();
  while (istr) {
    // just "peek" at the next character in the stream
    c = istr.peek();
    if (isspace(c)) {
      // skip whitespace before a word starts
      istr.get(c);
      if (word != "") {
	// break words at whitespace
	return true;
      }
    } else if (c == '"') {
      // double quotes are a delimiter and a special "word"
      if (word == "") {
	istr.get(c);
	word.push_back(c);
      }
      return true;
    } else if (isalpha(c)) {
      // this a an alphabetic word character
      istr.get(c);
      word.push_back(tolower(c));
    } else {
      // ignore this character (probably punctuation)
      istr.get(c);
    }
  }
  return false;
}


// Custom helper function that reads the input stream looking a
// sequence of words inside a pair of double quotes.  The words are
// separated by white space, but the double quotes might not have
// space between them and the neighboring word.  Punctuation is
// ignored and words are lowercased.
std::vector<std::string> ReadQuotedWords(std::istream &istr) {
  // returns a vector of strings of the different words
  std::vector<std::string> answer;
  std::string word;
  bool open_quote = false;
  while (ReadNextWord(istr,word)) {
    if (word == "\"") {
      if (open_quote == false) { open_quote=true; }
      else { break; }
    } else {
      // add each word to the vector
      answer.push_back(word);
    }
  }
  return answer;
}



// Loads the sample text from the file, storing it in the map data
// structure Window specifies the width of the context (>= 2) of the
// sequencing stored in the map.  parse_method is a placeholder for
// optional extra credit extensions that use punctuation.
void LoadSampleText(std::map<std::string, MY_MAP>& big_data, MY_MAP &data, ROOT_MAP& word_counts, const std::string &filename, int window, const std::string &parse_method) {
  // open the file stream
  std::ifstream istr(filename.c_str());
  if (!istr) { 
    std::cerr << "ERROR cannot open file: " << filename << std::endl; 
    exit(1);
  } 
  // verify the window parameter is appropriate
  if (window < 2) {
    std::cerr << "ERROR window size must be >= 2:" << window << std::endl;
  }
  // verify that the parse method is appropriate
  bool ignore_punctuation = false;
  if (parse_method == "ignore_punctuation") {
    ignore_punctuation = true;
  } else {
    std::cerr << "ERROR unknown parse method: " << parse_method << std::endl;
    exit(1);
  }



  //
  // ASSIGNMENT:SETUP YOUR MAP DATA AS NEEDED
  //

	  /*
	  		I can wrap MY_MAP in more maps here for variable window sizes
	  		Probably done best recursively? Who knows. Not me
	  		For now, though, I can just use the MY_MAP structure directly
	  */


  std::string double_prev_word = "";
  std::string prev_word = "";
  std::string word = "";

  while (ReadNextWord(istr, word)) {
    // skip the quotation marks (not used for this part)

    if (word == "\"") continue;

    //
    // ASSIGNMENT: PROCESS ALL THE NON PUNCTUATION WORDS
    // INSERTING THESE WORDS INTO THE DATA STRUCTURE
    //

    	/*
    		I'm reasonably certain that I need to keep track of the previous word somehow
    		The best way is to update a prev_word var
    		I think. 
    	*/

    if (window == 2) {
	    if (prev_word == "") { //account for the beginning of the file
	    	prev_word = word;
	    	word_counts[prev_word]++; //i update a separate map of word counts to make my life easier
	    	ReadNextWord(istr, word);
	    }

	    word_counts[word]++;
	    data[prev_word][word]++; //update the MY_MAP structure
	    prev_word = word;
	} else if (window == 3) {//same thing as window=2, but I keep track of the previous 2 words
		if (double_prev_word == "") {
			double_prev_word = word;
			word_counts[double_prev_word]++;
			ReadNextWord(istr, word);
			prev_word = word;
			word_counts[prev_word]++;
			ReadNextWord(istr, word);
		}
		std::string combined_word = double_prev_word + " " + prev_word;
		word_counts[combined_word]++;
		word_counts[word]++;
		big_data[double_prev_word][prev_word][word]++;
		data[prev_word][word]++;
		double_prev_word = prev_word;
		prev_word = word;
	}

  }

}

int main () {

  // ASSIGNMENT: THE MAIN DATA STRUCTURE
  MY_MAP data;
  ROOT_MAP word_counts;
  std::map<std::string, MY_MAP> big_data; //for window = 3

  // Parse each command
  std::string command;    
  while (std::cin >> command) {

    // load the sample text file
    if (command == "load") {
      std::string filename;
      int window;
      std::string parse_method;
      std::cin >> filename >> window >> parse_method;      

      //
      // ASSIGNMENT: ADD YOUR COMMANDS HERE
      //

      data.clear(); //just in case
      big_data.clear();
      LoadSampleText(big_data, data, word_counts, filename, window, parse_method);
      std::cout << "Loaded " << filename << " with window = " << window << " and parse method = " << parse_method << std::endl << std::endl;

    } 

    // print the portion of the map structure with the choices for the
    // next word given a particular sequence.
    else if (command == "print") {
        std::vector<std::string> sentence = ReadQuotedWords(std::cin);
        //sentence is a vector of query-able strings (they match the ones in the map)

	    std::string query = "";
	    for (unsigned int i = 0; i < sentence.size(); i++) {
	    	query += sentence[i];
	      	std::cout << sentence[i];
	      	if (i != sentence.size() - 1) {
	      		std::cout << " ";
	      		query += " ";
	      	}
	    }

	    std::cout << " (" << word_counts[query] << ")" << std::endl;

	    if (sentence.size() == 1) {	      
	      	for (ROOT_MAP::iterator itr = data[sentence[0]].begin(); itr != data[sentence[0]].end(); itr++) {

	      		std::cout << sentence[0] << " " << itr->first << " (" << itr->second << ")" << std::endl;
	      	
	      	}
	      	std::cout << std::endl;
		          
		} else if (sentence.size() > 1) {
			for (ROOT_MAP::iterator itr = big_data[sentence[0]][sentence[1]].begin(); itr != big_data[sentence[0]][sentence[1]].end(); itr++) {
				std::cout << sentence[0] << " " << sentence[1] << " " << itr->first << " (" << itr->second << ")" << std::endl;
			}
			std::cout << std::endl;

		}
	}

    // generate the specified number of words 
    else if (command == "generate") {
      std::vector<std::string> sentence = ReadQuotedWords(std::cin);
      // how many additional words to generate
      unsigned int length;
      std::cin >> length;
      std::string selection_method;
      std::cin >> selection_method;
      bool random_flag;
      if (selection_method == "random") {
		random_flag = true;
      } else {
		assert(selection_method == "most_common");
		random_flag = false;
    }


    //
    // ASSIGNMENT: ADD YOUR COMMANDS HERE
    //

    std::vector<std::string> markov_sentence;
    for (unsigned int k = 0; k < sentence.size(); k++) {
    	markov_sentence.push_back(sentence[k]);
    }
    bool window_is_3 = false;

    if (big_data.size() > 0) {
    	window_is_3 = true;
    }

    if (random_flag) {
		//i use different functions for different window sizes
		int query_length = markov_sentence.size();
		if (query_length == 1) {
			generateRandomChain(data, markov_sentence, length, 0);
		} else if (query_length > 1) {
			generateBigRandomChain(big_data, markov_sentence, length, 0, query_length);
		}
    } else {
    	if (window_is_3) {
    		generateBigCommonChain(big_data, markov_sentence, length, 0, markov_sentence.size());
    	} else {
    		generateCommonChain(data, markov_sentence, length, 0);
    	}
    }

    for (unsigned int j = 0; j < markov_sentence.size(); j++) {
  		std::cout << markov_sentence[j];
  		if (j != markov_sentence.size() - 1) {
  			std::cout << " ";
  		}
  	}
  	std::cout << std::endl << std::endl;



    } else if (command == "quit") {
      break;
    } else {
      std::cout << "WARNING: Unknown command: " << command << std::endl;
    }
  }
}
