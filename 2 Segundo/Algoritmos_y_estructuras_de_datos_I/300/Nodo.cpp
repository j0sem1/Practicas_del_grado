#include <list>
#include <iostream>
#include "Nodo.h"
#include "Pagina.h"

using namespace std;

Nodo::Nodo(char l){
	letra = l;
	siguiente = NULL;
	hijo = NULL;
}

Nodo::~Nodo(){
	delete siguiente;
	delete hijo;
}


Nodo* Nodo::consulta(char caracter){
	if (letra == caracter){
		return this;
	} else {
		if (siguiente != NULL){
			return siguiente->consulta(caracter);
		} else {
			return NULL;
		}
	}
}

void Nodo::inserta(char caracter){
	if (siguiente == NULL){
		siguiente = new Nodo(caracter);
		siguiente->hijo = new Nodo(' ');
	} else {
		siguiente->inserta(caracter);
	}
}

Nodo* Nodo::hayMarca(){
	if (letra == '$'){
		return this;
	} else if (siguiente == NULL){
		return NULL;
	} else {
		return siguiente->hayMarca();
	}
}

void Nodo::ponMarca(Pagina* ref){
	if (siguiente == NULL){
		siguiente = new Nodo('$');
		siguiente->ponEnLista(ref);
	} else {
		siguiente->ponMarca(ref);
	}
}

Nodo* Nodo::getHijo(){
	return hijo;
}

void Nodo::ponEnLista(Pagina* ref){
	list<Pagina*>::iterator iter = lista.begin();
	for (iter; iter != lista.end(); iter++){
		if (*iter == ref){
			return;
		}
	}
	if (lista.size() == 0){
		lista.push_back(ref);
	} else {
		iter = lista.begin();

		for (iter; iter != lista.end(); iter++){
			if (ref->comparar(*iter)){
				lista.insert(iter, ref);
				return;
			}
		}
		lista.push_back(ref);
	}
}


list<Pagina*> Nodo::getLista(){
	return lista;
}