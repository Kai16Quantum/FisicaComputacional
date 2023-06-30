import os
import matplotlib.pyplot as plt
import numpy as np

plt.style.use('seaborn')

def leer_archivo(nombre_archivo):
    with open(nombre_archivo, 'r') as archivo:
        lineas = archivo.readlines()
        solapamientos = [abs(float(linea.strip())) for linea in lineas]
        temperatura = float(nombre_archivo.split('_T')[1].split('_PATRON')[0][1:])
        patron = int(nombre_archivo.split('_PATRON_')[1].split('.')[0])
        return solapamientos, temperatura, patron

def calcular_desviacion(datos):
    media = np.mean(datos)
    desviacion = np.std(datos)
    return [media, desviacion]

def graficar_patrones(carpeta):
    carpeta = os.path.join(os.path.dirname(__file__),carpeta)
    fig, ax = plt.subplots()
    archivos = os.listdir(carpeta)
    temperaturas = []
    patrones = []

    #Calculamos la desviación típica
    desv_tipicas = []
    desv_medias = []
    medias = []
    n_temperatura = -1


    #Calculamos la media y desviación
    for archivo in archivos:
        ruta_archivo = os.path.join(carpeta, archivo)
        if os.path.isfile(ruta_archivo):
            solapamientos, temperatura, patron = leer_archivo(ruta_archivo)
            x = np.arange(len(solapamientos))

            if temperatura in temperaturas:
                medias[n_temperatura].append(np.mean(solapamientos[-5:]))
                new_label='_nolegend_'
            else:
                n_temperatura += 1
                medias.append([np.mean(solapamientos[-5:])])
                new_label=f'T = {temperatura:.6f}K'
                temperaturas.append(temperatura)
            n_nueva_temp = temperaturas.index(temperatura)
    
    errors = []
    for temp in medias:
        salida_array = calcular_desviacion(temp)
        desv_medias.append(salida_array[0])
        desv_tipicas.append(salida_array[1])
        
        error = 3.0*salida_array[1]/np.sqrt(len(temp))
        errors.append(error)


    temperaturas = []
    for archivo in archivos:
        ruta_archivo = os.path.join(carpeta, archivo)
        if os.path.isfile(ruta_archivo):
            solapamientos, temperatura, patron = leer_archivo(ruta_archivo)
            x = np.arange(len(solapamientos))

            if temperatura in temperaturas:
                new_label='_nolegend_'
            else:
                temperaturas.append(temperatura)
                new_label=f'T = {temperatura:.3f}K'
            
            n_nueva_temp = temperaturas.index(temperatura)
            error = errors[n_nueva_temp]

            patrones.append(patron)
            ax.errorbar(x, solapamientos, error, 0, label=new_label, color=f'C{n_nueva_temp}', elinewidth=1.0, capsize=1.0, capthick=1)

    ax.set_xlabel('Número de pasos Monte Carlo (N^2)')
    ax.set_ylabel('Solapamiento (Valor)')
    

    archivo_final = os.path.join(os.path.dirname(__file__),'Graficas/Solapamientos_Patrones.png')
    plt.savefig(archivo_final)
    plt.legend(loc = "upper right", framealpha = 1, frameon = True, fancybox = True)
    plt.show()


    

carpeta_salida = 'Solapamientos_Patrones'
graficar_patrones(carpeta_salida)