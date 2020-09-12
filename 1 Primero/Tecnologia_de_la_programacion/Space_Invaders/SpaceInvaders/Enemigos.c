#include "Enemigos.h"
#include "Colision.h"
#include <stdlib.h>

struct EnemigosRep{
    Enemigo e;
    struct EnemigosRep * sig;   //Si en lugar de struct EnemigosRep ponemos Enemigos no haría falta poner la * antes de sig
};

Enemigos EnemigosCrea(){
    Enemigos cabecera = malloc(sizeof(struct EnemigosRep));
    cabecera->sig = NULL;
    cabecera->e = NULL;
    return cabecera;
}

void EnemigosLibera(Enemigos ee){
    Enemigos aux = ee->sig;
    while(aux != NULL){
        Enemigos borrar = aux;
        aux = aux->sig;
        free(borrar);
    }
    free(ee);
}

void EnemigosInserta(Enemigos ee, Imagen i, int x, int y, int w, int h, int vx, int vy){
    while(ee->sig != NULL){
        ee = ee->sig;
    }
    ee->sig = malloc(sizeof(struct EnemigosRep));
    ee->sig->e = EnemigoCrea(i,x,y,w,h,vx,vy);
    ee->sig->sig = NULL;
}

void EnemigosMueve(Enemigos ee){
    while(ee->sig != NULL){
        ee = ee->sig;
        EnemigoMueve(ee->e);
    }
}

void EnemigosDibuja(Enemigos ee){
    while(ee->sig != NULL){
        ee = ee->sig;
        EnemigoDibuja(ee->e);
    }
}

void EnemigosColision(Enemigos ee, int x, int y, int w, int h){
    while(ee->sig != NULL){
        ee = ee->sig;
        if (Colision(EnemigoGetX(ee->e),EnemigoGetY(ee->e),EnemigoGetW(ee->e),EnemigoGetH(ee->e),x,y,w,h)){
            Enemigos borrar = ee;
            ee = ee->sig;
            free(borrar);
        };
    }
}
