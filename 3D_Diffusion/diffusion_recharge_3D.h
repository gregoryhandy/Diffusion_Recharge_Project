/****************************************************
 ** File: diffusion_recharge_3D.h
 ** Author: Gregory Handy
 ** Date: 12/05/17
 ** Email: handy@math.utah.edu
 **
 ** This file contains the function prototypes
 ** for the functions whose definitions are
 ** found in diffusion_recharge_3D.c
 ****************************************************/

#define NUM_TRIALS 100			// number of trials
#define M 1						// number of receptors

// returns a pseudorandom value drawn from the standard normal distribution
double randn (double mu, double sigma);

// reads in receptor locations
double read_rec_3D(double *rec_center_x, double *rec_center_y);

// print out receptor locations to a file
int print_rec_3D(double L_z, double rec_rad, double rec_center_x[], double rec_center_y[]);

// creates and prints header for files
FILE* create_output_file_3D(double tau_r, int N, double x_start_loc, double y_start_loc, double L_y, double dt, double rec_rad);

