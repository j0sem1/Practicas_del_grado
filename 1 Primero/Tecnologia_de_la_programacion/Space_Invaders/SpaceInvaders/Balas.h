#ifndef BALAS_H_INCLUDED
#define BALAS_H_INCLUDED
#include "Pantalla.h"

typedef struct BalasRep * Balas;

Balas BalasCrea();

void BalasLibera(Balas bb);

void BalasInserta(Balas bb, Imagen i, int x, int y, int w, int h, int vy);

void BalasMueve(Balas bb);

void BalasDibuja(Balas bb);

void BalasColisionPersonaje(Balas bb, int x, int y, int w, int h);

void BalasColisionEnemigos(Balas bb, Enemigos ee);

#endif // BALAS_H_INCLUDED
