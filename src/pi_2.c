/*
   Example of thread-parallelized numerical integration to obtain value of pi. The total sum
   is equally divided among all the threads. Each thread computes part of the sum. This partial 
   sum is accumulated in the shared variable pi.
*/


#include <stdio.h>
#include <omp.h>

/* global variables */
static long num_steps = 1000000000;

int main()
{

int i, threads_request;
double pi, sum = 0.0, t1, t2;
double step;

printf("Enter how many threads would you like to request... \n");
scanf("%d",&threads_request);

t1 = omp_get_wtime();

/* set integral step size */
step  = 1.0/(double)num_steps;    

/* request for threads*/
omp_set_num_threads(threads_request);

/* start openmp parallel construct*/
#pragma omp parallel
{

    double x; /* thread local x to prevent race condition inside loop*/

    /* Divide loop iterations among threads using the "omp for' workshare construct. 
       Apply reduction clause to add up the partial sums from all threads
    */ 
    #pragma omp for reduction(+:sum) schedule(static)
    for(i = 0 ; i < num_steps; i++)
    {
        x = (i+0.5) * step;
        sum = sum + 4.0/(1.0+x*x);
    } 

    #pragma omp single nowait
    {
        pi = sum * step;  /* only one thread needs to do this, hence the single workshare construct*/
    } 
    /* nowait clause gets rid of implied barrier at the end of this single construct to free up all other threads*/

}
/* end of openmp parallel construct*/



t2 = omp_get_wtime();

printf("Pi = %f \n",pi);
printf("Time Elapsed(sec)=%f \n",t2-t1);

}
