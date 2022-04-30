#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <stdbool.h>


#define MAX 10

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
    return tam(f) == MAX;
}

bool vazia (fila *f)
{
    return tam(f) == 0;
}



bool inserir(CLOCK clock, fila *f)
{
    
    //(&mutex);
    //sem_init(&semaphore, 0, 1);
    while(cheia(f))
    {
        printf("cheio");
    }
    
    f->fim = (f->fim + 1) % MAX;
    f->clocks[f->fim] = clock;
    f->tam++;
    

    return true;
}



CLOCK retirar(fila *f)
{
    CLOCK c;

    while(vazia(f))
    {
        printf("vazio");
        
    }
    
    c = f->clocks[f->inicio];
    f->inicio = (f->inicio + 1) % MAX;
    f->tam--;
    
    
    return c;
}



void imprimir(fila *q)
{
    printf("tam = %d\n", tam(q));

}

int main(void)
{
  fila f;
  inicializar(&f);
  
  

}