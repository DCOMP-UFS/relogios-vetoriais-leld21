#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <stdbool.h>


#define MAX 100

typedef struct 
{
    int pid;
    int destination;
    int p[3];
} CLOCK;

typedef struct
{
   CLOCK clocks[MAX];
   int inicio;
   int fim;
   int tam;
} fila;


void inicializar(fila *q)
{
    q->inicio = 0;
    q->fim = -1;
    q->tam = 0;
}


int tam(fila *q)
{
    return q->tam;
}

bool cheia(fila *q)
{
    return tam(q) == MAX;
}



bool inserir(CLOCK clock, fila *q)
{
    
    pthread_mutex_lock(&mutex);
    while(cheia(q))
    {
        pthread_cond_wait(&condFull, &mutex);
    }
    
    q->fim = (q->fim + 1) % MAX;
    q->clocks[q->fim] = clock;
    q->tam++;
    
    pthread_mutex_unlock(&mutex);
    pthread_cond_signal(&condEmpty);
    return true;
}



void imprimir(fila *q)
{
    printf("tamam = %d\n", tam(q));
    
}
