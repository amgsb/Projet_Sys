NOM=BlackJack

all: BlackJack

#Pour lancer l'executable
run: BlackJack
	./BlackJack fichier

# Edition de liens
BlackJack: main.o read_FILE.o deck.o
	gcc main.o read_FILE.o deck.o -o BlackJack -pthread

# Compilation
main.o: main.h read_FILE.h deck.h
	gcc -c -Wall main.c -pthread
	
read_FILE.o: read_FILE.h main.h
	gcc -c -Wall read_FILE.c -pthread
	
deck.o: deck.h
	gcc -c -Wall deck.c -pthread
	
clean:
	rm -f *.o
	rm -f slider
