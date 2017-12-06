/************************************************************
 ** File: diffusion_recharge_main_1D.c
 ** Author: Gregory Handy
 ** Date: 12/05/17
 ** Email: handy@math.utah.edu
 **
 ** This file contains the main program for a 1D simulation
 ** on the interval [0, 1] with an absorbing boundary at 
 ** x = 0 and a recharging receptor at x = L_x. 
 ** Fixes the recharge rate and loops through different initial
 ** number of particles. 
 ** Creates a file that records the number of captured particles
 ** for each trial (first line contains parameter values).
 **
 ** This file makes use of functions found in a separately
 ** compiled object file.
 ** To compile: gcc diffusion_recharge_main_1D.c diffusion_recharge_1D.c -o file.out
 ************************************************************/

#include <stdio.h>
#include <math.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include "diffusion_recharge_1D.h"

int main()
{
	
	double *x;           // position of the molecules 
	int *absorbed;       // keeps track whether the particle has left the domain
	double current_time;
	int count;           // keeps track of the total number of particles that have left the domain
	double dt;
	double k_constant;   // variance of the brownian noise
	double switch_time;  // time that the receptor should switch
	int num_activated[NUM_TRIALS];  // total number of receptor activations
	int j,h;
	double tau_r;         // mean recharge time
	double D, L_x;
	double *decay_time;
	int N[6],outer_count;
		
	double start_loc=0.5;   // starting location
	
	// initial number of molecule in each simulation
	N[0] = 100;
	N[1] = 250;
	N[2] = 500;
	N[3] = 1000;
	N[4] = 5000;
	N[5] = 10000;	
	
	// mean recharge time
	tau_r = 0.02;
	dt = 0.0001;              // time step
	
	// diffusion and length of interval
	D = 1;
	L_x = 1;
	
	// variance of brownian noise
	k_constant = sqrt(2*D*dt);
	
	// loop for parameter value
	for(outer_count=0;outer_count < 6; outer_count++){	
		
		// make file name
		char str_number1[10];
		sprintf(str_number1, "%d_", N[outer_count]);
		char str_number2[10];
		sprintf(str_number2, "%.2f", tau_r);
		char str1[50] = "num_activated_";
		char str_end[5] = ".dat";
		strcat(str1, str_number1);
		strcat(str1, str_number2);
		strcat(str1, str_end);
		FILE *outputFile1 = fopen(str1, "w");
		
		// seed random number generator
		srand ( time(NULL) );
	
		// file header
		fprintf(outputFile1, "tau_r=%f, N=%d, Num_Trials=%d, Start_loc =%f, dt=%f", tau_r,N[outer_count],NUM_TRIALS,start_loc,dt);
		fprintf(outputFile1, "\n");
		
		
		// allocate memory
		x = (double *)malloc(sizeof(double)*N[outer_count]);  
		absorbed = (int *)malloc(sizeof(int)*N[outer_count]);
		decay_time = (double *)malloc(sizeof(int)*N[outer_count]);
		
		// loop for each trial
		for(h=0;h<NUM_TRIALS;h++){
			
			// trial initialization
			num_activated[h]=0;
			count = 0;
			switch_time=-10;
			current_time = dt;
		
			// all particles start at the same point
			for(j=0;j<N[outer_count];j++){
				x[j]=start_loc;
				absorbed[j] = 0;
			}
			
			
			// main loop
			while(count < N[outer_count] || current_time < switch_time){

				// update particle movement
				for(j=0;j<N[outer_count];j++){
					
					// do nothing if the particle has already left the domain
					if(absorbed[j] ==0){

						// find the next point via brownian noise
						x[j] = x[j]+k_constant*randn(0,1);

						//particle hits open receptor and is removed from the system
						if(x[j]>=L_x && current_time > switch_time){

							absorbed[j]=1;
							count= count + 1;

							num_activated[h] = num_activated[h] + 1;

							switch_time = current_time + -log(((double) rand () / RAND_MAX))*tau_r;
						}
						//particle hits closed receptor and is reflected
						else if(x[j]>L_x){
							x[j] = 2*L_x-x[j];
						}
						//particle hits absorbing boundary and is removed from the system
						else if(x[j]<0)
						{
							absorbed[j]=1;
							count= count + 1;
						}
						
					}

					// if all particles have left the domain, end trial
					if(count == N[outer_count]){
						j = N[outer_count]+1;
					}

				}

				current_time = current_time + dt;
			}
 		}

		//print out the number activated in each trial
		for(h=0;h<NUM_TRIALS;h++){
			fprintf(outputFile1, "%d\n", num_activated[h]);
		}

		fclose(outputFile1);
	}

	// bell sound at end of simulation
	printf("\a");
    
    return 0;
}
