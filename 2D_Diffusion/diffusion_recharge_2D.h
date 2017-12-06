/****************************************************
 ** File: diffusion_recharge_2D.h
 ** Author: Gregory Handy
 ** Date: 12/05/17
 ** Email: handy@math.utah.edu
 **
 ** This file contains the function prototypes
 ** for the functions whose definitions are
 ** found in diffusion_recharge_2D.c
 ****************************************************/

#define NUM_TRIALS 100			// number of trials
#define M 8						// number of receptors

// returns a pseudorandom value drawn from the standard normal distribution 
double randn (double mu, double sigma);

int print_rec(double L_y, double rec_individual, double rec_start[], double rec_end[]);

FILE* create_output_file_2D(double tau_r, int N, double x_start_loc, double y_start_loc, double L_y, double dt, double rec_individual);
