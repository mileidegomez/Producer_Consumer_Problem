
#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#define N_THREADS 2
#define BUFFER 100
#define PRODUCAO 1000 // quantidade de itens que serão produzidos

int buffer[BUFFER]; //buffer que será partilhado entre o produtor e o consumidor
int posidobuffer=0;
int posidesaida=0;
int controlodobuffer=0;


pthread_mutex_t MUTEX; // Variável que pode ter dois estados: 0 ou 1 e vai controlar o estado do buffer
pthread_cond_t condconsumidor, condprodutor;  // para permitir bloquear se uma condição não for satisfeita


void produtor(){
    
    
    for(int i=0; i<=PRODUCAO; i++){
        int item;
        item = i; // item igual a posição do vetor
        pthread_mutex_lock(&MUTEX); // tem acesso ao mutex
        
        do{
            pthread_cond_wait(&condprodutor, &MUTEX); // funções são usadas para bloquear uma condition variable. 
            //são chamados com o mutex bloqueado pelo encadeamento de chamada
        }while (controlodobuffer == BUFFER);
        
        buffer[posidobuffer] = item;		// adiciona itens ao buffer
        
        pthread_cond_signal(&condconsumidor);		// acorda o consumidor
		pthread_mutex_unlock(&MUTEX);	// e liberta acesso ao buffer
		
        printf ("Foi produzido o item %d", item);
        posidobuffer++;

		
		if (posidobuffer == BUFFER){
		   posidobuffer =0;
		}
	controlodobuffer++;
    }
    pthread_exit(0);
}

void consumidor(){
    
    int item; 
    
        do{
            
            pthread_mutex_lock(&MUTEX); //bloqueia o consumidor
            
        while (buffer[controlodobuffer] == 0){ // buffer está vazio 
         pthread_cond_wait(&condconsumidor, &MUTEX);
         
        }
        
        item = buffer[posidesaida];
        pthread_cond_signal(&condprodutor); // wake up producer
        pthread_mutex_unlock(&MUTEX);  // release access to buffer
        
        printf("Consumiu o item %d", item);
        
        posidesaida++;
        
        if(posidesaida == BUFFER){
            posidesaida = 0;
        }
        
        controlodobuffer--;
        } while (item != (PRODUCAO - 1) );
    pthread_exit(0);
}


int main(int argc, char *argv[]){
    pthread_t threads[N_THREADS];
    
    pthread_mutex_init(&MUTEX, 0); // Criação de mutexes
    pthread_cond_init(&condprodutor, 0); // condition variable que vai bloquear o produtor
	pthread_cond_init(&condconsumidor, 0);  // condition variable que vai bloquear o consumidor
    
    pthread_create (&threads[0], NULL, (void *) &produtor, NULL);
    pthread_create (&threads[1], NULL, (void *) &consumidor, NULL);
    
     for (int i=0; i< N_THREADS ; i++){
        pthread_join (threads[i], NULL);
        // printf ("Terminaram os threads");
    }
    
    pthread_cond_destroy(&condprodutor);
	pthread_cond_destroy(&condconsumidor);
	pthread_mutex_destroy(&MUTEX);
	
	return 0;
	
}

