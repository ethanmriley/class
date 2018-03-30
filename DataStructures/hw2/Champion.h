#include <string>

class Champion{
	private:
		std::string champ_name;
		int champ_games;
		int champ_games_won;
		int minion_deaths;
		int champ_kills;
		int champ_deaths;
		int champ_assists;
		int early_champ_assists;
		int late_champ_kills;
		std::string current_player;

	public:
		Champion(std::string champ_name, int game_value, std::string init_player_name);
		float getChampWinRate() const;
		std::string getChampName() const;
		void winGame();
		void loseGame();
		std::string getCurrentPlayer() const;
		void killedByMinion();
		void updateCurrentPlayer(std::string player_name);
		void champKill();
		void champKilled();
		int getChampKills() const;
		int getChampDeaths() const;
		int getChampAssists() const;
		void champAssist();
		int getChampWins() const;
		int getChampLosses() const;
		int getMinionDeaths() const;
		void addChampGame(int game_value);
		void earlyGameAssist();
		int getEarlyChampAssists() const;
		void lateGameKill();
		int getLateChampKills() const;
};