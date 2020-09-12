#!/usr/bin/env python

import cv2   as cv
import sys
import time
from glob import glob
from umucv.stream import autoStream
from umucv.util import putText

sift = cv.xfeatures2d.SIFT_create(nfeatures=500)

matcher = cv.BFMatcher()

files = glob(sys.argv[1])

k0 = []
d0 = []
x0 = []
p0 = []

cv.namedWindow("Camara")
cv.namedWindow("SIFT")
cv.destroyWindow("SIFT")

print("Leyendo imágenes...")

for fn in files:
    img = cv.imread(fn)
    if img is None:
        print("Falló al cargar", fn)
        continue
    else:
        k0Aux, d0Aux = sift.detectAndCompute(img, mask=None)
        if (len(k0Aux) > 0):
            # Hacemos esta comprobación porque d0Aux podría ser None si no se detecta
            # ningún keypoint
            x0.append(img)
            k0.append(k0Aux)
            d0.append(d0Aux)
            p0.append(0)

if (len(x0) > 0):
    print("Imágenes leídas")
else:
    print("Sin imágenes cargadas")
    exit()

imgMatcheada = None

for key, frame in autoStream():

    t0 = time.time()
    keypoints , descriptors = sift.detectAndCompute(frame, mask=None)
    t1 = time.time()
    putText(frame, f'{len(keypoints)} pts  {1000*(t1-t0):.0f} ms')

    i = 0
    if key == ord('c'):
        for i in range(i, len(x0)-1):
            t2 = time.time()
            # solicitamos las dos mejores coincidencias de cada punto, no solo la mejor
            matches = matcher.knnMatch(descriptors, d0[i], k=2)
            t3 = time.time()

            # ratio test
            # nos quedamos solo con las coincidencias que son mucho mejores que
            # que la "segunda opción". Es decir, si un punto se parece más o menos lo mismo
            # a dos puntos diferentes del modelo lo eliminamos.
            good = []
            for m in matches:
                if len(m) >= 2:
                    best,second = m
                    if best.distance < 0.75*second.distance:
                        good.append(best)

            # Porcentaje de puntos matcheados
            p0[i] = len(good)/len(k0[i])*100
        
        # Imagen con mejor porcentaje
        bestPerc = max(p0)
        print(bestPerc)
        if (bestPerc > 7):
            i = p0.index(bestPerc)
            imgMatcheada = i
            # Imprimir foto y texto
            putText(x0[imgMatcheada], f'{len(good)} matches  {1000*(t3-t2):.0f} ms', color=(200,255,200))
            putText(x0[imgMatcheada], f'{round(bestPerc, 2)} % matches', orig=(5,36), color=(200,255,200))           
            cv.imshow("SIFT", x0[imgMatcheada])
        else:
            cv.destroyWindow("SIFT")
    
    cv.imshow("Camara", frame)