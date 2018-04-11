TABLE read_table(SE_FIC f);

PLAYER read_players(SE_FIC f);

PLAYER* read_file(char * path,TABLE *t,PLAYER* info_players);
void write_file(char * path,PLAYER* p, card_t* b);
