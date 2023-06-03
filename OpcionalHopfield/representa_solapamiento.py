import os
import matplotlib.pyplot as plt
import numpy as np

def leer_archivo(nombre_archivo):
    with open(nombre_archivo, 'r') as archivo:
        lineas = archivo.readlines()
        y = []
        for linea in lineas:
            y.append(float(linea.strip()))
        return y

def graficar_solapamiento(carpeta):
    fig, ax = plt.subplots()
    archivos = os.listdir(carpeta)
    for archivo in archivos:
        ruta_archivo = os.path.join(carpeta, archivo)
        if os.path.isfile(ruta_archivo):
            y = leer_archivo(ruta_archivo)
            x = np.arange(len(y))
            ax.plot(x, y, label=archivo)
    
    ax.set_xlabel('Eje X')
    ax.set_ylabel('Eje Y')
    ax.legend()
    plt.show()

carpeta_salida = 'Solapamientos'
graficar_solapamiento(carpeta_salida)