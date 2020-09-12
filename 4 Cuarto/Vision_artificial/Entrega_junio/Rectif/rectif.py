#!/usr/bin/env python

import cv2 as cv
import numpy as np
from umucv.stream import autoStream, sourceArgs
from collections import deque
from umucv.util import putText
import matplotlib.pyplot as plt
import sys

def fig(w,h):
    return plt.figure(figsize=(w,h))

# dibuja un polígono cuyos nodos son las filas de un array 2D
def shcont(c, color='blue', nodes=True):
    x = c[:,0]
    y = c[:,1]
    x = np.append(x,x[0])
    y = np.append(y,y[0])
    plt.plot(x,y,color)
    if nodes: plt.plot(x,y,'.',color=color, markerSize=11)

def readrgb(file):
    return cv.cvtColor( cv.imread(file), cv.COLOR_BGR2RGB)
    
def fun(event, x, y, flags, param):
    if event == cv.EVENT_LBUTTONDOWN:
        points.append((x,y))    # En lugar de imprimir los puntos por pantalla los almacenamos en la lista


# Lee el archivo que contiene la información sobre la imagen
file = None
if (len(sys.argv) == 1):
    print("Introducir el nombre de un archivo")
else:
    file = open(sys.argv[1], 'r')
    lines = file.readlines()

if (file == None or len(lines) < 4):
    print("Archivo no válido")
else:
    imgName = lines[0].split(' ', -1)[0]
    originalPoints = lines[1].split(' ', -1)
    newPoints = lines[2].split(' ', -1)
    pixelsXMil = float(lines[3].split(' ', -1)[0])
    file.close()

    # Carga la imagen
    img = readrgb(imgName)
    imgForStream = cv.cvtColor(img, cv.COLOR_RGB2BGR)

    

    # Creamos una lista llamada points que almacenará los puntos. Como máximo almacenará 2 puntos
    points = deque(maxlen = 2)

    # Le hemos cambiado el nombre a la ventana
    cv.namedWindow("Imagen")
    cv.setMouseCallback("Imagen", fun)

    previousFigures = None

    # Dibuja la imagen y los puntos sobre ella
    view = np.array([
                [int(originalPoints[0]),int(originalPoints[1])],
                [int(originalPoints[2]),int(originalPoints[3])],
                [int(originalPoints[4]),int(originalPoints[5])],
                [int(originalPoints[6]),int(originalPoints[7])]])
    fig = plt.figure(figsize=(6,8))
    plt.imshow(img)
    shcont(view)
    fig.show()

    # Obtiene la imagen rectificada a partir de la homografía
    real = np.array([
            [  float(newPoints[0]),   float(newPoints[1])],
            [ float(newPoints[2]),   float(newPoints[3])],
            [ float(newPoints[4]),  float(newPoints[5])],
            [  float(newPoints[6]),  float(newPoints[7])]])

    H,_ = cv.findHomography(view, real)
    rec = cv.warpPerspective(imgForStream,H,(800,1000))

    for key, frame in autoStream():

        for p in points:

            # Dibuja un círculo en el frame. P es la posición en la que dibujará el punto.
            # El punto tendrá radio tres, color verde, y el grosor lo ponemos negativo para que rellene la bola
            cv.circle(rec, p, 3, (0,255,0), -1)

        if len(points) == 2:

            # Machacará el frame original escribiendo una línea que va del primer punto al segundo. Otra vez de color verde.
            cv.line(rec, points[0], points[1], (0,255,0))

            # Calcula la media por columnas de los puntos.
            # Es decir, calcula el punto medio entre dos puntos
            # Calcula la media en el eje 0.
            c = np.mean(points, axis = 0).astype(int) # el astype(int) es para putText

            # Número de pixeles
            d = np.linalg.norm( np.array(points[0]) - points[1])

            # Calcular la distancia entre los puntos
            distance = np.sqrt( np.square(int(points[1][0]) - int(points[0][0])) + np.square(int(points[1][1]) - int(points[0][1])) )
            print("Distancia: " + str(distance) + " pixels.")
            centimeters = (distance / pixelsXMil) / 10  # mm / 10
            print("Distancia: " + str(centimeters) + " cm.")

            # Resetear el array de puntos, para poder seleccionar dos nuevos
            points = deque(maxlen = 2)

            # Escribimos la cadena indicada en el punto c, que en nuestro caso será la media
            putText(rec, f'{centimeters:.1f} cm.', c)
        
        cv.imshow("Imagen", rec)

cv.destroyAllWindows()