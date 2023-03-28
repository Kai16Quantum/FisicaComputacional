#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

int n_planetas = 9;
double h_step = 0.1;
double max_time = 40.0;
const double d_sol_tierra = 1.469e11;
const double masa_sol = 1.989e30;

void guardar_datos(FILE *, double[][2]);
void aceleracion_f(double[], double[][2], double[][2]);
void w_calcular(double[][2], double[][2], double[][2]);
void r_calcular(double[][2], double[][2], double[][2], double[], double[]);
void v_calcular(double[][2], double[][2], double[][2]);
void v_calcular(double[][2], double[][2], double[][2]);
void rescalar(double[], double[], double[]);
double e_calcular(double[], double[][2], double[][2]);
void copyarray(double[], double[], int);

int main() {
    FILE *f_inicial, *f_salida, *planetas_salida, *energia_salida, *periodos_salida;
    f_inicial= fopen("datos_iniciales.txt","r");
    f_salida = fopen("datos_salida.txt", "w");
    planetas_salida = fopen("planetas_salida.dat","w");
    energia_salida = fopen("energia_salida.dat", "w");
    periodos_salida = fopen("periodos_salida.dat", "w");

    int i=0;
    double masa, distancia, velocidad;
    char nombre[50];
    char* nombres[n_planetas];
    double r_inicial[n_planetas];
    double v_inicial[n_planetas];
    double m_inicial[n_planetas];
    double angulo_inicial[n_planetas];
    while (fscanf(f_inicial, "%49s\t%lf\t%lf\t%lf", nombre, &masa, &distancia, &velocidad)!=EOF) {
        r_inicial[i] = distancia;
        v_inicial[i] = velocidad;
        m_inicial[i] = masa;
        nombres[i] = malloc(strlen(nombre) + 1);
        strcpy(nombres[i], nombre);
        i += 1;
    }


    rescalar(m_inicial, r_inicial, v_inicial);
    for (int i=0; i<n_planetas; i++){
        fprintf(f_salida, "%lg\t%lg\t%lg\n", m_inicial[i], r_inicial[i], v_inicial[i]);
    }

    fclose(f_inicial);
    fclose(f_salida);
    printf("Guardados.");
    //Hemos obtenido y reescalado las condiciones iniciales.
    //Colocamos la posición en una matriz. [ejex,ejey]
    double matriz_posiciones[n_planetas][2];
    double matriz_velocidades[n_planetas][2];
    double matriz_aceleraciones[n_planetas][2];
    double matriz_w[n_planetas][2];
    double array_angulos[n_planetas];
    double array_periodos[n_planetas];
    for (int i=0; i<n_planetas; i++) {
        //Centradas en Y=0.
        matriz_posiciones[i][0] = r_inicial[i];
        matriz_posiciones[i][1] = 0;
        //Velocidad positiva en Y.
        matriz_velocidades[i][0] = 0;
        matriz_velocidades[i][1] = v_inicial[i];
        //Angulo inicial
        angulo_inicial[i] = atan2(matriz_posiciones[i][1], matriz_posiciones[i][0]);
        array_periodos[i] = -1;
    }

    //Calculamos la aceleración para las R y M actuales.
    aceleracion_f(m_inicial, matriz_posiciones, matriz_aceleraciones);
    guardar_datos(planetas_salida, matriz_posiciones);
    copyarray(angulo_inicial, array_angulos, n_planetas);
    for (float t=0.0; t < max_time; t+=h_step){
        //Guardamos el antiguo ángulo.
        double antiguos_angulos[n_planetas];
        copyarray(array_angulos, antiguos_angulos, n_planetas);

        //Obtenemos el nuevo R y ángulo:
        r_calcular(matriz_posiciones, matriz_velocidades, matriz_aceleraciones, array_angulos, angulo_inicial);
        //Obtenemos w:
        w_calcular(matriz_velocidades, matriz_aceleraciones, matriz_w);

        //Obtenemos a:
        aceleracion_f(m_inicial, matriz_posiciones, matriz_aceleraciones);
        //Obtenemos v:
        v_calcular(matriz_velocidades, matriz_aceleraciones, matriz_w);

        //Guardamos al archivo
        guardar_datos(planetas_salida, matriz_posiciones);
        fprintf(energia_salida, "%lg, %lg\n", t, e_calcular(m_inicial, matriz_posiciones, matriz_velocidades));

        for (int k=0; k<n_planetas; k++) {
            if (array_angulos[k]-antiguos_angulos[k] < 0) {
                if (array_periodos[k] == -1) {
                    array_periodos[k] = t;
                    fprintf(periodos_salida, "%s\t%lg\n", nombres[k], array_periodos[k] * (1.0/(pow((6.67430e-11 * masa_sol) / (pow(d_sol_tierra, 3)), 0.5))/(365.0*3600.0*24.0)));
                }
            }
        }
    }

    //Comprobamos si ha dado una vuelta completa.

     fclose(planetas_salida);
     fclose(energia_salida);
     fclose(periodos_salida);
}

