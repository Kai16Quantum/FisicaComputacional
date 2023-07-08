import numpy as np
import glob
import matplotlib.pyplot as plt

# Leemos los archivos
archivos = glob.glob("Datos/Estabilidad/datos_estabilidadEnergia_*.txt")

distancias_por_energia = {}
línea_final = 200
cambio_angulo_experiencia = 0.14

# Para todos los archivos:
for archivo in archivos:
    # Leemos el nombre.
    nombre_archivo = archivo.split("/")[-1]
    # datos_estabilidadEnergia_1.000000_EXPERIENCIA_2.txt
    partes_nombre = nombre_archivo.split("_")
    # [datos, estabilidadEnergia, 1.000000, EXPERIENCIA, 2.txt]
    energia_str = partes_nombre[2]
    experiencia_str = partes_nombre[4].split(".")[0]
    energia = float(energia_str)
    experiencia = float(experiencia_str)
    distancia_inicial_energia = []

    # Leemos las 200 primeras líneas y nos quedamos con la primera y útlima.
    with open(archivo, 'r') as f:
        lineas_totales = f.readlines()[:línea_final]
        lineas = [lineas_totales[0],lineas_totales[-1]]

    # Obtenemos los valores, al estar separados por comas
    valores = []
    for linea in lineas:
        linea = linea.strip()
        valores_linea = linea.split(",")
        valores_linea = [float(valor) for valor in valores_linea[:-1]]
        valores.append(valores_linea)

    if energia not in distancias_por_energia:
        distancias_por_energia[energia] = []

    # Calcular la distancia final.
    delta_distancia = np.sqrt((valores[1][1] - valores[0][1]) ** 2 + (valores[1][2] - valores[0][2]) ** 2)
    distancias_por_energia[energia].append(delta_distancia)

# Preparar los valores del ángulo inicial para el eje x
angulos_iniciales = np.arange(len(distancias_por_energia)-1) * cambio_angulo_experiencia

# Obtenemos la recta de ajuste con la librería Polyfit de NumPy.
pendientes = []
for energia, distancias in distancias_por_energia.items():
    coeficientes = np.polyfit(angulos_iniciales, distancias, 1)
    pendiente = coeficientes[0]
    pendientes.append(pendiente)

# Ordenamos la gráfica.
energias_ordenadas = sorted(distancias_por_energia.keys())

# Dibujamos.
for energia in energias_ordenadas:
    distancias = distancias_por_energia[energia]
    pendiente = pendientes[energias_ordenadas.index(energia)]
    plt.plot(angulos_iniciales, distancias, label=f"Energía: {energia}")
    
    #Se puede eliminar el comentario de abajo para mostrar la pendiente sobre el último valor de la gráfica.
    #plt.text(angulos_iniciales[-1], distancias[-1], f"Pendiente: {pendiente:.2f}", ha='right', va='center')

plt.xlabel("Ángulo Inicial (radianes)")
plt.ylabel("Incremento de Distancia")
plt.title("Distancias iniciales y finales por experiencia")
plt.legend()
plt.grid(True)
plt.show()
