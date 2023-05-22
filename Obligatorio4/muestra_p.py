# ================================================================================
# ANIMACION SISTEMA SOLAR
#
# Genera una animación a partir de un fichero de datos con las posiciones
# de los planetas en diferentes instantes de tiempo.
# 
# El fichero debe estructurarse de la siguiente forma:
# 
#   x1_1, y1_1
#   x2_1, y2_1
#   x3_1, y3_1
#   (...)
#   xN_1, yN_1
#   
#   x1_2, y1_2
#   x2_2, y2_2
#   x3_2, y3_2
#   (...)
#   xN_2, yN_2
#
#   x1_3, y1_3
#   x2_3, y2_3
#   x3_3, y3_3
#   (...)
#   xN_3, yN_3
#   
#   (...)
#
# donde xi_j es la componente x del planeta i-ésimo en el instante de
# tiempo j-ésimo, e yi_j lo mismo en la componente y. El programa asume que
# el nº de planetas es siempre el mismo.
# ¡OJO! Los datos están separados por comas.
# 
# Si solo se especifica un instante de tiempo, se genera una imagen en pdf
# en lugar de una animación
#
# Se puede configurar la animación cambiando el valor de las variables
# de la sección "Parámetros"
#
# ================================================================================

# Importa los módulos necesarios
from matplotlib import pyplot as plt
import numpy as np
import os

dir = os.path.dirname(__file__)
file_in = os.path.join(dir, 'hamiltoniano.txt')
# Parámetros
# ========================================
file_out = "hamilton" # Nombre del fichero de salida (sin extensión)

# Límites de los ejes X e Y

X, Y = np.loadtxt(file_in, delimiter=',', unpack=True)
plt.plot(X, Y)
plt.title('Line Graph using NUMPY')
plt.xlabel('X')
plt.ylabel('Y')
plt.show()
plt.savefig('hamiltoniano.png')