#ifndef BALA_H_INCLUDED
#define BALA_H_INCLUDED
#include "Pantalla.h"

typedef struct BalaRep * Bala;

Bala BalaCrea(Imagen i, int x, int y, int w, int h, int vy);

void BalaLibera(Bala b);

void BalaMueve(Bala b);

int BalaGetX(Bala b);

int BalaGetY(Bala b);

int BalaGetW(Bala b);

int BalaGetH(Bala b);

void BalaDibuja(Bala b);

#endif // BALA_H_INCLUDED
