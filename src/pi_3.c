/*
   Example of thread-parallelized numerical integration to obtain value of pi. The total sum
   is recursively divided among a team of threads ("divide and conquer"). At the base level of the recursion, 
   each thread computes the sum of MIN_BLK/2 terms.These partial sums are accumulated after all 
   threads are done with their task.
*/


#include <stdio.h>
#include <omp.h>

/* global */
static long num_steps = 1000000000;
#define MIN_BLK 100000


double partial_sum(int ilow, int ihi, double step)
{
    int i, iblk;
    double x, sum = 0.0, sum1 = 0.0, sum2 = 0.0;
    
    if(ihi-ilow > MIN_BLK)
    {    
        iblk = ihi-ilow;
        #pragma omp task shared(sum1) 
            sum1 = partial_sum(ilow,ihi-iblk/2,step);  
        #pragma omp task shared(sum2) 
            sum2 = partial_sum(ihi-iblk/2,ihi,step); 
        #pragma omp taskwait
            sum = sum1 + sum2  ;      
    }        
    else
    { 
        for(i = ilow ; i < ihi; i++)
        {
            x = (i+0.5) * step;
            sum = sum + 4.0/(1.0+x*x);
        } 
    }
    
    return sum;
    
}



int main()
{

int i;
double pi, sum, t1, t2;
double step;



t1 = omp_get_wtime();

/* set integral step size */
step  = 1.0/(double)num_steps;    


/* start openmp parallel construct*/
#pragma omp parallel
{

    /* have master create tasks recursively*/
    #pragma omp master  
        sum = partial_sum(0,num_steps-1,step);         


}
/* end of openmp parallel construct*/

pi = sum * step;

t2 = omp_get_wtime();

printf("Pi = %f \n",pi);
printf("Time Elapsed(sec)=%f \n",t2-t1);

}