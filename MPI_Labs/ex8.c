//forteze lansarea in executie a 6 procese 
//va defini 2 grupuri din grupupl initial: g proceselor de rang par si cel de rang impar
//operatie de reducere in interiorul fiecarui subgrup
//procesul de rang0 din fiecare subgrup va afisa rezultatul reducerii
//reducere prin sumare
//transmite rangul fiecarui proces

//de folosit:
// MPI_Init()
// MPI_Finalize()
// MPI_Comm_size()
// MPI_Comm_rank()
// MPI_Comm_group()
// MPI_Group_incl()
// MPI_Group_rank()
// MPI_Reduce()
// MPI_Comm_create()


#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

#define MASTER 0
#define SLAVE 1
#define NUMPROCS 6

int main(int argc, char ** argv)
{
    int nproc,i,my_rank,my_new_rank;
    int odd_ranks[3] = {1,3,5};
    int even_ranks[3] = {0,2,4};
    int sendbuf,recvbuf;
    
    MPI_Group initial_group,new_group;
    MPI_Comm initial_comm = MPI_COMM_WORLD, new_comm;
    
    MPI_Init(&argc,&argv);
    MPI_Comm_size(initial_comm, &nproc);
    MPI_Comm_rank(initial_comm, &my_rank);
    
    if(nproc!= NUMPROCS)
    {
        if(my_rank == MASTER)
            printf("Programul functioneaza corect daca sunt lansate 6 procese in executie \n");
     
        MPI_Finalize();
        return 1;
    }
    else
    {
        MPI_Comm_group(initial_comm, &initial_group);
        sendbuf = my_rank; 
        return 0;
        if(my_rank % 2)
            MPI_Group_incl(initial_group,3,odd_ranks,&new_group);
         else
            MPI_Group_incl(initial_group,3,even_ranks,&new_group); 
   
    
        MPI_Comm_create(initial_comm,new_group,&new_comm);
        
        MPI_Group_rank(new_group,&my_new_rank);
        
        
        MPI_Reduce(&sendbuf,&recvbuf,1,MPI_INT,MPI_SUM,0,new_comm);
        
        
        if(my_new_rank == MASTER)
        {
            printf("Procesul cu rang initial %d, rang actual %d, declara recvbuf = %d \n",my_rank,my_new_rank,recvbuf);
        }
        
       
    }
    
    MPI_Finalize();
    return 0;
}
