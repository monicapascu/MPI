#include <stdio.h>
#include <string.h>
#include <mpi.h>
#include <math.h>

int main(int argc, char ** argv)
{
    int nproc, my_rank,my_rank_in_row,my_row,n;
    char text[32];
    MPI_Comm initial_comm = MPI_COMM_WORLD, comm_in_row;
    MPI_Group initial_group, group_in_row;
    
    
    MPI_Init(&argc,&argv);
    MPI_Comm_size(initial_comm, &nproc);
    MPI_Comm_rank(initial_comm, &my_rank);
    
    n = (int)sqrt((double)nproc);
    my_row = my_rank/n;
    
    MPI_Comm_split(initial_comm,my_row,my_rank,&comm_in_row);
    MPI_Comm_rank(comm_in_row,&my_rank_in_row);
    
    if(my_rank_in_row == 0)
        strcpy(text, "Succes!");
    
    MPI_Bcast(text,32,MPI_CHAR,0,comm_in_row);
    
    printf("Procesul cu rangul initial %d, rang in linie %d, din linia %d, are textul %s \n",my_rank,my_rank_in_row,my_row,text);
    
    MPI_Finalize();
    return 0;
}

