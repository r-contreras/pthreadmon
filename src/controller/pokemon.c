#include "../model/pokemon.h"
//interno
poke_attack_t get_attack(int id)
{
	poke_attack_t attack;
	attack.move_name = get_move_name(id);
	attack.id = id;
	attack.typeId = get_move_type_id(id);
	attack.cooldown = get_move_cooldown(id);
	attack.power = get_move_power(id);
	attack.energy = get_move_energy(id);
	attack.energy_gain = get_move_energy_gain(id);

	return attack;
}

void apply_attack(int power, int moveTypeID, pokemon_t* target)
{
	int targetType = target->typeId;
	double effectiveness = 1;
	//switch
	if(resistances_matrix[targetType][moveTypeID]) //si attacker no es muy efectivo contra targetType
	{
		effectiveness = 0.625;
	}
	else
	{
		if(weaknesses_matrix[targetType][moveTypeID]) //si attacker es muy efectivo contra target
		{
			effectiveness = 1.6;
		}
		else
		{
			if(immunities_matrix[targetType][moveTypeID]) //si target es immune a attacker
			{
				effectiveness = 0.390625;
			}
		}
	}
	//end of switch
	target->hp -= floor(power * effectiveness * BONUS) + 1;
}

//declarados en headers
pokemon_t* pokemon_create(int id)
{
	//se crea en memoria dinamica
	pokemon_t* pokemon = (pokemon_t*) malloc(sizeof(pokemon_t));
	//se inicializan sus stats
	pokemon->id = id;
	pokemon->species_name = get_pokemon_species_name(id);
	pokemon->hp = MAX_HEALTH;
	pokemon->typeId = get_pokemon_type_id(id);
	pokemon->battle_time = 0;
	pokemon->current_energy = 0;
	//se asignan sus ataques
	pokemon->fast_attack = get_attack(get_pokemon_fast_move_id(id));
	pokemon->charged_attack = get_attack(get_pokemon_charged_move_id(id));
	//se asigna el mutex
	//se inicializa y bloquea
	pthread_mutex_init(&pokemon->pokeball,NULL);
	pthread_mutex_lock(&pokemon->pokeball);
	//variables de calculo de tiempo
	pokemon->battle_time = pokemon->thrown_mark = pokemon-> retrieved_mark = 0.0;
	return pokemon;
}

void pokemon_destroy(pokemon_t* pokemon)
{
	pthread_mutex_destroy(&pokemon->pokeball);
	free(pokemon);
}

void pokemon_fast_attack(pokemon_t* attacker, pokemon_t* target)
{
	apply_attack(attacker->fast_attack.power,attacker->fast_attack.typeId,target);
	//Se le suma al contador de energia
	attacker->current_energy+=attacker->fast_attack.energy_gain;
}

void pokemon_charge_attack(pokemon_t* attacker, pokemon_t* target)
{
	apply_attack(attacker->charged_attack.power,attacker->charged_attack.typeId,target);
	//Se resetea la energia actual
	attacker->current_energy = 0;
}

void pokemon_reset_health(pokemon_t* pokemon)
{
	pokemon->hp = MAX_HEALTH;
}

pokemon_t* pokemon_throw_into_battle(pokemon_t* pokemon)
{
	pthread_mutex_unlock(&pokemon->pokeball); //me saca de la pokebola
	pokemon->thrown_mark = clock(); //toma el tiempo actual
	return pokemon;
}

void pokemon_retrieve_from_battle(pokemon_t* pokemon)
{
	pokemon->retrieved_mark = clock(); //toma el tiempo actual
	pokemon->battle_time = (double) (pokemon->retrieved_mark - pokemon->thrown_mark)/100000;
	pthread_mutex_lock(&pokemon->pokeball); //me guarda en la pokebola
}

//gets
int pokemon_get_id(pokemon_t* pokemon)
{
	return pokemon->id;
}

double pokemon_get_battle_time(pokemon_t* pokemon)
{
	return pokemon->battle_time; //calculo interno
}
