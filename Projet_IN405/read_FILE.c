#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include "main.h"


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

void SE_readint(SE_FIC f, int* i, int b)
{
	int nb=0;
	size_t r=1;
	char c='b';
	r = read(f.descript,&c,sizeof(char));	
	if (f.acces==O_RDONLY)
	{
		while (c=='\n') r = read(f.descript,&c,sizeof(char)); // zappe retours chariots
		for(int j=0; c!=';' || c!=' ' || r!=0; j++)
		{
			if (c=='0' || c=='1' || c=='2' || c=='3' || c=='4' || c=='5' || c=='6' || c=='7' ||c=='8' || c=='9')
				{
					nb=(int)c-48;
					(*i)=(10*(*i))+nb;
					//printf("j=%d ;   nb=%d \n",j, (*i));
				}
				
			else if ((b==0 && c==';') || (b==1 && (c==';' || c=='+'|| c=='-'))) 
			{
				
					if ((b==1 && *i==200 && c!='-' )|| (b==1 && *i==10 && c!='+') ) {printf("c= %c > WRONG FILE \n \n",c); exit(0);}
					if (b==1 && (c=='+'|| c=='-'))
					{ 
						read(f.descript,&c,sizeof(char));
						if (c!=';') 
						{
							printf("c= %c > WRONG FILE \n \n",c);
							exit(0);
						}
					}
					break ;
			}
			else
				{
					printf("c= %c > WRONG FILE \n \n",c);
					exit(0);
				}
		r = read(f.descript,&c,sizeof(char));
		}		
	}
	
	else 
	{
		printf("Bad access :O_RDONLY needed \n");
	}
}	

TABLE read_table(SE_FIC f)
{  
	TABLE t;
	t.nb_players=t.nb_decks=t.nb_hands=0;
	SE_readint(f,&t.nb_players,0);
	//printf("joueurs = %d \n", t.nb_players);
	SE_readint(f,&t.nb_decks,0);
	//printf("decks = %d \n", t.nb_decks);
	SE_readint(f,&t.nb_hands,0);
	//printf("hands = %d \n", t.nb_hands);
	return t;
}

PLAYER read_players(SE_FIC f)
{  
	PLAYER p;
	char sign=' ';
	p.nb_chips = p.type_bet = p.stop_val = p.obj = 0;
	SE_readint(f, &p.nb_chips,0);
	SE_readint(f,&p.type_bet,1);
	//printf("chips = %d bet = %d \n",p.nb_chips, p.type_bet);
		if (p.type_bet!=10 && p.type_bet!=200 && p.type_bet!=50)
		{
			printf("error bet \n");
		    exit(1);
		}
	SE_readint(f, &p.stop_val,0);
	SE_readint(f, &p.obj,0);
	  
	printf("chips = %d ; type_bet = %d %c; stop_val = %d ; obj = %d \n",p.nb_chips, p.type_bet, sign, p.stop_val, p.obj);
	return p;
  
}

PLAYER* read_file(char * path,TABLE *t,PLAYER* info_players)
{	
	SE_FIC f;
	
	TABLE tab;

	
	f=open_file(path, O_RDONLY);
	tab=read_table(f);
	
	*t=tab;

    info_players=malloc(tab.nb_players*sizeof(PLAYER));
       
    for(int i=0; i<tab.nb_players; i++)
    {
		info_players[i]=read_players(f);
		PLAYER p=info_players[i];
		printf("FONCTION info_players[%d]= %d \n", i, p.nb_chips);	
	}
	
	if (close(f.descript)==-1) fprintf(stderr,"ERRRRREUR fermeture");
	
	return &info_players[0];
}

void write_file(char * path,PLAYER* p, card_t* b)
{
	SE_FIC f;
	f=open_file(path, O_CREAT);
	f=open_file(path, O_WRONLY);
	char nb_chips,type_bet,stop_val,obj;
	
	nb_chips =(char)p->nb_chips;						//cast en char
	type_bet =(char)p->type_bet;
	stop_val =(char)p->stop_val;
	obj =(char)p->obj;
	
	f.descript,&c,sizeof(char)
	//printf("EEEEEEEEEEEEEEEEEEp.nb_chips= %d",nb_chips);
//	write(f.descript,&nb_chips,sizeof(char));
	
//	p.nb_chips, p.type_bet, p.stop_val, p.obj;
	
	
	
	
	
}
