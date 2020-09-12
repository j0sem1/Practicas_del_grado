#include <stdlib.h>
#include <math.h>
#include "Colision.h"

int Colision(int x1, int y1, int w1, int h1, int x2, int y2, int w2, int h2){

    int r1=(double)h1/2.0;

    int r2=sqrt(pow(w2/2,2)+pow(h2/2,2));

    int c1x=(double)x1+(double)w1/2.0;
    int c1y=(double)y1+(double)h1/2.0;

    int c2x=(double)x2+(double)w2/2.0;
    int c2y=(double)y2+(double)h2/2.0;

    int d=sqrt(pow(c1x-c2x,2)+pow(c1y-c2y,2));

    return d<r1+r2;
};
