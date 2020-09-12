#!/usr/bin/env python

import numpy             as np
import cv2               as cv
import time
from umucv.stream import autoStream
from matplotlib.pyplot import imshow, subplot, title

def rgb2yuv(x):
    return cv.cvtColor(x,cv.COLOR_RGB2YUV)

threshold = 150
def updateThreshold(v):
    global threshold
    threshold = v

# El cero será RGB y el 1 UV
method = 0
def updateMethod(v):
    global method
    method = v

backSaved = False

# Imagen actual: frame
# Fondo: back
back = None
# Fondo destino: dst
dst = cv.imread("fondoDestino.jpg")

# Creamos la ventana donde se mostrará lo que se con la cámara
# y donde estarán los trackbar
cv.namedWindow("Camara")

# Trackbar que actúa de Switch para cambiar entre RGB y UV
cv.createTrackbar("0 : RGB \n1 : UV", "Camara", method, 1, updateMethod)
# Trackbar para cambiar el umbral
cv.createTrackbar("Umbral", "Camara", threshold, 300, updateThreshold)

# Botón que iba a ser creado
#cv.createButton("Camara", variab, None, cv.QT_CHECKBOX, 0)

for key,frame in autoStream():
    # Mostrar la webcam
    cv.imshow("Camara",frame)
    
    # Capturar la imagen con el fondo
    if key == ord('a'):
        back = frame
        backSaved = True
    
    if backSaved:
        t0 = time.time()
        # Método RGB
        if (method == 0):
            # Diferencia de imágenes en el espacio RGB
            value = np.sum(abs(frame.astype(float)-back.astype(float)),axis=2)
       
        # Método UV
        else:
            # Diferencia en UV
            backyuv = rgb2yuv(back)
            objyuv  = rgb2yuv(frame)

            # Paso a float y selecciono canales UV
            obuv = objyuv.astype(float)[:,:,[1,2]]
            bkuv = backyuv.astype(float)[:,:,[1,2]]

            # Diferencia de imágenes en el espacio UV
            value = np.sum(abs(obuv-bkuv),axis=2)

        # Mostrar la diferencia
        cv.imshow("Diferencia", value)

        mask = value > threshold

        # Nos aseguramos de que tenga el mismo tamaño que las imágenes anteriores
        r,c = mask.shape
        res = cv.resize(dst,(c,r))

        # Hay que convertir la mask a 3 canales para poder copiar rgb
        mask3 = mask.reshape(r,c,1)

        np.copyto(res,frame, where = mask3) 

        cv.imshow("Resultado", res)
        t1 = time.time()
        print('{:.0f}ms'.format(1000*(t1-t0)))

cv.destroyAllWindows()