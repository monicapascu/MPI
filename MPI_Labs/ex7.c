//P0 : x0,....xn     
//P1 [x1]
//.
//.
//.
//Pn [xn]
//sbuf = nproc*10*sizeof(int) 
//rbuf = 10*sizeof(int)
//nproc = 4
//facem fiecare proces sa-si scrie bufferul local, inclsiv procesul 0

#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

#define MASTER 0
#define SLAVE 1
#define NMAX 10

int main(int argc, char ** argv)
{
    int nproc,i,my_rank,*sbuf,*rbuf;
    
    MPI_Comm my_comm=MPI_COMM_WORLD;
    MPI_Status status; 
    
    //Programarea paralela.
    MPI_Init(&argc,&argv);
    MPI_Comm_size(my_comm,&nproc);
    MPI_Comm_rank(my_comm,&my_rank);
    
    
    if(my_rank == MASTER){
    
        sbuf = (int*)malloc(nproc*NMAX*sizeof(int));
        for(i=0; i<NMAX*nproc; i++)
            *(sbuf+i) = i+1;
    }
    
    rbuf = (int*)malloc(NMAX*sizeof(int));
    MPI_Scatter(sbuf, NMAX, MPI_INT, rbuf,NMAX, MPI_INT, MASTER, my_comm);
    
    for(i=0; i<NMAX; i++)
        printf("Procesul %d: rbuf[%d] = %d \n",my_rank,i,*(rbuf+i));
        

        
    
    if(my_rank == MASTER) free(sbuf);
    
    MPI_Finalize();
    free(rbuf);
    

    
    return 0;
}
