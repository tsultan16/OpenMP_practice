/* Parallel Fibonnaci using tasks construct. Very inefficient as n gets larger */

#include <stdio.h>
#include <omp.h>

int count, f0, f1, fn;

/* this function returns the nth number in the fibonacci sequence*/
int fib(int n)
{
    
    int x, y;
    
    if(n <= 2) 
    {
        fn = f0;
    }        
    else
    {     
        #pragma omp task shared(x) 
        x = fib(n-1);
        #pragma omp task shared(y) 
        y = fib(n-2);
        
        #pragma omp taskwait /* barrier before x,y are summed*/
        
        fn = x + y;
    }                

    return fn;

}

int main()
{
    int n;
    
    count = 0;
    f0 = 1;
    fn = 0;    
    
    printf("Enter n: \n");
    scanf("%d",&n);
    
    #pragma omp parallel
    {
        #pragma omp master
        printf("fn = %d \n",fib(n));
    }

} 