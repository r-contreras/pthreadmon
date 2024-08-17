#ifndef ARENA_H
#define ARENA_H
#include "pokemon.h"
#include <pthread.h>
#include <stdbool.h>

#define TEAM_SIZE 3
#define NO_POKEMON -1
typedef struct
{
	bool is_running;
	char* trainer_1;
	char* trainer_2;
	char* winner;
	int team_1[TEAM_SIZE]; //son punteros de IDs, los pokemones se crean al mandar los hilos.
	int team_2[TEAM_SIZE];
	int team_1_count;
	int team_2_count;
	//info para hilos
	pokemon_data_t pokemon_data_list[2*TEAM_SIZE]; //aqui van los 6 pokemones
	pokemon_t* team_1_attacker;
	pokemon_t* team_2_attacker;
	//tiempo de battalla
	double total_battle_time;
	clock_t start_mark;
	clock_t end_mark;
} arena_t;

extern arena_t* arena;

arena_t* arena_create();

void arena_destroy(arena_t* arena);

bool arena_ready();

void* arena_start_fight(void* args);

bool arena_add_pokemon_team_1(int id);

bool arena_add_pokemon_team_2(int id);

void arena_clear_team_1();

void arena_clear_team_2();

void arena_delete_pokemons();

void arena_set_trainer_1_name(char* name);

void arena_set_trainer_2_name(char* name);
#endif
