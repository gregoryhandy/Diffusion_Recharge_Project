/************************************************************
 ** File: diffusion_recharge_main_3D.c
 ** Author: Gregory Handy
 ** Date: 12/05/17
 ** Email: handy@math.utah.edu
 **
 ** This file contains the main program for a 3D simulation
 ** on the cylinder [0,R] x [0,L_z] with an absorbing boundary at 
 ** x^2+y^2 = R^2, a reflecting boundary at z=L_z, and receptors
 ** along z=0.
 ** Fixes the initial number of particles and and loops through 
 ** different recharge rates
 ** Creates a file that records the number of captured particles
 ** for each trial.
 **
 ** This file makes use of functions found in a separately
 ** compiled object file.
 ** To compile: gcc diffusion_recharge_main_3D.c diffusion_recharge_3D.c -o file.out
 ************************************************************/

#include <stdio.h>
#include <math.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include "diffusion_recharge_3D.h"

int main()
{
	
	double *x,*y,*z;                  // position of the molecules 
	int *absorbed;                    // keeps track whether the particle has left the domain
	double current_time;
	int count;                        // keeps track of the total number of particles that have left the domain
	double dt;
	double k_constant;                // variance of the brownian noise
	double L_z, R;                    // height and radius of cylinder
	double D;
	int num_activated;                // total number of receptor activations
	double switch_time[M];
	int i,j,h,rec_loop;
	double tau_r[10];
	double x_start_loc,y_start_loc,z_start_loc;   // starting location
	
	int N=100,outer_count;
	
	// diffusion and size of cylinder
	D = 1;
	L_z = 0.25;
	R = 1;
	// receptor starts in the middle of the circle, and slightly off the reflecting boundary
	x_start_loc = 0;
	y_start_loc = 0;
	z_start_loc = L_z;
	
	// mean recharge rate
	tau_r[0] = 0.1;
	tau_r[1] = 0.31;
	tau_r[2] = 1;
	tau_r[3] = 3.1;
	tau_r[4] = 10;
	tau_r[5] = 31;
	tau_r[6] = 100;
	tau_r[7] = 200;
	tau_r[8] = 1000;
	tau_r[9] = 10000;
		
	
	// read in receptor locations
	double rec_center_x[M], rec_center_y[M];
	double rec_rad = read_rec_3D(rec_center_x,rec_center_y);

	// print out receptor locations to a file	
	print_rec_3D(L_z, rec_rad, rec_center_x, rec_center_y);
         
	dt = 0.00001;              // time step in seconds

	// loop for parameter value
	for(outer_count=0;outer_count < 10; outer_count++){	
		
		// allocate memory
		x = (double *)malloc(sizeof(double)*N);  
		y = (double *)malloc(sizeof(double)*N); 
		z = (double *)malloc(sizeof(double)*N);
		absorbed = (int *)malloc(sizeof(int)*N);
		
		// make file for output data (header included)
		FILE* outputFile1 = create_output_file_3D(tau_r[outer_count],N,x_start_loc,y_start_loc,L_z,dt,rec_rad);
	
		// seed random number generator
		srand ( time(NULL) );
	
		// variance of brownian noise
		k_constant = sqrt(2*D*dt);
			
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
				z[j]=z_start_loc;
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
						z[j] = z[j]+k_constant*randn(0,1);
						
						// particle potentially hit a receptor
						if(z[j] <=0){
							for(rec_loop=0;rec_loop<M;rec_loop++)
							{
						        if((pow(x[j]-rec_center_x[rec_loop], 2)+pow(y[j]-rec_center_y[rec_loop],2))<=pow(rec_rad, 2) 
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
								 z[j] = -z[j];
							}		
						}
						// particle hit upper reflecting boundary 
						else if(z[j]>=L_z){
					        z[j] = 2*L_z-z[j];
						}
						// particle exits the domain
						else if(pow(x[j], 2)+pow(y[j], 2)>=pow(R, 2)){
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
			
			// print out the number activated in each trial
			fprintf(outputFile1, "%d ", num_activated);
			fprintf(outputFile1, "\n");
		
		}
	
		fclose(outputFile1);
	}

	// bell sound at end of simulation
	printf("\a");
    
    return 0;
}
