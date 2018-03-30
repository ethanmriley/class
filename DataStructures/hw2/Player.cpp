#include "Player.h"
#include <cstdlib>
#include <string>

Player::Player(std::string init_player_name, int game_value, std::string init_champ_name) {
	player_games = 1;
	player_games_won += game_value;
	player_kills = 0;
	player_deaths = 0;
	player_assists = 0;
	player_name = init_player_name;
	champs_played.push_back(init_champ_name);
}

float Player::getWinRate() const{
	float win_rate = 100 * (player_games_won / player_games);
	return win_rate;
}

float Player::getKDR() const{
	if (player_deaths == 0) {
		return (float) player_kills;
	}
	else {
		float kdr = (float) player_kills / player_deaths;
		return kdr;
	}
}

std::string Player::getPlayerName() const{
	return player_name;
}

int Player::getPlayerKills() const{
	return player_kills;
}

void Player::playerKill() {
	player_kills++;
}

void Player::addChamp(std::string champ_name) {
	for (unsigned int i = 0; i < champs_played.size(); i++) {
		if (champs_played[i] == champ_name) {
			return;
		}
	}
	champs_played.push_back(champ_name);
}

void Player::playerAssistKill() {
	player_assists++;
}

void Player::playerKilled() {
	player_deaths++;
}

int Player::getPlayerDeaths() const{
	return player_deaths;
}

std::vector<std::string> Player::getChampsPlayed() const {
	return champs_played;
}

void Player::winGame() {
	player_games++;
	player_games_won++;
}

void Player::loseGame() {
	player_games++;
}

void Player::addPlayerGame(int game_value) {
	player_games++;
	player_games_won += game_value;
}