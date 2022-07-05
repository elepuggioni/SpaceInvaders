#include "define.h"
#include "buffer.h"
#include "collisioni.h"

void collisioni(){
	extern Buffer buffer;

	//chiama tutte le funzioni che controllano le collisioni tra diversi tipi di oggetti
	nemiciTraLoro();
	nemici2TraLoro();
	
	nemiciAstronave();
	nemici2Astronave();
	
	nemiciProiettili();
	nemici2Proiettili();
	
	bombeAstronave();
	//resetta il counter del buffer
	buffer.last = 0;
}

//controlla se due nemici stanno per collidere, se sì inverte la traiettoria
void nemiciTraLoro(){
	extern Object nemico[];

	//controlla la posizione di un nemico con tutti gli altri, per ogni nemico
	for(int i = 0; i < NUM_NEMICI; i++){
		for(int j = 0; j < NUM_NEMICI; j++){
			//se nemico[i] e nemico[j] non sono lo stesso nemico, controlla se collidono
			if(i!=j && collidono(&nemico[i], &nemico[j], AREA_NEM, AREA_NEM)){
				if(nemico[i].vita && nemico[j].vita){
					//se sì, inverte la traiettoria
					nemico[i].inversione = 1;
					nemico[j].inversione = 1;
				}
			}
		}
	}
}

//controlla se due nemici di secondo livello stanno per collidere, se sì inverte la traiettoria
void nemici2TraLoro(){
	extern Object nemico2[];
	
	//controlla la posizione di un nemico con tutti gli altri, per ogni nemico
	for(int i = 0; i < NUM_NEMICI; i++){
		for(int j = 0; j < NUM_NEMICI; j++){
			//se nemico[i] e nemico[j] non sono lo stesso nemico, controlla se collidono
			if(i!=j && collidono(&nemico2[i], &nemico2[j], AREA_NEM2, AREA_NEM2)){
				//se sì, inverte la traiettoria
				if(nemico2[i].vita && nemico2[j].vita){
					nemico2[i].inversione = 1;
					nemico2[j].inversione = 1;
				}
			}
		}
	}
}

//controlla se un nemico collide con l'astronave
void nemiciAstronave(){
	extern Object astronave;
	extern Object nemico[];
	extern _Bool flagLose;

	//per ogni nemico
	for(int i = 0; i < NUM_NEMICI; i++){
		//se il nemico è vivo
		if(nemico[i].vita){
			//se un nemico tocca l'astrnave hai perso
			if(collidono(&nemico[i], &astronave, AREA_NEM, RIGHE_NAVE)){
				astronave.vita = 0;
				flagLose = true;
				break;
			}
		}
	}
}

//controlla se un nemico di secondo livello collide con l'astronave
void nemici2Astronave(){
	extern Object astronave;
	extern Object nemico2[];
	extern _Bool flagLose;;

	//per ogni nemico
	for(int i = 0; i < NUM_NEMICI; i++){
		//se il nemico è vivo
		if(nemico2[i].vita){
			//se un nemico tocca l'astrnave hai perso
			if(collidono(&nemico2[i], &astronave, AREA_NEM2, RIGHE_NAVE)){
				astronave.vita = 0;
				flagLose = true;
				break;			
			}
		}
	}
}

