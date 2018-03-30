#include <iostream>
#include <fstream>
#include "Player.h"
#include "Champion.h"
#include <string>
#include <vector>
#include <algorithm>
#include <iomanip>
#include <cstring>

void findPlayer(std::vector<Player>& players, std::vector<int>& player_indices, std::string init_player_name, std::string init_champ_name, int game_value) { //maybe make custom differentiate these? would justify it more and make me feel better
	if (players.size() > 0) {
		for (unsigned int i = 0; i < players.size(); i++) {
			if (init_player_name == players[i].getPlayerName()) {
				player_indices.push_back(i);
				players[i].addPlayerGame(game_value);
				players[i].addChamp(init_champ_name);
				return;
			}

		}
		player_indices.push_back(players.size());
		players.push_back(Player(init_player_name, game_value, init_champ_name)); //adds player to list	
	}
	else {
		player_indices.push_back(players.size());
		players.push_back(Player(init_player_name, game_value, init_champ_name)); //adds player to list
	}
	return;
}	

void findChamp(std::vector<Champion>& champions, std::vector<int>& champ_indices, std::string champ_name, std::string init_player_name, int game_value) {
	if(champions.size() > 0) {
		for (unsigned int j = 0; j < champions.size(); j++) {
			if (champ_name == champions[j].getChampName()) {
				champ_indices.push_back(j);
				champions[j].addChampGame(game_value);
				return;
			}
		}
		champ_indices.push_back(champions.size());
		champions.push_back(Champion(champ_name, game_value, init_player_name));

	}
	else {
		champ_indices.push_back(champions.size());
		champions.push_back(Champion(champ_name, game_value, init_player_name)); //adds champ to list
	}
	return;
}




void getChamps(std::ifstream& games, std::vector<Player>& players, std::vector<Champion>& champions, std::vector<int>& player_indices, std::vector<int>& champ_indices) {
	int champ_count = 0;
	int game_value;
	int team_champ_count = 0;
	std::string init_player_name;
	std::string init_champ_name;
	std::string word;
	games >> word;
	while (champ_count < 10) {
		games >> word;
		if (word == "LOSING" || word == "WINNING") {
			if (word == "LOSING") {
				game_value = 0;
			}
			else if (word == "WINNING") {
				game_value = 1;
			}
			games >> word; //TEAM
			while (team_champ_count < 5) { //cycles through one team
				games >> word; //player_name
				init_player_name = word;
				games >> word; //playingin
				games >> word; //champion
				games >> word; //champ_name
				init_champ_name = word;
				findPlayer(players, player_indices, init_player_name, init_champ_name, game_value);
				findChamp(champions, champ_indices, init_champ_name, init_player_name, game_value);
				champ_count++;
				team_champ_count++;
				for (unsigned int i = 0; i < champ_indices.size(); i++) {
					if (champions[champ_indices[i]].getChampName() == init_champ_name) {
						champions[champ_indices[i]].updateCurrentPlayer(init_player_name);
					}
				}
			}

			team_champ_count = 0;
		}
	}
}


void readEvents(std::ifstream& games, std::vector<Champion>& champions, std::vector<Player>& players, std::vector<int> player_indices, std::vector<int> champ_indices) {
	std::string word;
	std::string killing_player;
	std::string killed_player;
	std::string assist_player;
	std::string game_time;
	games >> word; //removes the leading @
	while (true) { //i wish there was a better way to do this TODO
		std::string assist_tmp;
		std::string main_tmp;
		games >> game_time; // time
		games >> killing_player;
		games >> word; //either killed or [ or assist player
		
		if (word == "[") {
			games >> assist_player;
			while (true) { //reads through list of champs who got an assist
				for (unsigned int i = 0; i < player_indices.size(); i++) {
					if (assist_player == players[player_indices[i]].getPlayerName()) {
						players[player_indices[i]].playerAssistKill();
					}
				}
				for (unsigned int l = 0; l < champ_indices.size(); l++) {
					if (champions[champ_indices[l]].getCurrentPlayer() == assist_player) {
						champions[champ_indices[l]].champAssist();
						if (std::atoi(game_time.c_str()) < 600000) {
							champions[champ_indices[l]].earlyGameAssist();
						}
					}
				}
				games >> assist_tmp;

				if (assist_tmp == "]") {
					break;
				}
				else {
					assist_player = assist_tmp;
				}
			}
			games >> word; //killed
		}

		games >> killed_player;

		//this chunk reads through champions and players, updating the info
		for (unsigned int j = 0; j < player_indices.size(); j++) {
			if (players[player_indices[j]].getPlayerName() == killed_player) {
				players[player_indices[j]].playerKilled();

			}
			else if (players[player_indices[j]].getPlayerName() == killing_player) {
				players[player_indices[j]].playerKill();
			}
		}

		for (unsigned int k = 0; k < champ_indices.size(); k++) {
			if (killing_player == champions[champ_indices[k]].getCurrentPlayer()) {
				champions[champ_indices[k]].champKill();
				if (std::atoi(game_time.c_str()) > 780000) {
					champions[champ_indices[k]].lateGameKill();
				}
			}
			else if (killed_player == champions[champ_indices[k]].getCurrentPlayer()) {
				champions[champ_indices[k]].champKilled();
				if (killing_player == "minion") {
					champions[champ_indices[k]].killedByMinion();
				}
			}
		}
		games >> main_tmp;
		if (main_tmp == "END") {
			break;
		}
		else {
			word = main_tmp;
		}

	}
}

