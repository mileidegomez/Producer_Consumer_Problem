#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#define N_THREADS 2
#define BUFFER 100
#define PRODUCAO 1000000000 // quantidade de itens que serão produzidos

int buffer[BUFFER]; //buffer que será partilhado entre o produtor e o consumidor
int posidobuffer = 0;
int posidesaida = 0;
int controlodobuffer = 0;


pthread_mutex_t mutex1; // Variável que pode ter dois estados: 0 ou 1 e vai controlar o estado do buffer
pthread_cond_t condconsumidor;
pthread_cond_t condprodutor;  // para permitir bloquear se uma condição não for satisfeita


void produtor(){
int item;
    for(int i=0; i<=PRODUCAO; i++){
        item = i;
        pthread_mutex_lock(&mutex1); //  acesso à secção cítica única , bloqueia mutex
         while(controlodobuffer == BUFFER){
            pthread_cond_wait(&condprodutor, &mutex1); // quando buffer tiver cheio, bloqueia o produtor 
            //são chamados com o mutex bloqueado pelo encadeamento de chamada
        }

        buffer[posidobuffer] = item;		// adiciona itens ao buffer

        pthread_cond_signal(&condconsumidor);		//e  acorda o consumidor
	pthread_mutex_unlock(&mutex1);	// sai da secção crítica, desbloqueia o mutex
        printf ("Foi produzido o item %d \n \n ", item);
        posidobuffer++;
        if (posidobuffer == BUFFER){
		   posidobuffer = 0;
		}
	controlodobuffer++;
    }
    pthread_exit(0);
}

void *consumidor(void *n){

    int item2; 
    
        do{
            
            pthread_mutex_lock(&mutex1); //bloqueia o consumidor
            
        while (controlodobuffer == 0){ // enquanto o buffer estiver vazio 
         pthread_cond_wait(&condconsumidor, &mutex1); //bloqueia o consumidor
         
        }
        
        item2 = buffer[posidesaida]; // recebe o valor que está no buffer , e consome o item do buffer
        pthread_cond_signal(&condprodutor); // acorda o produtor
        pthread_mutex_unlock(&mutex1);  // release access to buffer
        
        printf("Consumiu o item %d \n \n", item2);
        
        posidesaida++;
        
        if(posidesaida == BUFFER){
            posidesaida = 0;
        }
        
        controlodobuffer--;
        } while (item2 != (PRODUCAO - 1) );
    pthread_exit(0);
}


int main(int argc, char *argv[]){
    pthread_t threads[N_THREADS];
    
    pthread_mutex_init(&mutex1, NULL); // Criação de mutexes
    pthread_cond_init(&condprodutor, NULL); // condition variable que vai bloquear o produtor
	pthread_cond_init(&condconsumidor, NULL);  // condition variable que vai bloquear o consumidor
    
    pthread_create(&threads[0], NULL, (void *) &produtor, NULL);
    pthread_create(&threads[1], NULL, (void *) &consumidor, NULL);
    
     for (int i=0; i< N_THREADS ; i++){
        pthread_join (threads[i], NULL);
        // printf ("Terminaram os threads");
    }
    
    pthread_cond_destroy(&condprodutor);
	pthread_cond_destroy(&condconsumidor);
	pthread_mutex_destroy(&mutex1);
	
	return 0;
	
}

