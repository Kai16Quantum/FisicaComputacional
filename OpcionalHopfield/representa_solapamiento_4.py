import os
import matplotlib.pyplot as plt
import numpy as np

plt.style.use('seaborn')


def leer_archivo(nombre_archivo):
    with open(nombre_archivo, 'r') as archivo:
        lineas = archivo.readlines()
        valores = [abs(float(linea.strip())) for linea in lineas[-4:]]
        solapamiento_final = np.mean(valores)
        patron_total = int(nombre_archivo.split('_')[2])
        patron = int(nombre_archivo.split('_')[4])
        experiencia = int(nombre_archivo.split('_')[6].split('.')[0])
        return solapamiento_final, patron_total, patron, experiencia


def obtener_promedio_ultimos_4(solapamientos, patrones_totales):
    promedios_ultimos_4 = []
    errors = []
    for patron_total in range(1,patrones_totales+1):
        array_valores = solapamientos[patron_total]
        promedio = np.mean(array_valores)
        promedios_ultimos_4.append(promedio)
        errors.append(2.0*np.std(array_valores)/np.sqrt(len(solapamientos)))
    return np.array(promedios_ultimos_4), errors


def graficar_solapamiento(carpeta):
    carpeta = os.path.join(os.path.dirname(__file__), carpeta)
    solapamientos = {}


    for archivo in os.listdir(carpeta):
        ruta_archivo = os.path.join(carpeta, archivo)
        if os.path.isfile(ruta_archivo):
            solapamiento_final, patron_total, patron, experiencia = leer_archivo(ruta_archivo)
            
            if (experiencia == 0):
                if patron_total in solapamientos:
                    solapamientos[patron_total] = np.append(solapamientos[patron_total],solapamiento_final)
                else:
                    solapamientos[patron_total] = np.array([abs(solapamiento_final)])


    #promedios_ultimos_4, errors = obtener_promedio_ultimos_4(solapamientos, len(solapamientos))
    #Contamos el numero de patrones recordados
    numeros_recordados = {}
    errors = []
    for patron_total in range(1,len(solapamientos)+1):
        numero_recordado = 0
        array_valores = solapamientos[patron_total]
        
        for valor in array_valores:
            if valor > 0.75:
                numero_recordado += 1
        numeros_recordados[patron_total] = numero_recordado
        errors.append(2.0*np.std(array_valores)/np.sqrt(len(solapamientos)))

    fig, ax = plt.subplots(figsize=(8, 6))
    for patron_total in range(1, len(solapamientos)+1):
        ax.errorbar(
            patron_total,
            numeros_recordados[patron_total],
            errors[patron_total-1],
            marker='o',
            linestyle='None',
            capsize=3.0,
            capthick=2,
            markersize=3,
            label=f'Nº de Patrones Totales: {patron_total}'
        )


    handles, labels = ax.get_legend_handles_labels()
    handles.append(plt.Line2D([0], [0], color='black', linewidth=2))
    labels.append('Promedio últimos 4')

    plt.xlabel('Nº de Patrones')
    plt.ylabel('Solapamiento Final')
    plt.title('Solapamiento Final frente al Nº de Patrones Totales')
    plt.grid(True)
    plt.legend(handles, labels)
    plt.savefig("Solapamiento_4.png")
    plt.show()


carpeta_salida = '4'
graficar_solapamiento(carpeta_salida)
