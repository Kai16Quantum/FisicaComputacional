
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

const double g = 9.80665;
const double h = 0.001;
const int N = 4;
const double PI = 3.141592653589;
const int max_experiencias = 4;
const int max_energias = 1;
const double offset_angulo_experiencia = 0.14;
const int numero_de_pasos = 4000000;
const int imprimir_cada_x_pasos = 3;
const int numero_pasos_trayectoria = 25;
const int offset_trayectoria = 0.1;

void f_derivadas(double[], double[], double);
void runge_kutta(double[], double);
void mult_array(double[], double[], int, double);
double hamiltoniano(double[], double);
double hamiltoniano_derivadas(double[], double, double);
double phi_punto_desde_energia(double, double, double);

int main(){
    FILE *f_hamiltoniano;
    char nombre_f_lyapunov[64] = "Datos/datos_lyapunov";

    //Tomemos los valores iniciales
    double array_energias[5] = {1.0,3.0,5.0,10.0,15.0};
    double valores_iniciales[N], valores_cercanos[N];
    double t = 0.0;
    valores_iniciales[0] = 0.0;
    valores_iniciales[1] = 0.0;
    valores_iniciales[2] = 0.0;
    valores_iniciales[3] = 0.0;

    //Obtenemos el Phi punto con la energía y los ángulos iniciales.
    double phi_punto = phi_punto_desde_energia(array_energias[0], valores_iniciales[0], valores_iniciales[1]);
    //Obtenemos P_phi con Phi punto cuandoo Theta punto = 0
    valores_iniciales[2] = 2.0*phi_punto;
    //Obtenemos P_theta con Phi punto y theta punto
    valores_iniciales[3] = phi_punto*cos(valores_iniciales[0]-valores_iniciales[1]);
    //Por cada experiencia

    for (int exp_n=0; exp_n<max_experiencias; exp_n++){
        char exp_string[64] = "";
        sprintf(exp_string, "%d", exp_n);

        //Para cada energía
        for (int l=0; l<max_energias; l++) {
            t = 0.0;
            valores_iniciales[0] = exp_n*offset_angulo_experiencia;
            valores_iniciales[1] = 0.0;
            phi_punto = phi_punto_desde_energia(array_energias[l], valores_iniciales[0], valores_iniciales[1]);
            valores_iniciales[2] = 2.0*phi_punto;
            valores_iniciales[3] = phi_punto*cos(valores_iniciales[0]-valores_iniciales[1]);
            
            //Creemos el nombre del archivo.
            char filename[64] = "";
            char e_string[64] = "";
            sprintf(e_string, "%f", array_energias[l]);

            strcpy(filename, nombre_f_lyapunov);
            strcat(filename, "Energia_");
            strcat(filename, e_string);
            strcat(filename, "_EXPERIENCIA_");
            strcat(filename, exp_string);
            strcat(filename, ".txt");
            FILE* f_lyapunov = fopen(filename, "w");

            
            for (int w=0;w<4;w++) {
                valores_cercanos[w] = valores_iniciales[w];
            }
            

            //Para un número de pasos determinado
            for (int k = 0; k<numero_de_pasos; k++) {
                
                //Creamos una trayectoria cercana
                if (k%numero_pasos_trayectoria == 0){
                    for (int w=0;w<4;w++) {
                        valores_cercanos[w] = valores_iniciales[w];
                    }
                    valores_cercanos[0] += offset_trayectoria;
                    fprintf(f_lyapunov, "#\n", t);
                }
                    

                //Condiciones de contorno
                for (int i=0; i<2; i++) {
                    if (valores_iniciales[i] < -PI) {valores_iniciales[i] += 2.0*PI;}
                    else if (valores_iniciales[i] > PI) {valores_iniciales[i] -= 2.0*PI;}
                }
                for (int i=0; i<2; i++) {
                    if (valores_cercanos[i] < -PI) {valores_cercanos[i] += 2.0*PI;}
                    else if (valores_cercanos[i] > PI) {valores_cercanos[i] -= 2.0*PI;}
                }


                //Imprimimos los valores de ambas trayectorias
                if (k%imprimir_cada_x_pasos == 0){
                    fprintf(f_lyapunov, "%f,\t", t);
                    for (int i=0; i<2; i++) {
                        fprintf(f_lyapunov, "%f,\t", valores_iniciales[i]);
                    }
                    for (int i=0; i<2; i++) {
                        fprintf(f_lyapunov, "%f,\t", valores_cercanos[i]);
                    }
                    fprintf(f_lyapunov, "\n");

                }

                //Calculamos la evolución de la trayectoria normal y la trayectoria cercana.
                t += h;
                runge_kutta(valores_iniciales, t); 
                runge_kutta(valores_cercanos, t); 
            }
        fclose(f_lyapunov);
        }
    }
}


void f_derivadas(double valores_iniciales[], double valores_finales[], double _t) {
    double phi = valores_iniciales[0];
    double theta = valores_iniciales[1];
    double P_phi = valores_iniciales[2];
    double P_theta = valores_iniciales[3];
    double h1 = (P_phi * P_theta * sin(phi - theta)) / ((1.0 + sin(phi - theta) * sin(phi - theta)));
    double h2 = (P_phi * P_phi + 2.0*P_theta * P_theta - 2.0 * P_phi * P_theta * cos(phi - theta)) / (2.0 * pow(1.0 + sin(phi - theta) * sin(phi - theta), 2.0));



    valores_finales[0] = (P_phi-P_theta*cos(phi-theta))/(1.0+sin(phi-theta)*sin(phi-theta));
    valores_finales[1] = (-P_phi*cos(phi-theta)+2.0*P_theta) / (1.0+sin(phi-theta)*sin(phi-theta));
    valores_finales[2] = -2.0*g*sin(phi)-h1+h2*sin(2.0*(phi-theta));
    valores_finales[3] = -g*sin(theta)+h1-h2*sin(2.0*(phi-theta));

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

// -- Función ayudante, multiplica un array de un tamaño N por un double y lo escribe en otro array. --
void mult_array(double array_valores[], double array_guardado[], int size, double mult) {
    for (int i=0; i<size; i++) {
        array_guardado[i] = array_valores[i] * mult;
    }
    return;
}

double hamiltoniano(double array_valores[],double _t) {
    double hamiltoniano = 0.0;
    double phi = array_valores[0];
    double theta = array_valores[1];
    double P_phi = array_valores[2];
    double P_theta = array_valores[3];

    hamiltoniano = (P_phi*P_phi + 2.0*P_theta*P_theta - 2.0*P_phi*P_theta*cos(phi-theta)) / (2.0*(1.0+sin(phi-theta)*sin(phi-theta))) - 2.0*g*cos(phi) -g*cos(theta);

    return hamiltoniano;
}

double hamiltoniano_derivadas(double array_valores[], double phi, double theta) {
    double hamiltoniano = 0.0;
    double phi_punto = array_valores[0];
    double theta_punto = array_valores[1];
    double P_phi_punto = array_valores[2];
    double P_theta_punto = array_valores[3];
    hamiltoniano = phi_punto*phi_punto + 0.5*theta_punto*theta_punto + phi_punto*theta_punto*cos(phi-theta)-2.0*g*cos(phi)-g*cos(theta);

    return hamiltoniano;
}   

double phi_punto_desde_energia(double E, double phi, double theta){
    float phi_punto = sqrt(E+2.0*g*cos(phi)+g*cos(theta));
    return phi_punto;
}
