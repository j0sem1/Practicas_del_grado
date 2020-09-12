#!/usr/bin/env python

# La solución anterior (corners3.py) tiene el problema de que las asociaciones de puntos
# a veces son incorrectas y el número de trayectorias crece indefinidamente
# repitiendo puntos. Esto se soluciona con dos mejoras.

import cv2 as cv
import numpy as np
from umucv.stream import autoStream, sourceArgs
from umucv.util import putText
import time
from umucv.util import putText
import math

focalLenght = 462.87

tracks = []
# Longitud del tracking
track_len = 5
# Detecta el movimiento cada 5 frames
detect_interval = 5

corners_params = dict( maxCorners = 500,
                       qualityLevel= 0.1,
                       minDistance = 10,
                       blockSize = 7)

lk_params = dict( winSize  = (15, 15),
                  maxLevel = 2,
                  criteria = (cv.TERM_CRITERIA_EPS | cv.TERM_CRITERIA_COUNT, 10, 0.03))

times = []

dxTotal = 0
dyTotal = 0

for n, (key, frame) in enumerate(autoStream()):
    gray = cv.cvtColor(frame, cv.COLOR_BGR2GRAY)
    
    t0 = time.time()
    if len(tracks):
        
        # el criterio para considerar bueno un punto siguiente es que si lo proyectamos
        # hacia el pasado, vuelva muy cerca del punto incial, es decir:
        # "back-tracking for match verification between frames"
        p0 = np.float32( [t[-1] for t in tracks] )
        p1,  _, _ =  cv.calcOpticalFlowPyrLK(prevgray, gray, p0, None, **lk_params)
        p0r, _, _ =  cv.calcOpticalFlowPyrLK(gray, prevgray, p1, None, **lk_params)
        d = abs(p0-p0r).reshape(-1,2).max(axis=1)
        good = d < 1
        
        new_tracks = []
        for t, (x,y), ok in zip(tracks, p1.reshape(-1,2), good):
            if not ok:
                continue
            t.append( [x,y] )
            if len(t) > track_len:
                del t[0]
            new_tracks.append(t)

        tracks = new_tracks


        # dibujamos las trayectorias
        cv.polylines(frame, [ np.int32(t) for t in tracks ], isClosed=False, color=(0,0,255))
        for t in tracks:
            x,y = np.int32(t[-1])
            # Dibuja el punto donde se encuentra ahora
            cv.circle(frame, (x, y), 2, (0, 0, 255), -1)

    t1 = time.time()

    times.append(t1 - t0)
    if (len(times) > 5):
        del times[0]

    # Aquí vamos a dibujar el eso intermedio
    if (len(tracks) > 0):

        dx = 0
        dy = 0

        # Acumular la rotación total
        for t in tracks:
            if (len(t) > 1):
                xIn,yIn = np.int32(t[-2])
                xFin,yFin = np.int32(t[-1])
                dx += xFin - xIn
                dy += yFin - yIn

        dx /= len(tracks)
        dy /= len(tracks)

        dxTotal += dx
        dyTotal += dy

        putText(frame, f'{round(math.degrees(2*np.arctan((dxTotal/2)/focalLenght)), 2)} grados en X acumulados', orig=(5,72), color=(200,255,200))
        putText(frame, f'{round(math.degrees(2*np.arctan((dyTotal/2)/focalLenght)), 2)} grados en Y acumulados', orig=(5,108), color=(200,255,200))

        new = []

        dx = 0
        dy = 0

        for t in tracks:
            xIn,yIn = np.int32(t[0])
            xFin,yFin = np.int32(t[-1])
            dx += xFin - xIn
            dy += yFin - yIn

        dx /= len(tracks)
        dy /= len(tracks)

        new.append( [320,240] )
        new.append( [320-dx,240-dy] )

        cv.polylines(frame, [np.int32(new)], isClosed=False, color=(0,255,0))
        for t in tracks:
            x,y = np.int32([320-dx,240-dy])
            # Dibuja el punto donde se encuentra ahora
            cv.circle(frame, (x, y), 4, (0, 255, 0), -1)
        
        # Pixels / frame
        pf = (dx+dy) / 5
        print("Pixels/frame: " + str(pf))

        # Grados / frame
        gf = math.degrees(2*np.arctan((dx+dy/2)/focalLenght)) / 5
        print("Grados/frame: " + str(gf))

        # Grados / segundo
        gs = math.degrees(2*np.arctan((dx+dy/2)/focalLenght)) / sum(times)
        print("Grados/seg: " + str(gs))

        putText(frame, f'{round(gs, 2)} grados x seg', orig=(5,36), color=(200,255,200))


    
    # resetear el tracking
    if n % detect_interval == 0:
        # Creamos una máscara para indicar al detector de puntos nuevos las zona
        # permitida, que es toda la imagen, quitando círculos alrededor de los puntos
        # existentes (los últimos de las trayectorias).
        mask = np.zeros_like(gray)
        mask[:] = 255
        for x,y in [np.int32(t[-1]) for t in tracks]:
            cv.circle(mask, (x,y), 5, 0, -1)
        
        corners = cv.goodFeaturesToTrack(gray, mask=mask, **corners_params)
        if corners is not None:
            for [(x, y)] in np.float32(corners):
                tracks.append( [  [ x,y ]  ] )

    putText(frame, f'{len(tracks)} corners, {(t1-t0)*1000:.0f}ms' )
    cv.imshow('input', frame)
    prevgray = gray