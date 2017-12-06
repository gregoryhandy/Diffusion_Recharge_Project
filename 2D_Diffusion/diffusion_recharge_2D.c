/************************************************
 ** File: diffusion_recharge_2D.c
 ** Author: Gregory Handy
 ** Date: 12/05/17
 ** Email: handy@math.utah.edu
 **
 ** This file contains all the necessary functions
 ** for the diffusion_recharge_main_2D.c
 **
 ** This file is meant to be compiled separately
 ** from the main program.
 **
 ************************************************/

#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include "diffusion_recharge_2D.h"

/* returns a pseudorandom value drawn from the standard normal distribution */
double randn (double mu, double sigma)
{
  double U1, U2, W, mult;
  static double X1, X2;
  static int call = 0;
 
  if (call == 1)
    {
      call = !call;
      return (mu + sigma * (double) X2);
    }
 
  do
    {
      U1 = -1 + ((double) rand () / RAND_MAX) * 2;
      U2 = -1 + ((double) rand () / RAND_MAX) * 2;
      W = pow (U1, 2) + pow (U2, 2);
    }
  while (W >= 1 || W == 0);
 
  mult = sqrt ((-2 * log (W)) / W);
  X1 = U1 * mult;
  X2 = U2 * mult;
 
  call = !call;
 
  return (mu + sigma * (double) X1);
}

// print out receptor locations to a file
int print_rec(double L_y, double rec_individual, double rec_start[], double rec_end[])
{
	char rec_file[60];
	sprintf(rec_file, "rec_locations_check_%.2f.dat", L_y);
	FILE *locationFile1 = fopen(rec_file, "w");
	fprintf(locationFile1, "Receptor Width=%.3f\n",rec_individual);
	for(int rec_loop=0;rec_loop<M;rec_loop++)
	{
		fprintf(locationFile1, "Receptor %d: (%.3f, %.3f) \n",rec_loop,rec_start[rec_loop],rec_end[rec_loop]);
	}
	fclose(locationFile1);

	return 0;
}

// creates and prints header for files
FILE* create_output_file_2D(double tau_r, int N, double x_start_loc, double y_start_loc, double L_y, double dt, double rec_individual)
{
	
	// make file name
	char filename[60];
	sprintf(filename, "num_act_2D_%d_%.2f_%d_%.2f.dat", N,L_y,M,tau_r);
	FILE* outputFile1 = fopen(filename, "w");
	
	// file header
	fprintf(outputFile1, "tau_r=%.3f, N=%d, Num. Trials=%d, Start loc. X =%.3f, Start loc. Y =%.3f, L_y=%.3f \n"
	"dt=%f, Number of Receptors=%d, Receptor Width=%.3f",
		tau_r,N, NUM_TRIALS,x_start_loc,y_start_loc,L_y,dt,M,rec_individual);
	fprintf(outputFile1, "\n");		
	
	return outputFile1;
}


