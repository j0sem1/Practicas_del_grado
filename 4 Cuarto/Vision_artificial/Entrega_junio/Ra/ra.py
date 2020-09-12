#!/usr/bin/env python

# estimación de pose a partir del marcador images/ref.png
# En esta versión añadimos una imagen fuera del plano

# pruébalo con el vídeo de siempre

# ./pose3.py --dev=file:../images/rot4.mjpg

# con la imagen de prueba

# ./pose3.py --dev=--dir:../../images/marker.png

# o con la webcam poniéndolo en el teléfono o el monitor.

import cv2          as cv
import numpy        as np
import matplotlib.pyplot as plt

from umucv.stream   import autoStream
from umucv.htrans   import htrans, Pose
from umucv.contours import extractContours, redu


class Box:
    def __init__(self, x, z, y, size):
        self.x = x
        self.z = z
        self.y = y
        self.size = size
        self.xMaxSpeed = 0
        self.zMaxSpeed = 0
        self.yMaxSpeed = 0
        self.xSpeed = 0
        self.zSpeed = 0
        self.ySpeed = 0
        self.objective = [0,0,0]
        self.originalDistance = 0


# matriz de calibración sencilla dada la
# resolución de la imagen y el fov horizontal en grados
def Kfov(sz,hfovd):
    hfov = np.radians(hfovd)
    f = 1/np.tan(hfov/2)
    # print(f)
    w,h = sz
    w2 = w / 2
    h2 = h / 2
    return np.array([[f*w2, 0,    w2],
                     [0,    f*w2, h2],
                     [0,    0,    1 ]])


# intenta detectar polígonos de n lados
def polygons(cs,n,prec=2):
    rs = [ redu(c,prec) for c in cs ]
    return [ r for r in rs if len(r) == n ]


def rots(c):
    return [np.roll(c,k,0) for k in range(len(c))]


# probamos todas las asociaciones de puntos imagen con modelo
# y nos quedamos con la que produzca menos error
def bestPose(K,view,model):
    poses = [ Pose(K, v.astype(float), model) for v in rots(view) ]
    return sorted(poses,key=lambda p: p.rms)[0]


def applyImg(src, M, world, img, frame):
    # calculamos dónde se proyectarán en la imagen esas esquinas
    # usamos la matriz de cámara estimada
    dst = htrans(M, world)

    # calculamos la transformación
    # igual que findHomography pero solo con 4 correspondencias
    H = cv.getPerspectiveTransform(src.astype(np.float32), dst.astype(np.float32))

    # la aplicamos encima de la imagen de cámara
    cv.warpPerspective(img,H,size,frame,0,cv.BORDER_TRANSPARENT)


def drawBox(imgSide, box, M, frame):

    # las coordenadas de sus 4 esquinas
    # (se pueden sacar del bucle de captura)
    h,w = imgSide.shape[:2]
    src = np.array([[0,0],[0,h],[w,h],[w,0]])

    # Esquina abajo izquierda
    abI = [box.x - box.size, box.z - box.size, ]
    # Esquina abajo derecha
    abD = [box.x + box.size, box.z - box.size]
    # Esquina arriba derecha
    arD = [box.x + box.size, box.z + box.size]
    # Esquina arriba izquierda
    arI = [box.x - box.size, box.z + box.size]
    # Arriba
    arr = box.y + box.size
    # Abajo
    ab = box.y - box.size

    # Base
    # decidimos dónde queremos poner esas esquinas en el sistema de referencia del marcador
    # (si no cambian se puede sacar del bucle de captura)
    # [espacio del borde izq. al borde dcho., espacio de borde inferior a superior, alto]
    world = np.array([[abI[0],abI[1],ab],[abD[0],abD[1],ab],[arD[0],arD[1],ab],[arI[0],arI[1],ab]])
    applyImg(src, M, world, imgSide, frame)
    # Superior
    world = np.array([[abI[0],abI[1],arr],[abD[0],abD[1],arr],[arD[0],arD[1],arr],[arI[0],arI[1],arr]])
    applyImg(src, M, world, imgSide, frame)
    # Lado 1
    world = np.array([[abI[0],abI[1],ab],[abD[0],abD[1],ab],[abD[0],abD[1],arr],[abI[0],abI[1],arr]])
    applyImg(src, M, world, imgSide, frame)
    # Lado 2 - paralelo con 1
    world = np.array([[arI[0],arI[1],ab],[arD[0],arD[1],ab],[arD[0],arD[1],arr],[arI[0],arI[1],arr]])
    applyImg(src, M, world, imgSide, frame)
    # Lado 3
    world = np.array([[arI[0],arI[1],ab],[abI[0],abI[1],ab],[abI[0],abI[1],arr],[arI[0],arI[1],arr]])
    applyImg(src, M, world, imgSide, frame)
    # Lado 4 - paralelo con 3
    world = np.array([[arD[0],arD[1],ab],[abD[0],abD[1],ab],[abD[0],abD[1],arr],[arD[0],arD[1],arr]])
    applyImg(src, M, world, imgSide, frame)


