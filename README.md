# SerialBench

This is small benchmark program that does a numerical integral
(which can be analytically integrated) that is used in many
quantum chemistry programs.  This integral was selected because
it requires an inner loop if statement to handle the parametric
singularity.  This makes it difficult for the compiler
to utilize vector instructions (sse, avx, avx-512, etc).

When this was written and tested, GCC compiler 8.x could not 
vectorize it.  Intel compiler 19.x and Portland Group 19.x 
could vectorize it.

Thus the benchmark program can be easily compiled with optimizations
for various chip architectures, but it will only measure the 
performance of the serial instructions for that architecture.

This can be compiled as a single threaded program or with 
OpenMP parallelization.

WARNING:  The OpenMP pragma in the current version works correctly
with the Intel compiler, but not the GCC or PGI compilers.

SerialBench is a program for numerical integration of GTO primitive functions
It does the integral <1s1s|1/r12|1s1s> where all primitives are on the
same center at the origin.
This is a 6 dimensional integral
This integral has a parametric singularity at x1==x2, y1==y2, z1==z2
This is a simple rieman sum integral which sums points right up to the
singularity, but not at the singularity.

Edit the Makefile to use the desired compiler and compile flags.
If the compile flag for OpenMP support is not included, there
will be a harmless warning about not recognizing the OpenMP pragma
at compile time.

Compile with the command "make".  This should create an executable
named "sb".  

Running "sb" gives help info.

sb requires a single argument, which is the number of integration
steps in each of the six dimensions.  Thus sb can run with 

  sb 10
  sb 20
  sb 80

**WARNING:**  This scales as N^6 so doubling the number of steps
means it will take 64 times as long to run.

**NOTE:**  For best efficiency with OpenMP, the number of points
should be a multiple of OMP_NUM_THREADS

A simple script to run and time it might look like this

#!/bin/bash
export OMP_NUM_THREADS=20
start_time=$(date '+%s')
./sb 40
end_time=$(date '+%s')
elapsed_time=$((end_time - start_time))
sec=$((elapsed_time % 60))
min=$(((elapsed_time / 60) % 60))
hr=$((elapsed_time / 3600))
printf '%d:%02d:%02d' $hr $min $sec
echo

This is just a great quote, so we are including it.
"Numerical analysis is partially a science and partially an art."
from "Handbook of Mathematical Functions", eds M. Abramowitz, and 
I.A. Stegun, Dover Press, New York, 1970

