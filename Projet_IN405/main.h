#include "deck.h"

struct player {
	int nb_chips, type_bet, stop_val, obj;
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
