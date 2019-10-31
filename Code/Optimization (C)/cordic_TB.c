#include <stdio.h>
#include <math.h> //atan&pow functions


int main() {

  double x, y, z;
  int x_in, y_in, z_in, i;

  x=1.0;
  y=1.0;
  z=atan(y/z) * (1<< 15);



  for(i=0;i<16;i++){
    printf( " %i \n", (int)(atan (pow( 2.0, (double)(-i))) * (1 << 15)));
  }
  return 0;
}
