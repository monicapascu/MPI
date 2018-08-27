#include <stdio.h>
#include <mpi.h>

#define NUMPROCS 8

int main(int argc, char ** argv)
{
    int nproc, my_rank, my_new_rank, sendbuf, recvbuf;
    int odd_ranks[4] = {1,3,5,7};
    int even_ranks[4] = {0,2,4,6};
    MPI_Comm initial_comm = MPI_COMM_WORLD, new_comm;
    MPI_Group initial_group, new_group;
    
    MPI_Init(&argc, &argv);
    MPI_Comm_size(initial_comm, &nproc);
    MPI_Comm_rank(initial_comm, &my_rank);
    
    if(nproc != NUMPROCS)
    {
        if(my_rank == 0)
            printf("Programul functioneaza corect daca este lansat in executie cu %d procese.\n",NUMPROCS);
        MPI_Finalize();
        return 1;
    }
    
    sendbuf = my_rank;
    
    MPI_Comm_group(initial_comm, &initial_group);
    
    if(my_rank % 2)
        MPI_Group_incl(initial_group,4,odd_ranks,&new_group);
    else
        MPI_Group_incl(initial_group,4,even_ranks,&new_group);
    
    MPI_Group_rank(new_group,&my_new_rank);
    MPI_Comm_create(initial_comm,new_group,&new_comm);
    MPI_Reduce(&sendbuf,&recvbuf,1,MPI_INT,MPI_SUM,0,new_comm);
    
    if(!my_new_rank)
        printf("Procesul cu rang initial %d \n \trang actual %d are recvbuf = %d \n",my_rank,my_new_rank,recvbuf);
    
    MPI_Finalize();
    return 0;
}
