#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <stdbool.h>
#include <semaphore.h>


sem_t seminserir;
sem_t semretirar;

#define MAX 2

typedef struct 
{
    //int pid;
    //int destination;
    int p[3];
} Clock;

typedef struct
{
   Clock clocks[MAX];
   int inicio;
   int fim;
   int tam;
} fila;


void inicializar(fila *f)
{
    f->inicio = -1;
    f->fim = -1;
    f->tam = 0;
}


int tam(fila *f)
{
    return f->tam;
}



bool cheia(fila *f)
{
    return tam(f) == MAX;
}

bool vazia (fila *f)
{
    return tam(f) == 0;
}



bool inserir(Clock clock, fila *f)
{
    

    if(cheia(f))
    {
        printf("Fila cheia");
        sem_wait(&semretirar);
    }if (vazia(f))
    {
        f->fim = (f->fim + 1) % MAX;
        f->clocks[f->fim] = clock;
        f->tam++;
        sem_post(&seminserir);
        }else
        f->fim = (f->fim + 1) % MAX;
        f->clocks[f->fim] = clock;
        f->tam++;
                
    return true;
}



void retirar(fila *f)
{
    //Clock c;

    if(vazia(f))
    {
        printf("Fila Vazia");
        sem_wait(&seminserir);
        
    }if(cheia(f))
        {
        //c = f->clocks[f->inicio];
        f->inicio = (f->inicio + 1) % MAX;
        f->tam--;
        sem_post(&semretirar);
        }else
        printf("Process: %d, Clock: (%d, %d, %d)\n", 2, f->clocks[f->inicio].p[0], f->clocks[f->inicio].p[1], f->clocks[f->inicio].p[2]);
        //c = f->clocks[f->inicio];
        f->inicio = (f->inicio + 1) % MAX;
        f->tam--;
    
    //return c;
}



void imprimir(fila *q)
{
    printf("tam = %d\n", tam(q));

}

int main(void)
{
    
  sem_init(&seminserir, 0, 0);
  sem_init(&semretirar, 0, 0);
 
  fila f;
  inicializar(&f);
  retirar(&f);

  

}