#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <pthread.h>
#include <fcntl.h>
#include <unistd.h>
#include "main.h"
#include "read_FILE.h"
#include "deck.h"

void *game(void* players)
{
	PLAYER* p;
	
	p=(PLAYER*)players;
	printf("BJR p.nb_chips = %d \n", p->nb_chips);
	
	printf("\n    \n \n CARTE : ");
	printCard (p->c->value);
		
	pthread_exit(NULL);
}

card_t* players_cards(deck_t* d,card_t* c)
{
	card_t* tmp =malloc(sizeof(card_t));
	tmp->value=drawCard(d);
	tmp->next=c;
	return tmp;
}

card_t* free_players_cards(card_t* c)
{
	 card_t* tmp ;
  while (tmp != NULL)
    {
      tmp = c;
      c = c->next;
      free (tmp);
    }
  free (c);
	return NULL;
}

card_t* bank(deck_t* d)
{
	card_t* b;
	b = NULL;
	b = players_cards(d,b);
	b = players_cards(d,b);
	return b;
}


int main(int argc, char **argv)
{
	PLAYER* info_players=NULL;
	TABLE t;
	pthread_t thread;
	deck_t* d;
	initDeckLib ();
	
	info_players=read_file(argv[1],&t,info_players);
	
	d=initDeck(P52,t.nb_decks);
	shuffleDeck (d);
	printf("NB_PLAYERS = %d",t.nb_players);
	bank(d);
	for (int i=0; i<t.nb_players; i++)
	{
		info_players[i].c=NULL;
		info_players[i].c=players_cards(d,info_players[i].c);
		printf("\n ICI    ");
		printCard (info_players[i].c->value);
		printf("    LA \n");
		info_players[i].c=players_cards(d,info_players[i].c);		
		printf("\n ICI    ");
		printCard (info_players[i].c->value);
		printf("    LA \n");
		pthread_create(&thread,NULL,game,&info_players[i]);	
		pthread_join(thread,NULL);
		
	}
	
	
	for (int i=0; i<t.nb_players; i++)
		free_players_cards(info_players[i].c);
	//free(info_players);
	return 0;
}
