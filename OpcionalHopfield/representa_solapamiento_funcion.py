import os
import matplotlib.pyplot as plt
import numpy as np

plt.style.use('seaborn')

def leer_archivo(nombre_archivo):
    with open(nombre_archivo, 'r') as archivo:
        lineas = archivo.readlines()
        valores = [abs(float(linea.strip())) for linea in lineas[-4:]]
        solapamiento_final = np.mean(valores)
        error = 3.0*np.std(valores)/(len(valores))
        temperatura = float(nombre_archivo.split('_T')[1][:-4])
        return solapamiento_final, temperatura, error

def graficar_solapamiento(carpeta):
    carpeta = os.path.join(os.path.dirname(__file__),carpeta)
    solapamientos = []
    temperaturas = []
    errors = []
    for archivo in os.listdir(carpeta):
        ruta_archivo = os.path.join(carpeta, archivo)
        if os.path.isfile(ruta_archivo):
            solapamiento_final, temperatura, error = leer_archivo(ruta_archivo)
            solapamientos.append(solapamiento_final)
            temperaturas.append(temperatura)
            errors.append(error)


    plt.errorbar(temperaturas, solapamientos, errors, marker='o', linestyle='None', color='black', capsize=3.0, capthick=2, markersize = 3)

    plt.vlines(temperaturas, 0, solapamientos, linestyle=(0, (1, 3)))
    plt.xlabel('Temperatura')
    plt.ylabel('Solapamiento Final (Promedio últimos 4 valores)')
    plt.title('Solapamiento Final frente a la Temperatura')
    plt.grid(True)
    plt.savefig("Solapamientos_Frente_a_T.png")
    plt.show()
    

carpeta_salida = 'Solapamientos'
graficar_solapamiento(carpeta_salida)