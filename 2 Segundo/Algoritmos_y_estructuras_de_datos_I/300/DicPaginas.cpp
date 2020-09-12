#include "DicPaginas.h"
#include "Pagina.h"

Pagina* DicPaginas::insertar(Pagina nueva){
	return tabla.insertar(nueva);
}

Pagina* DicPaginas::consultar(string url){
	return tabla.buscar(url);
}

int DicPaginas::numPaginas(){
	return tabla.numElem();
}

int DicPaginas::numPalabras(){
	return arbol.numElem();
}

void DicPaginas::insertar(string palabra, Pagina* ref){
	arbol.insertar(palabra, ref);
}

list<Pagina*> DicPaginas::buscar(string palabra){
	return arbol.buscar(palabra);
}