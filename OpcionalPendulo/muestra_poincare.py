import numpy as np
import matplotlib.pyplot as plt

# Leer los datos del archivo
data = np.genfromtxt("poincare.txt", delimiter=",")

# Extraer las columnas correspondientes a phi y theta
phi = data[:, 0]
theta = data[:, 1]

# Plot del mapa de Poincaré
plt.scatter(phi, theta, s=1)
plt.xlabel("phi")
plt.ylabel("theta")
plt.title("Mapa de Poincaré")
plt.show()