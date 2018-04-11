#include "Champion.h"
#include <string>
#include <iostream>

Champion::Champion(std::string init_champ_name, int game_value, std::string init_player_name) {
	champ_name = init_champ_name;
	champ_games = 1;
	champ_games_won = 0 + game_value;
	minion_deaths = 0;
	champ_kills = 0;
	champ_deaths = 0;
	champ_assists = 0;
	current_player = init_player_name;
	early_champ_assists = 0;
	late_champ_kills = 0;
}

float Champion::getChampWinRate() const{
	float win_rate = (float) champ_games_won / champ_games;
	return win_rate;
}

std::string Champion::getChampName() const{
	return champ_name;
}

std::string Champion::getCurrentPlayer() const{
	return current_player;

}

void Champion::killedByMinion() {
	minion_deaths++;
}

void Champion::updateCurrentPlayer(std::string player_name) {
	current_player = player_name;
}

int Champion::getChampKills() const{
	return champ_kills;
}

int Champion::getChampDeaths() const{
	return champ_deaths;
}

int Champion::getChampAssists() const{
	return champ_assists;
}

void Champion::champKill() {
	champ_kills++;
}
void Champion::champKilled() {
	champ_deaths++;
}

void Champion::champAssist() {
	champ_assists++;
}

int Champion::getChampWins() const{
	return champ_games_won;
}

int Champion::getChampLosses() const {
	return champ_games - champ_games_won;
}

int Champion::getMinionDeaths() const {
	return minion_deaths;
}

void Champion::winGame() {
	champ_games++;
	champ_games_won++;
}

void Champion::loseGame() {
	champ_games++;
}

void Champion::addChampGame(int game_value) {
	champ_games++;
	champ_games_won += game_value;
}

void Champion::earlyGameAssist() {
	early_champ_assists++;
}

int Champion::getEarlyChampAssists() const{
	return early_champ_assists;
}

void Champion::lateGameKill() {
	late_champ_kills++;
}

int Champion::getLateChampKills() const{
	return late_champ_kills;
}