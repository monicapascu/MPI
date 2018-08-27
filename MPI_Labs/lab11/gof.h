#ifndef _GOF_H_

#define _GOF_H_ 

//dimensiunile pe orizontala si pe verticala ale imaginii pgm si nr total de pixeli care compun imaginea
#define XSIZE 128 
#define YSIZE 128
#define GSIZE XSIZE*YSIZE

//pentru definirea topologiei virtuale bidimensionale: True si False sunt definite pt tipul de periodicitate
#define NDIM 2
#define TRUE 1
#define FALSE 0

//pentru definirea tipurilor de date derivate
typedef struct _XYTYPE_{
    int x;
    int y;
} xytype;

#define ITER 30
#define MAXGREY 100
void pgm_size (char *filename, int *nx, int *ny);
void pgm_read (char *filename, void *vx, int nx, int ny);
void pgm_write(char *filename, void *vx, int nx, int ny);

//se scrie numai deasupra acestei linii
#endif
