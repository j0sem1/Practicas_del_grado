#include "TablaHash.h"
#include "Pagina.h"
#include <list>
#include <iostream>

using namespace std;

TablaHash::TablaHash(){
	nElem = 0;
	/*
	Tamaño inicial de la lista:
	Lo inicializaremos en la tercera parte del número de elementos aproximado que
	tiene el archivo de entrada de prueba del Mooshak -> 18000 aprox / 3 = 6000
	*/
	M = proximoPrimo(18000);								// Primer numero primo despues de 9000
	T = new list<Pagina>[M];				// ¿¿¡¡ ESTO ES ESTÁTICO O DINÁMICO ??!!
	/*
	for (int i = 0; i < M; i++){
		T[i] = 
	}
	*/
}

/*
TablaHash::~TablaHash()
{
	
	delete[] T;
}
*/

int TablaHash::proximoPrimo(int M)
{
	int m = M;
	while (true)
	{
		m++;
		int i = 2;
		for (i; i < m; i++)
		{
			if(m % i == 0)
			{
				i = m+1;
			}
		}
		if( i == m ){ return m; }
	}
}

void TablaHash::insertaOrdenado(int k, Pagina nueva)
{
	list<Pagina>::iterator iter = T[k].begin();
		for (iter; iter != T[k].end(); iter++)
		{
			Pagina actual = *iter;
			if(nueva.getUrl() < actual.getUrl())
			{
				T[k].insert(iter, nueva);
				return;
			}
		}
		T[k].push_back(nueva);
}


/*
int TablaHash::dispersion(string url){				// Devuelve la posición donde debe insertarse
	int tamanourl = 0;
	while(url[tamanourl++] != '\0');
	tamanourl--;
	// Vamos a utilizar tamañoUrl mod M
	return tamanourl%M;
}
*/

/*
int TablaHash::dispersion(string url){				// Devuelve la posición donde debe insertarse
	unsigned int k = 0;
	char c;
	for (int i = 0; i < url.size(); i++){
		c = url[i];
		k = k * 33 + c;
	}
	k = k%M;
	return k;
}
*/


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
	int antiguaM = M;
	M = M + proximoPrimo(8000);				// Ampliamos el número de casillas para la nueva tabla
	TablaHash N;
	N.M = M;			// Creamos la nueva tabla
	N.T = new list<Pagina>[M];
	for (int i = 0; i < antiguaM; i++){
	//	if (T[i].size() >= 1){
			list<Pagina>::iterator itLista;
			itLista = T[i].begin();
			//if (T[i].size() > 1){
				for (itLista; itLista != T[i].end(); itLista++){
					N.insertar(*itLista);
				}
			//} else {
			//	N.insertar(*itLista);
			//}
	//	}
	}
	
	this->T = N.T;
}



bool TablaHash::insertar(Pagina nueva){
	
	if (nElem > 2*M){
		this->ampliar();
	}
	
	//cout << "Se mete en insertar de TablaHash" << endl;				// Traza
	if (nElem == 0){
		int k = dispersion(nueva.getUrl());
		T[k].push_back(nueva);
		nElem = 1;
		//cout << "Ha insertado la primera palabra" << endl;			// Traza
		return true;
	} else {
		Pagina* pagina = buscar(nueva.getUrl());
		if (pagina == NULL){
			// Función de dispersión [k mod M]
			int k = 0;
			k = dispersion(nueva.getUrl());

			this->insertaOrdenado(k, nueva);

			nElem++;
			return true;
		} else {
			pagina->setTitulo(nueva.getTitulo());
			pagina->setRelevancia(nueva.getRelevancia());
			return false;
		}
	}
}

Pagina* TablaHash::buscar(string url){
	if (nElem != 0){
		int k = dispersion(url);
		list<Pagina>::iterator iter = T[k].begin();
		for (iter; iter != T[k].end(); iter++){
			Pagina actual = *iter;
			if (iter != T[k].end()){				// Creo que se puede quitar
				if (actual.getUrl() == url){
					return &(*iter);
				}
			}

		}
	} else {
		return NULL;
	}
	return NULL;
}

/* ANTIGUA:
Pagina* TablaHash::buscar(string url){
	//cout << "Ha entrado en buscar" << endl;								// Traza
	if (nElem != 0){
		//cout << "Va a recorrer el array por nº de elementos" << endl;	// Traza
		for (int i = 0; i < M; i++){
			list<Pagina>::iterator iter = T[i].begin();
			//cout << "Hace una variable para iterar la lista de la casilla " << i << endl;	// Traza
			//int it = 0;
			//int tamano = T[i].size();
			//cout << "Va a recorrer la lista" << endl;	// Traza
			for (iter; iter != T[i].end(); iter++){
				Pagina actual = *iter;
				//cout << "Ahora se comprueba si las Urls coinciden" << endl;	// Traza
				if (iter != T[i].end()){
					if (actual.getUrl() == url){
						//cout << "Las Urls coinciden" << endl;
						return &(*iter);
					}
					//cout << "No ha encontrado la Url, seguimos con el bucle" << endl;
				}

			}
		}
	} else {
		return NULL;
	}
	return NULL;
}
*/