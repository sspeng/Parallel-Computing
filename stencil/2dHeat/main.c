#include <stdio.h>
#include <stdlib.h>
#include "util.h"
#include "cycle.h"
#include "common.h"
#include "heat-blocked.h"
#include "heat.h"

#define NUM_TRIALS 2
#define CUTOFF 4096

int main(int argc,char *argv[])
{
	double *Anext;
  double *A0;
  int nx;
  int ny;
  int tx;
  int ty;
  int timesteps;
  int i;

  #ifdef DEBUG
  nx = 8192;
  ny = 8192;
  tx = 1024;
  ty = 1024;
  timesteps = 100;
  #else
  nx = atoi(argv[1]);
  ny = atoi(argv[2]);
  tx = atoi(argv[3]);
  ty = atoi(argv[4]);
  timesteps = atoi(argv[5]);
  #endif
  
  ticks t1, t2;
  double spt;

  /* find conversion factor from ticks to seconds */
  spt = seconds_per_tick();

  /* allocate arrays */ 
  Anext=(double*)malloc(sizeof(double)*nx*ny);
  A0=(double*)malloc(sizeof(double)*nx*ny);
  
  printf("USING TIMER: %s \t  SECONDS PER TICK:%g \n", TIMER_DESC, spt);

	for (i=0;i<NUM_TRIALS;i++) 
	{
   	// clear_cache();

   	/* Initialise Arrays */
    StencilInit(nx,ny,Anext);
    StencilInit(nx,ny,A0);
 
    t1 = getticks();	
    
    /* stencil function */ 
    #ifdef HEAT
    heat(A0, Anext, nx, ny, timesteps);
    #endif

    #ifdef HEATBLOCK
    heatBlocked(A0, Anext, nx, ny, tx, ty, timesteps);
  	#endif
    t2 = getticks();
    
    printf("elapsed ticks: %g  time:%g \n", elapsed(t2, t1), spt * elapsed(t2,t1));
  }
  double total = 0.0;
  for (int j = 0; j < ny; j++) {
    for (int i = 0; i < nx; i++) {
      total += A0[Index2D (nx, i, j)];
    }
  }
  printf("Checksum: %f\n", total);
}

