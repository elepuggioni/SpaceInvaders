#include "define.h"
#include "astronave.h"
#include "buffer.h"
#include "proiettili.h"

/*Thread che inizializza e stampa due proiettili sparati dall'astronave nello schermo finché non escono da esso o non colpiscono qualcosa,
	a quel punto il thread termina
*/
void* creaProiettili(){
    extern Object astronave;
	extern pthread_mutex_t mutex_stampe;
	extern Object pl;
	extern Object pr;
	extern _Bool stopBullsx;
	extern _Bool stopBulldx;
	extern int bullNum;
   	int maxrow, maxcol;

	pthread_mutex_lock(&mutex_stampe);

	//inizializza i proiettili sx e dx
	pl.tipo = PROIETTILE;
    pl.x = astronave.x + 1;
    pl.y = astronave.y - 2;
    pl.direz = LEFT;
	pl.id = 420;
	pl.vita = false;
	pl.disegno = (char*)malloc(sizeof(char));
    pl.disegno[0] = '.';
	pl.tid = pthread_self();
	
	pr.tipo = PROIETTILE;
    pr.x = astronave.x + 4;
    pr.y = astronave.y - 2;
    pr.direz = RIGHT;
	pr.id = 420;
	pr.vita = false;
	pr.disegno = (char*)malloc(sizeof(char));
    pr.disegno[0] = '.';
    pr.tid = pthread_self();
    
	bullNum = 2;
	pthread_mutex_unlock(&mutex_stampe);
	
	getmaxyx(stdscr, maxrow, maxcol);
	
	//finché i proiettili non escono dallo schermo
    while((pr.x <= maxcol && pr.y >= -1) && (pl.x >= -1 && pl.y >= -1)){
		pthread_mutex_lock(&mutex_stampe);
		
		//se entrambi i proiettili sono fermati esci dal ciclo
		if(stopBullsx && stopBulldx){
			pthread_mutex_unlock(&mutex_stampe);
			break;
		}
		//se il proiettile sx non è da fermare lo stampa nella posiione successiva
		if(!stopBullsx){
			pl.x -= 1;
			pl.y -= 1;
			scriviBuffer(&pl);
		}
		//se il proiettile dx non è da fermare lo stampa nella posiione successiva
		if(!stopBulldx){
			pr.x += 1;
			pr.y -= 1;
			scriviBuffer(&pr);
		}
		pthread_mutex_unlock(&mutex_stampe);
        usleep(40000);   
	} 
	
	pthread_mutex_lock(&mutex_stampe);
	mvaddch(pl.y, pl.x, ' '); //cancella l'ultima posizione dei proiettili
	mvaddch(pr.y, pr.x, ' ');
	refresh();
	//resetta counter, flag , coordinate dei proiettili
	bullNum = 0;
	stopBullsx = false;
	stopBulldx = false;
	
	pr.x = astronave.x +2;
	pr.y = astronave.y +3;
	pl.x = astronave.x +2;
	pl.y = astronave.y +3;

	pthread_mutex_unlock(&mutex_stampe);
	pthread_exit(NULL);
}