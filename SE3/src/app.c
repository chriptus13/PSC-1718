#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "fut.h"

/**
 * Imprime a informação de s Competitions no stdout
 **/
void print_comps(Competition *comps, size_t s){
	size_t i = 0;
	for (Competition *c = comps; i<s; c++, i++) printf("=> %s\n\t-ID: %d\n\t-Year: %d\n\t-Current Matchday: %d\n\t-Matchdays: %d\n\t-Number of Teams: %d\n\t-Number of Games: %d\n\n", c->name, c->id, c->year, c->currMatchday, c->nMatchdays, c->nTeams, c->nGames);
}

/**
 * Imprime a informação de s Teams no stdout
 **/
void print_teams(Team *teams, size_t s){
	size_t i = 0;
	for (Team *t = teams; i<s; t++, i++) printf("=> %s\n", t->name);
}

/**
 * Imprime a informação de s Fixtures no stdout
 **/
void print_fixtures(Fixture *fix, size_t s){
	if(fix==NULL) return;
	size_t i = 0;
	for (Fixture *f = fix; i<s; f++, i++) printf("=> %-23s (%d) vs (%d) \t%-23s\n", f->home_team_name, f->goals_home_team, f->goals_away_team, f->away_team_name);
}

/**
 * Lê e retorna uma string do stdin
 **/
char *readline(FILE *fin){
	static char *buffer = NULL;
	static size_t buffer_size = 100;
	
	if(buffer == NULL) buffer = malloc(buffer_size);
	int c = fgetc(fin);
	int i = 0;
	while (c != '\n' && c != EOF) {
		if ( i == buffer_size-1) {
			buffer_size += 100;
			buffer = realloc(buffer, buffer_size);
		}
		buffer[i++] = c;
		c = fgetc(fin);
	}
	buffer[i] = 0;
	return buffer;
}

//Struct Node que representa um nó numa lista
typedef struct node{
	int id;
	size_t size;
	void *arr;
	struct node *next;
}Node;

//Listas
static Node list_competitions, list_teams, list_fixtures;

/**
 * Liberta todo o espaço usado pelas listas
 **/
void free_lists(){
	//list_competitions
	Node *prev = NULL;
	for(Node *it = list_competitions.next; it!=NULL; prev = it, it = it->next){
		for(Competition *itC = (Competition *)it->arr; it->size>0; itC++, it->size--) free(itC->name);
		free(it->arr);
		if(prev!=NULL) free(prev);
	}
	
	//list_teams
	for(Node *it = list_teams.next; it!=NULL; prev = it, it = it->next){
		for(Team *itT = (Team *)it->arr; it->size>0; itT++, it->size--) free(itT->name);
		free(it->arr);
		if(prev!=NULL) free(prev);
	}
	
	//list_fixtures
	for(Node *it = list_fixtures.next; it!=NULL; prev = it, it = it->next){
		for(Fixture *itF = (Fixture *)it->arr; it->size>0; itF++, it->size--) {
			free(itF->home_team_name);
			free(itF->away_team_name);
		}
		free(it->arr);
		if(prev!=NULL) free(prev);
	}
	free(prev);
}

/**
 * Verifica e retorna se existir o array de Competitions na list_competitions da season passada como parâmetro. Colocando o número de comptitions em size
 **/
Competition *contains_comps(int season, size_t *size){
	for(Node *it = list_competitions.next; it!=NULL; it=it->next) if(it->id == season){
		*size = it->size;
		return (Competition *) it->arr;
	}
	return NULL;
}

/**
 * Verifica e retorna se existir o array de Teams na list_teams com o id passado como parâmetro. Colocando o número de teams em size
 **/
Team *contains_team(int id, size_t *size){
	for(Node *it = list_teams.next; it!=NULL; it=it->next) if(it->id == id){
		*size = it->size;
		return (Team *) it->arr;
	}
	return NULL;
}

/**
 * Verifica e retorna se existir o array de Fixtures na list_fixtures com o id passado como parâmetro. Colocando o número de fixtures em size
 **/
Fixture *contains_fixs(int id, size_t *size) {
	for(Node *it = list_fixtures.next; it!=NULL; it=it->next) if(it->id == id) { 
		*size = it->size;
		return (Fixture *)it->arr;
	}
	return NULL;
}

