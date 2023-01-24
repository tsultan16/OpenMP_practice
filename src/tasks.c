/* Example of simple race condition that can arise when using the tasks construct */

#include <stdio.h>
#include <omp.h>


int main()
{

char str[12]="";

printf("I think");

/*begin parallel construct*/
#pragma omp parallel
{
    #pragma omp single
    {
        #pragma omp task
        {
            strncat(str," race",5);
        }
    
        #pragma omp task
        {
            strncat(str," car",5);
        }
    }/*end of single construct*/

}/*end of parallel construct*/
 
printf("%ss are fun. \n",str);
 
}

