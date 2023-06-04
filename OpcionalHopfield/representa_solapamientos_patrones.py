import os
import matplotlib.pyplot as plt
import numpy as np

def leer_archivo(nombre_archivo):
    with open(nombre_archivo, 'r') as archivo:
        lineas = archivo.readlines()
        solapamientos = [abs(float(linea.strip())) for linea in lineas]
        temperatura = float(nombre_archivo.split('_T')[1].split('_PATRON')[0][1:])
        patron = int(nombre_archivo.split('_PATRON_')[1].split('.')[0])
        return solapamientos, temperatura, patron

def graficar_patrones(carpeta):
    fig, ax = plt.subplots()
    archivos = os.listdir(carpeta)
    temperaturas = []
    patrones = []

    for archivo in archivos:
        ruta_archivo = os.path.join(carpeta, archivo)
        if os.path.isfile(ruta_archivo):
            solapamientos, temperatura, patron = leer_archivo(ruta_archivo)
            x = np.arange(len(solapamientos))
            temperaturas.append(temperatura)
            patrones.append(patron)
            ax.plot(x, solapamientos, label=f'T = {temperatura:.6f}', color=f'C{patron}')

    ax.set_xlabel('Eje X')
    ax.set_ylabel('Eje Y')
    ax.legend()
    plt.show()

carpeta_salida = 'Solapamientos_Patrones'
graficar_patrones(carpeta_salida)