#include "define.h"
#include "astronave.h"
#include "nemici1l.h"
#include "controllo.h"
#include "buffer.h"
#include "title.h"

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t mutex_stampe = PTHREAD_MUTEX_INITIALIZER;
sem_t semaforo_buffer;

Buffer buffer;
Object astronave;
Object nemico[NUM_NEMICI];
Object nemico2[NUM_NEMICI];
Object pl;
Object pr;
Object bomba;
int lettori_stampe;

_Bool flagEnd = false;
_Bool flagLose = false;
_Bool flagWin = false;
_Bool stopBullsx = false;
_Bool stopBulldx = false;
_Bool stopBomb = false;

int bullNum = 0;
int bombNum = 1;
int killCounter = 0;
int killCounter2 = 0;

int main(){
	int choice = 0;
	//prepariamo lo schermo
    srand (time(NULL));	
    initscr();
	start_color();
    noecho();
    cbreak();
	curs_set(0);
    keypad(stdscr, TRUE);

	//modifica la sfumatura dei colori
	init_color(COLOR_RED, 1000, 500, 500);
	init_color(COLOR_MAGENTA, 800, 700, 1000);
	init_color(COLOR_YELLOW, 1000, 800, 300);
	init_color(COLOR_GREEN, 0, 900, 750);
	init_color(COLOR_CYAN, 0, 900, 650);
	init_color(COLOR_BLUE, 350, 350, 800);
	//crea coppie di colori (il primo è il colore del carattere, il secondo dello sfondo)
	//colore astronave
	init_pair(1, COLOR_WHITE, COLOR_BLACK);
	//colore nemici
	init_pair(2, COLOR_MAGENTA, COLOR_BLACK);
	//colore nemici2
	init_pair(3, COLOR_CYAN, COLOR_BLACK);
	//colore proiettili
	init_pair(4, COLOR_YELLOW, COLOR_BLACK);
	//colore bombe
	init_pair(5, COLOR_RED, COLOR_BLACK);
	//colore nella schermata hai perso
	init_pair(7, COLOR_BLUE, COLOR_BLACK);
	
	
	//inizializza buffer e semaforo del buffer
	init_buffer();
	sem_init(&semaforo_buffer, 0, NUM_LETTORI);
	lettori_stampe = 0;
	
	//schermata iniziale, return la scelta dal menu
	choice = title();
	
	//se scegli start inizia il gioco, altrimenti esce
	if(choice == 1){		
		//inizializza nave
		initNave();
		//inizializza nemici
		initNemico();
		//funzione principale
		controllo();
	}
	//schermata di vittoria o sconfitta
	if(flagLose){
		clear();
		keypad(stdscr, TRUE);
		hai_perso();
	}

	if(flagWin){
		clear();
		keypad(stdscr, TRUE);
		hai_vinto();
	}
    
	//fine
    endwin();
    return 0;
}

//genera un numero casuale compreso tra min e max
int numRand(int min, int max){
    return min + rand()%(max - min +1);
}

//funzione solo di debug, stampa parametri di un oggetto
void stampaParametriNave(Object *n){
    mvprintw(0,0,"indirizzo nemico: %p", n);
    if(n->tipo == NAVE){
		mvprintw(1,0,"tipo: NAVE");
	}else if(n->tipo == NEMICO){
		mvprintw(1,0,"tipo: NEMICO");
	}else if(n->tipo == PROIETTILE){
		mvprintw(1,0,"tipo: PROIETTILE");
	}else if(n->tipo == BOMBA){
		mvprintw(1,0,"tipo: BOMBA");
	}else if(n->tipo == NEMICO2){
		mvprintw(1,0,"tipo: NEMICO2");
	}else{
		mvprintw(1,0,"tipo: SCONOSCIUTO");
	}
    mvprintw(2,0,"x: %d, y: %d", n->x, n->y);
	mvprintw(3,0,"indirizzo disegno: %p", n->disegno);
	mvprintw(4,0,"id nemico: %d", n->id);
	mvprintw(5,0, "vita nemico: %d", n->vita);
	if(n->direz == LEFT){
		mvprintw(6,0,"direz: LEFT");
	}else if(n->direz == RIGHT){
		mvprintw(6,0,"direz: RIGHT");
	}else if(n->direz == UP){
		mvprintw(6,0,"direz: UP");
	}else if(n->direz == DOWN){
		mvprintw(6,0,"direz: DOWN");
	}else if(n->direz == NONE){
		mvprintw(6,0,"direz: NONE");
	}else{
		mvprintw(6,0,"direz: SCONOSCIUTO");
	}
	mvprintw(7,0, "tid nemico: %d", n->tid);
    refresh();
	getchar();
    
}