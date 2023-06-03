 
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>

#define N_MAX 25
#define N_MAX_PATRONES 25

void leer_patrones(int, int[][N_MAX][N_MAX], FILE*);
void copiar_matriz(int[][N_MAX], int[][N_MAX]);
void algoritmo_hopfield(int, double, int[][N_MAX], int[][N_MAX][N_MAX], FILE*, FILE*, int);
double incremento_energia(int[][N_MAX], int[][N_MAX][N_MAX], int, int, int);
void inicializar_nodos(int[][N_MAX], int);
int s(int[][N_MAX], int, int);
void print_matriz(FILE*, int[][N_MAX]);
double theta(int, int, int, int[][N_MAX][N_MAX]);
double omega(int, int, int, int, int, int[][N_MAX][N_MAX]);
double xi(int, int, int, int[][N_MAX][N_MAX]);
double a(int, int[][N_MAX][N_MAX]);
void alterar_nodos(int[][N_MAX], double);
double solapamiento(int[][N_MAX], int[][N_MAX][N_MAX], int);

int main()
{
    srand(time(NULL));
    FILE *f_salida, *f_patrones, *f_solapamiento;
    // Parámetros iniciales
    double temp = 10e-4;
    int step_number = 25;
    int matriz_nodos[N_MAX][N_MAX];
    int array_patrones[N_MAX_PATRONES][N_MAX][N_MAX];
    f_salida = fopen(".\\Salida\\datos_salida.txt", "w");
    f_patrones = fopen("patrones.txt", "r");
    f_solapamiento = fopen("\\Salida\\solapamiento.txt", "w");

    // Inicializamos la matriz de nodos
    inicializar_nodos(matriz_nodos, 0);
    
    //Leemos el patrón desde el archivo
    leer_patrones(1, array_patrones, f_patrones);
    

    // EJERCICIO 1: Ver como la red es capaz de recordar un solo patrón almacenado, para T = 10^-4.
    // ----------------[] Partiendo de a) Una condición aleatoria. []---------------------
    inicializar_nodos(matriz_nodos, -1);
    algoritmo_hopfield(step_number, temp, matriz_nodos, array_patrones, f_salida, f_solapamiento, 1);
    
    
    // ----------------[] Partiendo de b) El patrón deformado.     []---------------------
    f_salida = fopen(".\\Salida\\datos_salida_1b.txt", "w");
    f_solapamiento = fopen(".\\Salida\\solapamiento_1b.txt", "w");
    copiar_matriz(array_patrones[0], matriz_nodos);
    alterar_nodos(matriz_nodos,0.5);
    algoritmo_hopfield(step_number, temp, matriz_nodos, array_patrones, f_salida, f_solapamiento, 1);
    
    // EJERCICIO 2: Hacer lo anterior pero para diferentes temperaturas, de manera que se pueda obtener la curva de 
    // solapamiento frente a temperatura.

    for (int k=0; k<10; k++) {
        char extension[12], number[24], filename[64];
        double temp_nueva = temp * pow(2.0,k);
        //Creamos el nombre de la extensión y del archivo
        strcpy(extension, ".txt");
        strcpy(filename, ".\\Salida\\datos_salida_2_T");
        //Guardamos el k actual en un string
        sprintf(number, "%f", temp_nueva);
        //Lo juntamos todo
        strcat(filename, number);
        strcat(filename, extension);
        f_salida = fopen(filename, "w");

        //Creamos el nombre del archivo
        strcpy(filename, ".\\Salida\\solapamiento_2_T");
        //Lo juntamos todo
        strcat(filename, number);
        strcat(filename, extension);
        f_solapamiento = fopen(filename, "w");
        inicializar_nodos(matriz_nodos, -1);
        algoritmo_hopfield(step_number, temp_nueva, matriz_nodos, array_patrones, f_salida, f_solapamiento, 1);
    }

    // EJERCICIO 3: Hacer lo mismo que en el punto 1 y 2 pero para varios patrones.
    int n_patrones = 4;
    f_patrones = fopen("letras.txt", "r");
    leer_patrones(n_patrones, array_patrones, f_patrones);
    // ----------------[] Partiendo de a) Una condición aleatoria. []---------------------
    for (int mu=0; mu<n_patrones; mu++){
        char mu_extension[10];
        sprintf(mu_extension, "%d", mu);
        for (int k=0; k<10; k++) {
            char extension[12], number[24], filename[64];
            double temp_nueva = temp * pow(2.0,k);
            //Creamos el nombre de la extensión y del archivo
            strcpy(extension, ".txt");
            strcpy(filename, ".\\Salida\\datos_salida_3_a_T");
            //Guardamos el k actual en un string
            sprintf(number, "%f", temp_nueva);
            sprintf(mu_extension, "%d", mu);
            //Lo juntamos todo
            strcat(filename, number);
            strcat(filename, "PATRON_");
            strcat(filename, mu_extension);
            strcat(filename, extension);
            f_salida = fopen(filename, "w");

            //Creamos el nombre del archivo
            strcpy(filename, ".\\Salida\\solapamiento_3_a_T");
            //Lo juntamos todo
            strcat(filename, number);
            strcat(filename, "_PATRON_");
            strcat(filename, mu_extension);
            strcat(filename, extension);
            f_solapamiento = fopen(filename, "w");
            inicializar_nodos(matriz_nodos, -1);
            algoritmo_hopfield(step_number, temp_nueva, matriz_nodos, array_patrones, f_salida, f_solapamiento, 1);
        }
    
    // ----------------[] Partiendo de b) El patrón deformado.     []---------------------
        for (int k=0; k<10; k++) {
            char extension[12], number[24], filename[64];
            double temp_nueva = temp * pow(2.0,k);
            //Creamos el nombre de la extensión y del archivo
            strcpy(extension, ".txt");
            strcpy(filename, ".\\Salida\\datos_salida_3_b_T");
            //Guardamos el k y mu actual en un string
            sprintf(number, "%f", temp_nueva);
            //Lo juntamos todo
            strcat(filename, number);
            strcat(filename, "_PATRON_");
            strcat(filename, mu_extension);
            strcat(filename, extension);
            f_salida = fopen(filename, "w");

            //Creamos el nombre del archivo
            strcpy(filename, ".\\Salida\\solapamiento_3_b_T");
            //Lo juntamos todo
            strcat(filename, number);
            strcat(filename, "_PATRON_");
            strcat(filename, mu_extension);
            strcat(filename, extension);
            f_solapamiento = fopen(filename, "w");

            //Copiamos y alteramos
            copiar_matriz(array_patrones[mu], matriz_nodos);
            alterar_nodos(matriz_nodos,0.5);
            algoritmo_hopfield(step_number, temp_nueva, matriz_nodos, array_patrones, f_salida, f_solapamiento, 1);
        }
    }
    
    return 1;
}