/**
 * Carrega na lista list_fixtures todas as Fixtures
 **/
void loadFixtures() {
	size_t size, aux;
	int *arr = get_comp_ids(&size);
	for(int i=0; i<size; i++) {
		Fixture *f = get_fixtures(*(arr+i), &aux);
		Node *e = malloc(sizeof(Node));
		e->id = *(arr+i);
		e->arr = f;
		e->size = aux;
		e->next = list_fixtures.next;
		list_fixtures.next = e;
	}
	free(arr);
}

/**
 * Caso a lista list_fixtures esteja vazia carrega-a. Imprime as Fixtures de uma certa Team especificada pelo id.
 **/
void fixturesForTeam(int team_id) {
	if(list_fixtures.next==NULL) loadFixtures();
	printf("\nFixtures for Team %d:\n", team_id);
	for(Node *it = list_fixtures.next; it!=NULL; it=it->next) {
		int size = it->size;
		for(Fixture *itF = (Fixture *)it->arr; size>0; itF++, size--) if(itF->home_team_id == team_id || itF->away_team_id == team_id) print_fixtures(itF, 1);
	}
}

/**
 * Caso a lista list_fixtures esteja vazia carrega-a, verifica ainda se a list_fixtures contem um Node com o id da Competition especificada, se não irá pedir ao site. 
 * Imprime as Fixtures dessa Competition.
 **/
void fixturesForComp(int comp_id) {
	size_t size;
	if(list_fixtures.next==NULL) loadFixtures();
	Fixture *fixtures = contains_fixs(comp_id, &size);
	if(fixtures==NULL) {
		fixtures = get_fixtures(comp_id, &size);
		Node *e = malloc(sizeof(Node));
		e->id = comp_id;
		e->arr = fixtures;
		e->size = size;
		e->next = list_fixtures.next;
		list_fixtures.next = e;
	}
	printf("\nFixtures for Competition %d:\n", comp_id);
	print_fixtures(fixtures, size);
}

/**
 * Verifica se a list_teams contem um Node com o id da Competition especificada, se não irá pedir ao site. 
 * Imprime as Teams dessa Competition.
 **/
void teamsForComp(int comp_id) {
	size_t size;
	Team *teams = contains_team(comp_id, &size);
	if(teams==NULL) {
		teams = get_teams(comp_id, &size);
		Node *e = malloc(sizeof(Node));
		e->id = comp_id;
		e->arr = teams;
		e->size = size;
		e->next = list_teams.next;
		list_teams.next = e;
	}
	printf("\nTeams for Competition %d:\n", comp_id);
	print_teams(teams, size);
}

/**
 * Verifica se a list_comps contem um Node com a Season especificada, se não irá pedir ao site. 
 * Imprime as Competitions dessa Season.
 **/
void compsForSeason(int season) {
	size_t size;
	Competition *comps = contains_comps(season, &size);
	if(comps==NULL) {
		comps = get_competitions(season, &size);
		Node *e = malloc(sizeof(Node));
		e->id = season;
		e->arr = comps;
		e->size = size;
		e->next = list_competitions.next;
		list_competitions.next = e;
	}
	printf("\nCompetitions for Season %d:\n", season);
	print_comps(comps, size);
}

int main() {
	do{
		printf("Qual a opção?\n");
		char *line = readline(stdin);
		if(line == NULL) exit(0);
		char *com = strtok(line, " \n");
		char *name = strtok(NULL, " \n");
		
		int season = 2017;
		int comp_id=-1, team_id=-1;
		
		switch (com[0]) {
			case 'q':
			case 'Q':
				printf("\nA sair...\n");
				free_lists();
				exit(0);
			case 'c':
			case 'C':
				if(name!=NULL) season = atoi(name);
				compsForSeason(season);
				break;
			case 't':
			case 'T':
				if(name!=NULL) comp_id = atoi(name);
				if(comp_id>=0) teamsForComp(comp_id);
				break;
			case 'x':
			case 'X':
				if(name!=NULL) comp_id = atoi(name);
				if(comp_id>=0) fixturesForComp(comp_id);
				break;
			case 'j':
			case 'J':
				if(name!=NULL) team_id = atoi(name);
				if(team_id>=0) fixturesForTeam(team_id);
		}
		printf("\n");
	}while(1);
}
