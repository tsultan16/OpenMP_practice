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

int threads_request, nsteps=0;
double pi, t1, t2 = 0.0;
double step;

printf("Enter how many threads would you like to request... \n");
scanf("%d",&threads_request);

/* request for threads*/
omp_set_num_threads(threads_request);

t1 = omp_get_wtime();


/* set integral step size */
step  = 1.0/(double)num_steps;

/* start openmp parallel construct*/
#pragma omp parallel
{
    int i, my_ID, nthreads, partial_num_steps;
    double x, partial_sum = 0.0;
    
     /* get my thread ID and total number of threads */
    my_ID = omp_get_thread_num();
    nthreads = omp_get_num_threads();

    /* Find out how many steps this thread needs to compute. 
       (The sum is block distributed)
    */
    if(my_ID != nthreads-1) 
    {
        partial_num_steps = num_steps/nthreads; 
    }
    else
    {
        partial_num_steps = num_steps - (nthreads-1)*(num_steps/nthreads); 
    }

    nsteps = nsteps + partial_num_steps;

    /* compute partial sum of block assigned to this thread*/
    for(i = my_ID*(num_steps/nthreads) ;i < my_ID*(num_steps/nthreads)+partial_num_steps; i++)
    {

        x = (i+0.5)*step;
        partial_sum = partial_sum + 4.0/(1.0+x*x);
    
    }

    printf("My ID = %d, Partial steps = %d, Partial Sum = %f \n",my_ID,partial_num_steps,partial_sum*step);

    /* Accumulate partial sum computed by this thread in the pi variable
       This portion is put inside an atomic construct to prevent conflict
       among threads updating the shared variable, preventing "false sharing".
    */
    #pragma omp atomic
        pi = pi + partial_sum * step;

}
/* end of openmp parallel construct*/

t2 = omp_get_wtime();

printf("Pi = %f \n",pi);
printf("nsteps = %d \n",nsteps);
printf("Time Elapsed(sec)=%f \n",t2-t1);

}