def applyMovement(exterior):
    # Mover la caja
    exterior.x += exterior.xSpeed
    exterior.z += exterior.zSpeed
    exterior.y += exterior.ySpeed


def horizontalMovement(exterior, threshold):

    distance = np.sqrt( np.square(exterior.objective[0] - exterior.x) + np.square(exterior.objective[1] - exterior.z) + np.square(exterior.objective[2] - exterior.y) )

    if (distance < threshold or distance == 0):
        # Cambiar objetivo en función de las velocidades base
        # Cambiar también las velocidades base
        if (exterior.xMaxSpeed > 0 and exterior.zMaxSpeed < 0):
            exterior.objective = [0.4,-0.2,1]
        elif (exterior.xSpeed < 0 and exterior.zSpeed < 0):
            exterior.objective = [-0.2,0.4,1]
        elif (exterior.xSpeed < 0 and exterior.zSpeed > 0):
            exterior.objective = [0.4,1,1]
        else:
            exterior.objective = [1,0.4,1]

        exterior.xMaxSpeed = (exterior.objective[0] - exterior.x) / 20
        exterior.zMaxSpeed = (exterior.objective[1] - exterior.z) / 20 
        exterior.originalDistance = np.sqrt( np.square(exterior.objective[0] - exterior.x) + np.square(exterior.objective[1] - exterior.z) + np.square(exterior.objective[2] - exterior.y) )
        distance = np.sqrt( np.square(exterior.objective[0] - exterior.x) + np.square(exterior.objective[1] - exterior.z) + np.square(exterior.objective[2] - exterior.y) )
    
    distanceRatio = distance / exterior.originalDistance
    closeRatio = 1 - distanceRatio
    distanceRatio *= 0.9

    if ((exterior.xMaxSpeed > 0 and exterior.zMaxSpeed < 0) or (exterior.xSpeed < 0 and exterior.zSpeed > 0)):
        exterior.xSpeed = exterior.xMaxSpeed * distanceRatio
        exterior.zSpeed = exterior.zMaxSpeed * closeRatio
    else:
        exterior.xSpeed = exterior.xMaxSpeed * closeRatio
        exterior.zSpeed = exterior.zMaxSpeed * distanceRatio

    applyMovement(exterior)



