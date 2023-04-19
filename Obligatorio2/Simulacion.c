
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>
#include <windows.h>

#define N_MAX 25

double f_energia(int[][N_MAX]);
double incremento_energia(int[][N_MAX], int, int);
void inicializar_espines(int[][N_MAX], int);
int s(int[][N_MAX], int, int);
void print_matriz(FILE*, int[][N_MAX]);

int main()
{
    srand(time(NULL));   // Initialization, should only be called once.
    FILE *f_salida;
    double temp = 2.0;
    int step_number = 100;
    int monte_carlo_step = N_MAX*N_MAX;
    int matriz_espines[N_MAX][N_MAX];
    f_salida = fopen("datos_salida.txt", "w");
    
    // Inicializamos la matriz de espines
    inicializar_espines(matriz_espines, 0);
    
    //Bucle principal
    for (int l=0; l < step_number; l++) {
        for (int k=0; k < monte_carlo_step; k++) {
            int n = (rand() % N_MAX);
            int m = (rand() % N_MAX);

            double p = min(1, exp(-incremento_energia(matriz_espines,n,m)/temp));

            //Random number between 1 and 0.
            double chi = (rand() % 10001) / 10000.0;
            if (chi < p) {
                matriz_espines[n][m] = -matriz_espines[n][m];
            }
        }
        print_matriz(f_salida, matriz_espines);
    }
    return 1;
}

// ------ FUNCIONES ESTADÍSTICAS --------
double f_energia(int matriz_espines[][N_MAX]) {
    double energia = 0.0;
    return energia;
}

double incremento_energia(int matriz_espines[][N_MAX], int n, int m) {
    return 2*s(matriz_espines,n,m) * (s(matriz_espines,n+1,m)+s(matriz_espines,n-1,m)+s(matriz_espines,n,m+1)+s(matriz_espines,n,m-1));
}

void inicializar_espines(int matriz_espines[][N_MAX], int valor_objetivo) {
    if (valor_objetivo != 0) {
        for (int i=0; i<N_MAX; i++) {
            for (int j=0; j<N_MAX; j++) {
                matriz_espines[i][j] = valor_objetivo;
            }
        } 
    } else {
        for (int i=0; i<N_MAX; i++) {
            for (int j=0; j<N_MAX; j++) {
                int valor = rand()%2 ;
                if (valor == 0) {
                    valor = -1;
                }
                matriz_espines[i][j] = valor;
            }
        } 
    }
    return;
}

int s(int matriz_espines[][N_MAX], int n, int m) {
    if (n >= N_MAX) n -= N_MAX;
    if (m >= N_MAX) m -= N_MAX;

    if (n < 0) n += N_MAX;
    if (m < 0) m += N_MAX;

    return matriz_espines[n][m];
}

void print_matriz(FILE* archivo, int matriz_espines[][N_MAX]) {
    for (int i=0; i<N_MAX; i++) {
        for (int j=0; j<N_MAX; j++) {
            fprintf(archivo, "%i", matriz_espines[i][j]);
            if (j < N_MAX-1){
                fprintf(archivo, ", ");
            }
        }
        fprintf(archivo,"\n");
    }
    fprintf(archivo,"\n");
    return;
}