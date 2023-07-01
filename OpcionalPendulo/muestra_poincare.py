import numpy as np
import matplotlib.pyplot as plt
import glob

# Obtener la lista de archivos que coinciden con el patrón en la carpeta "Salida"
archivos = glob.glob("Datos_POINCARE/datos_salidaEnergia_*.txt")
plt.style.use('dark_background')

# Phi / Theta / P_PHI / P_THETA / PHI PUNTO / THETA PUNTO
dato_eje_x = 0
dato_eje_y = 4
nombre_dato_eje_x = "Phi"
nombre_dato_eje_y = "Phi Punto"

# Diccionario para almacenar los datos por energía
datos_por_energia = {}

# Iterar sobre los archivos encontrados
for archivo in archivos:
    # Obtener la energía y experiencia del nombre de archivo
    nombre_archivo = archivo.split("/")[-1]  # Obtener solo el nombre del archivo sin la ruta
    partes_nombre = nombre_archivo.split("_")
    energia_str = partes_nombre[3]
    experiencia_str = partes_nombre[5].split(".")[0]
    energia = float(energia_str)
    experiencia = float(experiencia_str)

    # Leer los datos del archivo
    data = np.genfromtxt(archivo, delimiter=",")

    # Extraer las columnas correspondientes a phi y theta
    phi = data[:, dato_eje_x]
    theta = data[:, dato_eje_y]

    # Almacenar los datos en el diccionario por energía y experiencia
    if energia not in datos_por_energia:
        datos_por_energia[energia] = {}
    if experiencia not in datos_por_energia[energia]:
        datos_por_energia[energia][experiencia] = (phi, theta)

# Plot de los mapas de Poincaré para cada energía y experiencia
for energia, datos_experiencia in datos_por_energia.items():
    for experiencia, datos in datos_experiencia.items():
        phi, theta = datos
        plt.scatter(phi, theta, s=1, label=f"Energía: {energia:.0f} J, Experiencia: {experiencia:.0f}")
        plt.scatter(-phi, theta, s=1)  # Agregar el punto simétrico

plt.xlabel(nombre_dato_eje_x)
plt.ylabel(nombre_dato_eje_y)

# Configurar límites de los ejes
plt.xlim(-3.14, 3.14)
plt.ylim(-12, 12)

plt.legend()

plt.savefig("poincare.png")
plt.show()
