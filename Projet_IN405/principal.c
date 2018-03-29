#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

struct player {
	int nb_chips, type_bet, stop_val, obj;
	char sign;
	
};typedef struct player PLAYER;

typedef struct {
	int descript;
	char * path;
	int acces;
} SE_FIC;



SE_FIC open_file(const char * path, int acces)
{
   SE_FIC f;
    f.descript=open(path,acces,0777);
    f.acces=acces;
    
	if (f.descript==-1)
	{
		printf ("Not open");
	}
    
  return f;
}

int SE_close (SE_FIC f)
{
	if (close(f.descript)==0) return 0;
	
	else 
	{
		printf("Not close");
		return -1;
	}
}


int SE_readint(SE_FIC f, int* i)
{
	int cpt;
	if (f.acces==O_RDONLY)
	{
		size_t r = read(f.descript,&i,sizeof(int));
		cpt=(int)r;
		printf("i= %d ; cpt = %d \n",i, cpt);
	}
	
	else 
	{
		printf("Bad access :O_RDONLY needed \n");
		cpt=-1;
	}
	
	return cpt;
}	

int read_table(SE_FIC f)
{  
	int nb_players, nb_decks, nb_hands;
	int i;
	
	i=SE_readint(f,&nb_players);
	//i=SE_readint(f,&nb_decks);
	//i=SE_readint(f,&nb_hands);
	printf("joueurs = %d \n", nb_players);
	return 1;
	
	
}

PLAYER read_players(int f)
{  
	PLAYER p;
	p.sign=' ';
	size_t lecture = read(f,&p.nb_chips,sizeof(int));
	lecture = read(f,&p.type_bet,sizeof(int));

	printf("chips = %d bet = %d \n",p.nb_chips,p.type_bet);
		if (p.type_bet!=10 && p.type_bet!=200 && p.type_bet!=50)
		{
			printf("error bet \n");
		    exit(1);
		}
		if (p.type_bet==10 || p.type_bet==200)
		{
			lecture = read(f,&p.sign,sizeof(char));
		}
	lecture = read(f,&p.stop_val,sizeof(int));
	lecture = read(f,&p.obj,sizeof(int));
	  
	printf("chips = %d ; type_bet = %d %c; stop_val = %d ; obj = %d \n",p.nb_chips, p.type_bet, p.sign, p.stop_val, p.obj);
	return p;
  
}

void read_file(char * path, 	PLAYER* info)
{	
	int nb_players;
	SE_FIC f;
	char c;
	f=open_file(path, O_RDONLY);
	
    nb_players=read_table(f);
        
    if (close(f.descript)==-1) fprintf(stderr,"ERRRRREUR fermeture");
    
    //~ info=malloc(nb_players*sizeof(PLAYER));
        
    //~ for(int i=0; i<nb_players; i++)
    //~ {
		//~ info[i]=read_players(f);
		//~ printf("chips = %d ; type_bet = %d %c; stop_val = %d ; obj = %d \n",info[i].nb_chips, info[i].type_bet, info[i].sign,info[i].stop_val, info[i].obj);	
	//~ }
}

int main(int argc, char **argv)
{
	PLAYER* info;
	read_file(argv[1], info);
	//free(info);
	return 0;
}
