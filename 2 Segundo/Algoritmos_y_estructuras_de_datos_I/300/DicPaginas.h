#ifndef _DIC_PAGINAS_
#define _DIC_PAGINAS_

#include <iostream>
#include "Pagina.h"
#include "TablaHash.h"
#include "Arbol.h"

using namespace std;

class DicPaginas
{
	private:
		TablaHash tabla;
		Arbol arbol;

	public:
		Pagina* insertar(Pagina nueva);
		void insertar(string palabra, Pagina* ref);
		Pagina* consultar(string url);
		int numPaginas();
		int numPalabras();
		list<Pagina*> buscar(string palabra);
};

#endif