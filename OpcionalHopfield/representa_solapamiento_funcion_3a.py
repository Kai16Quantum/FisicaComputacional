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
        patron = int(nombre_archivo.split('_PATRON_')[1].split('_EXPERIENCIA')[0])
        experiencia = int(nombre_archivo.split('_EXPERIENCIA_')[1].split('.')[0])
        return solapamiento_final, temperatura, error, patron, experiencia

def graficar_solapamiento(carpeta):
    carpeta = os.path.join(os.path.dirname(__file__), carpeta)
    solapamientos = []
    temperaturas = []
    errors = []
    patrones = []
    experiencias = []
    legend_entries = []

    for archivo in os.listdir(carpeta):
        ruta_archivo = os.path.join(carpeta, archivo)
        if os.path.isfile(ruta_archivo):
            solapamiento_final, temperatura, error, patron, experiencia = leer_archivo(ruta_archivo)
            solapamientos.append(solapamiento_final)
            temperaturas.append(temperatura)
            errors.append(error)
            patrones.append(patron)
            experiencias.append(experiencia)

    max_pattern = max(patrones)
    colors = plt.cm.get_cmap('tab10')(np.linspace(0, 1, max_pattern + 1))

    plt.figure(figsize=(8, 6))

    for i, (solapamiento, temperatura, patron) in enumerate(zip(solapamientos, temperaturas, patrones)):
        indices = [j for j, p in enumerate(patrones) if p == patron]
        if patron not in [entry[0] for entry in legend_entries]:
            plt.errorbar(
                [temperaturas[j] for j in indices],
                [solapamientos[j] for j in indices],
                [errors[j] for j in indices],
                marker='o',
                color=colors[patron],
                linestyle='None',
                label=f'Patrón {patron}',
                capsize=3.0,
                capthick=2,
                markersize=3
            )
            legend_entries.append((patron, colors[patron]))
        else:
            plt.errorbar(
                [temperaturas[j] for j in indices],
                [solapamientos[j] for j in indices],
                [errors[j] for j in indices],
                marker='o',
                color=colors[patron],
                linestyle='None',
                capsize=3.0,
                capthick=2,
                markersize=3
            )

    handles = [plt.Line2D([0], [0], marker='o', color=color, markersize=3, linestyle='None') for _, color in legend_entries]
    labels = [f'Patrón {patron}' for patron, _ in legend_entries]
    plt.xlabel('Temperatura')
    plt.ylabel('Solapamiento Final (Promedio últimos 4 valores)')
    plt.title('Solapamiento Final frente a la Temperatura')
    plt.grid(True)
    plt.legend(handles, labels)
    plt.xscale('log')
    plt.savefig("3b_Solapamientos.png")
    plt.show()


carpeta_salida = '3b'
graficar_solapamiento(carpeta_salida)
