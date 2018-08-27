//P0 - verifica nproc=2
//-aloca dinamic 10*sizeof(int) repeta i<10
//- RAM[i]=i
//MPI_Ssend(RAM).
//MPI_Recv(err)
//P1:
//-RAM[I]=-1
//MPI_Recv(RAM) daca RAM[i]!=i
//MPI_Send(err)

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <mpi.h>

#define MASTER 0
#define SLAVE 1

int main(int argc, char ** argv)
{
    int nproc,i,my_rank;
    int *data;
    char err;
    
    MPI_Comm my_comm=MPI_COMM_WORLD;
    MPI_Status status; 


    data = (int*)malloc(10*sizeof(int));
    if(data == NULL){
        fprintf(stdout,"Nu se poate aloca memorie \n");
        exit(-1);
    }
    
    //Programarea paralela.
    MPI_Init(&argc,&argv);
    MPI_Comm_size(my_comm,&nproc);
    MPI_Comm_rank(my_comm,&my_rank);
    
    if(my_rank == MASTER)
    { 
        if (nproc != 2){
            printf("Trebuie lansate 2 procese in executie! \n");
            free(data);
            MPI_Abort(my_comm,1);
            return 1;
        
        }
        
        for(i=0; i<10; i++){
            *(data+i) = i;
            MPI_Ssend(data,10,MPI_INT,SLAVE,123,my_comm);
          
        }
        
          MPI_Recv(&err,1,MPI_CHAR,SLAVE,234,my_comm,&status);
          if(err == 'N')
              printf("Procesul %d a receptionat prost cel putin 1 element \n", SLAVE);
          
    }
    else{ //my_rank == SLAVE
        
        for(i=0; i<10; i++){
            *(data+i) = -1;
            MPI_Recv(data,10,MPI_INT,MASTER,123,my_comm,&status);
            if(*(data+i) != i){
                err = 'N';
                MPI_Send(&err,1,MPI_CHAR,MASTER,234,my_comm);
            }
            
        
            }
        }

    
    MPI_Finalize();
    free(data);

    
    return 0;
}
    
    
        
