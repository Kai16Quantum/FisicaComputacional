import numpy as np
import matplotlib.pyplot as plt
import glob

#Este progrma escribe en el archivo 'media_lyapunov.txt' el coeficiente Lyapunov medio y su error calculado.

archivos = glob.glob("Datos/Lyapunov/datos_lyapunovEnergia_*.txt")


coeficientes_medios = []
for archivo in archivos:
    with open(archivo, 'r') as f:
        lineas = f.readlines()

    # Separamos los bloques divividos por #.
    bloques = []
    bloque_actual = []
    for linea in lineas:
        linea = linea.strip()
        # Filtramos las líneas para eliminar #.
        if linea.startswith('#'):
            if bloque_actual:
                bloques.append(bloque_actual)
                bloque_actual = []
        else:
            bloque_actual.append(linea)
    if bloque_actual:
        bloques.append(bloque_actual)

    datos_filtrados = []
    for bloque in bloques:
        datos_bloque = np.genfromtxt(bloque, delimiter=',')
        datos_filtrados.append(datos_bloque)

    # Calculamos los coeficientes de Lyapunov.
    coeficientes_lyapunov = []
    for datos_bloque in datos_filtrados:
        tiempo = datos_bloque[:, 0]
        angulo1 = datos_bloque[:, 1]
        angulo2 = datos_bloque[:, 2]
        angulo1_punto = datos_bloque[:, 3]
        angulo2_punto = datos_bloque[:, 4]
        distancia_1 = np.sqrt((angulo1_punto[0] - angulo1[0]) ** 2 + (angulo2_punto[0] - angulo2[0]) ** 2)
        distancia_2 = np.sqrt((angulo1_punto[-1] - angulo1[-1]) ** 2 + (angulo2_punto[-1] - angulo2[-1]) ** 2)
        coeficiente = np.log(distancia_2 / distancia_1) / (tiempo[-1] - tiempo[0])
        
        # Filtramos las trayectorias problemáticas
        if abs(coeficiente) < 20:
            coeficientes_lyapunov.append(coeficiente)

    # Calcular el coeficiente de Lyapunov medio para el archivo actual
    coeficiente_medio = np.mean(coeficientes_lyapunov)
    coeficientes_medios.append(coeficiente_medio)

# Obtenemos la media y el error.
media = np.mean(coeficientes_medios)
error = 3 * np.std(coeficientes_medios) / np.sqrt(len(coeficientes_medios))

# Los escribimos en un documento.
with open('media_lyapunov.txt', 'w') as f:
    f.write(f'Media: {media}\n')
    f.write(f'Error: {error}\n')
