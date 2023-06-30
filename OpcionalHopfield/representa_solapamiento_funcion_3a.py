import os
import matplotlib.pyplot as plt
import numpy as np

plt.style.use('seaborn')

def leer_archivo(nombre_archivo):
    with open(nombre_archivo, 'r') as archivo:
        lineas = archivo.readlines()
        valores = [abs(float(linea.strip())) for linea in lineas[-4:]]
        solapamiento_final = np.mean(valores)
        error = 3.0 * np.std(valores) / len(valores)
        temperatura = float(nombre_archivo.split('_T')[1].split('_PATRON')[0])
        patron = int(nombre_archivo.split('_PATRON_')[1].split('.')[0])
        return solapamiento_final, temperatura, error, patron


def graficar_solapamiento(carpeta):
    carpeta = os.path.join(os.path.dirname(__file__), carpeta)
    solapamientos = []
    temperaturas = []
    errors = []
    patrones = []
    for archivo in os.listdir(carpeta):
        ruta_archivo = os.path.join(carpeta, archivo)
        if os.path.isfile(ruta_archivo):
            solapamiento_final, temperatura, error, patron = leer_archivo(ruta_archivo)
            solapamientos.append(solapamiento_final)
            temperaturas.append(temperatura)
            errors.append(error)
            patrones.append(patron)

    # Obtener patrones únicos
    patrones_unicos = list(set(patrones))

    # Crear un diccionario para asignar estilos a los patrones
    estilos = {
        0: 'o',
        1: 's',
        2: '^',
        3: 'd',
        # Agregar más estilos para patrones adicionales si es necesario
    }

    plt.figure(figsize=(8, 6))

    for patron_unico in patrones_unicos:
        indices = [i for i, patron in enumerate(patrones) if patron == patron_unico]
        plt.errorbar(
            [temperaturas[i] for i in indices],
            [solapamientos[i] for i in indices],
            [errors[i] for i in indices],
            marker=estilos[patron_unico],
            linestyle='None',
            label=f'Patrón {patron_unico}',
            capsize=3.0,
            capthick=2,
            markersize=3
        )

    plt.xlabel('Temperatura')
    plt.ylabel('Solapamiento Final (Promedio últimos 4 valores)')
    plt.title('Solapamiento Final frente a la Temperatura')
    plt.grid(True)
    plt.legend()
    plt.savefig("3a_Solapamientos.png")
    plt.show()


carpeta_salida = '3a'
graficar_solapamiento(carpeta_salida)
