#include "define.h"
#include "proiettili.h"
#include "controllo.h"
#include "buffer.h"
#include "nemici2l.h"
#include "astronave.h"

//inizializza l'astronave
void initNave(){
	extern Buffer buffer;
	extern Object astronave;
    int maxrow, maxcol;
    
    getmaxyx(stdscr, maxrow, maxcol);
	//inizializza
    astronave.y = maxrow - RIGHE_NAVE;
    astronave.x = maxcol/2 - COL_NAVE/2;
    astronave.direz = NONE;
    astronave.tid = pthread_self();
	astronave.tipo = NAVE;
	astronave.id = 420;
	astronave.vita = 1;

	//inizializza il disegno
	astronave.disegno = (char*)malloc(sizeof(char)*(RIGHE_NAVE*COL_NAVE));
	disegno:{
    astronave.disegno[0]=' ';
    astronave.disegno[1]=' ';
    astronave.disegno[2]='+';
    astronave.disegno[3]='+';
    astronave.disegno[4]=' ';
    astronave.disegno[5]=' ';
    astronave.disegno[6]=' ';
    astronave.disegno[7]=' ';
    astronave.disegno[8]='/';
    astronave.disegno[9]='\\';
    astronave.disegno[10]=' ';
    astronave.disegno[11]=' ';
    astronave.disegno[12]=' ';
    astronave.disegno[13]=':';
    astronave.disegno[14]=' ';
    astronave.disegno[15]=' ';
    astronave.disegno[16]=':';
    astronave.disegno[17]=' ';
    astronave.disegno[18]=' ';
    astronave.disegno[19]='|';
 	astronave.disegno[20]=' ';
    astronave.disegno[21]=' ';
    astronave.disegno[22]='|';
    astronave.disegno[23]=' ';
    astronave.disegno[24]='/';
    astronave.disegno[25]='\'';
    astronave.disegno[26]='.';
    astronave.disegno[27]='.';
    astronave.disegno[28]='\'';
    astronave.disegno[29]='\\';
    astronave.disegno[30]='\\';
    astronave.disegno[31]='/';
    astronave.disegno[32]=':';
    astronave.disegno[33]=':';
    astronave.disegno[34]='\\';
    astronave.disegno[35]='/';
	}
/*
  ++  
  /\ 
 :  :
 |  |
/'..'\
\/::\/
*/ 
    printNave();
	scriviBuffer(&astronave);
}

//thread che muove l'astronave prendendo i comandi dalla tastiera
void* muoviAstronave(){
	extern Object astronave;
	extern pthread_mutex_t mutex_stampe;
	extern sem_t semaforo_stampe;
	extern int lettori_stampe;
	extern _Bool flagEnd;
	extern _Bool flagLose;
	extern _Bool flagWin;
	extern int killCounter;
	extern int killCounter2;
	extern int bullNum;
	int maxrow, maxcol;
    int thread_fail = -1; //se la creazione del thread fallisce non vale zero
    pthread_t threadProiettili = -1; //ospiterà l'id del primo dei due thread dei proiettili

	getmaxyx(stdscr, maxrow, maxcol);
    //prende un tasto dalla tastiera
	while((astronave.direz = getch()) != KEY_F(1)){
		pthread_mutex_lock(&mutex_stampe);

		//se i nemici di primo livello sono morti inizializza quelli di secondo livello
		if(killCounter == NUM_NEMICI){
			initNemico2();
			killCounter = 0;
		}
		//se i nemici di secondo livello sono morti hai vinto il gioco
		if(killCounter2 == NUM_NEMICI_2){
			flagWin = true;
		}
		//se si ha vinto o perso si esce dal loop e si va alla fine del gioco
		if(flagLose || flagWin){
			pthread_mutex_unlock(&mutex_stampe);
			break;
		}
		//muove l'astronave a dx o sx o spara i proiettili
        switch(astronave.direz){
                case KEY_LEFT:
                    if(astronave.x > 0)
                        astronave.x -= 1;
                    break;
                case KEY_RIGHT:
                    if(astronave.x + RIGHE_NAVE +1 < maxcol)
                        astronave.x += 1;
                    break;
                //se viene premuto spazio, crea un thread per la gestione dei proiettili, solo se non ci sono proiettili già nello schermo
                case SPAZIO:
					if(bullNum == 0){
                    	thread_fail = pthread_create(&threadProiettili, NULL, &creaProiettili, NULL);
                        	if(thread_fail){
                        	    mvprintw(0,0,"Errore nella creazione del thread!");
                        	    _exit(-1);
                        	}
					}
                    break;
                default:
                    break;
            }
		scriviBuffer(&astronave);
		
		pthread_mutex_unlock(&mutex_stampe);
		usleep(20000);
	}
	//se si è usciti dal loop, o perché si è vinto o perso o perché si è premuto F1 allora il gioco finisce
	flagEnd = true;
	free(astronave.disegno);
	pthread_exit(NULL);
}

//stampa l'astronave
void printNave(){
	extern Object astronave;
    int i = 0 ,j = 0, k = 0;
    
    for(j=0; j < RIGHE_NAVE; j++){
        for(i=0; i < COL_NAVE; i++){
            mvaddch(astronave.y+j, astronave.x+i, astronave.disegno[k]);
            k++;
        }
    }
    refresh();
}

//cancella l'astronave nella posizione in cui si trovava in precedenza
void distruggiNave(){
	extern Object astronave;
	int i = 0 ,j = 0, k = 0;
    int x, y; //variabili ausiliarie
    
    x = astronave.x;
    y = astronave.y;
    
	//controlla dove si trovava prima l'astronave
    if(astronave.direz == KEY_LEFT){
        x += 1;
    }
    else{
        x -= 1;
    }
    //cancella quella posizione
    for(j=0; j < RIGHE_NAVE; j++){
        for(i=0; i < COL_NAVE; i++){
            mvaddch(y+j, x+i, ' ');
            k++;
        }
    }
}