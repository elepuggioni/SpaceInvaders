#include "define.h"
#include "buffer.h"

//inizializza il buffer
void init_buffer(){
	extern Buffer buffer;
	
	buffer.last = 0;
	buffer.size = DIM_BUFFER;
	buffer.array = (Object**) malloc(sizeof(Object*)*buffer.size);
}

//scrive un oggetto sul buffer
void scriviBuffer(Object* scrivi){
	extern pthread_mutex_t mutex;
	extern Buffer buffer;
	
	//scrive solo se nessun'altro sta scrivendo o leggendo
	pthread_mutex_lock(&mutex);
	//se il buffer è pieno torna all'inizio
	if (buffer.last == buffer.size){
		buffer.last = 0;
	}
	//scrive
	buffer.array[buffer.last] = scrivi;
	buffer.last++;
	
	pthread_mutex_unlock(&mutex);
}

//legge dal buffer l'oggetto nella posizione start
Object* leggiBuffer(int start){
	extern pthread_mutex_t mutex;
	extern sem_t semaforo_buffer;
	extern Buffer buffer;
	Object* letto;
	int numlettori = 0;
	
	//più lettori possono leggere assieme
	sem_wait(&semaforo_buffer);
	numlettori++;
	if(numlettori == 1)
		pthread_mutex_lock(&mutex); //ma non può leggere se qualcuno sta scrivendo
	sem_post(&semaforo_buffer);
	
	//legge dal buffer
	letto = buffer.array[start];
	
	sem_wait(&semaforo_buffer);
	numlettori--;
	if(numlettori == 0)
		pthread_mutex_unlock(&mutex);
	
	sem_post(&semaforo_buffer);
	return letto;
}
