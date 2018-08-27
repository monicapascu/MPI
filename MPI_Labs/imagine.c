#include <stdio.h>
#include <stdlib.h>
#include "pgm_IO.h"
#include "pgm_IO.c"


int main(int argc, char ** argv)
{
    int nx,ny,i,j,niter,crs;
    float *data,*new,*old,*edge;
    FILE *fp;
    char fname[64];
    
    if(argc != 3)
    {
        printf("Utilizare: %s fisier.pgm NITER \n", argv[0]);
        printf("NITER: nr de iteratii ale algoritmului de filtrare \n\n");
        return 1;
    }
    
    niter = atoi(argv[2]);
    
    pgm_size(argv[1],&nx,&ny);
    
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
    
    for(i=0 ; i<nx+2; i++)
    {
        *(old + i*(ny+2)+j) = 255;
        *(new + i*(ny+2)+j) = 255;
        *(edge + i*(ny+2)+j) = 255;
    }
    
    
    pgm_read(argv[1],data,nx,ny);
    
    for(i=1; i< nx+1; i++)
        for(j=1; j< ny+1; j++)
        *(edge + i*(ny+2) + j) = *(data+(i-1)*ny + j-1);
    
    for(crs=0; crs<niter; crs++)
    {
    
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
        
  sprintf(fname, "poza_%d_iter.pgm", niter);
  pgm_write(fname, data,nx,ny);
  free(data); free(old); free(new); free(edge);
  return 0;
}

    
    

    
    
