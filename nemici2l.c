#include "define.h"
#include "buffer.h"
#include "bomba.h"
#include "nemici1l.h"
#include "nemici2l.h"

//per ogni nemico, lo inizializza e chiama il thread che si occuperà di muoverlo
void initNemico2(){
	extern Object nemico2[];
    int maxrow, maxcol;
    int thread_fail = -1;

    getmaxyx(stdscr, maxrow, maxcol);
    
    for(int i = 0; i < NUM_NEMICI; i++){
		//inizializza i campi della struttura
		nemico2[i].y = 1;
        nemico2[i].x = (maxcol/NUM_NEMICI)*i+(maxcol/NUM_NEMICI)/2-1;
        nemico2[i].vita = 2;
        nemico2[i].id = i;
		nemico2[i].tipo = NEMICO2;
		nemico2[i].direz = NONE;
		nemico2[i].inversione = 0;
		//alloca lo spazio del disegno e lo inizializza
		nemico2[i].disegno = (char*)malloc(sizeof(char)*(AREA_NEM2*AREA_NEM2));
		int k = 0;
		for(int j = 0; j < 2; j++){
				nemico2[i].disegno[k] = '_'; k++; //prima/quarta riga
				nemico2[i].disegno[k] = 'M'; k++;
				nemico2[i].disegno[k] = '_'; k++;
				nemico2[i].disegno[k] = '_'; k++;
				nemico2[i].disegno[k] = 'M'; k++;
				nemico2[i].disegno[k] = '_'; k++;
				nemico2[i].disegno[k] = '\\'; k++; //seconda/quinta riga
				nemico2[i].disegno[k] = '|'; k++;
				nemico2[i].disegno[k] = '/'; k++;
				nemico2[i].disegno[k] = '\\'; k++; 
				nemico2[i].disegno[k] = '|'; k++;
				nemico2[i].disegno[k] = '/'; k++; 
				nemico2[i].disegno[k] = '\''; k++; //terza/sesta riga
				nemico2[i].disegno[k] = '\''; k++;
				nemico2[i].disegno[k] = '\''; k++;
				nemico2[i].disegno[k] = '\''; k++;
				nemico2[i].disegno[k] = '\''; k++;
				nemico2[i].disegno[k] = '\''; k++;
		}
/*
_M__M_
\|/\|/
''''''
_M__M_
\|/\|/
''''''

		}*/

		/*for(int j = 0; j < 2; j++){
				nemico2[i].disegno[k] = ' '; k++; //prima/quarta riga
				nemico2[i].disegno[k] = '.'; k++;
				nemico2[i].disegno[k] = '_'; k++;
				nemico2[i].disegno[k] = '.'; k++;
				nemico2[i].disegno[k] = ' '; k++;
				nemico2[i].disegno[k] = ' '; k++;
				nemico2[i].disegno[k] = ' '; k++;
				nemico2[i].disegno[k] = '.'; k++; 
				nemico2[i].disegno[k] = '_'; k++;
				nemico2[i].disegno[k] = '.'; k++;
				nemico2[i].disegno[k] = ' '; k++;
				nemico2[i].disegno[k] = '/'; k++; //seconda/quinta riga 
				nemico2[i].disegno[k] = '/'; k++;
				nemico2[i].disegno[k] = 'M'; k++; 
				nemico2[i].disegno[k] = '\\'; k++; 
				nemico2[i].disegno[k] = '\\'; k++;
				nemico2[i].disegno[k] = ' '; k++;
				nemico2[i].disegno[k] = '/'; k++;
				nemico2[i].disegno[k] = '/'; k++;
				nemico2[i].disegno[k] = 'M'; k++;
				nemico2[i].disegno[k] = '\\'; k++;
				nemico2[i].disegno[k] = '\\'; k++;
				nemico2[i].disegno[k] = ' '; k++; //terza/sesta riga
				nemico2[i].disegno[k] = '\''; k++;
				nemico2[i].disegno[k] = '\''; k++; 
				nemico2[i].disegno[k] = '\''; k++;
				nemico2[i].disegno[k] = ' '; k++;
				nemico2[i].disegno[k] = ' '; k++;
				nemico2[i].disegno[k] = ' '; k++;
				nemico2[i].disegno[k] = '\''; k++;
				nemico2[i].disegno[k] = '\''; k++;
				nemico2[i].disegno[k] = '\''; k++;
				nemico2[i].disegno[k] = ' '; k++;
		}

/*
 ._.   ._. 
//M\\ //M\\
 '''   '''
 ._.   ._.
//M\\ //M\\
 '''   '''
*/
		//chiama il thread che muoverà il nemico
        thread_fail = pthread_create(&nemico2[i].tid, NULL, &moveNemico2, &nemico2[i]);

        if(thread_fail){
               printf("Errore nella creazione del thread!");
            _exit(-1);
        }
    }
    
}