def leftDiagonalMovement(exterior, threshold):

    distance = np.sqrt( np.square(exterior.objective[0] - exterior.x) + np.square(exterior.objective[1] - exterior.z) + np.square(exterior.objective[2] - exterior.y) )

    if (distance < threshold or distance == 0):
        # Cambiar objetivo en función de las velocidades base
        # Cambiar también las velocidades base
        if (exterior.xSpeed > 0 and exterior.zSpeed < 0 and exterior.ySpeed > 0):
            exterior.objective = [1,-0.2,1]
        elif (exterior.xSpeed > 0 and exterior.zSpeed < 0 and exterior.ySpeed < 0):
            exterior.objective = [0.4,0.4,0]
        elif (exterior.xSpeed < 0 and exterior.zSpeed > 0 and exterior.ySpeed < 0):
            exterior.objective = [-0.2,1,1]
        elif (exterior.xSpeed < 0 and exterior.zSpeed > 0 and exterior.ySpeed > 0):
            exterior.objective = [0.4,0.4,2]
        
        exterior.xMaxSpeed = (exterior.objective[0] - exterior.x) / 30
        exterior.zMaxSpeed = (exterior.objective[1] - exterior.z) / 30
        exterior.yMaxSpeed = (exterior.objective[2] - exterior.y) / 30
        exterior.originalDistance = np.sqrt( np.square(exterior.objective[0] - exterior.x) + np.square(exterior.objective[1] - exterior.z) + np.square(exterior.objective[2] - exterior.y) )
        distance = np.sqrt( np.square(exterior.objective[0] - exterior.x) + np.square(exterior.objective[1] - exterior.z) + np.square(exterior.objective[2] - exterior.y) )

    distanceRatio = distance / exterior.originalDistance
    closeRatio = 1 - distanceRatio
    distanceRatio *= 0.9

    if ((exterior.xSpeed > 0 and exterior.zSpeed < 0 and exterior.ySpeed > 0) or (exterior.xSpeed < 0 and exterior.zSpeed > 0 and exterior.ySpeed < 0)):
        exterior.xSpeed = exterior.xMaxSpeed * closeRatio
        exterior.zSpeed = exterior.zMaxSpeed * closeRatio
        exterior.ySpeed = exterior.yMaxSpeed * distanceRatio
    else:
        exterior.xSpeed = exterior.xMaxSpeed * distanceRatio
        exterior.zSpeed = exterior.zMaxSpeed * distanceRatio
        exterior.ySpeed = exterior.yMaxSpeed * closeRatio

    applyMovement(exterior)



def rightDiagonalMovement(exterior, threshold):

    distance = np.sqrt( np.square(exterior.objective[0] - exterior.x) + np.square(exterior.objective[1] - exterior.z) + np.square(exterior.objective[2] - exterior.y) )

    if (distance < threshold or distance == 0):
        # Cambiar objetivo en función de las velocidades base
        # Cambiar también las velocidades base
        if (exterior.xSpeed < 0 and exterior.zSpeed < 0 and exterior.ySpeed > 0):#change
            exterior.objective = [-0.2,-0.2,1]
        elif (exterior.xSpeed < 0 and exterior.zSpeed < 0 and exterior.ySpeed < 0):#change
            exterior.objective = [0.4,0.4,0]
        elif (exterior.xSpeed > 0 and exterior.zSpeed > 0 and exterior.ySpeed < 0):#change
            exterior.objective = [1,1,1]
        elif (exterior.xSpeed > 0 and exterior.zSpeed > 0 and exterior.ySpeed > 0):#change
            exterior.objective = [0.4,0.4,2]
        
        exterior.xMaxSpeed = (exterior.objective[0] - exterior.x) / 25
        exterior.zMaxSpeed = (exterior.objective[1] - exterior.z) / 25
        exterior.yMaxSpeed = (exterior.objective[2] - exterior.y) / 25
        exterior.originalDistance = np.sqrt( np.square(exterior.objective[0] - exterior.x) + np.square(exterior.objective[1] - exterior.z) + np.square(exterior.objective[2] - exterior.y) )
        distance = np.sqrt( np.square(exterior.objective[0] - exterior.x) + np.square(exterior.objective[1] - exterior.z) + np.square(exterior.objective[2] - exterior.y) )

    distanceRatio = distance / exterior.originalDistance
    closeRatio = 1 - distanceRatio
    distanceRatio *= 0.9

    if ((exterior.xSpeed < 0 and exterior.zSpeed < 0 and exterior.ySpeed > 0) or (exterior.xSpeed > 0 and exterior.zSpeed > 0 and exterior.ySpeed < 0)):#change
        exterior.xSpeed = exterior.xMaxSpeed * closeRatio
        exterior.zSpeed = exterior.zMaxSpeed * closeRatio
        exterior.ySpeed = exterior.yMaxSpeed * distanceRatio
    else:
        exterior.xSpeed = exterior.xMaxSpeed * distanceRatio
        exterior.zSpeed = exterior.zMaxSpeed * distanceRatio
        exterior.ySpeed = exterior.yMaxSpeed * closeRatio

    applyMovement(exterior)



stream = autoStream()

HEIGHT, WIDTH = next(stream)[1].shape[:2]
size = WIDTH,HEIGHT


