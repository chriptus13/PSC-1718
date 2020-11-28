#include <curl/curl.h>
#include <string.h>
#include <stdio.h>
#include <errno.h>
#include <jansson.h>
#include "fut.h"

typedef struct buffer {
	size_t capacity;
	size_t size;
	char *str;
}Buffer;

static size_t my_write(char *ptr, size_t size, size_t nmemb, void *fd) {
    Buffer *b = (Buffer *) fd;
    size_t aux = b->capacity - b->size;
    if(aux<size*nmemb) {
		size_t new_cap = b->capacity+size*nmemb+1;
		b->str = realloc(b->str, new_cap);
		if(b->str!=NULL) b->capacity = new_cap;
		else{
			fprintf(stderr, "Can't allocate more space");
			exit(-1);
		}
	}
	memcpy(b->str + b->size, ptr, size*nmemb);
	b->size += size*nmemb;
    return size*nmemb;
}

json_t *http_get_json_data(const char *url) {
	Buffer *buffer = malloc(sizeof(Buffer));
	buffer->size = 0;
	buffer->capacity = 4096;
	buffer->str = malloc(4096);
	
	curl_global_init(CURL_GLOBAL_DEFAULT);
	CURL *curl = curl_easy_init();
	if(curl!=NULL) {
		curl_easy_setopt(curl, CURLOPT_URL, url);
		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, my_write);
		curl_easy_setopt(curl, CURLOPT_WRITEDATA, buffer);
		curl_easy_setopt(curl, CURLOPT_VERBOSE, 3L);
		
		struct curl_slist *list = NULL;
		// Get a token at https://www.football-data.org/
		list = curl_slist_append(list, "X-Auth-Token: <token>");
		curl_easy_setopt(curl, CURLOPT_HTTPHEADER, list);
		
		CURLcode res = curl_easy_perform(curl);
		curl_easy_cleanup(curl);
		
		if(CURLE_OK != res) fprintf(stderr, "curl told us %d\n", res); 
	}
	
	curl_global_cleanup();
	json_error_t error;
    json_t *data = json_loadb(buffer->str,buffer->size, 0, &error);
 
    free(buffer->str);
    free(buffer);
    return data;
}

int *get_comp_ids(size_t *size){
	char http[50] = "http://api.football-data.org/v1/competitions/";
	printf("%s\n", http);
	
	json_t *data = http_get_json_data(http), *value;
	size_t index, s = json_array_size(data);
	*size = s;
	
	int *res = malloc(sizeof(int)*s);
	
	json_array_foreach(data, index, value){
		int *c = res+index;
		*c = (int)json_number_value(json_object_get(value, "id"));
	}
	json_decref(data);
	return res;
}

Competition *get_competitions(int season, size_t *size){
	char http[60];
	sprintf(http, "http://api.football-data.org/v1/competitions/?season=%d", season);
	printf("%s\n", http);
	
	json_t *data = http_get_json_data(http), *value;
	const char * key;
	size_t index, s = json_array_size(data);
	*size = s;
	
	Competition *res = malloc(sizeof(Competition)*s);
	
	json_array_foreach(data, index, value) {
		json_t *object = value;
		Competition *c = res+index;
		json_object_foreach(object, key, value) {
			if (strcmp(key, "id") == 0) c->id = (int)json_number_value(value);
			else if (strcmp(key, "caption") == 0) {
				size_t len = json_string_length(value);
				c->name = malloc(len+1);
				strcpy(c->name, json_string_value(value));
			}
			else if (strcmp(key, "year") == 0) c->year = atoi(json_string_value(value));
			else if (strcmp(key, "currentMatchday") == 0) c->currMatchday = (int)json_number_value(value);
			else if (strcmp(key, "numberOfMatchdays") == 0) c->nMatchdays = (int)json_number_value(value);
			else if (strcmp(key, "numberOfTeams") == 0) c->nTeams = (int)json_number_value(value);
			else if (strcmp(key, "numberOfGames") == 0) c->nGames = (int)json_number_value(value);
		}
	}
	json_decref(data);
	return res;
}

Team *get_teams(int competition_id, size_t *size){
	char http[60];
	sprintf(http, "http://api.football-data.org/v1/competitions/%d/teams", competition_id);
	printf("%s\n", http);
	
	json_t *data = http_get_json_data(http), *value, *only_teams;
	size_t index, s;
	const char * key;
	
	json_object_foreach(data, key, value){
		if(strcmp(key, "teams") == 0) only_teams = value;
		else if(strcmp(key, "count") == 0) s = json_number_value(value);
	}
	
	*size = s;
	
	Team *teams = malloc(sizeof(Team)*s);
	json_array_foreach(only_teams, index, value) {
		json_t *object = value;
		Team *t = teams+index;
		json_object_foreach(object, key, value) {
			if (strcmp(key, "name") == 0) {
				size_t len = json_string_length(value);
				t->name = malloc(len+1);
				strcpy(t->name, json_string_value(value));
			}
		}
	}
	json_decref(data);
	return teams;
}

Fixture *get_fixtures(int competition_id, size_t *size){
	char http[60];
	sprintf(http, "http://api.football-data.org/v1/competitions/%d/fixtures", competition_id);
	printf("%s\n", http);
	
	json_t *data = http_get_json_data(http), *value, *only_fixtures, *ids, *goals;
	size_t index, s=0;
	const char * key;
	
	json_object_foreach(data, key, value){
		if(strcmp(key, "fixtures") == 0) 
			only_fixtures = value;
		else if(strcmp(key, "count") == 0) 
			s = json_number_value(value);
	}
	
	*size = s;
	
	Fixture *fixtures = malloc(sizeof(Fixture)*s);

	json_array_foreach(only_fixtures, index, value) {
		size_t len;
		json_t *object = value;
		Fixture *f = fixtures+index;
		json_object_foreach(object, key, value) {

			if (strcmp(key, "homeTeamName") == 0) {
				len = json_string_length(value);
				f->home_team_name = (char *) malloc(len+1);
				strcpy(f->home_team_name, json_string_value(value));
			} else if (strcmp(key, "awayTeamName") == 0){
				len = json_string_length(value);
				f->away_team_name = (char *) malloc(len+1);
				strcpy(f->away_team_name, json_string_value(value));
			} 
			else if(strcmp(key, "_links") == 0) ids = value;
			else if (strcmp(key, "result") == 0) goals = value;
		}
	
		//Obter ids para ambas as equipas
		json_t *obj;
		json_object_foreach(ids, key, value) {

			if (strcmp(key, "homeTeam") == 0){
				obj = json_object_get(value, "href");
				char *aux = (char *)json_string_value(obj)+38;
				f->home_team_id = atoi(aux);
			} else if (strcmp(key, "awayTeam") == 0){
				obj = json_object_get(value, "href");
				char *aux = (char *)json_string_value(obj)+38;
				f->away_team_id = atoi(aux);
			}
		}
		
		//Obter Goals para ambas as equipas
		json_object_foreach(goals, key, value) {
			if (strcmp(key, "goalsHomeTeam") == 0) f->goals_home_team = (int) json_number_value(value);
			else if (strcmp(key, "goalsAwayTeam") == 0) f->goals_away_team = (int) json_number_value(value);
		}
	}
	
	json_decref(data);
	return fixtures;
}
