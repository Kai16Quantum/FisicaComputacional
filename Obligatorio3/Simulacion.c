
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <windows.h>
#include "complex.h"

#define N 1000
#define lambda 1.0
#define PI 3.1415927
#define n_ciclos 5.0

double obtener_probabilidad(fcomplex _phi_array[N]);
void obtener_valores(fcomplex alpha_array[N], fcomplex beta_array[N], fcomplex phi_array[N], double _n);
fcomplex f_phi(int _j, int _n, double _k_0_tilde, fcomplex _chi, fcomplex _phi_array[N]);
fcomplex f_A_j_cero(double _s_tilde, double _V_j_tilde);
double f_V_j_tilde(int _j, double _k_0_tilde);
fcomplex f_b(double _s_tilde, fcomplex _phi);
fcomplex chi(int _j, int _n, fcomplex _chi_array[], fcomplex _alpha_array[N], fcomplex _beta_array[N]);

int main(){
    FILE *f_salida, *prob_salida;
    prob_salida = fopen("prob_salida.txt", "w");
    f_salida = fopen("datos_salida.txt", "w");
    double k_0_tilde = 2.0*PI*n_ciclos / N;
    double s_tilde = 1.0/(4.0*k_0_tilde*k_0_tilde);
    fcomplex alpha_array[N], beta_array[N], phi_array[N];

    //Términos (alfa y beta) para (t=0)
    alpha_array[N-1] = Complex(0.0,0.0);
    beta_array[N-1] = Complex(0.0,0.0);

    // Función inicial (t=0)
    phi_array[N-1] = Complex(0.0,0.0);
    phi_array[0] = Complex(0.0,0.0);

    for (int k=0; k<N-1; k++) {
        phi_array[k] = f_phi(k, 0, k_0_tilde, Complex(0.0,0.0), phi_array);
    }

    phi_array[N-1] = Complex(0.0,0.0);
    phi_array[0] = Complex(0.0,0.0);

    //Normalicemos la función
    double norma = obtener_probabilidad(phi_array);
    for (int k=0; k<N-1; k++) {
        phi_array[k] = RCmul(1.0/sqrt(norma),phi_array[k]);
    }

    //Bucle principal
    for (int t=1; t<500; t++) {
        obtener_valores(alpha_array, beta_array, phi_array, t);
        //Guardamos los valores
        for (int l=0;l<N;l++){
            //Para cada discretizado espacial
            fprintf(f_salida, "%f, %f, %f, %f", l*1.0, phi_array[l].r*phi_array[l].r, phi_array[l].i*phi_array[l].i, 
            Cabs(phi_array[l])*Cabs(phi_array[l]));
            fprintf(f_salida, "\n");
        }
        //Para cada discretizado temporal
        fprintf(prob_salida, "%i,%f\n", t, obtener_probabilidad(phi_array));
        fprintf(f_salida, "\n"); 
    }
    return 0;
}


void obtener_valores(fcomplex alpha_array[], fcomplex beta_array[], fcomplex phi_array[], double _n){
    double k_0_tilde = 2.0*PI*n_ciclos / N;
    double s_tilde = 1.0/(4.0*k_0_tilde*k_0_tilde);
    fcomplex chi_array[N];
    chi_array[0] = Complex(0.0,0.0);
    chi_array[N-1] = Complex(0.0,0.0);

    int j = N-1;
    //Obtenemos las alfa y beta de arriba a abajo
    for (j=N-2; j>=0; j--) {
        double V_j_tilde = f_V_j_tilde(j+1, k_0_tilde);
        fcomplex A_j_cero = f_A_j_cero(s_tilde, V_j_tilde);
        fcomplex denominador = Cadd(A_j_cero, alpha_array[j+1]);
        fcomplex gamma = Cdiv(Complex(1.0,0.0), denominador);

        alpha_array[j] = Cmul(gamma, Complex(-1.0,0.0));

        fcomplex par = Csub(f_b(s_tilde,phi_array[j+1]), beta_array[j+1]);
        beta_array[j] = Cmul(par, gamma);
    }
    beta_array[N-1] = Complex(0.0,0.0);


    //Obtenemos las chi de abajo a arriba
    for (j=1; j<N; j++) {
        chi_array[j] = chi(j, _n, chi_array, alpha_array, beta_array);
    }

    for (j=0; j<N; j++) {
        phi_array[j] = f_phi(j, _n, k_0_tilde, chi_array[j], phi_array);
    }
    return;
}

double obtener_probabilidad(fcomplex _phi_array[]) {
    double suma = 0.0;
    for (int i=0; i<N; i++) {
        double abs = Cabs(_phi_array[i]);
        suma += abs*abs;
    }
    return suma;
}

fcomplex f_phi(int _j, int _n, double _k_0_tilde, fcomplex _chi, fcomplex _phi_array[]){
    // Condiciones contorno
    if (_n == 0) {
        fcomplex complejo = Cgauss(1.0,_k_0_tilde*_j);
        fcomplex valor = RCmul(exp(-8.0*(4.0*_j-N)*(4.0*_j-N) / (N*N)), complejo);
        return valor;
    } else {
        //Se calcula con el anterior chi(j,n-1) - phi(j,n-1)
        return Csub(_chi,_phi_array[_j]);
    }
}   

fcomplex f_A_j_cero(double _s_tilde, double _V_j_tilde){
    fcomplex c1 = Complex(-2.0,0.0);
    fcomplex c2 = Complex(0.0,2.0/_s_tilde);
    fcomplex c3 = Complex(-_V_j_tilde,0.0);
    fcomplex suma = Cadd(c1,c2);
    suma = Cadd(suma,c3);
    return suma;
}

double f_V_j_tilde(int _j, double _k_0_tilde) {
    double valor = 0.0;
    if (_j > 2*N/5 && _j < 3*N/5) {
        valor = lambda * _k_0_tilde*_k_0_tilde;
    }

    return valor;
}

fcomplex f_b(double _s_tilde, fcomplex _phi){
    fcomplex valor = Cmul(Complex(0.0,4.0), Cdiv(_phi,Complex(_s_tilde,0.0)));
    return valor;
}

fcomplex chi(int _j, int _n, fcomplex _chi_array[], fcomplex _alpha_array[], fcomplex _beta_array[]){
    return Cadd(Cmul(_alpha_array[_j-1],_chi_array[_j-1]),_beta_array[_j-1]);
}