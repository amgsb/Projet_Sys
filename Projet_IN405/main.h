struct player {
	int nb_chips, type_bet, stop_val, obj;
};typedef struct player PLAYER;

struct table {
	int nb_players, nb_decks,nb_hands;
};typedef struct table TABLE;

typedef struct {
	int descript;
	char * path;
	int acces;
} SE_FIC;
