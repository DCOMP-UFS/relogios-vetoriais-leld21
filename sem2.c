#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <stdbool.h>
#include <semaphore.h>

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;


pthread_cond_t condFull= PTHREAD_COND_INITIALIZER;
pthread_cond_t condEmpty= PTHREAD_COND_INITIALIZER;

sem_t seminserir;
sem_t semretirar;

#define MAX 500
#define TAMANHOFILA 100

typedef struct 
{
    int pid;
    int destination;
    int p[3];
} Clock;

typedef struct
{
   Clock *clocks[MAX];
   int inicio;
   int fim;
   int tam;
} fila;

void inicializar(fila *f)
{
    f->inicio = 0;
    f->fim = -1;
    f->tam = 0;
}


int tam(fila *f)
{
    return f->tam;
}



bool cheia(fila *f)
{
    if(tam(f) == TAMANHOFILA){
        return 1;
    }else{
        return 0;
    }
}

bool vazia (fila *f)
{
    if(tam(f) == 0){
        return 1;
    }else{
        return 0;
    }
}

bool inserir(Clock *clock, fila *f,int pid)
{
    
    pthread_mutex_lock(&mutex);
    while(cheia(f))
    {
        printf("Fila cheia\n");
        pthread_cond_wait(&condFull, &mutex);
    }
    /*
    if (vazia(f)){
        clock->p[pid]++;
        f->clocks[f->inicio] = clock;
        f->tam=1;
        pthread_mutex_unlock(&mutex);
        pthread_cond_signal(&condEmpty);
    }else{
    */
        clock->p[pid]++;
        f->fim = (f->fim + 1)% TAMANHOFILA;;
        f->clocks[f->fim] = clock;
        f->tam++;
        pthread_mutex_unlock(&mutex);
        pthread_cond_signal(&condEmpty);
//}
return true;
}

void imprimir(fila *q)
{
    printf("tam = %d\n", tam(q));

}


Clock* retirar(fila *f,int pid)
{
    Clock *c;
    while(vazia(f))
    {
        printf("Fila Vazia devido a %d\n", pid);
        pthread_cond_wait(&condEmpty, &mutex);
        
        
    }
        c=f->clocks[f->inicio];
        f->inicio = (f->inicio + 1)% TAMANHOFILA;;
        f->tam--;
    pthread_cond_signal(&condFull);
    return c;
}
