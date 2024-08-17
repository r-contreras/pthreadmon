#include "../model/arena.h"
#include <stdio.h>

arena_t* arena;
int indexes[6] = {0,1,2,3,4,5};

arena_t* arena_create()
{
	initialize_data();
	arena = (arena_t*) malloc(sizeof(arena_t));
	arena->is_running = false;
	//arreglo de pokemones
	arena->trainer_1 = "Trainer 1";
	arena->trainer_2 = "Trainer 2";
	arena->team_1[0] = arena->team_1[1] = arena->team_1[2] = NO_POKEMON;
	arena->team_2[0] = arena->team_2[1] = arena->team_2[2] = NO_POKEMON;
	arena->team_1_count = 0;
	arena->team_2_count = 0;
	arena->team_1_attacker = NULL;
	arena->team_2_attacker = NULL;
	arena->winner = NULL;
	return arena;
}

void arena_destroy(arena_t* arena)
{
	arena_delete_pokemons();
	free(arena);
}

bool arena_ready()
{
	return arena->team_1_count == 3 && arena->team_2_count == 3;
}

void* arena_start_fight(void* args)
{
	if(!arena_ready())
	{
		return false;
	}
	//datos para los hilos
	pthread_t* pokemon_threads_1 = (pthread_t*) malloc(TEAM_SIZE * sizeof(pthread_t));
	pthread_t* pokemon_threads_2 = (pthread_t*) malloc(TEAM_SIZE * sizeof(pthread_t));
	pthread_mutex_t* battle_modifier = (pthread_mutex_t*) malloc(sizeof(pthread_mutex_t));
	pthread_barrier_t* init_barrier = (pthread_barrier_t*) malloc(sizeof(pthread_barrier_t));
	pthread_mutex_init(battle_modifier, NULL);
	pthread_barrier_init(init_barrier,NULL,6);

	//se crean los pokemones antes de lanzar los hilos
	for(int pokemon = 0; pokemon < TEAM_SIZE; ++pokemon)
	{
		arena->pokemon_data_list[pokemon].pokemon = pokemon_create(arena->team_1[pokemon]);
		arena->pokemon_data_list[pokemon+3].pokemon = pokemon_create(arena->team_2[pokemon]);
	}

	//TEAM 1
	for(int pokemon = 0; pokemon < TEAM_SIZE; ++pokemon)
	{
		arena->pokemon_data_list[pokemon].enemy_index = pokemon+3;
		arena->pokemon_data_list[pokemon].battle_modifier = battle_modifier;
		arena->pokemon_data_list[pokemon].init_barrier = init_barrier;
		if(pokemon < TEAM_SIZE-1)
		{
			arena->pokemon_data_list[pokemon].next_pokemon_index = pokemon+1;
		}
		else
		{
			arena->pokemon_data_list[pokemon].next_pokemon_index = -1;
		}
		pthread_create(&pokemon_threads_1[pokemon], NULL, fight, (void*)&indexes[pokemon]);
	}
	//TEAM 2
	for(int pokemon = 0; pokemon < TEAM_SIZE; ++pokemon)
	{
		arena->pokemon_data_list[pokemon+3].enemy_index = pokemon;
		arena->pokemon_data_list[pokemon+3].battle_modifier = battle_modifier;
		arena->pokemon_data_list[pokemon+3].init_barrier = init_barrier;
		if(pokemon < TEAM_SIZE-1)
		{
			arena->pokemon_data_list[pokemon+3].next_pokemon_index = pokemon+4;
		}
		else
		{
			arena->pokemon_data_list[pokemon+3].next_pokemon_index = -1;
		}
		pthread_create(&pokemon_threads_2[pokemon], NULL, fight, (void*)&indexes[pokemon+3]);
	}

	//se lanzan los primeros dos pokemones a la batalla
	arena->team_1_attacker = pokemon_throw_into_battle(arena->pokemon_data_list[0].pokemon);
	arena->team_2_attacker = pokemon_throw_into_battle(arena->pokemon_data_list[3].pokemon);
	arena->is_running = true;
	arena->start_mark = clock();

	//JOIN TEAM 1
	for(int pokemon = 0; pokemon < TEAM_SIZE; ++pokemon)
	{
		pthread_join(pokemon_threads_1[pokemon],NULL);
	}
	//JOIN TEAM 2
	for(int pokemon = 0; pokemon < TEAM_SIZE; ++pokemon)
	{
		pthread_join(pokemon_threads_2[pokemon],NULL);
	}
	arena->is_running = false;
	arena->end_mark = clock();

	arena->total_battle_time = (double) (arena->end_mark - arena->start_mark)/100000;

	pthread_mutex_destroy(battle_modifier);
	pthread_barrier_destroy(init_barrier);
	free(battle_modifier);
	free(init_barrier);
	free(pokemon_threads_1);
	free(pokemon_threads_2);
	return NULL;
}

