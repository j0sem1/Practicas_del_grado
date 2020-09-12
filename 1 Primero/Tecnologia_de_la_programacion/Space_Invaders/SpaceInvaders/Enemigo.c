#include "Enemigo.h"
#include <stdlib.h>

struct EnemigoRep{
        Imagen i;
        int x,y,w,h,vx,vy,aux2,aux1;
};

Enemigo EnemigoCrea(Imagen i, int x, int y, int w, int h, int vx, int vy){
    Enemigo e = malloc(sizeof(struct EnemigoRep));
    e->i = i;
    e->x = x;
    e->y = y;
    e->w = w;
    e->h = h;
    e->vx = vx;
    e->vy = vy;
    e->aux1 = e->x-45;
    e->aux2 = e->x+60;
    return e;
}

void EnemigoLibera(Enemigo e){
    free(e);
}

void EnemigoMueve(Enemigo e){   //En área limitada
    e->x+=e->vx;
    if (e->x<e->aux1){
        e->vx=e->vx*(-1);
        e->x+=e->vx;
    };
    if (e->x>e->aux2){
        e->vx=e->vx*(-1);
        e->x+=e->vx;
    };
}

int EnemigoGetX(Enemigo e){
    return e->x;
}

int EnemigoGetY(Enemigo e){
    return e->y;
}

int EnemigoGetW(Enemigo e){
    return e->w;
}

int EnemigoGetH(Enemigo e){
    return e->h;
}

void EnemigoDibuja(Enemigo e){
    Pantalla_DibujaImagen(e->i,e->x,e->y,e->w,e->h);
}
