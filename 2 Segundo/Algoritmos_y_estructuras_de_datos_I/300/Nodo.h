#ifndef _NODO_
#define _NODO_

#include <list>
#include <iostream>
#include "Pagina.h"

using namespace std;

class Nodo{
	private:
		char letra;
		Nodo* siguiente;
		Nodo* hijo;
		list<Pagina*> lista;

	public:
		Nodo(char l);
		~Nodo();
		Nodo* consulta(char caracter);
		void inserta(char caracter);
		Nodo* hayMarca();
		void ponMarca(Pagina* ref);
		Nodo* getHijo();
		void ponEnLista(Pagina* ref);
		list<Pagina*> getLista();

};

#endif