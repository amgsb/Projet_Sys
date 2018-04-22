#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <pthread.h>
#include <fcntl.h>
#include <unistd.h>
#include <semaphore.h>
#include "main.h"
#include "read_FILE.h"
#include "deck.h"

	pthread_mutex_t mutex=PTHREAD_MUTEX_INITIALIZER; 
	pthread_mutex_t mex=PTHREAD_MUTEX_INITIALIZER; 
	pthread_cond_t cond_cards=PTHREAD_COND_INITIALIZER;
	pthread_cond_t cond_morecards=PTHREAD_COND_INITIALIZER;
	pthread_barrier_t barr;
	
	pthread_cond_t att=PTHREAD_COND_INITIALIZER;
	pthread_cond_t attback=PTHREAD_COND_INITIALIZER;
	
	

void print_all_cards(card_t* c)
{
  while (c!= NULL)
    {
		printCard (c->value);	
		c=c->next;
    }
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
  while (c != NULL)
    {
      tmp = c;
      c = c->next;
      free (tmp);
    }
 
  free (c);
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
	p=(PLAYER*)players;
	p->sum=0;
	p->sum=sum_hand(p->c);
	p->bet=p->type_bet;
	//printf("ID= %d   chips = %d ; type_bet = %d; stop_val = %d ; obj = %d \n",p->id,p->nb_chips, p->type_bet, p->stop_val, p->obj);
	
	

	while ( ((p->nb_chips) < (p->obj)) && (((p->nb_chips) - (p->bet)) >0))
	{
		//fprintf(stderr," p->compt= %d &&  p->id= %d----- 1111111111111 \n", *p->compt, p->id);
		//~ fprintf(stderr," p->compt= %d &&  p->id= %d----- wait broadcast \n", *p->compt, p->id);
		//pthread_cond_wait(&attback,&mutex);
		pthread_barrier_wait(&barr);
		
		//~ fprintf(stderr," p->compt= %d &&  p->id= %d-----  broadcasted \n", *p->compt, p->id);
		if ((*p->compt) == (p->id))
		{
			//~ fprintf(stderr," p->compt= %d &&  p->id= %d----- 2222222222 \n", *p->compt, p->id);
		//	fprintf(stderr," sum = %d \n", sum);
			//fprintf(stderr," stop val = %d \n", p->stop_val);
			while (p->sum < (p->stop_val))
			{
				
				pthread_mutex_lock(&mutex);
				pthread_cond_signal(&cond_cards);
				//~ fprintf(stderr," p->compt= %d &&  p->id= %d----- 33333 \n", *p->compt, p->id);
				p->sum=sum_hand(p->c);
				fprintf(stderr," SUMMM= %d \n", p->sum);
				print_all_cards(p->c);
				pthread_cond_wait(&cond_morecards,&mutex);
				pthread_mutex_unlock(&mutex);
				//~ fprintf(stderr," p->compt= %d &&  p->id= %d----- FIN 33333 \n", *p->compt, p->id);
			}
			*p->compt=(*p->compt)+1;
			
			//if (*p->compt==p->nb_players+1)*p->compt=0;
			//~ fprintf(stderr," p->compt= %d &&  p->id= %d----- FIN 2222222222 \n", *p->compt, p->id);
		}
		//fprintf(stderr," p->compt= %d &&  p->id= %d----- FIN 1111111111111 \n", *p->compt, p->id);
		//fichier
	}
	pthread_exit(NULL);
}

int main(int argc, char **argv)
{
	PLAYER* info_players=NULL;
	TABLE t;
	int i=0;
	int sum_bank=0;
	deck_t* d;
	card_t* b;
	initDeckLib ();
	pthread_mutex_init(&mutex,NULL);
	pthread_t thread;
	info_players=read_file(argv[1],&t,info_players);
	d=initDeck(P52,t.nb_decks);
	shuffleDeck (d);
	printDrawPile(d);
	b=two_cards(d);
	fprintf(stderr,"\n bankcards = " );
	print_all_cards(b);
	
	pthread_barrier_init(&barr, NULL, t.nb_players);
		
	for (int j=0; j<t.nb_players; j++) //Cree tous les Threads
	{
		
		info_players[j].compt=&i; //Initialise le compteur à la meme adresse que i;
		info_players[j].nb_players=t.nb_players; 
		info_players[j].c=NULL; 
		info_players[j].c=two_cards(d); //Donne 2 cartes au joueur;
		info_players[j].id=j; //Donne l'id du joueur
		pthread_create(&thread,NULL,game,&info_players[j]); //Creation thread un par un
	}
		
	while (i<t.nb_players+1)  // PLUS 1 !!!!!!!!!!!!!
	{
		fprintf(stderr,"là \n");
		 
		for (int k=0; k<t.nb_players;k++)
			pthread_cond_signal(&attback);

		pthread_mutex_lock(&mex);	
		pthread_cond_wait(&cond_cards,&mutex);	//marche pas
		fprintf(stderr,"\n PIOOOOOOOOOCHE  i= %d \n",i);
		info_players[i].c=create_cards(d,info_players[i].c);
		pthread_cond_signal(&cond_morecards); 
		fprintf(stderr,"\n FIN PIOOOOOOOOOCHE \n");
		pthread_mutex_unlock(&mex);		
		
		if (i==t.nb_players+1)
		{
			i=0;
			while(sum_bank < 16)
			{
				b=create_cards(d,b);
			}
			while(i<t.nb_players)
			{
				if(info_players[i].sum < sum_bank && sum_bank<=21)
				{
					info_players[i].nb_chips= info_players[i].nb_chips-info_players[i].bet;
				}
				else if (info_players[i].sum > sum_bank && info_players[i].sum < 21 )
				{
					info_players[i].nb_chips=info_players[i].nb_chips+info_players[i].bet;
				}
				if(info_players[i].sum > 21)
				{
					info_players[i].nb_chips= info_players[i].nb_chips-info_players[i].bet;
				}
				free_cards(info_players[i].c);
			}
			
		}
		printf(stderr,"IIIIICCCCCCCCCIIIIIII");
		shuffleDeck(d);
		i=0;
	}
	
	for (int k=0; k<t.nb_players; k++)
	{
		pthread_join(thread,NULL);
	}
	
	free(info_players);
	removeDeck(d);

	
	return 0;
}
