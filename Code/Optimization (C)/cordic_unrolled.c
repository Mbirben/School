#include <stdio.h>
#include <stdlib.h>
#include <math.h>


typedef struct vector3{
    int x;
    int y;
    int z;

}*VECTOR,vec;

int elementary_angle[15]={  25735,
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

      x_in=(double)(v->x << 15);
      y_in=(double)(v->y << 15);
      z_in= 0;


                if(y_in>0){
                    x_out = x_in + (y_in >> 0);
                    y_out = y_in - (x_in >> 0);
                    z_in += elementary_angle[0];
                }else{
                    x_out = x_in - (y_in >> 0);
                    y_out = y_in + (x_in >> 0);
                    z_in -= elementary_angle[0];
                }
                x_in=x_out;
                y_in=y_out;

                if(y_in>0){
                    x_out = x_in + (y_in >> 1);
                    y_out = y_in - (x_in >> 1);
                    z_in += elementary_angle[1];
                }else{
                    x_out = x_in - (y_in >> 1);
                    y_out = y_in + (x_in >> 1);
                    z_in -= elementary_angle[1];
                }
                x_in=x_out;
                y_in=y_out;

                if(y_in>0){
                    x_out = x_in + (y_in >> 2);
                    y_out = y_in - (x_in >> 2);
                    z_in += elementary_angle[2];
                }else{
                    x_out = x_in - (y_in >> 2);
                    y_out = y_in + (x_in >> 2);
                    z_in -= elementary_angle[2];
                }
                x_in=x_out;
                y_in=y_out;

                if(y_in>0){
                    x_out = x_in + (y_in >> 3);
                    y_out = y_in - (x_in >> 3);
                    z_in += elementary_angle[3];
                }else{
                    x_out = x_in - (y_in >> 3);
                    y_out = y_in + (x_in >> 3);
                    z_in -= elementary_angle[3];
                }
                x_in=x_out;
                y_in=y_out;

                if(y_in>0){
                    x_out = x_in + (y_in >> 4);
                    y_out = y_in - (x_in >> 4);
                    z_in += elementary_angle[4];
                }else{
                    x_out = x_in - (y_in >> 4);
                    y_out = y_in + (x_in >> 4);
                    z_in -= elementary_angle[4];
                }
                x_in=x_out;
                y_in=y_out;

                if(y_in>0){
                    x_out = x_in + (y_in >> 5);
                    y_out = y_in - (x_in >> 5);
                    z_in += elementary_angle[5];
                }else{
                    x_out = x_in - (y_in >> 5);
                    y_out = y_in + (x_in >> 5);
                    z_in -= elementary_angle[5];
                }
                x_in=x_out;
                y_in=y_out;

                if(y_in>0){
                    x_out = x_in + (y_in >> 6);
                    y_out = y_in - (x_in >> 6);
                    z_in += elementary_angle[6];
                }else{
                    x_out = x_in - (y_in >> 6);
                    y_out = y_in + (x_in >> 6);
                    z_in -= elementary_angle[6];
                }
                x_in=x_out;
                y_in=y_out;

                if(y_in>0){
                    x_out = x_in + (y_in >> 7);
                    y_out = y_in - (x_in >> 7);
                    z_in += elementary_angle[7];
                }else{
                    x_out = x_in - (y_in >> 7);
                    y_out = y_in + (x_in >> 7);
                    z_in -= elementary_angle[7];
                }
                x_in=x_out;
                y_in=y_out;

                if(y_in>0){
                    x_out = x_in + (y_in >> 8);
                    y_out = y_in - (x_in >> 8);
                    z_in += elementary_angle[8];
                }else{
                    x_out = x_in - (y_in >> 8);
                    y_out = y_in + (x_in >> 8);
                    z_in -= elementary_angle[8];
                }
                x_in=x_out;
                y_in=y_out;

                if(y_in>0){
                    x_out = x_in + (y_in >> 9);
                    y_out = y_in - (x_in >> 9);
                    z_in += elementary_angle[9];
                }else{
                    x_out = x_in - (y_in >> 9);
                    y_out = y_in + (x_in >> 9);
                    z_in -= elementary_angle[9];
                }
                x_in=x_out;
                y_in=y_out;

                if(y_in>0){
                    x_out = x_in + (y_in >> 10);
                    y_out = y_in - (x_in >> 10);
                    z_in += elementary_angle[10];
                }else{
                    x_out = x_in - (y_in >> 10);
                    y_out = y_in + (x_in >> 10);
                    z_in -= elementary_angle[10];
                }
                x_in=x_out;
                y_in=y_out;

                if(y_in>0){
                    x_out = x_in + (y_in >> 11);
                    y_out = y_in - (x_in >> 11);
                    z_in += elementary_angle[11];
                }else{
                    x_out = x_in - (y_in >> 11);
                    y_out = y_in + (x_in >> 11);
                    z_in -= elementary_angle[11];
                }
                x_in=x_out;
                y_in=y_out;

                if(y_in>0){
                    x_out = x_in + (y_in >> 12);
                    y_out = y_in - (x_in >> 12);
                    z_in += elementary_angle[12];
                }else{
                    x_out = x_in - (y_in >> 12);
                    y_out = y_in + (x_in >> 12);
                    z_in -= elementary_angle[12];
                }
                x_in=x_out;
                y_in=y_out;

                if(y_in>0){
                    x_out = x_in + (y_in >> 13);
                    y_out = y_in - (x_in >> 13);
                    z_in += elementary_angle[13];
                }else{
                    x_out = x_in - (y_in >> 13);
                    y_out = y_in + (x_in >> 13);
                    z_in -= elementary_angle[13];
                }
                x_in=x_out;
                y_in=y_out;

                if(y_in>0){
                    x_out = x_in + (y_in >> 14);
                    y_out = y_in - (x_in >> 14);
                    z_in += elementary_angle[14];
                }else{
                    x_out = x_in - (y_in >> 14);
                    y_out = y_in + (x_in >> 14);
                    z_in -= elementary_angle[14];
                }
                x_in=x_out;
                y_in=y_out;

            v->x = x_in;
            v->y = y_in;
            v->z = z_in;



}