bool playerSort(const Player player1, const Player player2) {
	if (player1.getKDR() > player2.getKDR()) {
		return true;
	}
	else if (player1.getKDR() == player2.getKDR()) {
		if (player1.getPlayerKills() > player2.getPlayerKills()) {
			return true;
		}
		else if (player1.getPlayerKills() == player2.getPlayerKills()) {
			if (player1.getPlayerDeaths() < player2.getPlayerDeaths()) {
				return true;
			}
			else if(player1.getPlayerDeaths() == player2.getPlayerDeaths()) {
				return player1.getPlayerName() < player2.getPlayerName();
			}
		}
	}
	return false;
}

bool championSort(const Champion champ1, const Champion champ2) {
	if (champ1.getChampWinRate() > champ2.getChampWinRate()) {
		return true;
	}
	else if (champ1.getChampWinRate() == champ2.getChampWinRate()) {
		if (champ1.getChampWins() > champ2.getChampWins()) {
			return true;
		}
		else if (champ1.getChampWins() == champ2.getChampWins()) {
			if (champ1.getChampLosses() < champ2.getChampLosses()) {
				return true;
			}
			else if (champ1.getChampLosses() == champ2.getChampLosses()) {
				return champ1.getChampName() < champ2.getChampName();
			}
		}
	}
	return false;
}

bool customSort(const Champion champ1, const Champion champ2) {
	return (champ1.getChampDeaths() > champ2.getChampDeaths() && champ1.getLateChampKills() < champ2.getLateChampKills() && champ1.getChampWinRate() > champ2.getChampWinRate());
}


void printPlayers(std::vector<Player> players, std::ofstream &output) {
	output << std::left << std::setw(25) << "PLAYER NAME" << std::right << "KILLS" << "  " <<  std::right << "DEATHS" << "  " << std::right << std::setw(6) << "KDR  " << std::right << "PLAYED WITH CHAMPION(S)" << std::endl;
	std::sort(players.begin(), players.end(), playerSort);
	for (unsigned int i = 0; i < players.size(); i++) {
		float KDR = players[i].getKDR();
		output << std::setprecision(2) << std::fixed << std::left << std::setw(25) << players[i].getPlayerName() << std::right << std::setw(5) <<  players[i].getPlayerKills() << std::right << std::setw(8) <<  players[i].getPlayerDeaths() <<  std::right << std::setw(6) <<  KDR;
		std::vector<std::string> champs_played = players[i].getChampsPlayed();
		std::sort(champs_played.begin(), champs_played.end());
		if (champs_played.size() == 1) {
			output << std::right << "  " << champs_played[0] << std::endl;
		}
		else if (champs_played.size() > 1) {
			for (unsigned int j = 0; j < champs_played.size(); j++) {
				if (j == champs_played.size() - 1) { 
					output << std::right << champs_played[j] << std::endl;
				}
				else {
				output << std::right << "  " << champs_played[j] + ", ";
				}
			}
		}
	}
	return;
}

void printChampions(std::vector<Champion> champions, std::ofstream &output) {
	output << std::left << std::setw(25) << "CHAMPION NAME" << std::right << "WINS" << "  " <<  std::right << "LOSSES" << "  " << std::right << std::setw(6) << "WIN%  " << std::right << "MINION DEATHS" << std::endl;
	std::sort(champions.begin(), champions.end(), championSort);
	for (unsigned int i = 0; i < champions.size(); i++) {
		float win_rate = champions[i].getChampWinRate();
		output << std::left << std::setw(25) << champions[i].getChampName() << std::right << std::setw(4) << champions[i].getChampWins() << std::right << std::setw(8) <<  champions[i].getChampLosses() << std::setw(6) << std::setprecision(2) << std::fixed << win_rate <<  std::right << std::setw(3) <<  champions[i].getMinionDeaths() << std::endl;

	}
}

void printCustom(std::vector<Champion> champions, std::ofstream &output) {
	output << std::left << std::setw(25) << "CHAMPION NAME" << std::right << "EARLY GAME ASSISTS" << "  " <<  std::right << "DEATHS" << "  " << std::right << std::setw(6) << "WIN%  " << std::right << "LATE GAME KILLS" << std::endl;
	std::sort(champions.begin(), champions.end(), customSort);
	for (unsigned int i = 0; i < champions.size(); i++) {
		float win_rate = champions[i].getChampWinRate();
		output << std::left << std::setw(25) << champions[i].getChampName() << std::right << std::setw(18) << champions[i].getEarlyChampAssists() << " " << std::right << std::setw(7) << champions[i].getChampDeaths() << std::setw(6) << std::setprecision(2) << std::fixed << win_rate << std::setw(5) << champions[i].getLateChampKills() << std::endl;
	}
}


int main(int argc, char* argv[]) {
	std::vector<Player> players;
	std::vector<Champion> champions;
	std::string word;
	std::ifstream games;
	std::ofstream output;
	output.open(argv[2], std::ofstream::out | std::ofstream::trunc);
	games.open(argv[1]);
	while (games >> word) {
		std::vector<int> player_indices;
		std::vector<int> champ_indices;
		std::string fresh_var;
		getChamps(games, players, champions, player_indices, champ_indices); //populates players and champs with new info, as well 
		games >> word;
		//std::string current = champions[champ_indices[0]].getCurrentPlayer();
		readEvents(games, champions, players, player_indices, champ_indices);
	}


	//printPlayers(players, output);

	if (std::string(argv[3]) == "players") {
		printPlayers(players, output);
	}
	else if (std::string(argv[3]) == "champions") {
		printChampions(champions, output);
	}
	else if (std::string(argv[3]) == "custom") {
		printCustom(champions, output);
	}
	output.close();

}