#include "define.h"
#include "buffer.h"
#include "bomba.h"

//thread che inizializza e muove una bomba lanciata da un nemico
void* lanciaBomba(void* nemico_void){
    extern Object nemico[];
	extern Object nemico2[];
	extern Object bomba;
	extern pthread_mutex_t mutex_stampe;
	extern _Bool stopBomb;
	extern int bombNum;
	Object* nem;
	int maxrow, maxcol, id;
	//id del nemico che lancia la bomba
	nem = (Object*) nemico_void;
	id = nem->id;

	pthread_mutex_lock(&mutex_stampe);
	//inizializza la bomba
	bomba.tipo = BOMBA;
	if(nem->tipo == NEMICO){
		bomba.x = nemico[id].x + 1;
    	bomba.y = nemico[id].y + 3;
	}
	else if(nem-> tipo == NEMICO2){
		bomba.x = nemico2[id].x +2;
		bomba.y = nemico2[id].y +6;
	}
    
    bomba.direz = DOWN;
	bomba.id = 420;
	bomba.vita = false;
	bomba.disegno = (char*)malloc(sizeof(char));
    bomba.disegno[0] = '*';
    bomba.tid = pthread_self();
    
	getmaxyx(stdscr, maxrow, maxcol);
	//decrementa il numero di bombe ammesse contemporaneamente nello schermo
	bombNum--;
	pthread_mutex_unlock(&mutex_stampe);

	//finchè la bomba non tocca il fondo
    while(bomba.y <= maxrow){
		pthread_mutex_lock(&mutex_stampe);
		//se la bomba va fermata (tocca l'astronave) esce dal loop
		if(stopBomb){
			stopBomb = false;
			
			pthread_mutex_unlock(&mutex_stampe);
			break;
		}
		//muove la bomba
        bomba.y += 1;
        scriviBuffer(&bomba);
       
		pthread_mutex_unlock(&mutex_stampe);
        usleep(100000);   
	}
	//cancella l'ultima posizione della bomba e resetta le coordinate
	pthread_mutex_lock(&mutex_stampe);
	mvaddch(bomba.y, bomba.x, ' ');
	refresh();
	bomba.y = -1;
	bomba.x = -1;
	bombNum++;

	pthread_mutex_unlock(&mutex_stampe);
	pthread_exit(NULL);
}
