/************************************************
 ** File: diffusion_recharge_3D.c
 ** Author: Gregory Handy
 ** Date: 12/05/17
 ** Email: handy@math.utah.edu
 **
 ** This file contains all the necessary functions
 ** for the diffusion_recharge_main_3D.c
 **
 ** This file is meant to be compiled separately
 ** from the main program.
 **
 ************************************************/

#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include "diffusion_recharge_3D.h"


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

// reads in receptor locations
double read_rec_3D(double *rec_center_x, double *rec_center_y)
{
	double rec_rad; 
	
	// Read in the receptor locations
	FILE *myFile;
	myFile = fopen("rec_loc_1.txt", "r");
	//read file into array
	fscanf(myFile, "%lf", &rec_rad);
	for (int rec_loop = 0; rec_loop < M; rec_loop++)
	{
	  fscanf(myFile, "%lf %lf", &rec_center_x[rec_loop],&rec_center_y[rec_loop]);
	}
	
	return rec_rad;
}



// print out receptor locations to a file
int print_rec_3D(double L_z, double rec_rad, double rec_center_x[], double rec_center_y[])
{
	
	char rec_file[60];
	sprintf(rec_file, "rec_locations_check_%.2f.dat", L_z);
	
	FILE *locationFile1 = fopen(rec_file, "w");
	fprintf(locationFile1, "Receptor Radius=%.3f \n",rec_rad);
  	for(int rec_loop=0;rec_loop<M;rec_loop++){
    	fprintf(locationFile1, "X Center=%.3f, Y Center=%.3f \n",rec_center_x[rec_loop],rec_center_y[rec_loop]);
  	}
	fclose(locationFile1);

	return 0;
}


// creates and prints header for files
FILE* create_output_file_3D(double tau_r, int N, double x_start_loc, double y_start_loc, double L_z, double dt, double rec_rad)
{
	
	// make file name
	char filename[60];
	sprintf(filename, "num_act_3D_%d_%.2f_%.2f.dat", N,L_z,tau_r);
	FILE* outputFile1 = fopen(filename, "w");
	
	// file header
	fprintf(outputFile1, "tau_r=%.3f, N=%d, Num. Trials=%d, Start loc. X =%.3f, Start loc. Y =%.3f, L_z=%.3f \n"
	"dt=%f, Number of Receptors=%d, Receptor Width=%.3f",
		tau_r,N,NUM_TRIALS,x_start_loc,y_start_loc,L_z,dt,M,rec_rad);
	fprintf(outputFile1, "\n");
		
	return outputFile1;
}


