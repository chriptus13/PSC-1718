#define FUT_H

typedef struct competition {
	int id, year, currMatchday, nMatchdays, nTeams, nGames;
	char *name;
} Competition;

typedef struct team {
	char *name;
} Team;

typedef struct fixture {
	int goals_home_team, goals_away_team, home_team_id, away_team_id;
	char *home_team_name, *away_team_name;
} Fixture;

int *get_comp_ids(size_t *size);

Competition *get_competitions(int season, size_t *size);

Team *get_teams(int competition_id, size_t *size);

Fixture *get_fixtures(int competition_id, size_t *size);
