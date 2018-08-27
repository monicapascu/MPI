#include <stdio.h>
#include <stdlib.h>
#include "pgm_IO.h"
#include "pgm_IO.c"
#include <mpi.h>
#define ITER 1600
#define FREQ 200

// Paralelizare:
// - definire sarcini elementare
// - aglomerare ( reorganizare sarcini)
// - comunicare
// - "strangere" date

//Obs: procesul de rang 0 este procesul master, el se va ocupa de mesaje, el citeste, el scrie fisierul de intrare/iesire
//distribuie catre celelalte procese, inclusiv catre sine, parti din imaginea initiala
//Comunicarea dintre procese: topologie de tip inel. 

//MPI_PROC_NULL -> ultila pt margini

int main(int argc, char ** argv)
{
    float *masterdata;
    int nproc,my_rank,nrows,ncols,nx,ny,i,j,crs;
    char *input;
    int next,prev;
    float *new,*old,*edge,*data;
    FILE *fp;
    char fname[64];
    
    MPI_Comm initial_comm = MPI_COMM_WORLD;
    MPI_Init(&argc,&argv);
    MPI_Comm_size(initial_comm, &nproc);
    MPI_Comm_rank(initial_comm, &my_rank);
    MPI_Status status;
    
    next = my_rank+1;
    prev = my_rank-1;
    
    //sau next = (my_rank+1)%nproc;
    // prev = (my_rank-1+nproc)%nproc;
    // si nu mai trebuie aia cu mpi proc null
    
    if(next >= nproc)
    {
        next = MPI_PROC_NULL;
    }
    
    if(prev < 0)
    {
        prev = MPI_PROC_NULL;
    }
    
    if (my_rank == 0)
    {
        input = "edge_img192x128.pgm";
        printf("Se trateaza fisierul pgm <%s> \n",input);
        pgm_size(input,&nrows,&ncols);
        masterdata = (float*)malloc(nrows*ncols*sizeof(float));
        
        if(masterdata == NULL)
        {
        printf("Nu am putut sa alocam memorie \n");
        return -1;
        }
        
        nx = nrows/nproc;
        ny = ncols;
        
        pgm_read(input,masterdata,nrows,ncols);
    }
    
    MPI_Bcast(&nx,1,MPI_INT,0,MPI_COMM_WORLD);
    MPI_Bcast(&ny,1,MPI_INT,0,MPI_COMM_WORLD);
    
    
     if((data = malloc(nx*ny*sizeof(float))) == NULL)
    {
        printf("Nu am putut sa alocam memorie \n");
        return -1;
    }
    
      if((old = malloc((nx+2)*(ny+2)*sizeof(float))) == NULL)
    {
        printf("Nu am putut sa alocam memorie \n");
        return -1;
    }
    
     if((new = malloc((nx+2)*(ny+2)*sizeof(float))) == NULL)
    {
        printf("Nu am putut sa alocam memorie \n");
        return -1;
    }
    
    
     if((edge = malloc((nx+2)*(ny+2)*sizeof(float))) == NULL)
    {
        printf("Nu am putut sa alocam memorie \n");
        return -1;
    }
    
    MPI_Scatter(masterdata,nx*ny,MPI_FLOAT,data,nx*ny,MPI_FLOAT,0,MPI_COMM_WORLD);
    
    for(i=1; i< nx+1; i++)
        for(j=1; j< ny+1; j++)
        *(edge + i*(ny+2) + j) = *(data+(i-1)*ny + j-1);
    
    for(crs=0; crs<ITER; crs++)
    {
        if(!crs%FREQ)
            if(my_rank == 0)
                printf("Iteratia %d... \n",crs);
        
        //m[i][j]= *(m+i*ncol+j)    
        MPI_Sendrecv(old+ny+2+1,ny,MPI_FLOAT,prev,123,old+(nx+1)*(ny+2)+1,ny,MPI_FLOAT,next,123,MPI_COMM_WORLD,&status);
        MPI_Sendrecv(old+nx*(ny+2)+1,ny,MPI_FLOAT,next,321,old+1,ny,MPI_FLOAT,prev,321,MPI_COMM_WORLD,&status);
        
         for(i=1; i< nx+1; i++)
          for(j=1; j< ny+1; j++)
             *(new + i*(ny+2)+j) = 0.25 * (  *(old + (i-1)*(ny+2)+j) + *(old + (i+1)*(ny+2)+ j) + *(old + i*(ny+2)+j-1) + *(old + i*(ny+2)+j+1) - *(edge + i*(ny+2)+j) );   
    
        for(i=1; i<nx+1; i++)
            for(j=1; j< ny+1; j++)
                *(old + i*(ny+2) +j) = *(new + i*(ny+2)+j);
    }
    
        
        
        for(i=0; i<nx; i++)
        for(j=0; j<ny; j++)
            *(data+i*ny+j) = *(old + (i+1)*(ny+2)+j+1);
        
        MPI_Gather(data,nx*ny,MPI_FLOAT,masterdata,nx*ny,MPI_FLOAT,0,MPI_COMM_WORLD);
        
        if(my_rank == 0)
        {
            sprintf(fname, "poza_%d_iter.pgm",ITER);
            pgm_write(fname,masterdata,nrows,ncols);
            printf("Finalizare!\n\n");
            free(masterdata);
        }
        
        free(data); free(old); free(new); free(edge);
        MPI_Finalize();
        return 0;
}
        
