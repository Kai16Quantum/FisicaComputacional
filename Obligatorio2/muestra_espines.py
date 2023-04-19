# ================================================================================
# ANIMACION ESPINES
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
from matplotlib.animation import FuncAnimation
from matplotlib.patches import Circle
import numpy as np

# Parámetros
# ========================================
file_in = "datos_salida.txt" # Nombre del fichero de datos
file_out = "espines" # Nombre del fichero de salida (sin extensión)

color_matrix = []
previous_matrix = []
N = 6

target_fps = 14
n_filter = 100
max_frames = 12000

# Lectura del fichero de datos
# ========================================
frames_data = []

with open(file_in, "r") as f:
    n = 0
    lines = f.readlines()
    frames_data = []
    frame_data = []
    for line in lines:
        if (n>max_frames*n_filter):
            break
        if (line == '\n'):
            n += 1
            if len(frame_data) > 0:
                frames_data.append(frame_data)
                frame_data = []
        elif (n % n_filter == 1):
            values = [int(x.strip()) for x in line.split(",") if x.strip()]
            frame_data.append(values)
            

nespin = len(frames_data[0])

for k in range(N):
    v_array = []
    for j in range(N):
        v_array.append(0)
    color_matrix.append(v_array.copy())
    previous_matrix.append(v_array.copy())
# Creación de la animación/gráfico
# ========================================
# Crea los objetos figure y axis
fig, ax = plt.subplots()

# Define la función que dibuja un fotograma
def update(frame):
    # Limpia el eje
    ax.clear()
    
    # Remove the previous circles from the plot
    for collection in ax.collections:
        collection.remove()
    
    # Dibuja los espines
    circles = []
    for i in range(N):
        for j in range(N):
            color_mod = (1.0+color_matrix[i][j])/2.0
            if (frame < len(frames_data) and 
                i < len(frames_data[frame]) and 
                j < len(frames_data[frame][i]) and 
                frames_data[frame][i][j] == 1):
                if (previous_matrix[i][j] != 1):
                    color_matrix[i][j] = 1.0
                previous_matrix[i][j] = 1
                #ROJO
                color = (0.8*color_mod,0.3*color_mod,0.2*color_mod) 
            else:
                if (previous_matrix[i][j] != -1):
                    color_matrix[i][j] = 1.0
                previous_matrix[i][j] = -1
                #AZUL
                color = (0.3*color_mod,0.2*color_mod,0.7*color_mod) 
            color_matrix[i][j] = max(0.0, color_matrix[i][j]-0.1)
            circle = Circle((j*1.0-N/2.0 + 0.5, i*1.0-N/2.0 + 0.5), 0.4, color=color)
            ax.add_artist(circle)
            circles.append(circle)
    
    # Establece los límites de los ejes
    ax.set_xlim(-10, 10)
    ax.set_ylim(-10, 10)
    
    # Etiqueta los ejes
    ax.set_xlabel("X")
    ax.set_ylabel("Y")
    
    # Etiqueta del tiempo
    ax.set_title(f"Tiempo {frame+1}")
    
    # Redraw the canvas and release the memory for the previous frame
    fig.canvas.draw_idle()
    
    # Devuelve los objetos que se han dibujado
    return circles

# Crea la animación
anim = FuncAnimation(fig, update, frames=len(frames_data), interval=1/target_fps)

# Guarda la animación en un archivo de vídeo
anim.save(f"{file_out}.mp4", dpi=120, fps=target_fps)