//procesul de rang 0 e procesul master. trimite a,b,c,N si Ni catre procesele de calcul.
//procesele de calcul calculeaza integrala aia si returneaza valoarea obtinuta catre procesul master 
//procesul master calculeaza o variabila medie si afiseaza
//f(x) = 5*sqrt(1-x^2)
// I = int f(x)dx = N1/N * (b-a)*c
//c= max(fx) pe intervalul [a,b]
//N: valori generate random (xi,yi)
// N1 valori pt care yi<=f(xi)
//Ni = N/nproc-1

#include <stdio.h>
#include <stdlib.h>
#include <gsl/gsl_rng.h>
#include <math.h>
#include <mpi.h>

#define MASTER 0
#define SLAVE 1
//#define N 1000000
#define MY_SUCCESS 0

double f(double x)
{
    double f = 5.*sqrt(1.-x*x);
    return f;
}

double IMC(double a,double b, double c, long N)
{
    long N1=0;
    double I,x,y,u;
    int i;
    const gsl_rng_type *T;
    gsl_rng *r;
    gsl_rng_env_setup();
    T = gsl_rng_default;
    r = gsl_rng_alloc (T);
    for (i = 0; i < N; i++)
    {
        u = gsl_rng_uniform (r);
        x = a + (b-a)*u;
        u = gsl_rng_uniform (r);
        y = c*u;
        if(y<=f(x)) N1++;
    }
    
    I= ((double)N1)/((double)N)*(b-a)*c;
    return I;
    
}

int main(int argc, char ** argv)
{
    
    int nproc,my_rank,i;   
    double a,b,c,I,val_I = 0.;
    long Ni,N=1000000;
    MPI_Comm my_comm=MPI_COMM_WORLD;
    MPI_Status status; 

    //Programarea paralela.
    MPI_Init(&argc,&argv);
    MPI_Comm_size(my_comm,&nproc);
    MPI_Comm_rank(my_comm,&my_rank);


    if(my_rank==MASTER)
    {
        a = 0.; b = 1.; c = 5.; I = 0.;
        Ni = (long)((double)N)/((double)(nproc-1));
        for(i = 1; i<nproc; i++)
        {
            MPI_Send(&a,1,MPI_DOUBLE,i,10,my_comm);
            MPI_Send(&b,1,MPI_DOUBLE,i,11,my_comm);
            MPI_Send(&c,1,MPI_DOUBLE,i,12,my_comm);
            MPI_Send(&Ni,1,MPI_LONG,i,13,my_comm);
        }
        
        for(i = 1; i<nproc; i++)
        {
            MPI_Recv(&I,1,MPI_DOUBLE,MPI_ANY_SOURCE,14,my_comm,&status);
            val_I = val_I+I;
        }
        
        val_I = val_I/(nproc-1);
        printf("Integrala = %.8lf \n\n", val_I);
        
        
    }
    else //my_rank==SLAVE
    {
       MPI_Recv(&a,1,MPI_DOUBLE,MASTER,10,my_comm,&status);
       MPI_Recv(&b,1,MPI_DOUBLE,MASTER,11,my_comm,&status);
       MPI_Recv(&c,1,MPI_DOUBLE,MASTER,12,my_comm,&status);
       MPI_Recv(&Ni,1,MPI_LONG,MASTER,13,my_comm,&status);
       I = IMC(a,b,c,Ni);
       MPI_Send(&I,1,MPI_DOUBLE,MASTER,14,my_comm);
       
     }
    
    MPI_Finalize();

    
    return MY_SUCCESS;
}
