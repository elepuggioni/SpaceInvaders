#include <ncurses.h>
#include <unistd.h>
#include <pthread.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include <semaphore.h>
#include <sys/wait.h>
#include <math.h>
//numero elementi
#define NUM_NAVE 1
#define NUM_NEMICI 5
#define NUM_NEMICI_2 5
#define NUM_PROIETTILI 2
#define NUM_BOMBE 1
//buffer
#define DIM_BUFFER 400
#define NUM_LETTORI 20
//aree
#define RIGHE_NAVE 6
#define COL_NAVE 6
#define AREA_NAVE 6
#define AREA_NEM 3
#define AREA_NEM2 6
#define MOVE_TYPES 3

#define SPAZIO 32 //chiave ASCII del tasto spazio

//spostamento di una nave nemica
typedef enum spostamento{NONE = 0, UP = 1, DOWN = 2, RIGHT = 260, LEFT = 261} Spostamento;
//tipo di oggetto nello schermo
typedef enum tipo{NAVE = 1, NEMICO = 2, PROIETTILE = 3, BOMBA = 4, NEMICO2 = 5} Tipo;
//questa struttura ospita tutti i tipi di oggetti
typedef struct obj{
	Tipo tipo;
	int x, y;
	char* disegno;
	int id;
	int vita;
	Spostamento direz;
	pthread_t tid;
	int inversione;
} Object;
//struttura del buffer
typedef struct buffer{
	int last;
	int size;
	Object **array;
} Buffer;

int numRand(int min, int max);
void stampaParametriNave(Object *n);