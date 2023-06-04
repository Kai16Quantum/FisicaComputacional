import os
import matplotlib.pyplot as plt
import numpy as np

def leer_archivo(nombre_archivo):
    with open(nombre_archivo, 'r') as archivo:
        lineas = archivo.readlines()
        valores = [abs(float(linea.strip())) for linea in lineas[-4:]]
        solapamiento_final = np.mean(valores)
        temperatura = float(nombre_archivo.split('_T')[1][:-4])
        return solapamiento_final, temperatura

def graficar_solapamiento(carpeta):
    solapamientos = []
    temperaturas = []

    for archivo in os.listdir(carpeta):
        ruta_archivo = os.path.join(carpeta, archivo)
        if os.path.isfile(ruta_archivo):
            solapamiento_final, temperatura = leer_archivo(ruta_archivo)
            solapamientos.append(solapamiento_final)
            temperaturas.append(temperatura)

    plt.plot(temperaturas, solapamientos, marker='o', linestyle='-', color='b')
    plt.xlabel('Temperatura')
    plt.ylabel('Solapamiento Final (Promedio últimos 4 valores)')
    plt.title('Solapamiento Final frente a la Temperatura')
    plt.grid(True)
    plt.savefig("Solapamientos_Frente_a_T.png")
    plt.show()
    

carpeta_salida = 'Solapamientos'
graficar_solapamiento(carpeta_salida)