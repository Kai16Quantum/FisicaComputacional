import os
import matplotlib.pyplot as plt
import numpy as np

plt.style.use('seaborn')

#Leemos.
def leer_archivo(nombre_archivo):
    with open(nombre_archivo, 'r') as archivo:
        lineas = archivo.readlines()
        valores = [abs(float(linea.strip())) for linea in lineas[-4:]]
        solapamiento_final = np.mean(valores)
        # Obtenemos el error con la fórmula que incluye la desviación típica.
        error = 3.0 * np.std(valores) / len(valores)
        # solapamiento_3_b_T0.001000_PATRON_0_EXPERIENCIA_1
        temperatura = float(nombre_archivo.split('_T')[1].split('_PATRON')[0])
        patron = int(nombre_archivo.split('_PATRON_')[1].split('_EXPERIENCIA')[0])
        experiencia = int(nombre_archivo.split('_EXPERIENCIA_')[1].split('.')[0])
        return solapamiento_final, temperatura, error, patron, experiencia

def graficar_solapamiento(carpeta):
    carpeta = os.path.join(os.path.dirname(__file__), carpeta)
    solapamientos = []
    temperaturas = []
    errores = []
    patrones = []
    experiencias = []
    leyenda = []

    for archivo in os.listdir(carpeta):
        ruta_archivo = os.path.join(carpeta, archivo)
        if os.path.isfile(ruta_archivo):
            solapamiento_final, temperatura, error, patron, experiencia = leer_archivo(ruta_archivo)
            solapamientos.append(solapamiento_final)
            temperaturas.append(temperatura)
            errores.append(error)
            patrones.append(patron)
            experiencias.append(experiencia)

    maximo_patrones = max(patrones)
    colores = plt.cm.get_cmap('tab10')(np.linspace(0, 1, maximo_patrones + 1))

    plt.figure(figsize=(8, 6))

    for i, (solapamiento, temperatura, patron) in enumerate(zip(solapamientos, temperaturas, patrones)):
        indices = [j for j, p in enumerate(patrones) if p == patron]
        if patron not in [entry[0] for entry in leyenda]:
            #Colocamos el punto con su error.
            plt.errorbar(
                [temperaturas[j] for j in indices],
                [solapamientos[j] for j in indices],
                [errores[j] for j in indices],
                marker='o',
                color=colores[patron],
                linestyle='None',
                label=f'Patrón {patron}',
                capsize=3.0,
                capthick=2,
                markersize=3
            )
            leyenda.append((patron, colores[patron]))
        else:
            plt.errorbar(
                [temperaturas[j] for j in indices],
                [solapamientos[j] for j in indices],
                [errores[j] for j in indices],
                marker='o',
                color=colores[patron],
                linestyle='None',
                capsize=3.0,
                capthick=2,
                markersize=3
            )

    puntos = [plt.Line2D([0], [0], marker='o', color=color, markersize=3, linestyle='None') for _, color in leyenda]
    nombres = [f'Patrón {patron}' for patron, _ in leyenda]

    plt.xlabel('Temperatura')
    plt.ylabel('Solapamiento Final (Promedio últimos 4 valores)')
    plt.title('Solapamiento Final frente a la Temperatura')
    plt.grid(True)
    plt.legend(puntos, nombres)

    #Se utiliza una escala logarítmica, al aumentar las temperaturas en múltiplos de 2.
    plt.xscale('log')
    
    plt.savefig("3b_Solapamientos.png")
    plt.show()


carpeta_salida = '3b'
graficar_solapamiento(carpeta_salida)