bool arena_add_pokemon_team_1(int id)
{
	for(int pokemon = 0; pokemon < arena->team_1_count; ++pokemon)
	{
		if(arena->team_1[pokemon] == id)
		{
			return false;
		}
	}
	if(arena->team_1_count < TEAM_SIZE)
	{
		arena->team_1[arena->team_1_count++] = id;
	}
	return true;
}

bool arena_add_pokemon_team_2(int id)
{
	for(int pokemon = 0; pokemon < arena->team_1_count; ++pokemon)
	{
		if(arena->team_2[pokemon] == id)
		{
			return false;
		}
	}

	if(arena->team_2_count < TEAM_SIZE)
	{
		arena->team_2[arena->team_2_count++] = id;
	}
	return true;
}

void arena_clear_team_1()
{
	for(int pokemon = 0; pokemon < TEAM_SIZE; ++pokemon)
	{
		arena->team_1[pokemon] = NO_POKEMON;
	}
	arena->team_1_count = 0;
}

void arena_clear_team_2()
{
	for(int pokemon = 0; pokemon < TEAM_SIZE; ++pokemon)
	{
		arena->team_2[pokemon] = NO_POKEMON;
	}
	arena->team_2_count = 0;
}

void arena_delete_pokemons()
{
	//Se borran los pokemones
	for(int pokemon = 0; pokemon < 2*TEAM_SIZE; ++pokemon)
	{
		pokemon_destroy(arena->pokemon_data_list[pokemon].pokemon);
	}
}

void arena_set_trainer_1_name(char* name)
{
	arena->trainer_1 = name;
}

void arena_set_trainer_2_name(char* name)
{
	arena->trainer_2 = name;
}


//funcion de los hilos
void* fight(void* args)
{
	int index = (int) (* (int*)args);
	pokemon_data_t* me = &arena->pokemon_data_list[index];
	pthread_barrier_wait(me->init_barrier);
	pthread_mutex_lock(&me->pokemon->pokeball); //si la pokebola esta abierta entonces acabo de salir
	while(me->pokemon->hp > 0)
	{
		pthread_mutex_lock(me->battle_modifier);
		if(me->enemy_index == NO_POKEMON || !arena->is_running){
			pthread_mutex_unlock(&me->pokemon->pokeball); //se abre la pokebola para guardarme
			pokemon_retrieve_from_battle(me->pokemon); //me guarda en la pokebola
			pthread_mutex_unlock(me->battle_modifier);
			if(!arena->is_running)
			{
				return NULL; //Esto indica que la simulacion no esta corriendo y por lo tanto el hilo debe morir.
			}
			if(index < 3)
			{
				arena->winner = arena->trainer_1;
			}
			else
			{
				arena->winner = arena->trainer_2;
			}
			return NULL;
		}

		//si tengo suficiente energia, el ataque es cargado.
		if(me->pokemon->current_energy >= me->pokemon->charged_attack.energy)
		{
			pokemon_charge_attack(me->pokemon, arena->pokemon_data_list[me->enemy_index].pokemon);
			pthread_mutex_unlock(me->battle_modifier);
			//me espero al cooldown del charged_attack
			usleep(me->pokemon->charged_attack.cooldown*1000);
		}
		else //si no, hago un ataque rapido
		{
			pokemon_fast_attack(me->pokemon, arena->pokemon_data_list[me->enemy_index].pokemon);
			pthread_mutex_unlock(me->battle_modifier);
			//me espero al cooldown del fast attack
			usleep(me->pokemon->fast_attack.cooldown*1000);
		}
	}
	//si llego aqui es porque mori :(
	pthread_mutex_lock(me->battle_modifier);

	pthread_mutex_unlock(&me->pokemon->pokeball); //se abre la pokebola para guardarme
	pokemon_retrieve_from_battle(me->pokemon); //me guarda en la pokebola

	if(me->next_pokemon_index!= NO_POKEMON) //si no soy el ultimo
	{
		arena->pokemon_data_list[me->enemy_index].enemy_index = me->next_pokemon_index;
		arena->pokemon_data_list[me->next_pokemon_index].enemy_index = me->enemy_index;

		//se cambia la referencia al que ataca
		if(index < 3)
		{
			arena->team_1_attacker = pokemon_throw_into_battle(arena->pokemon_data_list[me->next_pokemon_index].pokemon); //esto llama al pokemon y lo activa de una
		}
		else
		{
			arena->team_2_attacker = pokemon_throw_into_battle(arena->pokemon_data_list[me->next_pokemon_index].pokemon);
		}

	}
	else //si soy el ultimo
	{
		arena->pokemon_data_list[me->enemy_index].enemy_index = NO_POKEMON;
	}

	pthread_mutex_unlock(me->battle_modifier);
	return NULL;
}
