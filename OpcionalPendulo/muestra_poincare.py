import numpy as np
import matplotlib.pyplot as plt
import glob

# Leemos los archivos
poincare = True

#Si "poincare = false", no añadimos los puntos simétricos y la escala es un poco diferente. Este programa sirve su función de mostrar diagramas de fase y mapas de poincaré.

if poincare:
    archivos = glob.glob("Datos_POINCARE/E15/datos_salidaEnergia_*.txt")
else:
    archivos = glob.glob("Datos/E15/datos_salidaEnergia_*.txt")

plt.style.use('dark_background')

# 0:Phi / 1:Theta / 2:P_PHI / 3:P_THETA / 4:PHI PUNTO / 5:THETA PUNTO
dato_eje_x = 0
dato_eje_y = 2
nombre_dato_eje_x = "Phi"
nombre_dato_eje_y = "P_Phi"
datos_por_energia = {}

# Para todos los archivos:
for archivo in archivos:
    # Leemos el nombre.
    nombre_archivo = archivo.split("datos")[-1] 
    # datos_salidaEnergia_15.000000_EXPERIENCIA_2.txt
    partes_nombre = nombre_archivo.split("_")
    # [datos, salidaEnergia, 15.000000, EXPERIENCIA, 2.txt]
    energia_str = partes_nombre[2]
    experiencia_str = partes_nombre[4].split(".")[0]
    energia = float(energia_str)
    experiencia = float(experiencia_str)

    # Leemos.
    data = np.genfromtxt(archivo, delimiter=",")
    phi = data[:, dato_eje_x]
    theta = data[:, dato_eje_y]

    if energia not in datos_por_energia:
        datos_por_energia[energia] = {}
    if experiencia not in datos_por_energia[energia]:
        datos_por_energia[energia][experiencia] = (phi, theta)

# Mostramos los puntos.
for energia, datos_experiencia in datos_por_energia.items():
    for experiencia, datos in datos_experiencia.items():
        phi, theta = datos
        plt.scatter(phi, theta, s=1, label=f"Energía: {energia:.0f} J, Experiencia: {experiencia:.0f}")
        if (poincare):
            plt.scatter(-phi, theta, s=1)  # Agregamos el punto simétrico

#Creamos el plot.
plt.xlabel(nombre_dato_eje_x)
plt.ylabel(nombre_dato_eje_y)

if (poincare):
    plt.xlim(-3.14, 3.14)
    plt.ylim(-12, 12)

plt.legend()
plt.savefig("poincare.png")
plt.show()
