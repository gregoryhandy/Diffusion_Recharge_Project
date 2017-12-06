/************************************************************
 ** File: diffusion_recharge_main_2D.c
 ** Author: Gregory Handy
 ** Date: 12/05/17
 ** Email: handy@math.utah.edu
 **
 ** This file contains the main program for a 2D simulation
 ** on the rectangle [0,L_x] x [0,L_y] with an absorbing boundary at 
 ** x=0 and x=L_x, a reflecting boundary at y=L_y, and receptors
 ** along y=0. Receptors lie next to each other.
 **
 ** Fixes the initial number of particles and and loops through 
 ** different recharge rates
 **
 ** Creates a file that records the number of captured particles
 ** for each trial.
 **
 ** This file makes use of functions found in a separately
 ** compiled object file.
 ** To compile: gcc diffusion_recharge_main_2D.c diffusion_recharge_2D.c -o file.out
 ************************************************************/

#include <stdio.h>
#include <math.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include "diffusion_recharge_2D.h"

int main()
{
	
	double *x,*y;             // position of the molecules
	int *absorbed;            // keeps track whether the particle has left the domain
	double current_time;
	int count;                // keeps track of the total number of particles that have left the domain
	double dt;
	double k_constant;        // variance of the brownian noise
	double L_y, L_x;		  // length scales
	int num_activated;        // total number of receptor activations
	double switch_time[M];
	double rec_start[M], rec_end[M], rec_total, rec_individual;   // receptor parameters
	int i,j,h,rec_loop,outer_count;
	double x_start_loc,y_start_loc;   // starting location
	double D;           // diffusion coefficient and 
	double tau_r[9];
	
	int N=100;        // number of initial particles
	
	// mean recharge time
	tau_r[0] = 0.1;
	tau_r[1] = 0.31;
	tau_r[2] = 1;
	tau_r[3] = 3.1;
	tau_r[4] = 10;
	tau_r[5] = 31;
	tau_r[6] = 100;
	tau_r[7] = 200;
	tau_r[8] = 1000;
	
	// diffusion and length of interval
	D = 1;
	L_x = 1;
	L_y=0.1;
	
	// receptor starts in the middle of the x direction
	x_start_loc = L_x*0.05;
	y_start_loc = L_y;
	
    // time step
	dt = 0.00001;  
	
	// variance of brownian noise
	k_constant = sqrt(2*D*dt);
	
	// calculate the receptors' locations
	rec_total=0.5; // total width of ALL receptors
	rec_individual=rec_total/M; // width of individual receptors
	for(i=0;i<M;i++){
		rec_start[i] = 0.5-rec_total/2+rec_individual*i;
		rec_end[i] = 0.5-rec_total/2+rec_individual*(i+1);
	}
	
	// print out receptor locations to a file	
	print_rec(L_y, rec_individual, rec_start, rec_end);
	
	// seed random number generator
	srand ( time(NULL) );
	
	// loop for parameter value
	for(outer_count=0;outer_count < 9; outer_count++){	
		
		// allocate memory
		x = (double *)malloc(sizeof(double)*N);  
		y = (double *)malloc(sizeof(double)*N); 
		absorbed = (int *)malloc(sizeof(int)*N);
		
		// // make file for output data (header included)
		FILE* outputFile1 = create_output_file_2D(tau_r[outer_count],N,x_start_loc,y_start_loc,L_y,dt,rec_individual);
		
		// loop for each trial	
		for(h=0;h<NUM_TRIALS;h++){
			
			// trial initialization
			num_activated = 0;
			for(j=0;j<M;j++){
				switch_time[j]=-10;
			}
			count = 0;
			current_time = dt;
		
			// all particles start at the same point
			for(j=0;j<N;j++){
				x[j]=x_start_loc;
				y[j]=y_start_loc;
				absorbed[j] = 0;
			}
			
			// main loop
			while(count < N){
						
				// update particle movement  
				for(j=0;j<N;j++){
					if(absorbed[j] ==0){
						// find the next point via brownian noise
						x[j] = x[j]+k_constant*randn(0,1);
						y[j] = y[j]+k_constant*randn(0,1);
				
						// particle potentially hit a receptor
						if(y[j] <=0){
							for(rec_loop=0;rec_loop<M;rec_loop++)
							{
						        if(x[j]>(rec_start[rec_loop]) 
								&& x[j]<(rec_end[rec_loop]) 
								&& current_time>switch_time[rec_loop])
								{
									switch_time[rec_loop] = current_time + -log(((double) rand () / RAND_MAX))*tau_r[outer_count];
						        	num_activated = num_activated+1;
									count= count + 1;
									absorbed[j]=1;
									rec_loop=M+1;
						        }
							}
							// Receptor was not hit if rec_loop==M so reflect point
							if(rec_loop==M)
							{	
								 y[j] = -y[j];
							}		
						}
						// particle hit upper reflecting boundary 
						else if(y[j]>=L_y){
					        y[j] = 2*L_y-y[j];
						}
						// particle exits the domain
						else if(x[j]<=0 || x[j] >= 1){
							absorbed[j]=1;
							count= count + 1;
						}
									
					}
										
					// if all particles are removed, end trial
					if(count == N){
						j = N+1;	
					}
				}
			
				current_time = current_time + dt;	
			}
			
			// print out the number activated and duration activated in each trial
			fprintf(outputFile1, "%d \n", num_activated);

		}
	
		fclose(outputFile1);
	}

	// bell sound at end of simulation
	printf("\a");
    
    return 0;
}
