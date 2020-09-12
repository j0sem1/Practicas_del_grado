#include "Balas.h"
#include "Bala.h"
#include "Colision.h"
#include "Enemigos.h"
#include <stdlib.h>

struct BalasRep{
    Bala b;
    struct BalasRep * sig;
};

Balas BalasCrea(){
    Balas cabecera = malloc(sizeof(struct BalasRep));
    cabecera->sig = NULL;
    cabecera->b = NULL;
    return cabecera;
}

void BalasLibera(Balas bb){
    Balas aux = bb->sig;
    while(aux != NULL){
        Balas borrar = aux;
        aux = aux->sig;
        free(borrar);
    }
    free(bb);
}

void BalasInserta(Balas bb, Imagen i, int x, int y, int w, int h, int vy){
    while(bb->sig != NULL){
        bb = bb->sig;
    }
    bb->sig = malloc(sizeof(struct BalasRep));
    bb->sig->sig = NULL;
    bb->sig->b = BalaCrea(i,x,y,w,h,vy);
}

void BalasMueve(Balas bb){
    while(bb->sig != NULL){
        if(bb->b != NULL) BalaMueve(bb->b);
        bb = bb->sig;
    }
    if(bb->b != NULL) BalaMueve(bb->b);
}

void BalasDibuja(Balas bb){
    while(bb->sig != NULL){
        if(bb->b != NULL) BalaDibuja(bb->b);
        bb = bb->sig;
    }
    if(bb->b != NULL) BalaDibuja(bb->b);
}

void BalasColisionPersonaje(Balas bb, int x, int y, int w, int h){
    while(bb->sig != NULL){
        if(Colision(BalaGetX(bb->sig->b),BalaGetY(bb->sig->b),BalaGetW(bb->sig->b),BalaGetH(bb->sig->b),x,y,w,h)){
            Balas borrar = bb->sig;
            bb = bb->sig->sig;
            free(borrar);
        }else bb = bb->sig;
    }
}

void BalasColisionEnemigos(Balas bb, Enemigos ee){
    while(ee->sig != NULL){
        while(bb->sig != NULL){
            if(Colision(BalaGetX(bb->sig->b),BalaGetY(bb->sig->b),BalaGetW(bb->sig->b),BalaGetH(bb->sig->b),EnemigoGetX(ee->e),EnemigoGetY(ee->e),EnemigoGetW(ee->e),EnemigoGetH(ee->h))){
                Balas borrar = bb->sig;
                bb = bb->sig->sig;
                free(borrar);
            }else bb = bb->sig;
        }
        ee = ee->sig;
    }
}
