# ================================================================================
# ANIMACION SISTEMA SOLAR
#
# Genera una animación a partir de un fichero de datos con las posiciones
# de los pendulos en diferentes instantes de tiempo.
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
# donde xi_j es la componente x del pendulo i-ésimo en el instante de
# tiempo j-ésimo, e yi_j lo mismo en la componente y. El programa asume que
# el nº de pendulos es siempre el mismo.
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
import matplotlib.lines as lines
import numpy as np
import os

dir = os.path.dirname(__file__)
file_in = os.path.join(dir, 'coordenadas.txt')
# Parámetros
# ========================================fichero de datos
file_out = "pendulo_animacion" # Nombre del fichero de salida (sin extensión)

# Límites de los ejes X e Y
x_min = -2.75
x_max = 2.75
y_min = -2.75
y_max = 2.75

interval = 50 # Tiempo entre fotogramas en milisegundos
show_trail = False # Muestra la "estela" del pendulo
trail_width = 1 # Ancho de la estela
save_to_file = True # False: muestra la animación por pantalla,
                     # True: la guarda en un fichero
dpi = 150 # Calidad del vídeo de salida (dots per inch)

# Radio del pendulo, en las mismas unidades que la posición
# Puede ser un número (el radio de todos los pendulos) o una lista con
# el radio de cada uno
pendulo_radio = 0.03


# Lectura del fichero de datos
# ========================================

# Inicializa la lista con los datos de cada fotograma.
# frames_data[j] contiene los datos del fotograma j-ésimo
frames_data = list()

# Itera sobre los bloques de texto separados por líneas vacías
# (cada bloque corresponde a un instante de tiempo)
# Para cada archivo:
dirname = os.path.dirname(__file__)
filename = os.path.join(dirname, "Salida")
files = os.listdir(filename);

# Lee el fichero a una cadena de texto
data_array_str = []
for file_n in range(len(files)):
    with open(os.path.join(filename, files[file_n]), "r") as f:
        data_str = f.read()
        data_array_str.append(data_str)
    
split_datas = list()
for file_n in range(len(data_array_str)):
    split_datas.append(data_array_str[file_n].split("\n\n"))

#Para cada archivo
frame_number = 0;

for frame_data_str in data_array_str[file_n].split("\n\n"):
    # Inicializa la lista con la posición de cada pendulo
    
    frame_data = list()    
    #Separamos en lineas
    #Las separamos
    for file_n in range(len(split_datas)):
        data_pendulo_pos_str = split_datas[file_n].pop(0)
        for pendulo_pos_str in data_pendulo_pos_str.split("\n"):
            # Lee la componente x e y de la línea
            pendulo_pos = np.fromstring(pendulo_pos_str, sep=",")
            # Si la línea no está vacía, añade pendulo_pos a la lista de 
            # posiciones del fotograma
            if pendulo_pos.size > 0:
                frame_data.append(np.fromstring(pendulo_pos_str, sep=","))
    # Añade los datos de este fotograma a la lista
    if frame_number % 30 == 0:
        frames_data.append(frame_data)
    else:
        frame_number+=1

    
    
        
    

# El número de pendulos es el número de líneas en cada bloque
# Lo calculamos del primer bloque
npendulos = len(frames_data[0])


# Creación de la animación/gráfico
# ========================================
# Crea los objetos figure y axis
fig, ax = plt.subplots()

# Define el rango de los ejes
ax.axis("equal")  # Misma escala para ejes X e Y
ax.set_xlim(x_min, x_max)
ax.set_ylim(y_min, y_max)

# Si solo se ha dado un radio para todos los pendulos, conviértelo a una
# lista con todos los elementos iguales
if not hasattr(pendulo_radio, "__iter__"):
    pendulo_radio = pendulo_radio*np.ones(npendulos)
# En caso contrario, comprueba que el nº de radios coincide con el
# nº de pendulos y devuelve error en caso contrario
else:
    if not npendulos == len(pendulo_radio):
        raise ValueError(
                "El número de radios especificados no coincide con el número "
                "de pendulos")

