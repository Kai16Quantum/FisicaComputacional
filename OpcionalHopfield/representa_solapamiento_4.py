import os
import matplotlib.pyplot as plt
import numpy as np

plt.style.use('seaborn')

#Leemos los datos de entrada
def leer_archivo(nombre_archivo):
    with open(nombre_archivo, 'r') as archivo:
        lineas = archivo.read().splitlines()

        valores = []
        for linea in lineas:
            if linea.strip():
                valor = float(linea.strip().split(',')[1])
                valores.append(abs(valor))

        #[solapamiento, 4, 1, EXPERIENCIA, 0.txt]
        patron_total = int(nombre_archivo.split('_')[2])
        experiencia = int(nombre_archivo.split('_')[4].split('.')[0])-1

        #Tomamos los últimos valores, correspondiente al número de patrones en memoria.
        solapamientos_finales = valores[-patron_total:]
        return solapamientos_finales, patron_total, experiencia


def obtener_numero_patrones_recordados(solapamientos, patrones_totales):
    numeros_recordados = {}

    # Contamos el número de patrones recordados si el solapamiento de la experiencia concreta que se ha modificado se recuerda.
    for patron_total in range(1, patrones_totales + 1):
        numero_recordado = 0
        num_experiencias = min(patron_total, len(solapamientos[patron_total]))

        for experiencia in range(num_experiencias):
            array_valores = solapamientos[patron_total][experiencia]
            if (array_valores) > 0.75:
                # Se ha recordado, así que se suma 1 al número total.
                numero_recordado += 1

        numeros_recordados[patron_total] = numero_recordado

    return numeros_recordados


#Obtenemos la pendiente con la librería Polyfit de Python.
def calcular_pendiente(x, y):
    coeficientes = np.polyfit(x, y, 1)
    pendiente = coeficientes[0]
    return pendiente


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

    x_values = list(numeros_recordados.keys())
    y_values = list(numeros_recordados.values())

    pendiente = calcular_pendiente(x_values, y_values)

    fig, ax = plt.subplots(figsize=(8, 6))
    ax.plot(
        x_values,
        y_values,
        marker='o',
        linestyle='-',
        markersize=9
    )

    # Agregamos la línea con las condiciones iniciales.
    x_line = np.array([min(x_values), max(x_values)])
    y_line = 1.0+pendiente * x_line
    ax.plot(x_line, y_line, linestyle='--', color='green', label=f'Pendiente: {pendiente:.4f}')
    ax.legend()

    plt.xlabel('Nº de Patrones')
    plt.ylabel('Patrones Recordados')
    plt.title('Patrones Recordados frente al Nº de Patrones Totales')
    plt.grid(True)

    plt.savefig("Patrones_Recordados.png")
    plt.show()


carpeta_salida = '4'
graficar_solapamiento(carpeta_salida)
