#include <iostream>
#include <list>
#include "TablaHash.h"
#include "Pagina.h"

using namespace std;

TablaHash::TablaHash(){
	nElem = 0;
	M = 1;
	T = new list<Pagina*>[M];
}


TablaHash::~TablaHash(){
	list<Pagina*>::iterator iter;
	for (int i = 0; i < M; i++){
		iter = T[i].begin();
		while(iter != T[i].end()){
			delete *iter;
			iter++;
		}
	}
	delete[] T;
}

int TablaHash::numElem(void){
	return nElem;
}

int TablaHash::proximoPrimo(int m){
	if (m == 0){
		return 1;
	} else if (m == 1){
		return 2;
	}
	int n = 0;
	int resultado = m;
	int aux = 0;
	while(true){
		n = 1;
		resultado++;
		while(n < resultado){
			n = proximoPrimo(n);
			aux = resultado/n;
			if (aux < n){
				return resultado;	// Es primo
			} else if (resultado%n == 0){
				n = resultado;		// No es primo
			}
		}
	}
}

void TablaHash::insertaOrdenado(int k, Pagina* nueva){
	Pagina* actual = NULL;
	list<Pagina*>::iterator iter = T[k].begin();
	for (iter; iter != T[k].end(); iter++){
		actual = *iter;
		if(nueva->getUrl() < actual->getUrl()){
			T[k].insert(iter, nueva);
			return;
		}
	}
	T[k].push_back(nueva);
}

int TablaHash::dispersion(string url){
	unsigned int k = 0;
	char c;
	for (int i = 0; i < url.size(); i++){
		c = url[i];
		k = c + (k << 6) + (k << 16) - k ;		// c + k*64 + k*65536 - k
	}
	k = k%M;
	return k;
}

void TablaHash::ampliar(){
	Pagina* actual = NULL;
	int antiguoTamano = M;
	list<Pagina*> *Antigua = T;
	M = proximoPrimo(2*M);
	T = new list<Pagina*>[M];
	list<Pagina*>::iterator iter;
	
	for (int i = 0; i < antiguoTamano; i++){
		iter = Antigua[i].begin();
		while (iter != Antigua[i].end()){
			actual = *iter;
			insertaOrdenado(dispersion(actual->getUrl()), actual);
			iter++;
		}
	}

	delete[] Antigua;
}

Pagina* TablaHash::insertar(Pagina nueva){
	
	if (nElem > 2*M){
		ampliar();
	}
	
	Pagina* pagina = buscar(nueva.getUrl());
	if (pagina == NULL){
		int k = dispersion(nueva.getUrl());
		Pagina* ref = new Pagina(nueva.getRelevancia(), nueva.getUrl(), nueva.getTitulo());
		insertaOrdenado(k, ref);
		nElem++;
		return ref;
	} else {
		pagina->setTitulo(nueva.getTitulo());
		return pagina;
	}
}

Pagina* TablaHash::buscar(string url){
	if (nElem != 0){
		Pagina* actual = NULL;
		int k = dispersion(url);
		list<Pagina*>::iterator iter = T[k].begin();
		for (iter; iter != T[k].end(); iter++){
			actual = *iter;
			if (actual->getUrl() > url){
				return NULL;
			} else if (actual->getUrl() == url){
				return actual;
			}
		}
		return NULL;
	} else {
		return NULL;
	}
}