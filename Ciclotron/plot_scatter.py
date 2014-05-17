# -*- coding: utf-8 -*-
# <nbformat>3.0</nbformat>

# <codecell>
# <codecell>

#!/usr/bin/python
# -*- coding: latin-1 -*-
import math
import matplotlib.pyplot as plt
import numpy as np
import glob

"""
Programa que grafica archivos de texto con datos.
Autores: Oscar Fernando Garzon-Juan Diego Bermeo
Fecha: Martes 29 de Octubre de 2013.
"""
"""
Carga los datos desde un archivo "file" y los Grafica.
"""

files = glob.glob('*.dat')
for i in range(len(files)):
	print files[-i-1]

for i in range(len(files)):
	archivo1=np.loadtxt(open(files[-i-1],"r"))  
	plt.scatter(archivo1[:,0],archivo1[:,1])
    	plt.show()
    	plt.close()
           



