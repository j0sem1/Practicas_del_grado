#ifndef ENEMIGOSYBALAS_H_INCLUDED
#define ENEMIGOSYBALAS_H_INCLUDED
#include "Pantalla.h"
#include "Enemigo.h"
#include "Personaje.h"

//ENEMIGOS

typedef struct EnemigosRep * Enemigos;

Enemigos EnemigosCrea();

void EnemigosLibera(Enemigos ee);

void EnemigosInserta(Enemigos ee, Imagen i, int x, int y, int w, int h, int vx, int vy);

void EnemigosMueve(Enemigos ee);

void EnemigosDibuja(Enemigos ee);

Enemigo EnemigosRandom(Enemigos ee);

Enemigo EnemigosRandom2(Enemigos ee);

int NumeroEnemigos(Enemigos ee);

//Enemigo EnemigosCercaPersonaje(Enemigos ee, Personaje p, int distancia);

//BALAS

typedef struct BalasRep * Balas;

Balas BalasCrea();

void BalasLibera(Balas bb);

void BalasInserta(Balas bb, Imagen i, int x, int y, int w, int h, int vy);

void BalasMueve(Balas bb);

void BalasDibuja(Balas bb);

int BalasColisionPersonaje(Balas bb, int x, int y, int w, int h);

//BALAS Y ENEMIGOS

int BalasColisionEnemigos(Balas bb, Enemigos ee);

void funciondeprueba(Enemigos ee);

#endif // ENEMIGOSYBALAS_H_INCLUDED
