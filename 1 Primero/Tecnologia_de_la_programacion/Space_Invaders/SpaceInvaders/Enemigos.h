#ifndef ENEMIGOS_H_INCLUDED
#define ENEMIGOS_H_INCLUDED
#include "Pantalla.h"
#include "Enemigo.h"

typedef struct EnemigosRep * Enemigos;

Enemigos EnemigosCrea();

void EnemigosLibera(Enemigos ee);

void EnemigosInserta(Enemigos ee, Imagen i, int x, int y, int w, int h, int vx, int vy);

void EnemigosMueve(Enemigos ee);

void EnemigosDibuja(Enemigos ee);

void EnemigosColision(Enemigos ee, int x, int y, int w, int h); //TDA Balas? Cuando bala alcance a enemigo lo elimina


#endif // ENEMIGOS_H_INCLUDED
