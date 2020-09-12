#include <stdlib.h>
#include "Personaje.h"

struct PersonajeRep{
        Imagen i;
        int x,y,w,h;
};

Personaje PersonajeCrea(Imagen i,int x,int y,int w,int h){
    Personaje p = malloc(sizeof(struct PersonajeRep));
    p->i=i;
    p->x=x;
    p->y=y;
    p->h=h;
    p->w=w;
    return p;
}

void PersonajeLibera(Personaje p){
    free(p);
};

void PersonajeMueve(Personaje p, int vx,int vy){    //En área limitada
    p->x+=vx;
    p->y+=vy;
    if (p->x<0) p->x=0;
    if (p->x>Pantalla_Anchura()-p->w) p->x=Pantalla_Anchura()-p->w;
    if (p->y<400) p->y=400;
    if (p->y>Pantalla_Altura()-p->h) p->y=Pantalla_Altura()-p->h;
};

void PersonajeDibuja(Personaje p){
    Pantalla_DibujaImagen(p->i,p->x,p->y,p->w,p->h);
};

int PersonajeGetX(Personaje p){
    return p->x;
};

int PersonajeGetY(Personaje p){
    return p->y;
};

int PersonajeGetH(Personaje p){
    return p->h;
};

int PersonajeGetW(Personaje p){
    return p->w;
};
