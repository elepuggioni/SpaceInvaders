#include "define.h"
#include "astronave.h"
#include "proiettili.h"
#include "controllo.h"
#include "buffer.h"
#include "nemici1l.h"
#include "nemici2l.h"
#include "collisioni.h"

void controllo(){
	extern Buffer buffer;
	extern Object astronave;
	extern pthread_mutex_t mutex_stampe;
	extern sem_t semaforo_stampe;
	extern int lettori_stampe;
	extern _Bool flagEnd;
    int thread_fail = -1; //se la creazione del thread fallisce non vale zero
    
	//chiama il thread dell'astronave
	thread_fail = pthread_create(&(astronave.tid), NULL, &muoviAstronave, NULL);
	
	/*ciclo principale del gioco, questo thread stampa continuamente le posizioni degli oggetti
	  leggendoli dal buffer e controlla le collisioni
	*/
	while(flagEnd == false){
		pthread_mutex_lock(&mutex_stampe);
	
    	stampaTutto();
		if (buffer.last >= 1){
			collisioni();
		}
		
		pthread_mutex_unlock(&mutex_stampe);
		usleep(5000);
	}
	refresh();
	pthread_join(astronave.tid, NULL);
}

//stampa tutti gli oggetti nello schermo leggendoli dal buffer
void stampaTutto(){
	extern Buffer buffer;
	int numlettori = 0;
	Object* temp;
	
	//legge tutti gli oggetti dal buffer e a uno a uno cancella la loro vecchia posizione nello schermo
	//e stampa la nuova con i rispettivi colori
	for(int i = 0; i < buffer.last; i++){
		temp = leggiBuffer(i);
		
		switch(temp->tipo){
			case NAVE: //stampa nave
				distruggiNave();
				attron(COLOR_PAIR(1));
				printNave();
				attroff(COLOR_PAIR(1));
				break;
			case NEMICO: //stampa nemico
				distruggiNem(temp);
				attron(COLOR_PAIR(2));
				printNemico(temp);
				attroff(COLOR_PAIR(2));
				break;
			case NEMICO2: //stampa nemico di secondo livello
				distruggiNem2(temp);
				attron(COLOR_PAIR(3));
				printNemico2(temp);
				attroff(COLOR_PAIR(3));
				break;
			case PROIETTILE: //stampa proiettile sx
				if(temp->direz == LEFT){
					attron(COLOR_PAIR(4));
					mvaddch(temp->y, temp->x, *(temp->disegno));
					attroff(COLOR_PAIR(4));
					mvaddch(temp->y +1, temp->x +1, ' ');
				}
				else if(temp->direz == RIGHT){ //stampa proiettile dx
					attron(COLOR_PAIR(4));
					mvaddch(temp->y, temp->x, *(temp->disegno));
					attroff(COLOR_PAIR(4));
					mvaddch(temp->y +1, temp->x -1, ' ');
				}
				break;
			case BOMBA: //stampa bomba
				attron(COLOR_PAIR(5));
				mvaddch(temp->y, temp->x, *(temp->disegno));
				attroff(COLOR_PAIR(5));
				mvaddch(temp->y-1, temp->x, ' ');
				mvaddch(temp->y-2, temp->x, ' ');
				mvaddch(temp->y-3, temp->x, ' ');
				break;
			default:
				break;
		}
	}
	refresh();
}
