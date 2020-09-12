#ifndef ENEMIGO_H_INCLUDED
#define ENEMIGO_H_INCLUDED
#include "Pantalla.h"

typedef struct EnemigoRep * Enemigo;

Enemigo EnemigoCrea(Imagen i, int x, int y, int w, int h, int vx, int vy);

void EnemigoLibera(Enemigo e);

void EnemigoMueve(Enemigo e);

int EnemigoGetX(Enemigo e);

int EnemigoGetY(Enemigo e);

int EnemigoGetW(Enemigo e);

int EnemigoGetH(Enemigo e);

void EnemigoDibuja(Enemigo e);


#endif // ENEMIGO_H_INCLUDED
