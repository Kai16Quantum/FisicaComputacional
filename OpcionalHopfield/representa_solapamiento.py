import os
import matplotlib.pyplot as plt
import numpy as np

plt.style.use('seaborn')

#Leemos.
def leer_archivo(nombre_archivo):
    with open(nombre_archivo, 'r') as archivo:
        lineas = archivo.readlines()
        y = []
        for linea in lineas:
            y.append(abs(float(linea.strip())))
        
        # Obtenemos la temperatura del nombre.
        temperatura = float(nombre_archivo.split('_T')[1][:-4])
        return y, temperatura

def graficar_solapamiento(carpeta):
    carpeta = os.path.join(os.path.dirname(__file__),carpeta)
    fig, ax = plt.subplots()
    archivos = os.listdir(carpeta)
    max_x = -20
    temperaturas = []
    
    for archivo in archivos:
        ruta_archivo = os.path.join(carpeta, archivo)
        if os.path.isfile(ruta_archivo):
            y, temperatura = leer_archivo(ruta_archivo)
            x = np.arange(len(y))
            max_x = max(max_x, len(x))
            temperaturas.append((temperatura, archivo, x, y))
    
    #Ordenamos las temperaturas
    temperaturas.sort(key=lambda x: x[0], reverse=True)
    

    ax.set_xlabel('Número de pasos Monte Carlo (N^2)')
    ax.set_ylabel('Solapamiento')

    leyenda = []
    for i, (temperatura, archivo, x, y) in enumerate(temperaturas):
        linea, = ax.plot(x, y)
        nombre_leyenda = f'T = {temperatura:.3f}K'
        #Creamos la leyenda con colores-
        leyenda.append(nombre_leyenda)

    ax.legend(leyenda, loc='best', frameon=True, framealpha=0.8)
    plt.grid(True)
    plt.savefig(os.path.join(os.path.dirname(__file__),"Salida/Solapamiento_Para_cada_T.png"))
    plt.show()

carpeta_salida = 'Solapamientos'
graficar_solapamiento(carpeta_salida)