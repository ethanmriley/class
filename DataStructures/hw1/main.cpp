// main.cpp - a not-so-simple seating generator for a fictitious airline
#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib>
#include <vector>
#include <sstream>
#include <string>
#include <algorithm>

// a constant for generating seat aisle labels
const std::string ALPHABET("ABCDEFGHIJKLMNOPQRSTUVWXYZ");

//checks for validity of requested seat through seat_map indices (passed in main())
bool GetSeatRowCol(const std::string& seat_request,
		   unsigned int total_rows,
		   unsigned int total_columns,
		    int &seat_row,  int &seat_col) {
  
  std::string row_str;
  std::string col_str;
  for (unsigned int i = 0; i < seat_request.size(); ++i) {

    if (seat_request[i] >= '0' && seat_request[i] <= '9')
      row_str += seat_request[i];
    else
      col_str += seat_request[i];	
  }
    
  seat_row = atoi(row_str.c_str()) - 1;

  if (seat_row < 0 || seat_row > (int) total_rows - 1) {
    return false;
  }

  seat_col = ALPHABET.find(col_str);

  if (seat_col == std::string::npos || seat_col > total_columns - 1) {
    return false;
  }

  return true;
}
//creates a vector of vectors of strings which serves to store the seat map 
std::vector<std::vector<std::string> > mapSeats(std::string seat_file_name) {
	std::ifstream seat_file;
	seat_file.open(seat_file_name.c_str());
	std::vector<std::vector<std::string> > seat_map;
	std::vector<std::string> row;

	for (std::string line; std::getline(seat_file, line); ) {
		row.clear();

		for (unsigned int i = 0; i < line.size(); i++) {
			std::string seat (1,line[i]);

			if (seat != " " ) { //removes white space from stream to make indexing easier
				row.push_back(seat);
			}
		}
		seat_map.push_back(row); // each row vector has the info of one line of the seat map; this populates the seat_map with the newly-created row.
	}
	return seat_map;
}
//function to read and print the seat map, correctly formatted
void printChart(std::vector<std::vector<std::string> >seat_map, std::string flight_number) {
	std::cout << "Seating Chart for Flight " + flight_number << std::endl; 
	std::cout << "\t";
	
	int i = 0;
	while (i < seat_map[0].size() - 1 ) { //generates and prints seat row list 
		std::string letter(1, ALPHABET[i]);
		std::cout << letter + "    ";
		i++;
	}
	std::cout << "Seating Class" << std::endl;
	std::string out_string;
	std::string row_number;
	
	int j = 0;
	while ( j < seat_map.size()) {
		std::stringstream ss;
		std::string row_class = seat_map[j][0];
		ss << j + 1;
		ss >> row_number;
		std::cout << row_number + "\t";

		for (unsigned int k = 1; k < seat_map[j].size(); k++){//takes row info and transforms it into print info

			if (seat_map[j][k] == ".") {
				out_string = "....";
			}
			else if (seat_map[j][k].length() >= 4) {
				out_string = seat_map[j][k];
			}
			else if (seat_map[j][k] == "!") {
				out_string = "!!!!";
			}
			std::cout << out_string + " ";
		}

		std::cout << row_class << std::endl;
		j++;
	}

}

//takes passenger id, frequent flyer status, the seat map, and the upgrade list
//attempts to assign passenger to a seat in the same class they requested
//if it fails and they're a frequent flyer, it adds them to the upgrade list

