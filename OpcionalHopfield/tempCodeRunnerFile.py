import os
import matplotlib.pyplot as plt
import numpy as np

plt.style.use('seaborn')


def leer_archivo(nombre_archivo):
    with open(nombre_archivo, 'r') as archivo:
        lineas = archivo.read().splitlines()

        valores = []
        for linea in lineas:
            if linea.strip():  # Ignorar líneas vacías
                valor = float(linea.strip().split(',')[1])
                valores.append(abs(valor))

        patron_total = int(nombre_archivo.split('_')[2])
        experiencia = int(nombre_archivo.split('_')[4].split('.')[0])-1
        solapamientos_finales = valores[-patron_total:]
        return solapamientos_finales, patron_total, experiencia


def obtener_numero_patrones_recordados(solapamientos, patrones_totales):
    numeros_recordados = {}

    for patron_total in range(1, patrones_totales + 1):
        numero_recordado = 0
        num_experiencias = min(patron_total, len(solapamientos[patron_total]))

        for experiencia in range(num_experiencias):
            array_valores = solapamientos[patron_total][experiencia]
            if (array_valores) > 0.75:
                numero_recordado += 1

        numeros_recordados[patron_total] = numero_recordado/patron_total

    return numeros_recordados


def graficar_solapamiento(carpeta):
    carpeta = os.path.join(os.path.dirname(__file__), carpeta)
    solapamientos = {}

    for archivo in os.listdir(carpeta):
        ruta_archivo = os.path.join(carpeta, archivo)
        if os.path.isfile(ruta_archivo) and archivo.startswith('solapamiento_4'):
            solapamientos_finales, patron_total, experiencia = leer_archivo(ruta_archivo)

            if experiencia < patron_total:
                if patron_total in solapamientos:
                    solapamientos[patron_total].append(abs(solapamientos_finales[experiencia]))
                else:
                    solapamientos[patron_total] = [abs(solapamientos_finales[experiencia])]

    numeros_recordados = obtener_numero_patrones_recordados(solapamientos, len(solapamientos))

    fig, ax = plt.subplots(figsize=(8, 6))
    for patron_total in range(1, len(solapamientos) + 1):
        ax.plot(
            patron_total,
            numeros_recordados[patron_total],
            marker='o',
            linestyle='-',
            markersize=9,
            label=f'Nº de Patrones Totales: {patron_total}'
        )

    plt.xlabel('Nº de Patrones')
    plt.ylabel('Patrones Recordados')
    plt.title('Patrones Recordados frente al Nº de Patrones Totales')
    plt.grid(True)
    plt.legend(False)
    plt.savefig("Patrones_Recordados.png")
    plt.show()


carpeta_salida = '4'
graficar_solapamiento(carpeta_salida)
