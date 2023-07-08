
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>

void prueba_aritmetica();
void prueba_array();

int main()
{
    clock_t tic = clock();
    prueba_aritmetica();
    clock_t toc = clock();
    printf("Prueba Aritmetica: %f segundos.\n", (double)(toc - tic) / CLOCKS_PER_SEC);
    
    tic = clock();
    prueba_array();
    toc = clock();
    printf("Prueba Iteracion de Arrays: %f segundos.\n", (double)(toc - tic) / CLOCKS_PER_SEC);

    return 0;
}

void prueba_aritmetica() {
    double a,b,c,d,e;
    a=0;
    b=0;
    c=0;
    d=0;
    e=0;
    for (int i=0;i<82902300;i++) {
        a = 2 + c - d + b * a;
        e = pow(a,2);
        a = sqrt(abs(e-a+c+b+3.0*b));
    }
}

void prueba_array() {
    double array[9000];
    for (int i=0; i<5000; i++) {
        for (int j=0; j<5000; j++) {
            for (int k=0; k<20; k++) {
                array[i] = i*2.0 + j*1.0 - k*1.0;
            }
        }
    }
}