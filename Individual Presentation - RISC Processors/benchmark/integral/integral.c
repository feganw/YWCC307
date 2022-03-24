#include <stdio.h>
#include <math.h>
#include <time.h>

int main(){
  double sum = 0, delta = 0.025, x = -5;
  double y, z;
  int i, j,k;
  clock_t t_start, t_stop;
  
  printf("-----\n* C *\n-----\n");
  printf("Approximation of a triple integral over a ball.\n");
  
  t_start = clock();
 
  for(i=0; i <400; i++)
   {x += delta;
   y = -5;
   for(j=0; j<400; j++)
     {y += delta;
     z=-5;
     for(k=0;k<400;k++)
       {z += delta;
       if(x*x+y*y+z*z<25)
	 sum += exp(-x*x)*cos(y*y)*cos(z*z);}}}
  sum=sum*delta*delta*delta;
  t_stop = clock();
 printf("integral = %f10\n", sum);
 printf("elapsed time: %f10 (s)\n", ((double) t_stop - t_start) / CLOCKS_PER_SEC);
 return 0;
}