bool findSeat(std::string pass_id, std::string seat_info, std::string freq_fly, 
			std::vector<std::vector<std::string> > &seat_map, std::vector<std::vector<std::string> > &upgrade_list ) {
	std::string seat_classes("FBC");
	std::vector<int> seat_queue[2]; //sarray; seat_queue[0] hols "ideal" seats (those near windows/aisles); seat_queue[1] holds "unideal" seats
	std::vector<int> seat_candidate; //vector to hold individual seats (row and column numbers). 
	std::string requested_seat_class;

	if (seat_info.length() == 2) { //gets seat class from specific seat request
		std::stringstream ss;
		int seat_row;
		ss << seat_info[0];
		ss >> seat_row;
		seat_row-= 1;
		requested_seat_class = seat_map[seat_row][0];
	}
	else if (seat_info.length() == 1) {
		requested_seat_class = seat_info;
	}
	//this bit has to loop through all possible seats; it can't just wait until a possible seat is found, because that seat may be unideal
	//therefore, it loads possible seats into the seat_queue array, which is checked at the end 
	//this prevents the passenger being assigned, say, 1B (unideal), as opposed to 2G (ideal) just because 1B came first.

	if (seat_classes.find(requested_seat_class) != std::string::npos) { //checks that the user requested a valid seat class

		for (unsigned int i = 0; i < seat_map.size(); i++) { //starts looping through rows

			if (requested_seat_class == seat_map[i][0]) { //checks for correct seat class

				for (unsigned int j = 0; j < seat_map[i].size(); j++) { //start looping through seats
					std::string seat = seat_map[i][j];

					if (seat == ".") { //ensures the seat is empty
						seat_candidate.push_back(i);
						seat_candidate.push_back(j);

						if ((j == 1 || j == (seat_map[i].size() - 1) || seat_map[i][j+1] == "!" || seat_map[i][j-1] == "!") && ( seat_queue[0].size() == 0)) { //checks for window and aisle seats (if the adjacent seat is ! or if it is on the far edge of the plane)
							seat_queue[0] = seat_candidate;

						}
						else if (seat_queue[1].size() == 0){
							seat_queue[1] = seat_candidate;
						}
						seat_candidate.clear(); //after storing the seat in its appropriate place in seat_queue, clear the temporary seat holder for the next use
					}
				}
			}
		}
		std::vector<int> ideal_seat = seat_queue[0];
		std::vector<int> unideal_seat = seat_queue[1];
		std::vector<int> given_seat; //final row/col numbers to be used to assign passenger to seat

		if (ideal_seat.size() != 0 || unideal_seat.size() != 0) {

			if (ideal_seat.size() != 0) {
				given_seat = ideal_seat;
			}
			else {
				given_seat = unideal_seat;
			}
			seat_map[given_seat[0]][given_seat[1]]= pass_id; //assigns non-ideal seat after all other valid seats have been checked
			std::stringstream ss;
			std::string given_row;
			std::string given_col(1, ALPHABET[(given_seat[1] -1)]);
			ss << given_seat[0] + 1;
			ss >> given_row; 
			std::cout << "Passenger " + pass_id + " was given seat " + given_row + given_col + "." << std::endl;
			return true;
		}
		
		//if no seat (ideal or unideal) was found, add passenger to appropriate upgrade list if they are a frequent flyer.

		else { 

			if (requested_seat_class != "F" && freq_fly == "Y") { //checks if the passenger is eligible for upgrade
				int upgrade_class;

				if (requested_seat_class == "B") {
					upgrade_class = 0; //similat to seat_queue, i'm using indexing to hold different kinds of data. upgrade class is reflected by index position (0 = first, 1 = business)
				}
				else if (requested_seat_class == "C") {
					upgrade_class = 1;
				}
				upgrade_list[upgrade_class].push_back(pass_id);
			} 
			else {
				std::string print_class;
				if( requested_seat_class == "F") {
					print_class = "first class";
				}
				else if (requested_seat_class == "B") {
					print_class = "business class";
				}
				else{
					print_class = "coach";
				}
				std::cout << "Could not find a seat in the " + print_class + " section for passenger " + pass_id + "." << std::endl;
				return false;
			}
		}
	}
	else {
		std::cout << "Invalid seating class - " + requested_seat_class + "." << std::endl;
		return false;
	}
	return false;
}

//simply iterates through the upgrade list, printing out passengers
void printUpgrade(std::vector<std::vector<std::string> > upgrade_list) {
	std::string tbu; //to be upgraded
	std::cout << "The following passengers are scheduled to be upgraded to first class:" << std::endl;
	for (int i = 0; i < upgrade_list[0].size(); i++) {
		tbu = upgrade_list[0][i];

		if (tbu != "NULL") {//my way of "deleting" passengers from the upgrade list
			std::cout << upgrade_list[0][i] << std::endl;
		}
	}
	std::cout << "The following passengers are scheduled to be upgraded to business class:" << std::endl;
	for (int j = 0; j < upgrade_list[1].size(); j++) {
		tbu = upgrade_list[1][ j];

		if (tbu != "NULL") {
			std::cout << upgrade_list[1][j] << std::endl;
		}
	}	
}
//this function iterates through both the seat map and the upgrade list(s) to find the passenger to remove
//if the passenger isn't found, prints an error message
void cancelSeat(std::string pass_id, std::vector<std::vector<std::string> > &seat_map, std::vector<std::vector<std::string> > &upgrade_list) {
	int found = 0; //easy way to check if the passenger was found
	for (int i = 0; i < seat_map.size(); i++){

		for (int j = 0; j < seat_map[i].size(); j++) {

			if (seat_map[i][j] == pass_id) {
				seat_map[i][j] = ".";
				found++;
			}
		}
	}
	for (int l = 0; l < 2; l++) {

		for (int m = 0; m < upgrade_list[l].size(); m++) {

			if (upgrade_list[l][m] == pass_id) {
				upgrade_list[l][m] = "NULL";
				found++;
			}
		}
	}
	if (found == 0) { //if found was never increased (passenger wasn't found in either the upgrade list or seat map), print error message
		std::cout << "Passenger " + pass_id + " could not be found." << std::endl;
	}
}

