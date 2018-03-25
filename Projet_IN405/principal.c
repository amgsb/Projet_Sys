#include <stdio.h>
#include <stdlib.h>

void read_table(FILE *f)
{  
	int nb_players, nb_decks, nb_hands;
	fscanf (f, " %d", &nb_players);
	fscanf (f, "; %d", &nb_decks);
	fscanf (f, "; %d", &nb_hands);
	  
	printf("players = %d ; decks = %d ; hands = %d \n",nb_players,nb_decks,nb_hands);
}

void read_players(FILE *f)
{  
	int nb_chips, type_bet, stop_val, obj;
	fscanf (f, " %d", &nb_chips);
	fscanf (f, "; %d", &type_bet);
	fscanf (f, "; %d", &stop_val);
	fscanf (f, "; %d", &obj);
	  
	printf("chips = %d ; type_bet = %d ; stop_val = %d ; obj = %d \n",nb_chips, type_bet, stop_val, obj);
  
}

void read_file(char * name_file)
{	
	FILE *f = NULL;
	f = fopen (name_file, "r");
	if (f == NULL)
    {
		printf ("file not open");
		exit (0);
    }
    read_table(f);
    read_players(f);
    
    fclose(f);
}

int main(int argc, char **argv)
{
	read_file(argv[1]);
	return 0;
}