# Representa el primer fotograma
# Pinta un punto en la posición de cada paneta y guarda el objeto asociado
# al punto en una lista
pendulo_puntos = list()
pendulo_trails = list()
pendulo_lineas = list()

# EL ORIGEN
pendulo_punto = Circle((0, 0), 0.02)
ax.add_artist(pendulo_punto)

j = 0;
anterior_pos = [0,0]

g_color = tuple(np.random.random(size=3) * 1.0)
for pendulo_pos, radio in zip(frames_data[0], pendulo_radio):
    x, y = pendulo_pos
    pendulo_linea = lines.Line2D(anterior_pos,pendulo_pos)
    pendulo_linea.set_color(g_color)
    ax.add_artist(pendulo_linea)
    pendulo_lineas.append(pendulo_linea)
    if ((j-1)%2 == 0):
        anterior_pos = [0,0]
        g_color = tuple(np.random.random(size=3) * 1.0)
    else:
        anterior_pos = pendulo_pos
    pendulo_punto = Circle((x, y), radio, color="orange")

    ax.add_artist(pendulo_punto)
    pendulo_puntos.append(pendulo_punto)

    j+=1
    # Inicializa las estelas (si especificado en los parámetros)
    if show_trail:
        pendulo_trail, = ax.plot(
                x, y, "-", linewidth=trail_width,
                color=pendulo_puntos[-1].get_facecolor())
        pendulo_trails.append(pendulo_trail)
 
# Función que actualiza la posición de los pendulos en la animación 
def update(j_frame, frames_data, pendulo_puntos, pendulo_lineas, pendulo_trails, show_trail):
    # Actualiza la posición del correspondiente a cada pendulo
    for j_pendulo, pendulo_pos in enumerate(frames_data[j_frame]):
        x, y = pendulo_pos
        pendulo_puntos[j_pendulo].center = (x, y)
        if (j_pendulo>0) and (j_pendulo%2 != 0):
            n_xdata = [pendulo_puntos[j_pendulo-1].center[0],x]
            n_ydata = [pendulo_puntos[j_pendulo-1].center[1],y]
            pendulo_lineas[j_pendulo].set_data([n_xdata, n_ydata])
        else:
            pendulo_lineas[j_pendulo].set_data([[0,pendulo_puntos[j_pendulo].center[0]], [0,pendulo_puntos[j_pendulo].center[1]]])
        
        if show_trail:
            xs_old, ys_old = pendulo_trails[j_pendulo].get_data()
            xs_new = np.append(xs_old, x)
            ys_new = np.append(ys_old, y)

            pendulo_trails[j_pendulo].set_data(xs_new, ys_new)

    return pendulo_puntos + pendulo_lineas + pendulo_trails

def init_anim():
    # Clear trails
    if show_trail:
        for j_pendulo in range(npendulos):
            pendulo_trails[j_pendulo].set_data(list(), list())

    return pendulo_puntos + pendulo_lineas+ pendulo_trails 
# Calcula el nº de frames
nframes = len(frames_data)

# Si hay más de un instante de tiempo, genera la animación
if nframes > 1:
    # Info sobre FuncAnimation: https://matplotlib.org/stable/api/animation_api.html
    animation = FuncAnimation(
            fig, update, init_func=init_anim,
            fargs=(frames_data, pendulo_puntos, pendulo_lineas, pendulo_trails, show_trail),
            frames=len(frames_data), blit=True, interval=interval)

    # Muestra por pantalla o guarda según parámetros
    if save_to_file:
        animation.save("{}.mp4".format(file_out), dpi=dpi)
    else:
        plt.show()
# En caso contrario, muestra o guarda una imagen
else:
    # Muestra por pantalla o guarda según parámetros
    if save_to_file:
        fig.savefig("{}.pdf".format(file_out))
    else:
        plt.show()