import matplotlib.pyplot as plt
import numpy as np
import glob
import pylab

archivo1=np.loadtxt("Energia.dat") 
plt.plot(archivo1[:,0],archivo1[:,1])
plt.xlabel('Tiempo [ns]')
plt.ylabel('Energia [MeV]')
pylab.xlim([0,2200])
plt.suptitle('Cambio del energia de la particula en el tiempo')
plt.show()
plt.close()
