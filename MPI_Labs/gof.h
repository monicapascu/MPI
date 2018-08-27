#ifndef _GOF_H_
#define _GOF_H_

#define XSIZE 128 //NUMARUL DE COLOANE DE PIXELI
#define YSIZE 128 //NUMARUL DE LINII DE PIXELI
#define GSIZE XSIZE*YSIZE
#define MAXGREY 255

//pt definirea topologiei virtuale bidimensionale

#define NDIM 2
#define TRUE 1
#define FALSE 0

//pt definirea tipurilor de date derivate

#define ITER 30

typedef struct _XYTYPE_ {
    int x;
    int y;
}xytype;

void pgm_size (char *filename, int *nx, int *ny);
void pgm_read (char *filename, void *vx, int nx, int ny);
void pgm_write(char *filename, void *vx, int nx, int ny);

//se scrie numai deasupra acestei linii
#endif
