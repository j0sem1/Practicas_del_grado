#ifndef _TABLA_HASH_
#define _TABLA_HASH_

#include <list>
#include <iostream>
#include "Pagina.h"

using namespace std;

class TablaHash{
	private:
		//...
		list<Pagina> *T;
		int M;
		int nElem;
		int dispersion(string url);
		void ampliar();
		void insertaOrdenado(int k, Pagina nueva);
		int proximoPrimo(int M);

	public:
		TablaHash();
		//~TablaHash();
		bool insertar(Pagina nueva);
		Pagina* buscar(string url);
		int numElem(void){
			return nElem;
		};
};

#endif


/*
Quiero aplicar dispersión abierta, porque me preocupa más tener
una buena eficiencia y un tiempo de respuesta rápido a la hora de
hacer las operaciones que la memoria ocupada, eso actualmente lo
veo un recurso abundante y no tan valioso.

Características funcion de dispersión:
	- Evitar sumas
	- Evitar multiplicaciones
	- Evitar trabajar con números elevados
	- Trabajar con operaciones enteras
*Función: [Nº de referencia] mod Tamaño.
*Función: [Tamaño url] mod Tamaño
	+ Implementada mejora en la búsqueda, ahora utilizamos la función ahí también.
	+ Implementada mejora para aumentar tamaño de la tabla y que haya menos colisiones.
	+ Implementada mejora en el tamaño del array: Ahora se usan más huecos en el array para oscasionar menos colisiones.

Reestructuración dinámica

¿Tamaño inicial? -> Lo colocaremos en la tercera parte del número de elementos aproximado
que tiene el archivo de entrada del Mooshak. -> 18000 aprox / 3 = 6000.
*/