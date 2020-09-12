#include "Bala.h"
#include <stdlib.h>

struct BalaRep{
    Imagen i;
    int x,y,w,h,vy;
};

Bala BalaCrea(Imagen i, int x, int y, int w, int h, int vy){
    Bala b = malloc(sizeof(struct BalaRep));
    b->i = i;
    b->x = x;
    b->y = y;
    b->w = w;
    b->h = h;
    b->vy = vy;
    return b;
}

void BalaLibera(Bala b){
    free(b);
}

void BalaMueve(Bala b){
    b->y += b->vy;
}

int BalaGetX(Bala b){
    return b->x;
}

int BalaGetY(Bala b){
    return b->y;
}

int BalaGetW(Bala b){
    return b->w;
}

int BalaGetH(Bala b){
    return b->h;
}

void BalaDibuja(Bala b){
    Pantalla_DibujaImagen(b->i,b->x,b->y,b->w,b->h);
}
