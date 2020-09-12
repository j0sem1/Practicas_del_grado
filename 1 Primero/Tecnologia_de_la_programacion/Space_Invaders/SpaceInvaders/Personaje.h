#ifndef PERSONAJE_H_INCLUDED
#define PERSONAJE_H_INCLUDED
#include "Pantalla.h"

typedef struct PersonajeRep * Personaje;

Personaje PersonajeCrea(Imagen i, int w,int h,int x,int y);

void PersonajeLibera(Personaje p);

void PersonajeMueve(Personaje p, int vx,int vy);

void PersonajeDibuja(Personaje p);

int PersonajeGetX(Personaje p);

int PersonajeGetY(Personaje p);

int PersonajeGetW(Personaje p);

int PersonajeGetH(Personaje p);

#endif // PERSONAJE_H_INCLUDED
