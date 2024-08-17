#ifndef POKEMON_H
#define POKEMON_H

#include <stdlib.h>
#include <pthread.h>
#include <math.h>
#include <time.h>
#include <unistd.h>
#include "pokedex.h"

#define MAX_HEALTH 1500

typedef struct
{
	char* move_name;
	int id;
	int typeId;
	int cooldown;
	int power;
	int energy;
	int energy_gain;

} poke_attack_t;

typedef struct
{
	//stats
	int id;
	char* species_name;
	int hp;
	int typeId;
	int current_energy;
	//ataques
	poke_attack_t fast_attack;
	poke_attack_t charged_attack;
	//manejo de hilos
	pthread_mutex_t pokeball;
	//calculo de tiempo
	double battle_time;
	clock_t thrown_mark;
	clock_t retrieved_mark;
} pokemon_t;

typedef struct pokemon_data
{
	pokemon_t* pokemon;
	int enemy_index;
	int next_pokemon_index;
	pthread_mutex_t* battle_modifier;
	pthread_barrier_t* init_barrier;
} pokemon_data_t;

pokemon_t* pokemon_create(int id);

void pokemon_destroy(pokemon_t* pokemon);

void pokemon_fast_attack(pokemon_t* attacker, pokemon_t* target);

void pokemon_charge_attack(pokemon_t* attacker, pokemon_t* target);

void pokemon_reset_health(pokemon_t* pokemon);

pokemon_t* pokemon_throw_into_battle(pokemon_t* pokemon);

void pokemon_retrieve_from_battle(pokemon_t* pokemon);

//gets
int pokemon_get_id(pokemon_t* pokemon);

double pokemon_get_battle_time(pokemon_t* pokemon);
//hilo

void* fight(void* args);
#endif