//controlla se un proiettile colpisce un nemico
void nemiciProiettili(){
	extern Object nemico[];
	extern Object pr;
	extern Object pl;
	extern int bullNum;
	extern _Bool stopBullsx;
	extern _Bool stopBulldx;

	//per ogni nemico
	for(int i = 0; i < NUM_NEMICI; i++){
		//se il nemico è vivo, se nemico e proiettile collidono ferma il proiettile e decrementa la vita del nemico
		if(nemico[i].vita){
			if(collidono(&nemico[i], &pl, AREA_NEM, 1) && stopBullsx == false){
				stopBullsx = true;
				if(bullNum > 0){
					bullNum--;
				}
				nemico[i].vita = 0;
			}
			if(collidono(&nemico[i], &pr, AREA_NEM, 1) && stopBulldx == false){
				stopBulldx = true;
				if(bullNum > 0){
					bullNum--;
				}
				nemico[i].vita = 0;
			}
		}
	}
}
//controlla se un proiettile colpisce un nemico di secondo livello
void nemici2Proiettili(){
	extern Object nemico2[];
	extern Object pr;
	extern Object pl;
	extern int bullNum;
	extern _Bool stopBullsx;
	extern _Bool stopBulldx;
	
	//per ogni nemico
	for(int i = 0; i < NUM_NEMICI; i++){
		//se il nemico è vivo, se nemico e proiettile collidono ferma il proiettile e decrementa la vita del nemico
		if(nemico2[i].vita > 0){
			if(collidono(&nemico2[i], &pl, AREA_NEM2, 1) && stopBullsx == false){
				stopBullsx = true;
				if(bullNum > 0){
					bullNum--;
				}
				nemico2[i].vita--;
			}
			if(collidono(&nemico2[i], &pr, AREA_NEM2, 1) && stopBulldx == false){
				stopBulldx = true;
				if(bullNum > 0){
					bullNum--;
				}
				nemico2[i].vita--;
			}
		}
	}
}

//controlla se una bomba colpisce l'astronave
void bombeAstronave(){
	extern Object astronave;
	extern Object bomba;
	extern _Bool stopBomb;
	extern _Bool flagLose;
	//se collidono ferma la bomba, toglie la vita all'astronave, e la partita è persa
	if(collidono(&astronave, &bomba, AREA_NAVE, 1)){
		stopBomb = true;
		astronave.vita = 0;
		flagLose = true;
	}
}

//prende due oggetti e controlla se collidono in base al loro tipo
_Bool collidono(Object* uno, Object* due, int area1, int area2){
	_Bool flag = false;	
	int maxrow, maxcol;
	getmaxyx(stdscr, maxrow, maxcol);

	//nemico e nemico
	if(uno->tipo == NEMICO && due->tipo == NEMICO){
		if((abs(due->x - uno->x)<=AREA_NEM && abs(due->y- uno->y)<=AREA_NEM) || (abs(uno->x - due->x)<=AREA_NEM && abs(uno->y - due->y)<=AREA_NEM))
		flag = true;
	} //nemico2 e nemico2
	else if(uno->tipo == NEMICO2 && due->tipo == NEMICO2){
		if((abs(due->x - uno->x)<=AREA_NEM2 && abs(due->y- uno->y)<=AREA_NEM2) || (abs(uno->x - due->x)<=AREA_NEM2 && abs(uno->y - due->y)<=AREA_NEM2))
		flag = true;
	} //nemico e astronave
	else if((uno->tipo == NEMICO && due->tipo == NAVE)){
		if((abs(due->x - uno->x)<=AREA_NAVE && abs(due->y - uno->y+1)<=AREA_NAVE))
			flag = true;
	} //nemico2 e astronave
	else if((uno->tipo == NEMICO2 && due->tipo == NAVE)){
		if((abs(due->x - uno->x)<=AREA_NAVE && abs(due->y - uno->y+1)<=AREA_NAVE))
			flag = true;
	} //nemico e proiettile
	else if(uno->tipo == NEMICO && due->tipo == PROIETTILE){
		if(abs(uno->x+1 - due->x)<AREA_NEM && abs(uno->y - due->y)<AREA_NEM)
			flag = true;
	}	//nemico2 e proiettile
	else if(uno->tipo == NEMICO2 && due->tipo == PROIETTILE){
		if(abs(uno->x - due->x)<AREA_NEM2 && abs(uno->y - due->y)<AREA_NEM2)
			flag = true;
	}	//astronave e bomba
	else{
		if(abs(uno->x - due->x)<AREA_NAVE && abs(uno->y - due->y) == 1)
			flag = true;
	}
	
	return flag;
}


