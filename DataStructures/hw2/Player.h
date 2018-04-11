#include <vector>
#include <string>

class Player {
	private:
		std::string player_name;
		int player_games;
		int player_games_won;
		int player_kills;
		int player_deaths;
		int player_assists;
		std::vector<std::string> champs_played;

	public: 
		Player(std::string player_name, int game_value, std::string init_champ_name);
		float getWinRate() const;
		float getKDR() const;
		std::string getPlayerName() const;
		int getPlayerKills() const;
		int getPlayerDeaths() const;
		void playerKill();
		void addChamp(std::string champ_name);
		void playerAssistKill();
		void playerKilled();
		std::vector<std::string> getChampsPlayed() const;
		void winGame();
		void loseGame();
		void addPlayerGame(int game_value);
};