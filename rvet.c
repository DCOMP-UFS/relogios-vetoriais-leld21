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


typedef struct Clock { 
   int p[3];
} Clock;


void Event(int pid, Clock *clock){
   clock->p[pid]++;   
}


void Send(int pid, int pidR,  Clock *clock){
   clock->p[pid]++;
   MPI_Send(clock->p,3,MPI_INT,pidR,0,MPI_COMM_WORLD);
}
void comparaClocks(Clock *clock,int p0, int p1, int p2 )
{
   /*
     if (p0 == 0)
    {
        if (clock->p[1] >= p0)
            clock->p[1] = p0;

        if (clock->p[2] >= p0)
            clock->p[2] = p0;
    }
    else if (p1 == 1)
    {
        if (clock->p[0] >=p1)
            clock->p[0] = p1;

        if (clock->p[2] >= p1)
            clock->p[2] = p1;
    }
    else if (p2 == 2)
    {
        if (clock->p[0] >= p2)
            clock->p[0] = p2;

        if (clock->p[1] >= p2)
            clock->p[1] = p2;
    }
   */
   
   if(clock->p[0]<=p0){
      clock->p[0]=p0;
   }
   if(clock->p[1]<=p1){
      clock->p[1]=p1;
   }
   if(clock->p[2]<=p2){
      clock->p[2]=p2;
   }
   
}

void Receive(int pid, int pidS,  Clock *clock){
   int antigo0 = clock->p[0];
   int antigo1 = clock->p[1];
   int antigo2 = clock->p[2];
   


   MPI_Recv(clock->p, 3, MPI_INT, pidS, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
   
   /*
   if(clock->p[0]<=antigo0){
      clock->p[0]=antigo0;
   }
   if(clock->p[1]<=antigo1){
      clock->p[1]=antigo1;
   }
   if(clock->p[2]<=antigo2){
      clock->p[2]=antigo2;
   }
  */
   comparaClocks(clock,antigo0,antigo1,antigo2);
   clock->p[pid]++;
}

// Representa o processo de rank 0
void process0(){
   Clock clock = {{0,0,0}};
   printf("Process: %d, Clock: (%d, %d, %d)\n", 0, clock.p[0], clock.p[1], clock.p[2]);
   Event(0, &clock);
   printf("Process: %d, Clock: (%d, %d, %d)\n", 0, clock.p[0], clock.p[1], clock.p[2]);
   Send(0,1,&clock);
   printf("Process: %d, Clock: (%d, %d, %d)\n", 0, clock.p[0], clock.p[1], clock.p[2]);
   Receive(0,1,&clock);
   printf("Process: %d, Clock: (%d, %d, %d)\n", 0, clock.p[0], clock.p[1], clock.p[2]);
   Send(0,2,&clock);
   printf("Process: %d, Clock: (%d, %d, %d)\n", 0, clock.p[0], clock.p[1], clock.p[2]);
   Receive(0,2,&clock);
   printf("Process: %d, Clock: (%d, %d, %d)\n", 0, clock.p[0], clock.p[1], clock.p[2]);
   Send(0,1,&clock);
   printf("Process: %d, Clock: (%d, %d, %d)\n", 0, clock.p[0], clock.p[1], clock.p[2]);
   Event(0, &clock);
   printf("Process: %d, Clock: (%d, %d, %d)\n", 0, clock.p[0], clock.p[1], clock.p[2]);

}

// Representa o processo de rank 1
void process1(){
   Clock clock = {{0,0,0}};
   printf("Process: %d, Clock: (%d, %d, %d)\n", 1, clock.p[0], clock.p[1], clock.p[2]);
   Send(1,0,&clock);
   printf("Process: %d, Clock: (%d, %d, %d)\n", 1, clock.p[0], clock.p[1], clock.p[2]);
   Receive(1,0,&clock);
   printf("Process: %d, Clock: (%d, %d, %d)\n", 1, clock.p[0], clock.p[1], clock.p[2]);
   Receive(1,0,&clock);
   printf("Process: %d, Clock: (%d, %d, %d)\n", 1, clock.p[0], clock.p[1], clock.p[2]);
}

// Representa o processo de rank 2
void process2(){
   Clock clock = {{0,0,0}};
   printf("Process: %d, Clock: (%d, %d, %d)\n", 2, clock.p[0], clock.p[1], clock.p[2]);
   Event(2, &clock);
   printf("Process: %d, Clock: (%d, %d, %d)\n", 2, clock.p[0], clock.p[1], clock.p[2]);
   Send(2,0,&clock);
   printf("Process: %d, Clock: (%d, %d, %d)\n", 2, clock.p[0], clock.p[1], clock.p[2]);
   Receive(2,0,&clock);
   printf("Process: %d, Clock: (%d, %d, %d)\n", 2, clock.p[0], clock.p[1], clock.p[2]);
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
      process2();
   }

   /* Finaliza MPI */
   MPI_Finalize(); 

   return 0;
}  /* main */
