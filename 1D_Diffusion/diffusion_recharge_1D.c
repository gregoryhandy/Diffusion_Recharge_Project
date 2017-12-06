/************************************************
 ** File: diffusion_recharge_1D.c
 ** Author: Gregory Handy
 ** Date: 12/05/17
 ** Email: handy@math.utah.edu
 **
 ** This file contains all the necessary functions
 ** for the diffusion_recharge_main_1D.c
 **
 ** This file is meant to be compiled separately
 ** from the main program.
 **
 ************************************************/

#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include "diffusion_recharge_1D.h"

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




