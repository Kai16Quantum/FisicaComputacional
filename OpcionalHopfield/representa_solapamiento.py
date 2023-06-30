import os
import matplotlib.pyplot as plt
import numpy as np

def leer_archivo(nombre_archivo):
    with open(nombre_archivo, 'r') as archivo:
        lineas = archivo.readlines()
        y = []
        for linea in lineas:
            y.append(abs(float(linea.strip())))
        
        # Extraer la temperatura del nombre del archivo
        temperatura = float(nombre_archivo.split('_T')[1][:-4])
        
        return y, temperatura

def graficar_solapamiento(carpeta):
    carpeta = os.path.join(os.path.dirname(__file__),carpeta)
    fig, ax = plt.subplots()
    archivos = os.listdir(carpeta)
    separacion_texto = 0.08
    separacion_linea = 0.02
    max_x = 0
    temperaturas = []
    
    for archivo in archivos:
        ruta_archivo = os.path.join(carpeta, archivo)
        if os.path.isfile(ruta_archivo):
            y, temperatura = leer_archivo(ruta_archivo)
            x = np.arange(len(y))
            max_x = max(max_x, len(x))
            temperaturas.append((temperatura, archivo, x, y))
    
    temperaturas.sort(key=lambda x: x[0], reverse=True)  # Sort temperatures in reverse order
    
    for i, (temperatura, archivo, x, y) in enumerate(temperaturas):
        linea, = ax.plot(x, y, label=archivo)
        
        # Agregar línea de color junto al texto
        color = linea.get_color()
        ax.add_patch(plt.Rectangle((max_x, i * separacion_texto - separacion_linea), 0.25, separacion_linea*2, color=color))
        
        # Añadir texto fuera del gráfico
        texto_temperatura = f'T = {temperatura:.6f}'
        ax.text(max_x + 0.5, i * separacion_texto, texto_temperatura, ha='left', va='center')

    ax.set_xlabel('Número de pasos Monte Carlo (N^2)')
    ax.set_ylabel('Solapamiento')

    # Remove the right border
    ax.spines['right'].set_visible(False)

    plt.savefig(os.path.join(os.path.dirname(__file__),"Salida/Solapamiento_Para_cada_T.png"))
    plt.show()

carpeta_salida = 'Solapamientos'
graficar_solapamiento(carpeta_salida)