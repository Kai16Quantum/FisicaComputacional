 
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>
#include <windows.h>

#define N_MAX 25
#define N_PATRONES 1

double incremento_energia(int[][N_MAX], int[][N_MAX][N_MAX], int, int);
void inicializar_nodos(int[][N_MAX], int);
int s(int[][N_MAX], int, int);
void print_matriz(FILE*, int[][N_MAX]);
double theta(int, int, int[][N_MAX][N_MAX]);
double omega(int, int, int, int, int[][N_MAX][N_MAX]);
double xi(int, int, int, int[][N_MAX][N_MAX]);
double a(int, int[][N_MAX][N_MAX]);
void alterar_nodos(int[][N_MAX], double);

int main()
{
    srand(time(NULL));
    FILE *f_salida, *f_patrones;
    // Parámetros iniciales
    double temp = 10e-4;
    int step_number = 10;
    int monte_carlo_step = N_MAX*N_MAX;
    int matriz_nodos[N_MAX][N_MAX];
    int array_patrones[N_PATRONES][N_MAX][N_MAX];
    f_salida = fopen("datos_salida.txt", "w");
    f_patrones = fopen("patrones.txt", "r");

    // Inicializamos la matriz de nodos
    inicializar_nodos(matriz_nodos, 0);
    
    //Leemos el patrón desde el archivo
    for (int patron_n = 0; patron_n < N_PATRONES; patron_n++){
        for (int i = 0; i < N_MAX; i++) {
            for (int j = 0; j < N_MAX; j++) {
                fscanf(f_patrones, "%1d", &array_patrones[patron_n][i][j]);
                matriz_nodos[i][j] = array_patrones[patron_n][i][j];
            }
        }
        fscanf(f_patrones, "%*[^\n]\n");
    }
    alterar_nodos(matriz_nodos,0.5);
    
    //Bucle principal
    for (int l=0; l < step_number; l++) {
        for (int k=0; k < monte_carlo_step; k++) {
            int n = (rand() % N_MAX);
            int m = (rand() % N_MAX);

            double p = min(1, exp(-incremento_energia(matriz_nodos,array_patrones,n,m)/temp));

            //Generamos un número aleatorio entre 0 y 1.
            double chi = (rand() % 10001) / 10000.0;
            if (chi < p) {
                matriz_nodos[n][m] = 1-matriz_nodos[n][m];
            }
        }
        print_matriz(f_salida, matriz_nodos);
    }
    return 1;
}


// ------ FUNCIONES ESTADÍSTICAS --------
double incremento_energia(int matriz_nodos[][N_MAX], int array_patron[][N_MAX][N_MAX], int i, int j) {
    double suma = 0.0;
    for (int k=0;k<N_MAX;k++) {
        for (int l=0;l<N_MAX;l++){
            suma += omega(i,j,k,l,array_patron)*s(matriz_nodos,k,l);
        }
    }
    suma *= s(matriz_nodos,i,j) - 0.5;
    suma += theta(i,j,array_patron) *(1-2*s(matriz_nodos,i,j));
    return suma;
}

void inicializar_nodos(int matriz_nodos[][N_MAX], int valor_objetivo) {
    if (valor_objetivo != -1) {
        for (int i=0; i<N_MAX; i++) {
            for (int j=0; j<N_MAX; j++) {
                matriz_nodos[i][j] = valor_objetivo;
            }
        } 
    } else {
        for (int i=0; i<N_MAX; i++) {
            for (int j=0; j<N_MAX; j++) {
                int valor = rand()%2 ;
                if (valor == 0) {
                    valor = -1;
                }
                matriz_nodos[i][j] = valor;
            }
        } 
    }
    return;
}

void alterar_nodos(int matriz_nodos[][N_MAX], double prob) {
    for (int i=0; i<N_MAX; i++) {
        for (int j=0; j<N_MAX; j++) {
            //Generamos un número aleatorio entre 0 y 1.
            double chi = (rand() % 10001) / 10000.0;
            if(chi<prob) {
                matriz_nodos[i][j] = 1 - matriz_nodos[i][j];
            }
        }
    }
}

int s(int matriz_nodos[][N_MAX], int n, int m) {
    //Condiciones de contorno periódicas.
    if (n >= N_MAX) n -= N_MAX;
    if (m >= N_MAX) m -= N_MAX;

    if (n < 0) n += N_MAX;
    if (m < 0) m += N_MAX;

    return matriz_nodos[n][m];
}


void print_matriz(FILE* archivo, int matriz_nodos[][N_MAX]) {
    for (int i=0; i<N_MAX; i++) {
        for (int j=0; j<N_MAX; j++) {
            fprintf(archivo, "%i", matriz_nodos[i][j]);
            if (j < N_MAX-1){
                fprintf(archivo, ", ");
            }
        }
        fprintf(archivo,"\n");
    }
    fprintf(archivo,"\n");
    return;
}

double theta(int i, int j, int array_patron[][N_MAX][N_MAX]){
    double suma = 0.0;
    for (int k=0; k<N_MAX; k++) {
        for (int l=0; l<N_MAX; l++) {
            suma += omega(i,j,k,l, array_patron);
        }
    }
    return 0.5*suma;
}

double omega(int i, int j, int k, int l, int array_patron[][N_MAX][N_MAX]) {
    if ((i==k) && (j==l)){
        return 0.0;
    } else {
        double suma = 0.0;
        for (int mu=0; mu<N_PATRONES; mu++) {
            suma += (xi(mu, i, j, array_patron) - a(mu, array_patron)) * (xi(mu,k,l,array_patron)-a(mu, array_patron));
        }
        return suma/(N_MAX*N_MAX);
    }
}

double xi(int mu, int i, int j, int array_patron[][N_MAX][N_MAX]){
    //Condiciones de contorno periódicas.
    if (i >= N_MAX) i -= N_MAX;
    if (j >= N_MAX) j -= N_MAX;

    if (i < 0) i += N_MAX;
    if (j < 0) j += N_MAX;

    return array_patron[mu][i][j];
}

double a(int mu, int array_patron[][N_MAX][N_MAX]) {
    double suma = 0.0;
    for (int i=0; i<N_MAX; i++) {
        for (int j=0; j<N_MAX; j++) {
            suma += xi(mu, i, j, array_patron);
        }
    }
    return suma/(N_MAX*N_MAX);
}