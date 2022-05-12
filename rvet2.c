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

Clock clock1;

int pid;

void comparaClocks(Clock *clockv,int p0, int p1, int p2 );

void Event(int pid, Clock *clockt){
   clockt->p[pid]++;
}

void *Emissor (void* f){
   Clock *c = retirar((fila*)f);
   MPI_Send(c->p,3,MPI_INT,c->destination,0,MPI_COMM_WORLD);
}

void *Receptor (void* f){
   int antigo0 = clock1.p[0];
   int antigo1 = clock1.p[1];
   int antigo2 = clock1.p[2];
   MPI_Recv(clock1.p, 3, MPI_INT, clock1.pid, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
   comparaClocks(&clock1,antigo0,antigo1,antigo2);
   inserir(&clock1,((fila*)f),pid);
   
}



/*
void Send(int pid, int pidR,  Clock *clock){
   clock->p[pid]++;
   MPI_Send(clock->p,3,MPI_INT,pidR,0,MPI_COMM_WORLD);
}
*/
void comparaClocks(Clock *clockv,int p0, int p1, int p2 )
{
   if(clockv->p[0]<=p0){
      clockv->p[0]=p0;
   }
   if(clockv->p[1]<=p1){
      clockv->p[1]=p1;
   }
   if(clockv->p[2]<=p2){
      clockv->p[2]=p2;
   }
   
}
/*
void Receive(int pid, int pidS,  Clock *clock){
   int antigo0 = clock->p[0];
   int antigo1 = clock->p[1];
   int antigo2 = clock->p[2];
   


   MPI_Recv(clock->p, 3, MPI_INT, pidS, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
   comparaClocks(clock,antigo0,antigo1,antigo2);
   clock->p[pid]++;
}
*/
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
   
   clock1.destination=1;
   Event(pid, &clock1);
   
   printf("Process: %d, Clock: (%d, %d, %d)\n", 0, clock1.p[0], clock1.p[1], clock1.p[2]);
   inserir(&clock1,f_emissor,pid);
   printf("Process: %d, Clock: (%d, %d, %d)\n", 0, clock1.p[0], clock1.p[1], clock1.p[2]);
   retirar(f_receptor);
   printf("Process: %d, Clock: (%d, %d, %d)\n", 0, clock1.p[0], clock1.p[1], clock1.p[2]);
   /*
   Send(0,2,&clock);
   printf("Process: %d, Clock: (%d, %d, %d)\n", 0, clock.p[0], clock.p[1], clock.p[2]);
   Receive(0,2,&clock);
   printf("Process: %d, Clock: (%d, %d, %d)\n", 0, clock.p[0], clock.p[1], clock.p[2]);
   Send(0,1,&clock);
   printf("Process: %d, Clock: (%d, %d, %d)\n", 0, clock.p[0], clock.p[1], clock.p[2]);
   Event(0, &clock);
   printf("Process: %d, Clock: (%d, %d, %d)\n", 0, clock.p[0], clock.p[1], clock.p[2]);
   */
   pthread_join(t1, NULL); 
   //pthread_join(t2, NULL); 

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
   /*
   Send(1,0,&clock);
   */
   
   clock1.destination=0;
   
   inserir(&clock1,f_emissor,pid);
   printf("Process: %d, Clock: (%d, %d, %d)\n", 1, clock1.p[0], clock1.p[1], clock1.p[2]);
   retirar(f_receptor);
   //Receive(1,0,&clock);
   printf("Process: %d, Clock: (%d, %d, %d)\n", 1, clock1.p[0], clock1.p[1], clock1.p[2]);
   /*
   Receive(1,0,&clock);
   printf("Process: %d, Clock: (%d, %d, %d)\n", 1, clock.p[0], clock.p[1], clock.p[2]);
   */
}

// Representa o processo de rank 2
void process2(){
   pid = 2;
   clock1.p[0]=0;
   clock1.p[1]=0;
   clock1.p[2]=0;
   printf("Process: %d, Clock: (%d, %d, %d)\n", 2, clock1.p[0], clock1.p[1], clock1.p[2]);
   Event(pid, &clock1);
   /*
   printf("Process: %d, Clock: (%d, %d, %d)\n", 2, clock.p[0], clock.p[1], clock.p[2]);
   Send(2,0,&clock);
   printf("Process: %d, Clock: (%d, %d, %d)\n", 2, clock.p[0], clock.p[1], clock.p[2]);
   Receive(2,0,&clock);
   printf("Process: %d, Clock: (%d, %d, %d)\n", 2, clock.p[0], clock.p[1], clock.p[2]);
   */
}



int main(void) {
   int my_rank;    
   
   pthread_mutex_init(&mutex, NULL);

   MPI_Init(NULL, NULL); 
   MPI_Comm_rank(MPI_COMM_WORLD, &my_rank); 

   if (my_rank == 0) { 
      process0();
   } else if (my_rank == 1) {  
      process1();
   } else if (my_rank == 2) {  
      process2();
   }

   /* Finaliza MPI */
   MPI_Finalize(); 
   pthread_mutex_destroy(&mutex);
   
   return 0;
}  /* main */
