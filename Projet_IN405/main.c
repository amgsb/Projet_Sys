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
	card_t c;
	p=(PLAYER*)players;
	
	c.next=NULL;
	c.value=1;
	
	printf("p.nb_chips = %d \n", p->nb_chips);
		
	pthread_exit(NULL);
}

int main(int argc, char **argv)
{
	PLAYER* info_players=NULL;
	TABLE t;
	pthread_t thread;
	
	info_players=read_file(argv[1],&t,info_players);

	for (int i; i<t.nb_players; i++)
	{
		pthread_create(&thread,NULL,game,&info_players[i]);	
		pthread_join(thread,NULL);
	}
	
	
	free(info_players);
	return 0;
}
