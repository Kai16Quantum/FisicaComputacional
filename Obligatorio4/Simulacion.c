
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <windows.h>

#define G 6.67e-11
#define M_T 5.9736e24
#define M_L 0.07349e24
#define d_TL 3.844e8
#define w 2.6617e-6
#define R_T 6.378160e6
#define R_L 1.7374e6
const double h = 120.0;
const int N = 4;

#define m 1000

void f_derivadas(double[], double[], double);
void runge_kutta(double[], double);
void mult_array(double[], double[], int, double);
double hamiltoniano_prima(double[], double);

int main(){
    FILE *f_salida, *f_coordenadas, *f_hamiltoniano;
    f_salida = fopen("datos_salida.txt", "w");
    f_coordenadas = fopen("coordenadas.txt", "w");
    f_hamiltoniano = fopen("hamiltoniano.txt", "w");

    //Tomemos los valores iniciales
    double valores_iniciales[N];
    double t = 0.0;
    double v_escape = pow(((2.0*G*M_T)/R_T),0.5);
    double v_nave = v_escape * 1.101;
    double theta = 0.0;
    valores_iniciales[0] = R_T/d_TL;
    valores_iniciales[1] = -0.45;
    valores_iniciales[2] = (v_nave/d_TL)*cos(theta-valores_iniciales[1]);
    valores_iniciales[3] = (valores_iniciales[0])*(v_nave/d_TL)*sin(theta-valores_iniciales[1]);

    //Calculemos Runge-Kutta
    for (int k = 0; k<3500; k++) {
        for (int i=0; i<4; i++) {
            fprintf(f_salida, "%f,\t", valores_iniciales[i]);
        }
        fprintf(f_hamiltoniano, "%f,%f\n", t, hamiltoniano_prima(valores_iniciales, t));
        fprintf(f_salida, "\n");
        

        
        //Calculamos las coord de la luna
        double coord_luna[] = {cos(w*t), sin(w*t)};


        //Ahora en coordenadas:
        fprintf(f_coordenadas, "%f, %f", valores_iniciales[0]*cos(valores_iniciales[1]), valores_iniciales[0]*sin(valores_iniciales[1]));
        fprintf(f_coordenadas, "\n");
        fprintf(f_coordenadas, "%f, %f", coord_luna[0], coord_luna[1]);
        fprintf(f_coordenadas, "\n\n");
    

        runge_kutta(valores_iniciales, t);
        t += h;
        
    }
}


void f_derivadas(double valores_iniciales[], double valores_finales[], double _t) {
    /* [Array de valores iniciales]:
    0 : r
    1 : phi
    2 : P_r
    3 : P_phi
    */
    double r_barra = valores_iniciales[0];
    double phi = valores_iniciales[1];
    double P_r_barra = valores_iniciales[2];
    double P_phi_barra = valores_iniciales[3];
    double Delta = (G*M_T)/(d_TL*d_TL*d_TL);
    double mu = M_L/M_T;
    double r_barra_prima = pow(1.0+r_barra*r_barra-2.0*r_barra*cos(phi-w*_t),0.5);

    valores_finales[0] = P_r_barra;
    valores_finales[1] = P_phi_barra / (r_barra*r_barra);
    valores_finales[2] = (P_phi_barra*P_phi_barra)/(r_barra*r_barra*r_barra) - Delta*(
        1.0/(r_barra*r_barra) + mu/(pow(r_barra_prima,3.0)) * (r_barra - cos(phi-w*_t)));
    valores_finales[3] = - (Delta * mu * r_barra / (pow(r_barra_prima, 3.0))) * sin(phi-w*_t);

    return;
}

void runge_kutta(double array_valores[], double t) {

    //Tenemos las condiciones para yN para t
    double k1[N],k2[N],k3[N],k4[N];
    double array_derivadas[N];

    for (int i=0; i<N; i++) {
        f_derivadas(array_valores, array_derivadas, t);
        mult_array(array_derivadas, k1, N, h);

        double a2[] = {array_valores[0] + k1[i]/2.0, array_valores[1] + k1[i]/2.0, array_valores[2] + k1[i]/2.0, array_valores[3] + k1[i]/2.0};
        f_derivadas(a2, array_derivadas, t + h/2.0);
        mult_array(array_derivadas, k2, N, h);

        double a3[] = {array_valores[0] + k2[i]/2.0, array_valores[1] + k2[i]/2.0, array_valores[2] + k2[i]/2.0, array_valores[3] + k2[i]/2.0};
        f_derivadas(a3, array_derivadas, t + h/2.0);
        mult_array(array_derivadas, k3, N, h);


        double a4[] = {array_valores[0] + k3[i], array_valores[1] + k3[i], array_valores[2] + k3[i], array_valores[3] + k3[i]};
        f_derivadas(a4, array_derivadas, t + h);
        mult_array(array_derivadas, k4, N, h);
    }

    for (int k=0; k<N; k++) {
        array_valores[k] = array_valores[k] + ((1.0/6.0) * (k1[k]+2.0*k2[k]+2.0*k3[k]+k4[k]));
    }

    return;
}


void mult_array(double array_valores[], double array_guardado[], int size, double mult) {
    for (int i=0; i<size; i++) {
        array_guardado[i] = array_valores[i] * mult;
    }
    return;
}

double hamiltoniano_prima(double array_valores[],double _t) {
    double hamiltoniano = 0.0;
    double r = array_valores[0]*d_TL;
    double phi = array_valores[1];
    double P_r = array_valores[2]*m*d_TL;
    double P_phi = array_valores[3]*m*d_TL*d_TL;

    double r_L = (r*r+d_TL*d_TL - 2*r*d_TL*cos(phi-w*_t));

    hamiltoniano += (P_r*P_r)/(2*m) + (P_phi*P_phi)/(2*m*r*r) - G*(m*M_T)/r - G*(m*M_L)/(r_L);
    hamiltoniano -= w*P_phi;

    return hamiltoniano;
}