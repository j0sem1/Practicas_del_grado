#!/usr/bin/env python
import numpy             as np
import cv2               as cv
import numpy             as np
from umucv.util   import ROI
from umucv.stream import autoStream
from umucv.util import putText

# Concatena imágenes en horizontal. Función recursiva
def concatenateImages(models, i):
    if (i == 0):
        return cv.resize(models[i], (50,50))
    else:
        return cv.hconcat((concatenateImages(models, i-1), cv.resize(models[i], (50,50))))

# Devuelve el máximo de tres números
def max(a, b, c):
    if ((a > b) and (a > c)):
        return a
    elif (b > c):
        return b
    else:
        return c

models = []
mHistRed = []
mHistGreen = []
mHistBlue = []

# Creamos la ventana donde se mostrará lo que se con la cámara
# y donde estarán los trackbar
cv.namedWindow("Camara")
# selección de región de interés más cómoda que cv.selectROI
roi = ROI("Camara")

cv.namedWindow("Detectada")
cv.namedWindow("Histogramas")
cv.namedWindow("Modelos")

for key,frame in autoStream():

    # seleccionamos una región
    if roi.roi:
        [x1,y1,x2,y2] = roi.roi

        cv.rectangle(frame, (x1,y1), (x2,y2), color=(0,255,255), thickness=2)

        # Recortamos una región de la imagen y cambiamos su tamaño a 100x100
        reg = cv.resize(frame[y1:y2,x1:x2], (50,50))

        # Canales RGB
        blue = reg[:,:,0].copy()
        green = reg[:,:,1].copy()
        red = reg[:,:,2].copy()

        # Histogramas de los canales
        histBlue, binsBlue = np.histogram(blue, bins=32, range=(0,257))
        histGreen, binsGreen = np.histogram(green, bins=32, range=(0,257))
        histRed, binsRed = np.histogram(red, bins=32, range=(0,257))

        # Fondo negro
        blackBackground = np.zeros((480, 530, 3), dtype = "uint8")

        # ajustamos la escala del histograma para que se vea bien en la imagen
        # usaremos cv.polylines, que admite una lista de listas de puntos x,y enteros
        # las coordenadas x son los bins del histograma (quitando el primero)
        # y las coordenadas y son el propio histograma escalado y desplazado
        xsRed = 2*binsRed[1:]
        ysRed = 480-histRed*(480/1000)
        # trasponemos el array para emparejar cada x e y
        xysRed = np.array([xsRed,ysRed]).T.astype(int)
        cv.polylines(blackBackground, [xysRed], isClosed=False, color=(0,0,255), thickness=2)

        # ajustamos la escala del histograma para que se vea bien en la imagen
        # usaremos cv.polylines, que admite una lista de listas de puntos x,y enteros
        # las coordenadas x son los bins del histograma (quitando el primero)
        # y las coordenadas y son el propio histograma escalado y desplazado
        xsGreen = 2*binsGreen[1:]
        ysGreen = 480-histGreen*(480/1000)
        # trasponemos el array para emparejar cada x e y
        xysGreen = np.array([xsGreen,ysGreen]).T.astype(int)
        cv.polylines(blackBackground, [xysGreen], isClosed=False, color=(0,255,0), thickness=2)

        # ajustamos la escala del histograma para que se vea bien en la imagen
        # usaremos cv.polylines, que admite una lista de listas de puntos x,y enteros
        # las coordenadas x son los bins del histograma (quitando el primero)
        # y las coordenadas y son el propio histograma escalado y desplazado
        xsBlue = 2*binsBlue[1:]
        ysBlue = 480-histBlue*(480/1000)
        # trasponemos el array para emparejar cada x e y
        xysBlue = np.array([xsBlue,ysBlue]).T.astype(int)
        cv.polylines(blackBackground, [xysBlue], isClosed=False, color=(255,0,0), thickness=2)

        cv.imshow("Histogramas", blackBackground)

        # Guardar región actual
        if key == ord('r'):
            # Imprimir todos los modelos en una nueva ventana
            models.append(reg)
            mHistRed.append(histRed)
            mHistGreen.append(histGreen)
            mHistBlue.append(histBlue)
            cv.imshow("Modelos", concatenateImages(models, len(models)-1))

        # Diferencia de los histogramas RGB
        if (len(models) > 0):
            results = []
            texto = ""
            for i in range(0, len(models)):
                dR = np.sum(cv.absdiff(histRed, mHistRed[i]))
                dG = np.sum(cv.absdiff(histGreen, mHistGreen[i]))
                dB = np.sum(cv.absdiff(histBlue, mHistBlue[i]))
                results.append(max(dR, dG, dB)/1000)
                texto = texto + str(results[-1]) + "   "
            putText(frame, texto)

            bestResult = min(results)

            if (bestResult < 1.2):
                i = results.index(bestResult)
                cv.imshow("Detectada", models[i])
            else:
                cv.destroyWindow("Detectada")

    # Mostrar la webcam
    cv.imshow("Camara",frame)

cv.destroyAllWindows()