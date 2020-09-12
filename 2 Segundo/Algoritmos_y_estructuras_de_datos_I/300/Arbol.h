#ifndef _ARBOL_
#define _ARBOL_

#include <list>
#include <iostream>
#include "Nodo.h"

using namespace std;

class Arbol{
	private:
		Nodo* cabecera;
		int nElem;

	public:
		Arbol();
		~Arbol();
		void insertar(string palabra, Pagina* ref);
		list<Pagina*> buscar(string palabra);
		int numElem();

};

#endif