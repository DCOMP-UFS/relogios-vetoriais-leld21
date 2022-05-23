/**
 * Código base (incompleto) para implementação de relógios vetoriais.
 * Meta: implementar a interação entre três processos ilustrada na figura
 * da URL: 
 * 
 * https://people.cs.rutgers.edu/~pxk/417/notes/images/clocks-vector.png
 * 
 * Compilação: mpicc -o rvet rvet.c
 * Execução:   mpiexec -n 3 ./rvet
 */
 
#include <stdio.h>
#include <string.h>  
#include <mpi.h>     
#include <string.h>
#include <pthread.h>
#include <stdbool.h>
#include <semaphore.h>
#include "sem2.c"
//#include "queue.c"

pthread_mutex_t clockmutex = PTHREAD_MUTEX_INITIALIZER;


Clock clock1;

int pid;
int enviou;

void comparaClocks(Clock *clockv,int p0, int p1, int p2 );

void Event(int pid, Clock *clockt){
   clockt->p[pid]++;
}

void *Emissor (void* f){
   Clock *c = retirar((fila*)f,pid);
   MPI_Send(c->p,3,MPI_INT,c->destination,0,MPI_COMM_WORLD);
}

void *Receptor (void* f){
   int antigo0 = clock1.p[0];
   int antigo1 = clock1.p[1];
   int antigo2 = clock1.p[2];
   pthread_mutex_lock(&clockmutex);
   MPI_Recv(clock1.p, 3, MPI_INT, MPI_ANY_SOURCE, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
   pthread_mutex_unlock(&clockmutex);
   comparaClocks(&clock1,antigo0,antigo1,antigo2);
   inserir(&clock1,((fila*)f),pid);
   
}


void comparaClocks(Clock *clockv,int p0, int p1, int p2 )
{
   if(clockv->p[0]<=p0){
      pthread_mutex_lock(&clockmutex);
      clockv->p[0]=p0;
      pthread_mutex_unlock(&clockmutex);
   }
   if(clockv->p[1]<=p1){
      pthread_mutex_lock(&clockmutex);
      clockv->p[1]=p1;
      pthread_mutex_unlock(&clockmutex);
   }
   if(clockv->p[2]<=p2){
      pthread_mutex_lock(&clockmutex);
      clockv->p[2]=p2;
      pthread_mutex_unlock(&clockmutex);
   }
   
}

// Representa o processo de rank 0
void process0(){
   pid =0;
   clock1.p[0]=0;
   clock1.p[1]=0;
   clock1.p[2]=0;
   
   pthread_t t1, t2;
   
   fila *f_emissor=malloc(sizeof(fila));
   inicializar(f_emissor);
   
   fila *f_receptor=malloc(sizeof(fila));
   inicializar(f_receptor);
   
   pthread_create(&t1, NULL, Emissor, (void*) f_emissor);  
   pthread_create(&t2, NULL, Receptor, (void*) f_receptor);  
   
   printf("Process: %d, Clock: (%d, %d, %d)\n", 0, clock1.p[0], clock1.p[1], clock1.p[2]);
   
   Event(pid, &clock1);
   
   printf("Process: %d, Clock: (%d, %d, %d)\n", 0, clock1.p[0], clock1.p[1], clock1.p[2]);
   
   
   clock1.destination=1;
   inserir(&clock1,f_emissor,pid);
   
   printf("Process: %d, Clock: (%d, %d, %d)\n", 0, clock1.p[0], clock1.p[1], clock1.p[2]);
   
   enviou=1;
   clock1=*retirar(f_receptor,pid);
   
   printf("Process: %d, Clock: (%d, %d, %d)\n", 0, clock1.p[0], clock1.p[1], clock1.p[2]);
   /*
   clock1.destination=2;
   inserir(&clock1,f_emissor,pid);
   
   printf("Process: %d, Clock: (%d, %d, %d)\n", 0, clock1.p[0], clock1.p[1], clock1.p[2]);
   
   enviou=2;
   clock1=retirar(f_receptor,pid);
   
   printf("Process: %d, Clock: (%d, %d, %d)\n", 0, clock1.p[0], clock1.p[1], clock1.p[2]);
   */
   
   clock1.destination=1;
   inserir(&clock1,f_emissor,pid);
   
   printf("Process: %d, Clock: (%d, %d, %d)\n", 0, clock1.p[0], clock1.p[1], clock1.p[2]);
   
   Event(pid, &clock1);
   
   printf("Process: %d, Clock: (%d, %d, %d)\n", 0, clock1.p[0], clock1.p[1], clock1.p[2]);
   
   pthread_join(t1, NULL); 
   pthread_join(t2, NULL); 

}
void process1(){
   pid =1;
   clock1.p[0]=0;
   clock1.p[1]=0;
   clock1.p[2]=0;
   
   pthread_t t1, t2;
   
   fila *f_emissor=malloc(sizeof(fila));
   inicializar(f_emissor);
   
   fila *f_receptor=malloc(sizeof(fila));
   inicializar(f_receptor);
   
   pthread_create(&t1, NULL, Emissor, (void*) f_emissor);  
   pthread_create(&t2, NULL, Receptor, (void*) f_receptor);
   
   printf("Process: %d, Clock: (%d, %d, %d)\n", 1, clock1.p[0], clock1.p[1], clock1.p[2]);

   clock1.destination=0;
   inserir(&clock1,f_emissor,pid);
   
   printf("Process: %d, Clock: (%d, %d, %d)\n", 1, clock1.p[0], clock1.p[1], clock1.p[2]);
   
   enviou=0;
   clock1=*retirar(f_receptor,pid);

   printf("Process: %d, Clock: (%d, %d, %d)\n", 1, clock1.p[0], clock1.p[1], clock1.p[2]);
   
   enviou=0;
   clock1=*retirar(f_receptor,pid);

   printf("Process: %d, Clock: (%d, %d, %d)\n", 1, clock1.p[0], clock1.p[1], clock1.p[2]);
   
   pthread_join(t1, NULL); 
   pthread_join(t2, NULL); 
}

// Representa o processo de rank 2
void process2(){
   pid = 2;
   clock1.p[0]=0;
   clock1.p[1]=0;
   clock1.p[2]=0;
   
   pthread_t t1, t2;
   
   fila *f_emissor=malloc(sizeof(fila));
   inicializar(f_emissor);
   
   fila *f_receptor=malloc(sizeof(fila));
   inicializar(f_receptor);
   
   pthread_create(&t1, NULL, Emissor, (void*) f_emissor);  
   pthread_create(&t2, NULL, Receptor, (void*) f_receptor);
   
   printf("Process: %d, Clock: (%d, %d, %d)\n", 2, clock1.p[0], clock1.p[1], clock1.p[2]);
   
   Event(pid, &clock1);
   
   printf("Process: %d, Clock: (%d, %d, %d)\n", 2, clock1.p[0], clock1.p[1], clock1.p[2]);
   
   clock1.destination=0;
   inserir(&clock1,f_emissor,pid);
   printf("Process: %d, Clock: (%d, %d, %d)\n", 2, clock1.p[0], clock1.p[1], clock1.p[2]);
   enviou=0;
   retirar(f_receptor,pid);

   printf("Process: %d, Clock: (%d, %d, %d)\n", 2, clock1.p[0], clock1.p[1], clock1.p[2]);
   pthread_join(t1, NULL); 
   pthread_join(t2, NULL); 
}



int main(void) {
   int my_rank;    
   

   MPI_Init(NULL, NULL); 
   MPI_Comm_rank(MPI_COMM_WORLD, &my_rank); 

   if (my_rank == 0) { 
      process0();
   } else if (my_rank == 1) {  
      process1();
   } else if (my_rank == 2) {  
      //process2();
   }

   /* Finaliza MPI */
   MPI_Finalize(); 
   
   return 0;
}  /* main */
