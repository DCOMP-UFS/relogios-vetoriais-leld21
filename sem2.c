#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <stdbool.h>
#include <semaphore.h>

pthread_mutex_t mutex;


pthread_cond_t condFull;
pthread_cond_t condEmpty;

sem_t seminserir;
sem_t semretirar;

#define MAX 50
#define TAMANHOFILA 10

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
    f->fim = 0;
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
        //printf("Fila cheia\n");
        pthread_cond_wait(&condFull, &mutex);
    }
    if (vazia(f)){
        clock->p[pid]++;
        f->clocks[f->inicio] = clock;
        f->tam=1;
        pthread_mutex_unlock(&mutex);
        pthread_cond_signal(&condEmpty);
    }else{
        clock->p[pid]++;
        f->fim = (f->fim + 1);
        f->clocks[f->fim] = clock;
        f->tam++;
        pthread_mutex_unlock(&mutex);
        pthread_cond_signal(&condEmpty);
    }
    return true;
}

void imprimir(fila *q)
{
    printf("tam = %d\n", tam(q));

}


Clock* retirar(fila *f)
{
    Clock *c;
    while(vazia(f))
    {
        //printf("Fila Vazia\n");
        pthread_cond_wait(&condEmpty, &mutex);
        
        
    }
        c=f->clocks[f->inicio];
        f->inicio = (f->inicio + 1);
        f->tam--;
    pthread_cond_signal(&condFull);
    return c;
}
/*
void *criarthread (void* f){
    Clock c1;
    c1.p[0]=1;c1.p[1]=1;c1.p[2]=1;
    inserir(c1,(fila*)f);
    c1.p[0]=2;c1.p[1]=2;c1.p[2]=2;
    inserir(c1,(fila*)f);
    c1.p[0]=3;c1.p[1]=3;c1.p[2]=3;
    inserir(c1,(fila*)f);
    c1.p[0]=4;c1.p[1]=4;c1.p[2]=4;
    inserir(c1,(fila*)f);
    //inserir(c1,(fila*)f);
    return NULL;
}
 
void *removerthread (void* f){
    retirar((fila*)f);
    retirar((fila*)f);
    retirar((fila*)f);
    retirar((fila*)f);
    //retirar((fila*)f);
    return NULL;
}
*/
/*
//remover esse main 
int main(void)
{
    
  pthread_t t1, t2;
  
  pthread_mutex_init(&mutex, NULL);
    
  //sem_init(&seminserir, 0, 0);
  //sem_init(&semretirar, 0, 0);
 
  fila *f=malloc(sizeof(fila));
  inicializar(f);
  
  pthread_create(&t1, NULL, criarthread, (void*) f);  
  pthread_create(&t2, NULL, removerthread, (void*) f);  
  
  pthread_join(t1, NULL); 
  pthread_join(t2, NULL);
  
  pthread_mutex_destroy(&mutex);

}
*/