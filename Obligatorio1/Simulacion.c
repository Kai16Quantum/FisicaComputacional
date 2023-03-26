#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

int n_planetas = 9;
double h_step = 0.01;

int main() {
    FILE *f_inicial, *f_salida, *planetas_salida;
    f_inicial= fopen("datos_iniciales.txt","r");
    f_salida = fopen("datos_salida.txt", "w");
    planetas_salida = fopen("planetas_salida.dat","w");

    int i=0;
    double masa, distancia, velocidad;
    double r_inicial[n_planetas];
    double v_inicial[n_planetas];
    double m_inicial[n_planetas];
    while (fscanf(f_inicial, "%lf\t%lf\t%lf", &masa, &distancia, &velocidad)!=EOF) {
        r_inicial[i] = distancia;
        v_inicial[i] = velocidad;
        m_inicial[i] = masa;
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
    for (int i=0; i<n_planetas; i++) {
        //Centradas en Y=0.
        matriz_posiciones[i][0] = r_inicial[i];
        matriz_posiciones[i][1] = 0;
        //Velocidad positiva en Y.
        matriz_velocidades[i][0] = 0;
        matriz_velocidades[i][1] = v_inicial[i];
    }

    //Calculamos la aceleración para las R y M actuales.
    aceleracion_f(m_inicial, matriz_posiciones, matriz_aceleraciones);
    fprintf(planetas_salida, "%lg, %lg\n", matriz_posiciones[0][0], matriz_posiciones[0][1]);
    for (int t=0; t< 4; t+=h_step){
        //Obtenemos el nuevo R:
        r_calcular(matriz_posiciones, matriz_velocidades, matriz_aceleraciones);
        //Obtenemos w:
        w_calcular(matriz_velocidades, matriz_aceleraciones, matriz_w);

        //Obtenemos a:
        aceleracion_f(m_inicial, matriz_posiciones, matriz_aceleraciones);
        //Obtenemos v:
        v_calcular(matriz_velocidades, matriz_aceleraciones, matriz_w);

        //Guardamos al archivo
        fprintf(planetas_salida, "%lg, %lg\n", matriz_posiciones[0][0], matriz_posiciones[0][1]);
    }
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

void r_calcular(double matriz_posiciones[][2], double array_velocidades[][2], double array_aceleraciones[][2]) {
    for (int i=0; i<n_planetas; i++){
        matriz_posiciones[i][0] = matriz_posiciones[i][0] + h_step* array_velocidades[i][0] + ((h_step*h_step)/2.0)*array_aceleraciones[i][0];
        matriz_posiciones[i][1] = matriz_posiciones[i][1] + h_step* array_velocidades[i][1] + ((h_step*h_step)/2.0)*array_aceleraciones[i][1];
    }
}

void v_calcular(double array_velocidades[][2], double array_aceleraciones[][2], double array_w[][2]) {
    for (int i=0; i<n_planetas; i++) {
        array_velocidades[i][0] = array_w[i][0] + (h_step/2.0) * array_aceleraciones[i][0];
        array_velocidades[i][1] = array_w[i][1] + (h_step/2.0) * array_aceleraciones[i][1];
    }
}

void rescalar(double masas_i[], double pos_i[], double vel_i[]){
    double d_sol_tierra = 1.469e11;
    double masa_sol = 1.989e30;
    for (int i=0; i<n_planetas; i++) {
        // Dividimos entre la masa del sol Ms. (Kg -> Ms)
        masas_i[i] = masas_i[i] / (masa_sol);
        // Dividimos entre la distancia Sol-Tierra (m -> AU)
        pos_i[i] = pos_i[i] / (d_sol_tierra);
        // Velocidad (km/s) -> (AU/t')
        double tiempo = pow((6.67430e-11 * masa_sol) / (pow(d_sol_tierra, 3)), 0.5) ;
        vel_i[i] = ((vel_i[i] / tiempo)*1000.0)/d_sol_tierra;
    }
}