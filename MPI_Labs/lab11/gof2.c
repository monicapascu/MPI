#include "gof.h"
#include<stdlib.h>
#include<string.h>
#include<mpi.h>
#include "pgm_IO.c"

 int main (int argc, char **argv)
{
int src, dest, nproc, my_rank;    
int i, j, nx, ny, dims[NDIM], periods[NDIM], reorder=0, coords[NDIM];
int up, down, left, right;//pentru deplasarea pe topologia carteziana
float *mastergblock, *gblock,*gblocknext;
char fname[64];

MPI_Status status;
MPI_Datatype blocktype, linetype, coltype;
MPI_Comm cart_comm;
    
MPI_Init(&argc,&argv); //initializare MPI

MPI_Comm_size(MPI_COMM_WORLD, &nproc);//aflarea numarului de procese
MPI_Comm_rank(MPI_COMM_WORLD,&my_rank);//aflarea rangului 

if (!my_rank)
    if((mastergblock=(float *)malloc(GSIZE*sizeof(float)))==NULL)
        {
        printf("Eroare: nu pot aloca RAM pt mastergblock");
        return -1;
       }
       else
       {
           for(i=0;i<GSIZE;i++)
               *(mastergblock+i)=255.;
       }
dims[0]=dims[1]=0;    
MPI_Dims_create(nproc, NDIM, dims);//distribuirea in mod balansat procesele pe cele 2 dimensiuni ale topologiei virtuale. Rezultatul distribuitiei este continut in sirul dims
//ATENTIE dims[0] este nr de coloane nx si dims[1] este nr de linii ny!!!
//Se presupune ca dims[0] si dims[1] sunt divizori ai lu XSIZE SI YSIZE

nx=XSIZE/dims[0];
ny=YSIZE/dims[1];

if((gblock=(float *)malloc((nx+2)*(ny+2)*sizeof(float)))==NULL)
        {
        printf("Eroare: nu pot aloca RAM pt gblock");
        return -1;
       }
if((gblocknext=(float *)malloc((nx+2)*(ny+2)*sizeof(float)))==NULL)
        {
        printf("Eroare: nu pot aloca RAM pt gblocknext");
        return -1;
       }
for(i=0;i<(nx+2)*(ny+2);i++)
    *(gblock+i)=0.75*MAXGRE=*(gblocknext+i);
    

*(periods)=TRUE;
*(periods+1)=TRUE;

MPI_Cart_create(MPI_COMM_WORLD, NDIM, dims, periods,reorder, &cart_comm);// un nou comunicator pentru topologie
MPI_Cart_coords(cart_comm, my_rank, NDIM, coords);//determina coordonatele procesului curent in topologia noua

if((coords[0] + coords[1] + 1) % 2== 1)
    for(i=0;i<nx*ny;i++)
        *(gblock+i)=0.;
else
    for(i=0;i<nx*ny;i++)
        *(gblock+i)=MAXGREY;

        
MPI_Type_vector(ny, nx, XSIZE, MPI_FLOAT, &blocktype);
MPI_Type_commit(&blocktype);

MPI_Type_vector(nx,1,1, MPI_FLOAT, &linetype);
MPI_Type_commit(&linetype);

MPI_Type_vector(ny+2,1,nx+2, MPI_FLOAT, &coltype);
MPI_Type_commit(&coltype);

MPI_Cart_shift(cart_comm,0,1,&up,&down);
MPI_Cart_shift(cart_comm,1,1,&left,&right);

MPI_Sendrecv(gblock+nx+1, 1, coltype, right, 234, gblock+nx, 1, coltype, left, 234, cart_comm, &status);
MPI_Sendrecv(gblock+2*nx-2, 1, coltype, left, 345, gblock+2*nx-1, 1, coltype, right, 345, cart_comm, &status);

MPI_Sendrecv(gblock+(ny-2)*nx, 1, linetype, down, 456, gblock, 1, linetype, up, 456, cart_comm, &status);
MPI_Sendrecv(gblock+nx, 1, linetype, up, 567, gblock+(ny-1)*nx, 1, linetype, down, 567, cart_comm, &status);

if(!my_rank)
    for(i=0;i<ny;i++)
        for(j=0;j<nx;j++)
            *(mastergblock+i*XSIZE+j)=*(gblock+i*nx+j);
        
if (my_rank)
{
    MPI_Send(gblock, nx*ny, MPI_FLOAT, 0, 123, cart_comm);
}
else
{
    for(i=1;i<nproc;i++)
    {
        MPI_Cart_coords(cart_comm, i, NDIM, coords);//determina coordonatele procesului curent in topologie
        MPI_Recv(mastergblock+nx*coords[0]+coords[1]*ny*XSIZE,1,blocktype,i,123,cart_comm,&status);
    }

strcpy(fname,"chessy.pgm");
pgm_write(fname,mastergblock,YSIZE,XSIZE);
free(mastergblock);
}

free(gblock);
MPI_Finalize();
return 0;
}