int main(int argc, char* argv[]) {
	std::cout << argv[0];
	std::vector<std::vector<std::string> > seat_map = mapSeats(argv[1]); 
	std::ifstream command_file;
	std::string command_file_name = argv[1];
	command_file.open(command_file_name.c_str());
	std::string word; //used to store commands
	std::string tmp; //used to store temporary commands to check if they are valid
	std::string line; //used to store each line of the command file
	std::vector<std::vector<std::string> > upgrade_list(2);
	int seat_row;
	int seat_col;
	std::string flight_number;
	std::getline(command_file, flight_number);
	std::string command; //this is just a container to hold the leftover command if there's an invalid one.
	//this bit checks the command
	//because i know the required arguments, i can just check the initial word and act on that info, reading the appropriate amount of data from the file
	//i also check for command validity
	while (std::getline(command_file, line)) { //iterates through the command file
		
		std::stringstream command_stream; //store lines in this, then read the proper number of words out from it depending on the command
		command_stream << line;
		command_stream >> tmp;
		//while the command is invalid, read in lines, checking for the validity of the command each time	
		while (tmp != "SSR" && tmp != "SRN" && tmp != "CXL" && tmp != "UPP" && tmp != "UPL" && tmp != "PRT") { 
			std::cout << "Invalid command: " << tmp << "." << std::endl;
			std::getline(command_file, line); //reads in the next line to check
			std::stringstream tmp_ss;
			tmp_ss << line;
			tmp_ss >> tmp;

			if (not (tmp != "SSR" && tmp != "SRN" && tmp != "CXL" && tmp != "UPP" && tmp != "UPL" && tmp != "PRT")) {
				word = tmp;
				command_stream.str(line); //resets command_stream to the correct line
				command_stream >> command; //not actually used; this just places the pointer at the correct place in the line
				break;
			}
		}
		word = tmp;
		
		//assigning specifc seats is a pretty simple set of conditionals, so i do it here without a function
		//if the seat is occupied/invalid, call findSeat() regardless to find a seat in the same class (more complex)
		if (word == "SSR" || word == "SRN") {
			std::string pass_id, seat_info, freq_fly;
			command_stream >> pass_id >> seat_info >> freq_fly; //reads the required data for SSR/SRN from the file

			if (word == "SSR") {
				bool valid = GetSeatRowCol(seat_info, seat_map.size(), seat_map[0].size(), seat_row, seat_col);

				if (valid) {
					std::string tmp_seat = seat_map[seat_row][seat_col + 1]; //have to account for the class identifier at the beginning of rows (throws off indexing)

					if (tmp_seat == ".") {
						seat_map[seat_row][seat_col +1 ] = pass_id;
						std::cout << "Passenger " + pass_id + " was given seat " + seat_info + "." << std::endl;
						continue;
					} 
					else if (tmp_seat.length() > 1) { //if the entry is a pass_id
						std::cout << "Passenger " + pass_id + " requested an occupied seat " + seat_info + "." << std::endl;
						findSeat(pass_id, seat_info, freq_fly, seat_map, upgrade_list);
					}
					else if (tmp_seat == "!") {
						findSeat(pass_id, seat_info, freq_fly, seat_map, upgrade_list);
					}
				}
				else if (not valid){
					std::cout << "Passenger " + pass_id + " requested an invalid seat " + seat_info + "." << std::endl;
				}
			}
			else if (word == "SRN") { //directly calls findSeat() if the passenger just requested a class 
				findSeat(pass_id, seat_info, freq_fly, seat_map, upgrade_list);
			}
		}
		else if (word == "CXL") {
			std::string pass_id;
			command_stream >> pass_id;
			cancelSeat(pass_id, seat_map, upgrade_list);
		}
		else if (word == "UPL") {
			printUpgrade(upgrade_list);
		}
		else if (word == "UPP") {
			std::string upgrade_pass;

			for (int i = 0; i < upgrade_list[0].size(); i++) {
				upgrade_pass = upgrade_list[0][i];

				if (upgrade_pass != "NULL" && findSeat(upgrade_pass, "F", "N", seat_map, upgrade_list)) { //setting freq_fly to "N" prevents passengers from being upgraded twice
					std::cout << "Passenger " + upgrade_pass + " was upgraded to the first class section." << std::endl;
				}
			}
			for (int j = 0; j < upgrade_list[1].size(); j++) {
				upgrade_pass = upgrade_list[1][j];

				if (upgrade_pass != "NULL" && findSeat(upgrade_pass, "B", "N", seat_map, upgrade_list)) {
					std::cout << "Passenger " + upgrade_pass + " was upgraded to the business class section." << std::endl;
				}
			}
			//after attempting to upgrade all passengers, clear the upgrade list
			upgrade_list[0].clear();
			upgrade_list[1].clear();
		}
		else if (word == "PRT") {
			printChart(seat_map, flight_number);
		}
	
	}
	return 0;
}