#include <stdio.h>
#include <mpi.h>
#include <math.h>
#include <string.h>

int main(int argc, char ** argv)
{
    int nproc, my_rank, my_rank_in_row, my_row, n;
    MPI_Comm initial_comm = MPI_COMM_WORLD, comm_in_row;
    char text[32];
    
    MPI_Init(&argc, &argv);
    MPI_Comm_size(initial_comm, &nproc);
    MPI_Comm_rank(initial_comm, &my_rank);
    
    n = (int)sqrt((double)nproc);
    
    my_row = my_rank/n;
    
    MPI_Comm_split(initial_comm,my_row,my_rank,&comm_in_row);
    MPI_Comm_rank(comm_in_row, &my_rank_in_row);
    
    if(my_rank_in_row == 0)
        strcpy(text,"Iar e marti...");
        
    MPI_Bcast(text,32,MPI_CHAR,0,comm_in_row);
    
    printf("Procesul cu rang %d in linia %d, rang vechi %d, are textul: %s \n",my_rank_in_row, my_row, my_rank, text);
    
    MPI_Finalize();
    return 0;
}
