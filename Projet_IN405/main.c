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

	pthread_mutex_t mutex=PTHREAD_MUTEX_INITIALIZER; 
	pthread_cond_t cond_morecards=PTHREAD_COND_INITIALIZER;
	pthread_cond_t cond_cards=PTHREAD_COND_INITIALIZER;
	

void print_all_cards(card_t* c)
{
  while (c->next!= NULL)
    {
		printCard (c->value);	
		c=c->next;
    }
    printCard (c->value);	
}

int sum_hand(card_t * c)
{
cardvalue_t cv;
int count=0;

	while (c !=NULL)
	{
		cv=getValueFromCardID (c->value);
		if (cv==1 && count<11) {count=count+11;}
		else if (cv>10){ count=count+10;}
		else {count=count+cv;}
		c=c->next;
	}
	return count;
}

card_t* create_cards(deck_t* d,card_t* c)
{
	card_t* tmp =malloc(sizeof(card_t));
	tmp->value=drawCard(d);
	tmp->next=c;
	return tmp;
}

card_t* free_cards(card_t* c)
{
	 card_t* tmp ;
  while (tmp->next != NULL)
    {
      tmp = c;
      c = c->next;
      free (tmp);
    }
  free(tmp)
;  free (c);
	return NULL;
}

card_t* two_cards(deck_t* d)
{
	card_t* b;
	b = NULL;
	b = create_cards(d,b);
	b = create_cards(d,b);
	return b;
}

void *game(void* players)
{
	PLAYER* p;
	int bet;
	int i=0;
	
	p=(PLAYER*)players;
	printf("\n Thread \n");
	print_all_cards(p->c);	
	
	int a=sum_hand (p->c);	
	printf("\n \n a = %d \n \n",a);
	
	//envoyer mise
	fprintf (stderr,"a= %d    val_stop = %d \n",a,p->stop_val);
	
	while(a<p->stop_val) //mutex pour piocher
	{
		pthread_mutex_lock (&mutex);	
		pthread_cond_signal (&cond_morecards);
		a=sum_hand (p->c);	
		pthread_cond_wait (&cond_cards, &mutex);
		pthread_mutex_unlock (&mutex);	
	}
		pthread_cond_signal (&cond_morecards);
		*p->compt= *p->compt + 1;
	//comparaison avec la bank
	// recupere ou pas la mise
	printf("\n PLAYER : %d",p->nb_chips);
		print_all_cards(p->c);	
	printf("\n");
	
	if (p->type_bet==200) //mise moitié
	{
		p->type_bet=p->type_bet/2;
	}
	else if(p->type_bet==10) //mise double
	{
		p->type_bet=p->type_bet*2;
	}
	fprintf(stderr,"SORTIE THREAD PLAYER : %d \n",p->nb_chips);
	pthread_exit(NULL);
}

int main(int argc, char **argv)
{
	PLAYER* info_players=NULL;
	TABLE t;
	pthread_t thread;
	pthread_t cards;
	deck_t* d;
	card_t* b;
	initDeckLib ();
	int j=0;
		
	info_players=read_file(argv[1],&t,info_players);
	d=initDeck(P52,t.nb_decks);
	shuffleDeck (d);
	b=two_cards(d);
	fprintf(stderr,"\n bankcards = " );
	print_all_cards(b);
	int a=sum_hand (b);	
	printf("\n \n a = %d \n \n",a);
	
	pthread_mutex_init(&mutex,NULL);
	
	for (int i=0; i<t.nb_players; i++)
	{
		info_players[i].compt=&j;
		info_players[i].c=NULL;
		info_players[i].c=two_cards(d);
		pthread_create(&thread,NULL,game,&info_players[i]);
		
			while (j<t.nb_players)
			{
				pthread_mutex_lock (& mutex);
				pthread_cond_wait (& cond_morecards, & mutex);
				pthread_cond_signal (& cond_cards);
				//if(j<t.nb_players) // Pour le dernier signal
					info_players[i].c=create_cards(d,info_players[i].c);
				pthread_mutex_unlock (& mutex);
			}	
		pthread_join(thread,NULL);	
	}
	
	
	for(int i=0; i<t.nb_players; i++)
		free_cards(info_players[i].c);
	fprintf(stderr,"SORTIE");
	
	removeDeck(d);
	
	return 0;
}
