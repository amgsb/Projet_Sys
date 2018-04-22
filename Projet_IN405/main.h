#include "deck.h"

struct player {
	int nb_chips, type_bet,bet, stop_val, obj,id,nb_players,sum;
	
	int *compt; // Pour les mutex
	card_t* c;
};typedef struct player PLAYER;

struct table {
	int nb_players, nb_decks,nb_hands;
};typedef struct table TABLE;

typedef struct {
	int descript;
	char * path;
	int acces;
} SE_FIC;