K = Kfov( size, 60 )


marker = np.array(
       [[0,   0,   0],
        [0,   1,   0],
        [0.5, 1,   0],
        [0.5, 0.5, 0],
        [1,   0.5, 0],
        [1,   0,   0]])

square = np.array(
       [[0,   0,   0],
        [0,   1,   0],
        [1,   1,   0],
        [1,   0,   0]])

# Planos de color rojo y azul
imgNucleus = np.zeros((512,512,3), np.uint8)
imgNucleus[:] = (0,0,255)
imgElectron = np.zeros((512,512,3), np.uint8)
imgElectron[:] = (255,0,0)

# Cubos que se utilizan en la animación
# Cubo central. No se mueve
centerBox = Box(0.4,0.4,1,0.1)
# Cubo que se mueve horizontalmente
horizontalBox = Box(0.4,1,1,0.1)
horizontalBox.objective = [1,0.4,1]
horizontalBox.xMaxSpeed = (horizontalBox.objective[0] - horizontalBox.x) / 20
horizontalBox.zMaxSpeed = (horizontalBox.objective[1] - horizontalBox.z) / 20
horizontalBox.originalDistance = np.sqrt( np.square(horizontalBox.objective[0] - horizontalBox.x) + np.square(horizontalBox.objective[1] - horizontalBox.z) + np.square(horizontalBox.objective[2] - horizontalBox.y) )
# Cubo que se mueve diagonalmente de izquierda a derecha
leftDiagonalBox = Box(-0.2,1,1,0.1)
leftDiagonalBox.objective = [0.4,0.4,2]
leftDiagonalBox.xMaxSpeed = (leftDiagonalBox.objective[0] - leftDiagonalBox.x) / 30
leftDiagonalBox.zMaxSpeed = (leftDiagonalBox.objective[1] - leftDiagonalBox.z) / 30
leftDiagonalBox.yMaxSpeed = (leftDiagonalBox.objective[2] - leftDiagonalBox.y) / 30
leftDiagonalBox.originalDistance = np.sqrt( np.square(leftDiagonalBox.objective[0] - leftDiagonalBox.x) + np.square(leftDiagonalBox.objective[1] - leftDiagonalBox.z) + np.square(leftDiagonalBox.objective[2] - leftDiagonalBox.y) )
# Cubo que se mueve diagonalmente de derecha a izquierda
rightDiagonalBox = Box(1,1,1,0.1)
rightDiagonalBox.objective = [0.4,0.4,2]
rightDiagonalBox.xMaxSpeed = (rightDiagonalBox.objective[0] - rightDiagonalBox.x) / 25
rightDiagonalBox.zMaxSpeed = (rightDiagonalBox.objective[1] - rightDiagonalBox.z) / 25
rightDiagonalBox.yMaxSpeed = (rightDiagonalBox.objective[2] - rightDiagonalBox.y) / 25
rightDiagonalBox.originalDistance = np.sqrt( np.square(rightDiagonalBox.objective[0] - rightDiagonalBox.x) + np.square(rightDiagonalBox.objective[1] - rightDiagonalBox.z) + np.square(rightDiagonalBox.objective[2] - rightDiagonalBox.y) )

for n, (key,frame) in enumerate(stream):

    g = cv.cvtColor(frame,cv.COLOR_BGR2GRAY)
    # extractContours: detecta siluetas oscuras que no sean muy pequeñas ni demasiado alargadas
    cs = extractContours(g, minarea=5, reduprec=2)

    good = polygons(cs,6,3)
    poses = []
    for g in good:
        p = bestPose(K,g,marker)
        if p.rms < 2:
            poses += [p.M]

    for M in poses:

        horizontalMovement(horizontalBox, 0.1)

        leftDiagonalMovement(leftDiagonalBox, 0.2)

        rightDiagonalMovement(rightDiagonalBox, 0.2)

        drawBox(imgNucleus, centerBox, M, frame)
        drawBox(imgElectron, horizontalBox, M, frame)
        drawBox(imgElectron, leftDiagonalBox, M, frame)
        drawBox(imgElectron, rightDiagonalBox, M, frame)
        

    cv.imshow('source',frame)
    
cv.destroyAllWindows()