//thread che muove il nemico con movimenti generati casualmente
void* moveNemico2(void* nemico_void){
	extern Buffer buffer;
	extern Object nemico2[];
	extern pthread_mutex_t mutex_stampe;
	extern _Bool flagEnd;
	extern _Bool flagLose;
	extern int killCounter2;
	extern int bombNum;
	Object* nem;
	int id;
    int maxrow, maxcol, n, numCas;
	int thread_fail = -1;
	pthread_t threadBomba = -1;
	//prende l'id del nemico passato
	nem = (Object*) nemico_void;
	id = nem->id;
    getmaxyx(stdscr, maxrow, maxcol);
	
	//ciclo principale
    while(flagEnd == false){
		pthread_mutex_lock(&mutex_stampe);
		//se il nemico muore, lo distrugge e stampa una X
		if(nemico2[id].vita == 0){
			distruggiNem2(&nemico2[id]);
			attron(COLOR_PAIR(5));
			mvaddch(nemico2[id].y+1, nemico2[id].x+1, 'X');
			attroff(COLOR_PAIR(5));
			refresh();
			
			killCounter2++;
			
			pthread_mutex_unlock(&mutex_stampe);
			break;
		}
		//se bisogna invertire la direzione (il campo inversione è 1)
		if(nemico2[id].inversione == 1){
			switch(nemico2[id].direz){
				case RIGHT:
					nemico2[id].direz = LEFT;
                   	nemico2[id].x -= 1;
					break;
				case LEFT:
					nemico2[id].direz = RIGHT;
                   	nemico2[id].x += 1;
					break;
				case DOWN:
					nemico2[id].direz = UP;
                   	nemico2[id].y -= 1;
					break;
				default:
					break;
			}
			nemico2[id].inversione = 0; //resetta il campo inversione
		}
		//se non bisogna invertire la direzione
		else{ 
			/*genera un numero random tra 1 e 100; 
				1-30: spostamento a dx (30%)
				31-60: spostamento a sx (30%)
				61-90: spostamento in basso (30%)
				91-100: lancia una bomba (10%)
			*/
        	numCas = numRand(1, 100);
			if(numCas > 0 && numCas <= 30){
				n = 1;
			}else if(numCas > 30 && numCas <= 60){
				n = 2;
			}else if(numCas > 60 && numCas <= 90){
				n = 3;
			}else{
				n = 4;
			}
			
			switch(n){
				case 1: //destra
					if(nemico2[id].x + AREA_NEM2 +1 < maxcol){
						nemico2[id].direz = RIGHT;
						nemico2[id].x += 1;
					}
					break;
				case 2: //sinistra
					if(nemico2[id].x > 0){
						nemico2[id].direz = LEFT;
						nemico2[id].x -= 1;
					}
					break;
				case 3: //giù
					if(nemico2[id].y + AREA_NEM2 +1 < maxrow){
						nemico2[id].direz = DOWN;
						nemico2[id].y += 1;
					}
					//se il nemico arriva al fondo dello schermo hai perso
					else{
						flagLose = true;
					}
					break;
				/*
				case 4: //su
					if(nemico2[num].y > 0){
						nemico2[num].direz = UP;
						nemico2[num].y -= 1;
					}
					break;
				*/
				case 4: //bomba
					//lancia la bomba se non ce ne sono altre già nello schermo
					if(bombNum > 0){
						thread_fail = pthread_create(&threadBomba, NULL, &lanciaBomba, &nemico2[id]);
						if(thread_fail){
							mvprintw(0,0,"Errore nella creazione del thread!");
							_exit(-1);
						}
					}
					break;
				default:
					break;
			}
		}
		scriviBuffer(&nemico2[id]);
		pthread_mutex_unlock(&mutex_stampe);
		
		if(flagLose){
			break;
		}
		usleep(400000);
    }
	
	//se esce dal loop allora il nemico è morto, toglie la X stampata in precedenza dopo una pausa
	usleep(700000);
	mvaddch(nemico2[id].y+1, nemico2[id].x+1, ' ');
	//toglie lo spazio al disegno
	free(nemico2[id].disegno);
	pthread_exit(NULL);
}

//stampa un nemico passato come parametro
void printNemico2(Object* nave){
    int i=0, j=0, k=0;
    
    for(i=0; i < AREA_NEM2; i++){
        for(j=0; j < AREA_NEM2; j++){
            mvaddch(nave->y+i, nave->x+j, nave->disegno[k]);
            k++;
        }
    }  
}
//cancella un nemico passato come parametro
void distruggiNem2(Object* nave){
    int i = 0 ,j = 0;
    int x, y;	//variabili ausiliarie
    
    x = nave->x;
    y = nave->y;

	//se il nemico è morto non si è mosso e lo cancella
	if(nave->vita == 0){
		for(j=0; j < AREA_NEM2; j++){
        	for(i=0; i < AREA_NEM2; i++){
            	mvaddch(y+j, x+i, ' ');
        	}
    	}
	}
	//altrimenti x e y prendono la posizione in cui la nave si trovava prima per cancellarla
	else{
    	switch(nave->direz){
        	case RIGHT:
        	    x -= 1;
        	    break;
        	case LEFT:
        	    x += 1;
        	    break;
        	case UP:
        	    y += 1;
        	    break;
        	case DOWN:
        	    y -= 1;
        	    break;
        	default:
        	    break;
    	}

		//cancella
    	for(j=0; j < AREA_NEM2; j++){
    	    for(i=0; i < AREA_NEM2; i++){
    	        mvaddch(y+j, x+i, ' ');
    	    }
    	}
	}
}

