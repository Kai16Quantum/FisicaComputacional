import numpy as np
import matplotlib.pyplot as plt

archivo = 'Datos/datos_lyapunovEnergia_1.000000_EXPERIENCIA_1.txt'

# Leemos.
with open(archivo, 'r') as f:
    lineas = f.readlines()

# Cada archivo tendrá un número de experiencias separadas por #, como se puede ver en el archivo e la simulación.
bloques = []
bloque_actual = []
for linea in lineas:
    linea = linea.strip()
    if linea.startswith('#'):
        if bloque_actual:
            bloques.append(bloque_actual)
            bloque_actual = []
    else:
        bloque_actual.append(linea)
if bloque_actual:
    bloques.append(bloque_actual)

# Filtrar las líneas dentro de cada bloque
datos_filtrados = []
for bloque in bloques:
    datos_bloque = np.genfromtxt(bloque, delimiter=',')
    datos_filtrados.append(datos_bloque)

# Calcular los coeficientes de Lyapunov para cada bloque
coeficientes_lyapunov = []
tiempos = []
for datos_bloque in datos_filtrados:
    tiempo = datos_bloque[:, 0]
    angulo1 = datos_bloque[:, 1]
    angulo2 = datos_bloque[:, 2]
    angulo1_punto = datos_bloque[:, 3]
    angulo2_punto = datos_bloque[:, 4]
    distancia_1 = np.sqrt((angulo1_punto[0] - angulo1[0]) ** 2 + (angulo2_punto[0] - angulo2[0]) ** 2)
    distancia_2 = np.sqrt((angulo1_punto[-1] - angulo1[-1]) ** 2 + (angulo2_punto[-1] - angulo2[-1]) ** 2)
    coeficiente = np.log(distancia_2 / distancia_1) / (tiempo[-1] - tiempo[0])
    
    #Filtramos las trayectorias problemáticas.
    if (abs(coeficiente) < 20):
        tiempos.append(tiempo[-1])
        coeficientes_lyapunov.append(coeficiente)


# Graficar los coeficientes de Lyapunov por experiencia
fig, ax = plt.subplots()
ax.plot(tiempos, coeficientes_lyapunov)

media = y=np.nanmean(coeficientes_lyapunov)
ax.axhline(media)



yt = ax.get_yticks() 
yt= np.append(yt,media)

ax.set_xlabel('Tiempo')
ax.set_ylabel('Coeficiente de Lyapunov')
ax.grid(False)

# Mostrar la gráfica
plt.show()
