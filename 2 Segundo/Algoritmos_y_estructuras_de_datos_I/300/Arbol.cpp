#include <list>
#include <iostream>
#include "Pagina.h"
#include "Nodo.h"
#include "Arbol.h"

using namespace std;

Arbol::Arbol(){
	cabecera = new Nodo(' ');
	nElem = 0;
}

Arbol::~Arbol(){
	delete cabecera;
}

int Arbol::numElem(){
	return nElem;
}

list<Pagina*> Arbol::buscar(string palabra){
	list<Pagina*> vacia;
	Nodo* nodo = cabecera;
	Nodo* aux = NULL;
	for (int i = 0; i < palabra.size(); i++){
		aux = nodo->consulta(palabra[i]);
		if (aux != NULL){
			nodo = aux->getHijo();
		} else {
			return vacia;
		}
	}
	if (nodo->hayMarca() != NULL){
		return nodo->hayMarca()->getLista();
	} else {
		return vacia;
	}
}

void Arbol::insertar(string palabra, Pagina* ref){
	Nodo* nodo = cabecera;
	Nodo* aux = NULL;
	for (int i = 0; i < palabra.size(); i++){
		aux = nodo->consulta(palabra[i]);
		if (aux == NULL){
			nodo->inserta(palabra[i]);
			nodo = nodo->consulta(palabra[i])->getHijo();	// Aqui no se puede aprovechar aux porque tenemos que consultar despues de insertar una nueva palabra
		} else {
			nodo = aux->getHijo();
		}
	}
	if (buscar(palabra).size() == 0){
		nodo->ponMarca(ref);
		nElem++;
	} else {
		nodo = nodo->consulta('$');
		nodo->ponEnLista(ref);
	}
}