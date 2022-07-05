#include "define.h"
#include "buffer.h"
#include "bomba.h"
#include "nemici1l.h"

//per ogni nemico, lo inizializza e chiama il thread che si occuperà di muoverlo
void initNemico(){
	extern Object nemico[];
    int maxrow, maxcol;
    int thread_fail = -1;

    getmaxyx(stdscr, maxrow, maxcol);
    //nave->thread_id = pthread_self();    //non so se serve questo?
    
    for(int i = 0; i < NUM_NEMICI; i++){
        //inizializza i campi della struttura
		nemico[i].y = 1;
        nemico[i].x = (maxcol/NUM_NEMICI)*i+(maxcol/NUM_NEMICI)/2-1;
        nemico[i].vita = 1;
        nemico[i].id = i;
		nemico[i].tipo = NEMICO;
		nemico[i].direz = NONE;
		nemico[i].inversione = 0;
		//alloca lo spazio del disegno e lo inizializza
		nemico[i].disegno = (char*)malloc(sizeof(char)*(AREA_NEM*AREA_NEM));
        
        nemico[i].disegno[0] = '(';
        nemico[i].disegno[1] = '_';
        nemico[i].disegno[2] = ')';
        nemico[i].disegno[3] = '/';
        nemico[i].disegno[4] = 'M'; //i + '0';
        nemico[i].disegno[5] = '\\';
        nemico[i].disegno[6] = '\'';
        nemico[i].disegno[7] = ' ';
        nemico[i].disegno[8] = '\'';

/*
(_)
/=\ 
' '
*/
		//chiama il thread che muoverà il nemico
        thread_fail = pthread_create(&nemico[i].tid, NULL, &moveNemico, &nemico[i]);

        if(thread_fail){
               printf("Errore nella creazione del thread!");
            _exit(-1);
        }
    }
    
}

//thread che muove il nemico con movimenti generati casualmente
void* moveNemico(void* nemico_void){
	extern Buffer buffer;
	extern Object nemico[];
	extern pthread_mutex_t mutex_stampe;
	extern _Bool flagEnd;
	extern _Bool flagLose;
	extern int killCounter;
	extern int bombNum;
	Object* nem;
	int num;
    int maxrow, maxcol, n, numCas;
	int thread_fail = -1;
	pthread_t threadBomba = -1;
	//prende l'id del nemico passato

	nem = (Object*) nemico_void;
	num = nem->id;
    getmaxyx(stdscr, maxrow, maxcol);
	
	//ciclo principale
    while(flagEnd == false){
		pthread_mutex_lock(&mutex_stampe);
		
		//se il nemico muore, lo distrugge e stampa una X
		if(nemico[num].vita == 0){
			distruggiNem(&nemico[num]);
			attron(COLOR_PAIR(5));
			mvaddch(nemico[num].y+1, nemico[num].x+1, 'X');
			attroff(COLOR_PAIR(5));
			refresh();
			
			killCounter++;
			pthread_mutex_unlock(&mutex_stampe);
			break;
		}
		//se bisogna invertire la direzione (il campo inversione è 1)
		if(nemico[num].inversione == 1){
			switch(nemico[num].direz){
				case RIGHT:
					nemico[num].direz = LEFT;
                   	nemico[num].x -= 1;
					break;
				case LEFT:
					nemico[num].direz = RIGHT;
                   	nemico[num].x += 1;
					break;
				case DOWN:
					nemico[num].direz = UP;
                   	nemico[num].y -= 1;
					break;
				default:
					break;
			}
			nemico[num].inversione = 0;	//resetta il campo inversione
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
					if(nemico[num].x + AREA_NEM +1 < maxcol){
						nemico[num].direz = RIGHT;
						nemico[num].x += 1;
					}
					break;
				case 2: //sinistra
					if(nemico[num].x > 0){
						nemico[num].direz = LEFT;
						nemico[num].x -= 1;
					}
					break;
				case 3: //giù
					if(nemico[num].y + AREA_NEM +1 < maxrow){
						nemico[num].direz = DOWN;
						nemico[num].y += 1;
					}
					//se il nemico arriva al fondo dello schermo hai perso
					else{
						flagLose = true;
					}
					break;
				/*
				case 4: //su
					if(nemico[num].y > 0){
						nemico[num].direz = UP;
						nemico[num].y -= 1;
					}
					break;
				*/
				case 4: //bomba
					//lancia la bomba se non ce ne sono altre già nello schermo
					if(bombNum > 0){
						thread_fail = pthread_create(&threadBomba, NULL, &lanciaBomba, &nemico[num]);
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
		scriviBuffer(&nemico[num]);
	
		pthread_mutex_unlock(&mutex_stampe);
		if(flagLose){
				break;
			}
		usleep(400000);
    }
	//se esce dal loop allora il nemico è morto, toglie la X stampata in precedenza dopo una pausa

	usleep(700000);
	mvaddch(nemico[num].y+1, nemico[num].x+1, ' ');
	//toglie lo spazio al disegno

	free(nemico[num].disegno);
	pthread_exit(NULL);
}

//stampa un nemico passato come parametro
void printNemico(Object* nave){
    int i=0, j=0, k=0;
    
    for(i=0; i < AREA_NEM; i++){
        for(j=0; j < AREA_NEM; j++){
            mvaddch(nave->y+i, nave->x+j, nave->disegno[k]);
            k++;
        }
    }  
}

//cancella un nemico passato come parametro
void distruggiNem(Object* nave){
    int i = 0 ,j = 0, k = 0;
    int x, y;//variabili ausiliarie
    
    x = nave->x;
    y = nave->y;
	
	//se il nemico è morto non si è mosso e lo cancella
	if(nave->vita == 0){
		for(j=0; j < AREA_NEM; j++){
        	for(i=0; i < AREA_NEM; i++){
            	mvaddch(y+j, x+i, ' ');
            	k++;
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
    	for(j=0; j < AREA_NEM; j++){
    	    for(i=0; i < AREA_NEM; i++){
    	        mvaddch(y+j, x+i, ' ');
    	        k++;
    	    }
    	}
    	//refresh();
   } 
}