void cordic_rotate(VECTOR v){
      int x_in,y_in,z_in;
      int x_out,y_out;

      x_in=(double)(v->x << 15);
      y_in=(double)(v->y << 15);
      z_in= v->z ;


          if(z_in<0){
              x_out = x_in + (y_in >> 0);
              y_out = y_in - (x_in >> 0);
              z_in += elementary_angle[0];
          }else{
              x_out = x_in - (y_in >> 0);
              y_out = y_in + (x_in >> 0);
              z_in -= elementary_angle[0];
          }
          x_in=x_out;
          y_in=y_out;


          if(z_in<0){
              x_out = x_in + (y_in >> 1);
              y_out = y_in - (x_in >> 1);
              z_in += elementary_angle[1];
          }else{
              x_out = x_in - (y_in >> 1);
              y_out = y_in + (x_in >> 1);
              z_in -= elementary_angle[1];
          }
          x_in=x_out;
          y_in=y_out;

          if(z_in<0){
              x_out = x_in + (y_in >> 2);
              y_out = y_in - (x_in >> 2);
              z_in += elementary_angle[2];
          }else{
              x_out = x_in - (y_in >> 2);
              y_out = y_in + (x_in >> 2);
              z_in -= elementary_angle[2];
          }
          x_in=x_out;
          y_in=y_out;

          if(z_in<0){
              x_out = x_in + (y_in >> 3);
              y_out = y_in - (x_in >> 3);
              z_in += elementary_angle[3];
          }else{
              x_out = x_in - (y_in >> 3);
              y_out = y_in + (x_in >> 3);
              z_in -= elementary_angle[3];
          }
          x_in=x_out;
          y_in=y_out;

          if(z_in<0){
              x_out = x_in + (y_in >> 4);
              y_out = y_in - (x_in >> 4);
              z_in += elementary_angle[4];
          }else{
              x_out = x_in - (y_in >> 4);
              y_out = y_in + (x_in >> 4);
              z_in -= elementary_angle[4];
          }
          x_in=x_out;
          y_in=y_out;

          if(z_in<0){
              x_out = x_in + (y_in >> 5);
              y_out = y_in - (x_in >> 5);
              z_in += elementary_angle[5];
          }else{
              x_out = x_in - (y_in >> 5);
              y_out = y_in + (x_in >> 5);
              z_in -= elementary_angle[5];
          }
          x_in=x_out;
          y_in=y_out;

          if(z_in<0){
              x_out = x_in + (y_in >> 6);
              y_out = y_in - (x_in >> 6);
              z_in += elementary_angle[6];
          }else{
              x_out = x_in - (y_in >> 6);
              y_out = y_in + (x_in >> 6);
              z_in -= elementary_angle[6];
          }
          x_in=x_out;
          y_in=y_out;

          if(z_in<0){
              x_out = x_in + (y_in >> 7);
              y_out = y_in - (x_in >> 7);
              z_in += elementary_angle[7];
          }else{
              x_out = x_in - (y_in >> 7);
              y_out = y_in + (x_in >> 7);
              z_in -= elementary_angle[7];
          }
          x_in=x_out;
          y_in=y_out;

          if(z_in<0){
              x_out = x_in + (y_in >> 8);
              y_out = y_in - (x_in >> 8);
              z_in += elementary_angle[8];
          }else{
              x_out = x_in - (y_in >> 8);
              y_out = y_in + (x_in >> 8);
              z_in -= elementary_angle[8];
          }
          x_in=x_out;
          y_in=y_out;


          if(z_in<0){
              x_out = x_in + (y_in >> 9);
              y_out = y_in - (x_in >> 9);
              z_in += elementary_angle[9];
          }else{
              x_out = x_in - (y_in >> 9);
              y_out = y_in + (x_in >> 9);
              z_in -= elementary_angle[9];
          }
          x_in=x_out;
          y_in=y_out;

          if(z_in<0){
              x_out = x_in + (y_in >> 10);
              y_out = y_in - (x_in >> 10);
              z_in += elementary_angle[10];
          }else{
              x_out = x_in - (y_in >> 10);
              y_out = y_in + (x_in >> 10);
              z_in -= elementary_angle[10];
          }
          x_in=x_out;
          y_in=y_out;

          if(z_in<0){
              x_out = x_in + (y_in >> 11);
              y_out = y_in - (x_in >> 11);
              z_in += elementary_angle[11];
          }else{
              x_out = x_in - (y_in >> 11);
              y_out = y_in + (x_in >> 11);
              z_in -= elementary_angle[11];
          }
          x_in=x_out;
          y_in=y_out;

          if(z_in<0){
              x_out = x_in + (y_in >> 12);
              y_out = y_in - (x_in >> 12);
              z_in += elementary_angle[12];
          }else{
              x_out = x_in - (y_in >> 12);
              y_out = y_in + (x_in >> 12);
              z_in -= elementary_angle[12];
          }
          x_in=x_out;
          y_in=y_out;

          if(z_in<0){
              x_out = x_in + (y_in >> 13);
              y_out = y_in - (x_in >> 13);
              z_in += elementary_angle[13];
          }else{
              x_out = x_in - (y_in >> 13);
              y_out = y_in + (x_in >> 13);
              z_in -= elementary_angle[13];
          }
          x_in=x_out;
          y_in=y_out;

          if(z_in<0){
              x_out = x_in + (y_in >> 14);
              y_out = y_in - (x_in >> 14);
              z_in += elementary_angle[14];
          }else{
              x_out = x_in - (y_in >> 14);
              y_out = y_in + (x_in >> 14);
              z_in -= elementary_angle[14];
          }
          x_in=x_out;
          y_in=y_out;

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
