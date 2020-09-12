#ifndef _TABLA_HASH_
#define _TABLA_HASH_

#include <list>
#include <iostream>
#include "Pagina.h"

using namespace std;

class TablaHash{
	private:
		list<Pagina*> *T;
		int M;
		int nElem;
		int dispersion(string url);
		void ampliar();
		void insertaOrdenado(int k, Pagina* nueva);
		int proximoPrimo(int M);

	public:
		TablaHash();
		~TablaHash();
		Pagina* insertar(Pagina nueva);
		Pagina* buscar(string url);
		int numElem(void);
};

#endif