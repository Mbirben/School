#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define SHIFTER 32768
#define TABLE_SIZE 15

typedef struct vector3{
    int x;
    int y;
    int z;

}*VECTOR,vec;

int elementary_angle[TABLE_SIZE]={  25735,
                                     15192,
                                     8027,
                                     4074,
                                     2045,
                                     1023,
                                     511,
                                     255,
                                     127,
                                     63,
                                     31,
                                     15,
                                     7,
                                     3,
                                     1
                                            };


void cordic_vector(VECTOR v){
      int x_in,y_in,z_in;
      int x_out,y_out;
      int i;

      x_in=(double)(v->x << 15);
      y_in=(double)(v->y << 15);
      z_in= 0;



      for(i=0;i<15;i++){

          if(y_in>0){
              x_out = x_in + (y_in >> i);
              y_out = y_in - (x_in >> i);
              z_in += elementary_angle[i];
          }else{
              x_out = x_in - (y_in >> i);
              y_out = y_in + (x_in >> i);
              z_in -= elementary_angle[i];
          }
          x_in=x_out;
          y_in=y_out;
      }

      v->x = x_in;
      v->y = y_in;
      v->z = z_in;

}

void cordic_rotate(VECTOR v){
      int x_in,y_in,z_in;
      int x_out,y_out;
      int i;


      x_in=(double)(v->x << 15);
      y_in=(double)(v->y << 15);
      z_in=v->z;


      for(i=0;i<15;i++){

          if(z_in<0){
            x_out = x_in + (y_in >> i);
            y_out = y_in - (x_in >> i);
            z_in += elementary_angle[i];
          }else{
            x_out = x_in - (y_in >> i);
            y_out = y_in + (x_in >> i);
            z_in -= elementary_angle[i];
        }
        x_in=x_out;
        y_in=y_out;
    }

    v->x = x_in;
    v->y = y_in;
    v->z = z_in;

}

//rotate
void cordic_cossin(double theta){
    double x_out,y_out;
    int z_in;

    z_in=theta * (double)(1<<15);



    vec v={1,0,z_in};
    cordic_rotate(&v);
    x_out=(v.x/(double)(1<<15))/1.6476025812107;
    y_out=(v.y/(double)(1<<15))/1.6476025812107;


    //printf("REAL SOLUTION: cos(%f): %f \t sin(%f): %f \n",theta,cos(theta),theta,sin(theta));
    //printf("MY SOLUTION:   cos(%f): %f \t sin(%f): %f \n\n",theta,x_out,theta, y_out );
}



void cordic_arctan(int x, int y){
    double z_out,z_out_d;

    vec v={x,y,0};
    cordic_vector(&v);
    z_out= v.z/(double)(1<<15);
    z_out= z_out * (180/M_PI);

    //printf("\nREAL SOLUTION: atan(%d/%d): %f\n",y,x,atan((double)y/x)*(180/M_PI));
    //printf("MY SOLUTION:   Arctan(%d,%d): %f in degrees\n\n\n",x,y,z_out);

}


int main(){

    int i;
    for(i=0;i<1000000;i++){
        cordic_cossin(1.43);
        cordic_cossin(1.14);
        cordic_cossin(0.101);
        cordic_cossin(0.72);
        cordic_cossin(0.96);
        cordic_cossin(1);
        cordic_cossin(1.712);
        cordic_cossin(0.985);
        cordic_cossin(1.456);
        cordic_cossin(0.92);

        cordic_arctan(3,2);
        cordic_arctan(1,6);
        cordic_arctan(6,5);
        cordic_arctan(3,4);
        cordic_arctan(4,6);
        cordic_arctan(7,2);
        cordic_arctan(2,1);
        cordic_arctan(1,6);
        cordic_arctan(3,9);
        cordic_arctan(5,10);

    }


    return 0;
}
