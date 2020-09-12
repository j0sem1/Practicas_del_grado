#include "Pantalla.h"
#include "Personaje.h"
#include "Enemigo.h"
#include "Enemigosybalas.h"
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <windows.h>

int main(int argc, char *argv[]) //para incluir imagen pantalla_leeImagen("...") | para escribir tecto Pantalla_DibujaTexto(...)
{
    inicio_programa:    //Etiqueta
    srand(time(NULL));
    Pantalla_Crea("Pantalla",800,533); //Creamos la pantalla
    int terminar = 0;
    Imagen prejuego = Pantalla_ImagenLee("bewaretheinvasion.bmp",0);
    Imagen postjuego = Pantalla_ImagenLee("wecomeinpeace.bmp",0);
    Imagen ipersonaje = Pantalla_ImagenLee("personajenave.bmp",0);
    Imagen enemigo1 = Pantalla_ImagenLee("enemigo1.bmp",0);
    Imagen enemigo2 = Pantalla_ImagenLee("enemigo2.bmp",0);
    Imagen enemigo3 = Pantalla_ImagenLee("enemigo3.bmp",0);
    Imagen ibalapersonaje = Pantalla_ImagenLee("balahaciaarriba.bmp",0);
    Imagen ibalaenemigo = Pantalla_ImagenLee("balahaciaabajo.bmp",0);
    Imagen flechas = Pantalla_ImagenLee("flechas.bmp",0);
    Imagen barraespaciadora = Pantalla_ImagenLee("barraespaciadora.bmp",0);
    Personaje personaje = PersonajeCrea(ipersonaje,Pantalla_Anchura()/2-50/2,400,50,50);
    Enemigos enemigos = EnemigosCrea();
    Balas balaspersonaje = BalasCrea();
    Balas balasenemigos = BalasCrea();

    menu:
    while (Pantalla_Activa())
    {
        if(Pantalla_TeclaPulsada(SDL_SCANCODE_RETURN)) goto juego;
        if(Pantalla_TeclaPulsada(SDL_SCANCODE_F1)) goto ayuda;
        if(Pantalla_TeclaPulsada(SDL_SCANCODE_ESCAPE)) goto fin;
        Pantalla_DibujaImagen(prejuego,0,0,Pantalla_Anchura(),Pantalla_Altura());
        Pantalla_ColorTrazo(255,255,255,255);
        Pantalla_DibujaTexto("Pulsa ENTER para jugar",310,400);
        Pantalla_DibujaTexto("Pulsa ESC para salir",310,425);
        Pantalla_DibujaTexto("Pulsa F1 para ver pagina de ayuda",310,450);
        Pantalla_Actualiza();
        Pantalla_Espera(40);
    }

    ayuda:
    while (Pantalla_Activa())    //Patnalla de ayuda
    {
        if (Pantalla_TeclaPulsada(SDL_SCANCODE_SPACE)) goto menu;
        Pantalla_DibujaRellenoFondo(0,0,0,0);
        Pantalla_DibujaTexto("CONTROLES:",355,20);
        Pantalla_DibujaTexto("FLECHAS -> Movimiento del personaje",250,90);
        Pantalla_DibujaTexto("BARRA ESPACIADORA -> Disparo de las balas",220,135);
        Pantalla_DibujaTexto("Pulsa la barra espaciadora para volver al menu",200,500);
        Pantalla_DibujaImagen(flechas,150,200,140,100);
        Pantalla_DibujaImagen(barraespaciadora,420,200,240,100);
        Pantalla_Actualiza();
        Pantalla_Espera(40);
    }

    juego:


    EnemigosInserta(enemigos,enemigo1,50,250,50,40,1,1);
    EnemigosInserta(enemigos,enemigo1,120,250,50,40,1,1);
    EnemigosInserta(enemigos,enemigo1,190,250,50,40,1,1);
    EnemigosInserta(enemigos,enemigo1,260,250,50,40,1,1);
    EnemigosInserta(enemigos,enemigo1,330,250,50,40,1,1);
    EnemigosInserta(enemigos,enemigo1,400,250,50,40,1,1);
    EnemigosInserta(enemigos,enemigo1,470,250,50,40,1,1);
    EnemigosInserta(enemigos,enemigo1,540,250,50,40,1,1);
    EnemigosInserta(enemigos,enemigo1,610,250,50,40,1,1);
    EnemigosInserta(enemigos,enemigo1,680,250,50,40,1,1);
    EnemigosInserta(enemigos,enemigo2,50,150,50,40,1,1);
    EnemigosInserta(enemigos,enemigo2,120,150,50,40,1,1);
    EnemigosInserta(enemigos,enemigo2,190,150,50,40,1,1);
    EnemigosInserta(enemigos,enemigo2,260,150,50,40,1,1);
    EnemigosInserta(enemigos,enemigo2,330,150,50,40,1,1);
    EnemigosInserta(enemigos,enemigo2,400,150,50,40,1,1);
    EnemigosInserta(enemigos,enemigo2,470,150,50,40,1,1);
    EnemigosInserta(enemigos,enemigo2,540,150,50,40,1,1);
    EnemigosInserta(enemigos,enemigo2,610,150,50,40,1,1);
    EnemigosInserta(enemigos,enemigo2,680,150,50,40,1,1);
    EnemigosInserta(enemigos,enemigo3,50,50,50,40,1,1);
    EnemigosInserta(enemigos,enemigo3,120,50,50,40,1,1);
    EnemigosInserta(enemigos,enemigo3,190,50,50,40,1,1);
    EnemigosInserta(enemigos,enemigo3,260,50,50,40,1,1);
    EnemigosInserta(enemigos,enemigo3,330,50,50,40,1,1);
    EnemigosInserta(enemigos,enemigo3,400,50,50,40,1,1);
    EnemigosInserta(enemigos,enemigo3,470,50,50,40,1,1);
    EnemigosInserta(enemigos,enemigo3,540,50,50,40,1,1);
    EnemigosInserta(enemigos,enemigo3,610,50,50,40,1,1);
    EnemigosInserta(enemigos,enemigo3,680,50,50,40,1,1);

    int t1 = time(NULL);
    int t2 = time(NULL);
    int t = t2-t1;

    int tbalap = 20;  //velocidad del tiempo en el que puedes lanzar una bala
    int tiempobalap = 0;    //Función para controlar el tiempo en el que puedes lanzar una bala. Esta es la bala del personaje

    double tbalae = 40;
    int tiempobalae = 0;

    int dosbalas = 0;
    PlaySound("musica.wav",NULL,SND_ASYNC);

    int puntos = 0;

    double velocidadbalasenemigas = 5;
    while (Pantalla_Activa()&&!terminar)
    {
        if (Pantalla_TeclaPulsada(SDL_SCANCODE_ESCAPE)) terminar=1;


        t2=time(NULL);
        t=t2-t1;
        char texto_tiempo[100];
        sprintf(texto_tiempo,"%d segundos",t);

        Pantalla_DibujaRellenoFondo(0,0,0,0);

        if (Pantalla_TeclaPulsada(SDL_SCANCODE_SPACE)&&tiempobalap==0){
            BalasInserta(balaspersonaje,ibalapersonaje,PersonajeGetX(personaje)+PersonajeGetW(personaje)/2,PersonajeGetY(personaje),3,10,-5);
            tiempobalap = tbalap;
        };
        if (tiempobalap!=0){
          tiempobalap = tiempobalap-1;
        };  //Aquí implementamos tiempobala. Esta es la bala del personaje


        if (tiempobalae==0){    //Bala1
            Enemigo erandom = EnemigosRandom(enemigos);   //Situa en erandom un ememigo random de la estructura enlazada
            BalasInserta(balasenemigos,ibalaenemigo,EnemigoGetX(erandom)+EnemigoGetW(erandom)/2,EnemigoGetY(erandom)+EnemigoGetH(erandom),3,10,velocidadbalasenemigas);
            if(dosbalas == 1){
                Enemigo erandom2 = EnemigosRandom2(enemigos);
                BalasInserta(balasenemigos,ibalaenemigo,EnemigoGetX(erandom2)+EnemigoGetW(erandom2)/2,EnemigoGetY(erandom2)+EnemigoGetH(erandom2),3,10,velocidadbalasenemigas);
            };
            tiempobalae = tbalae;
        };
        if (tiempobalae!=0){
          tiempobalae = tiempobalae-1;
        };

        //Mover personaje
        if (Pantalla_TeclaPulsada(SDL_SCANCODE_RIGHT)) PersonajeMueve(personaje,10,0);
        if (Pantalla_TeclaPulsada(SDL_SCANCODE_LEFT)) PersonajeMueve(personaje,-10,0);
        if (Pantalla_TeclaPulsada(SDL_SCANCODE_DOWN)) PersonajeMueve(personaje,0,10);
        if (Pantalla_TeclaPulsada(SDL_SCANCODE_UP)) PersonajeMueve(personaje,0,-10);
        EnemigosMueve(enemigos);
        BalasMueve(balaspersonaje);
        BalasMueve(balasenemigos);


        puntos = puntos + BalasColisionEnemigos(balaspersonaje,enemigos);
        if (BalasColisionPersonaje(balasenemigos,PersonajeGetX(personaje),PersonajeGetY(personaje),PersonajeGetW(personaje),PersonajeGetH(personaje))) terminar=1;

        //Dibujar personaje
        PersonajeDibuja(personaje);
        EnemigosDibuja(enemigos);

        BalasDibuja(balaspersonaje);
        BalasDibuja(balasenemigos);

        if(velocidadbalasenemigas < 14) velocidadbalasenemigas += 0.02;
        if(tbalae > 20){
            tbalae -= 0.1;
        }else dosbalas = 1;

        if(NumeroEnemigos(enemigos) == 0){
            EnemigosInserta(enemigos,enemigo1,50,250,50,40,1,1);
            EnemigosInserta(enemigos,enemigo1,120,250,50,40,1,1);
            EnemigosInserta(enemigos,enemigo1,190,250,50,40,1,1);
            EnemigosInserta(enemigos,enemigo1,260,250,50,40,1,1);
            EnemigosInserta(enemigos,enemigo1,330,250,50,40,1,1);
            EnemigosInserta(enemigos,enemigo1,400,250,50,40,1,1);
            EnemigosInserta(enemigos,enemigo1,470,250,50,40,1,1);
            EnemigosInserta(enemigos,enemigo1,540,250,50,40,1,1);
            EnemigosInserta(enemigos,enemigo1,610,250,50,40,1,1);
            EnemigosInserta(enemigos,enemigo1,680,250,50,40,1,1);
            EnemigosInserta(enemigos,enemigo2,50,150,50,40,1,1);
            EnemigosInserta(enemigos,enemigo2,120,150,50,40,1,1);
            EnemigosInserta(enemigos,enemigo2,190,150,50,40,1,1);
            EnemigosInserta(enemigos,enemigo2,260,150,50,40,1,1);
            EnemigosInserta(enemigos,enemigo2,330,150,50,40,1,1);
            EnemigosInserta(enemigos,enemigo2,400,150,50,40,1,1);
            EnemigosInserta(enemigos,enemigo2,470,150,50,40,1,1);
            EnemigosInserta(enemigos,enemigo2,540,150,50,40,1,1);
            EnemigosInserta(enemigos,enemigo2,610,150,50,40,1,1);
            EnemigosInserta(enemigos,enemigo2,680,150,50,40,1,1);
            EnemigosInserta(enemigos,enemigo3,50,50,50,40,1,1);
            EnemigosInserta(enemigos,enemigo3,120,50,50,40,1,1);
            EnemigosInserta(enemigos,enemigo3,190,50,50,40,1,1);
            EnemigosInserta(enemigos,enemigo3,260,50,50,40,1,1);
            EnemigosInserta(enemigos,enemigo3,330,50,50,40,1,1);
            EnemigosInserta(enemigos,enemigo3,400,50,50,40,1,1);
            EnemigosInserta(enemigos,enemigo3,470,50,50,40,1,1);
            EnemigosInserta(enemigos,enemigo3,540,50,50,40,1,1);
            EnemigosInserta(enemigos,enemigo3,610,50,50,40,1,1);
            EnemigosInserta(enemigos,enemigo3,680,50,50,40,1,1);
        }

        Pantalla_DibujaTexto(texto_tiempo,620,20);

        Pantalla_Actualiza();
        Pantalla_Espera(40);
    };

    char texto[100];
    char texto_nuevor[100];
    char texto_record[100];

    int n=0;
    FILE * f = fopen("datos.txt","r");
    fscanf(f,"%d",&n);
    fclose(f);

    puntos += t;

    if (puntos>n)
    {
        f = fopen("datos.txt","w");
        fprintf(f,"%d",puntos);
        fclose(f);
        sprintf(texto_nuevor,"Has marcado un nuevo record de %d puntos",puntos);
    }
    else sprintf(texto_record,"No has conseguido batir el record de %d puntos",n);

    PlaySound(NULL,NULL,0);

    while (Pantalla_Activa())
    {
        Pantalla_DibujaImagen(postjuego,0,0,Pantalla_Anchura(),Pantalla_Altura());

        if(Pantalla_TeclaPulsada(SDL_SCANCODE_ESCAPE)) goto fin;
        if(Pantalla_TeclaPulsada(SDL_SCANCODE_RETURN)) goto inicio_programa;

        sprintf(texto,"Tu tiempo ha sido de %d puntos",puntos);
        Pantalla_DibujaTexto("Pulsa ESC para salir",300,450);
        Pantalla_DibujaTexto("Pulsa ENTER para volver a jugar",250,500);
        Pantalla_DibujaTexto(texto,30,50);

        Pantalla_DibujaTexto(texto_nuevor,450,50);
        Pantalla_DibujaTexto(texto_record,350,50);

        Pantalla_Actualiza();
        Pantalla_Espera(40);
    }

    fin:
    BalasLibera(balaspersonaje);
    EnemigosLibera(enemigos);
    Pantalla_ImagenLibera(prejuego);
    PersonajeLibera(personaje);
    Pantalla_ImagenLibera(postjuego);
    Pantalla_ImagenLibera(ibalaenemigo);
    Pantalla_ImagenLibera(ibalapersonaje);
    Pantalla_ImagenLibera(barraespaciadora);
    Pantalla_ImagenLibera(flechas);
    Pantalla_Libera();
    return 0;
}