void copyarray(double arr[], double copy[], int size) {
  for (int i = 0; i < size; ++i) {
    copy[i] = arr[i];
  }
}


void guardar_datos(FILE *nombre_archivo, double matriz_r[][2]){
    for (int i=0; i<n_planetas; i++){
        fprintf(nombre_archivo, "%lg, %lg\n", matriz_r[i][0], matriz_r[i][1]);
    }
    fprintf(nombre_archivo, "\n");
}

void aceleracion_f(double array_masas[], double matriz_posiciones[][2], double matriz_aceleraciones[][2]) {
    //Al haber usado unidades tal que G = 1:
    for (int i=0; i<n_planetas; i++){
        double resultado[2] = {0,0};
        for (int k=0; k<n_planetas; k++){
            if (k!=i){
                // R = [xi-xk, yi-yk]
                double x = matriz_posiciones[i][0]-matriz_posiciones[k][0];
                double y = matriz_posiciones[i][1]-matriz_posiciones[k][1];
                double R[2] = {x,y};

                double Modulo = pow(R[0]*R[0] + R[1]*R[1], 0.5);
                resultado[0] += array_masas[k]*R[0]/(pow(Modulo,3));
                resultado[1] += array_masas[k]*R[1]/(pow(Modulo,3));
            }
        }
        matriz_aceleraciones[i][0] = -resultado[0];
        matriz_aceleraciones[i][1] = -resultado[1];
    }
}

void w_calcular(double array_velocidades[][2], double array_aceleraciones[][2], double matriz_w[][2]) {
    for (int i=0; i<n_planetas; i++){
        //Eje X y Eje y
        matriz_w[i][0] = array_velocidades[i][0] + (h_step/2.0) * array_aceleraciones[i][0];
        matriz_w[i][1] = array_velocidades[i][1] + (h_step/2.0) * array_aceleraciones[i][1];
    }

}

void r_calcular(double matriz_posiciones[][2], double array_velocidades[][2], double array_aceleraciones[][2], double array_angulos[], double array_angulos_iniciales[]) {
    for (int i=0; i<n_planetas; i++){
        matriz_posiciones[i][0] = matriz_posiciones[i][0] + h_step* array_velocidades[i][0] + ((h_step*h_step)/2.0)*array_aceleraciones[i][0];
        matriz_posiciones[i][1] = matriz_posiciones[i][1] + h_step* array_velocidades[i][1] + ((h_step*h_step)/2.0)*array_aceleraciones[i][1];

        double angulo_p = atan2(matriz_posiciones[i][1], matriz_posiciones[i][0]);
        if (angulo_p < 0) {
            angulo_p += 2*3.14159265;
        }
        array_angulos[i] = angulo_p - array_angulos_iniciales[i];
    }
}

void v_calcular(double array_velocidades[][2], double array_aceleraciones[][2], double array_w[][2]) {
    for (int i=0; i<n_planetas; i++) {
        array_velocidades[i][0] = array_w[i][0] + (h_step/2.0) * array_aceleraciones[i][0];
        array_velocidades[i][1] = array_w[i][1] + (h_step/2.0) * array_aceleraciones[i][1];
    }
}

void rescalar(double masas_i[], double pos_i[], double vel_i[]){
    for (int i=0; i<n_planetas; i++) {
        // Dividimos entre la masa del sol Ms. (Kg -> Ms)
        masas_i[i] = masas_i[i] / (masa_sol);
        // Dividimos entre la distancia Sol-Tierra (km -> AU)
        pos_i[i] = (pos_i[i] * 1000.0) / (d_sol_tierra);
        // Velocidad (km/s) -> (AU/t')
        double tiempo = pow((6.67430e-11 * masa_sol) / (pow(d_sol_tierra, 3)), 0.5);
        vel_i[i] = ((vel_i[i] / tiempo)*1000.0)/d_sol_tierra;
    }
}

// !!! DEVUELVE LA ENERGÍA TOTAL
double e_calcular(double masas_i[], double matriz_posiciones[][2], double array_velocidades[][2]) {
    // E = T+V
    // T = 1/2 mv^2
    // V Gravitatorio = -GMm/r
    double energia = 0.0 ;
    for (int i=0; i<n_planetas; i++) {
        //T
        double velocidad_cuadrado = pow(array_velocidades[i][0],2) + pow(array_velocidades[i][1],2);
        energia += 0.5*masas_i[i]*velocidad_cuadrado;
        //V Gravitatorio, asumiendo que está reescalado
        for (int j=0; j<n_planetas; j++) {
            if (i!=j) {
                double vector[] = {matriz_posiciones[i][0] - matriz_posiciones[j][0], matriz_posiciones[i][1] - matriz_posiciones[j][1]};
                double r = pow(vector[0]*vector[0]+vector[1]*vector[1],0.5);
                energia += -masas_i[i]*masas_i[j] / r;
            }
        }
    }
    //Devolvemos la energía en las unidades escaladas.
    return energia;
}