void leer_patrones(int n_patrones_total, int array_patrones[][N_MAX][N_MAX], FILE* f_patrones) {
    for (int patron_n = 0; patron_n < n_patrones_total; patron_n++){
        for (int i = 0; i < N_MAX; i++) {
            for (int j = 0; j < N_MAX; j++) {
                fscanf(f_patrones, "%1d", &array_patrones[patron_n][i][j]);
            }
        }
        fscanf(f_patrones, "%*[^\n]\n");
    }
    fclose(f_patrones);
}

void copiar_matriz(int matriz_origen[][N_MAX], int matriz_destino[][N_MAX]) {
    for (int i=0; i<N_MAX; i++) {
        for (int j=0; j<N_MAX; j++) {
            matriz_destino[i][j] = matriz_origen[i][j];
        }
    }
}

void algoritmo_hopfield(int step_number, double temp, int matriz_nodos[][N_MAX], int array_patrones[][N_MAX][N_MAX], FILE* f_salida_matriz, FILE* f_salida_solapamiento, int n_patrones) {
    int monte_carlo_step = N_MAX*N_MAX;
    
    //Guardamos el resultado visual incial
    print_matriz(f_salida_matriz, matriz_nodos);
    //Calculamos el solapamiento
    fprintf(f_salida_solapamiento, "%f\n", solapamiento(matriz_nodos, array_patrones, 0));

    //Ahora hacemos el bucle principal para step_number pasos Montecarlo
    for (int l=0; l < step_number; l++) {
        for (int k=0; k < monte_carlo_step; k++) {
            int n = (rand() % N_MAX);
            int m = (rand() % N_MAX);

            double p = fmin(1, exp(-incremento_energia(matriz_nodos,array_patrones,n,m,n_patrones)/temp));

            //Generamos un número aleatorio entre 0 y 1.
            double chi = (rand() % 10001) / 10000.0;
            if (chi < p) {
                matriz_nodos[n][m] = 1-matriz_nodos[n][m];
            }
        }
        //Guardamos el resultado visual tras un paso monte carlo
        print_matriz(f_salida_matriz, matriz_nodos);
        //Calculamos el solapamiento
        fprintf(f_salida_solapamiento, "%f\n", solapamiento(matriz_nodos, array_patrones, 0));
    }

    //Close the files
    fclose(f_salida_matriz);
    fclose(f_salida_solapamiento);
}

// ------ FUNCIONES ESTADÍSTICAS --------
double incremento_energia(int matriz_nodos[][N_MAX], int array_patron[][N_MAX][N_MAX], int i, int j, int n_patrones) {
    double suma = 0.0;
    for (int k=0;k<N_MAX;k++) {
        for (int l=0;l<N_MAX;l++){
            suma += omega(i,j,k,l,n_patrones,array_patron)*s(matriz_nodos,k,l);
        }
    }
    suma *= s(matriz_nodos,i,j) - 0.5;
    suma += theta(i,j,n_patrones,array_patron) *(1-2*s(matriz_nodos,i,j));
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
                matriz_nodos[i][j] = (rand() % 10001) / 10000.0;
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

double theta(int i, int j, int n_patrones, int array_patron[][N_MAX][N_MAX]){
    double suma = 0.0;
    for (int k=0; k<N_MAX; k++) {
        for (int l=0; l<N_MAX; l++) {
            suma += omega(i,j,k,l,n_patrones, array_patron);
        }
    }
    return 0.5*suma;
}

double omega(int i, int j, int k, int l, int n_patrones, int array_patron[][N_MAX][N_MAX]) {
    if ((i==k) && (j==l)){
        return 0.0;
    } else {
        double suma = 0.0;
        for (int mu=0; mu<n_patrones; mu++) {
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

double solapamiento(int matriz_nodos[][N_MAX], int array_patron[][N_MAX][N_MAX], int patron_n) {
    double suma = 0.0;
    double a_mu = a(patron_n, array_patron);
    for (int i=0; i<N_MAX; i++) {
        for (int j=0; j<N_MAX; j++) {
            suma += (xi(patron_n,i,j,array_patron)-a_mu) * (s(matriz_nodos, i, j)-a_mu);
        }
    }
    suma *= 1.0/(N_MAX*N_MAX*a_mu*(1.0-a_mu));
    return suma;
}