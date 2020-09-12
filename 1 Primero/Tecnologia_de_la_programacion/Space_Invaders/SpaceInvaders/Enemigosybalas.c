//ENEMIGOS

#include "Enemigosybalas.h"
#include "Colision.h"
#include "Bala.h"
#include "Enemigos.h"
#include "Personaje.h"
#include <stdlib.h>
#include <stdio.h>
#include <time.h>

struct EnemigosRep{
    Enemigo e;
    struct EnemigosRep * sig;
    int n;   //Nº de enemigos
    int cont;   //Contador, cada enemigo tiene un número (La cabecera es el 0)
    //Si en lugar de struct EnemigosRep ponemos Enemigos no haría falta poner la * antes de sig
};

Enemigos EnemigosCrea(){
    Enemigos cabecera = malloc(sizeof(struct EnemigosRep));
    cabecera->sig = NULL;
    cabecera->e = NULL;
    cabecera->n = 0;
    cabecera->cont = 0;
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
    Enemigos inicio = ee;
    int cont = 0;
    while(ee->sig != NULL){
        ee = ee->sig;
        cont++;
    }
    ee->sig = malloc(sizeof(struct EnemigosRep));
    ee->sig->e = EnemigoCrea(i,x,y,w,h,vx,vy);
    ee->sig->sig = NULL;
    ee->sig->n = 0;
    ee->sig->cont = cont;
    ee = inicio;
    ee->n = ee->n + 1;
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

Enemigo EnemigosRandom(Enemigos ee){
    srand(time(NULL));
    int aux = rand()%ee->n + 1;    //Ponemos 31 porque queremos que el máx sea 30. El máx será 10 pk no introduciremos más enemigos
    int cont = 0;
    while ((ee->sig != NULL)&&(cont != aux)){
        ee = ee->sig;
        cont++;
    };
    return ee->e;
};

Enemigo EnemigosRandom2(Enemigos ee){
    srand(time(NULL));
    int aux = (rand()*23)%ee->n + 1;    //Ponemos 31 porque queremos que el máx sea 30. El máx será 10 pk no introduciremos más enemigos
    int cont = 0;
    while ((ee->sig != NULL)&&(cont != aux)){
        ee = ee->sig;
        cont++;
    };
    return ee->e;
};

int NumeroEnemigos(Enemigos ee){
    return ee->n;
}


//BALAS

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
        bb = bb->sig;
        BalaDibuja(bb->b);
    }
}


//BALAS Y ENEMIGOS

int BalasColisionPersonaje(Balas bb, int x, int y, int w, int h){
    int resultado = 0;
    while(bb->sig != NULL){
        if(Colision(BalaGetX(bb->sig->b),BalaGetY(bb->sig->b),BalaGetW(bb->sig->b),BalaGetH(bb->sig->b),x,y,w,h)){
            Balas borrarbala = bb->sig;
            if(borrarbala->sig != NULL){
                bb->sig = borrarbala->sig;
            }else bb->sig = NULL;
            free(borrarbala);   //REALMENTE LO BORRA????
            resultado = 1;
        }else bb = bb->sig;
    };
    return resultado;
}

int BalasColisionEnemigos(Balas bb, Enemigos ee){
    Balas indicebalas = bb;
    Enemigos indiceenemigos = ee;
    int cont = 0;
    while(ee->sig != NULL){
        bb = indicebalas;
        while((bb->sig != NULL)&&(ee->sig != NULL)){
            if(Colision(BalaGetX(bb->sig->b),BalaGetY(bb->sig->b),BalaGetW(bb->sig->b),BalaGetH(bb->sig->b),EnemigoGetX(ee->sig->e),EnemigoGetY(ee->sig->e),EnemigoGetW(ee->sig->e),EnemigoGetH(ee->sig->e))){
                cont += 2;
                Enemigos borrarenemigo = ee->sig;
                if(borrarenemigo->sig!=NULL){
                    ee->sig = borrarenemigo->sig;
                }else{
                    ee->sig = NULL;
                };
                free(borrarenemigo);   //REALMENTE LO BORRA????
                indiceenemigos->n -= 1;
                Balas borrarbala = bb->sig;
                if(borrarbala->sig != NULL){
                    bb->sig = borrarbala->sig;
                }else bb->sig = NULL;
                free(borrarbala);   //REALMENTE LO BORRA????
            }else bb = bb->sig;
        };
        if(ee->sig != NULL) ee = ee->sig;
    };
    return cont;
}
