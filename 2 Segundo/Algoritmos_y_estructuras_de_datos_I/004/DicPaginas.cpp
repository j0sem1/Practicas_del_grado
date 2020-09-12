#include <iostream>
#include <list>
#include "Pagina.h"
#include "DicPaginas.h"

using namespace std;

DicPaginas::DicPaginas()
{
	this->contador = 0;
}

bool DicPaginas::insertar(Pagina nueva)
{
	if (this->contador == 0)
	{
		this->lista.push_back(nueva);
		int a = this->lista.size();
		this->contador = 1;
		return true;
	} else {

	if ( this->consultar(nueva.getUrl()) == NULL) 
	{
		list<Pagina>::iterator iter = this->lista.begin();

		int it = 0;
		for (it; it < this->contador; it++)
			{
				Pagina actual = *iter;
				if( nueva.getUrl() < actual.getUrl() )
				{
					this->lista.insert(iter, nueva);
					this->contador++;
					return true;
				}
				iter++;
			}
		if (it == this->contador){
			this->lista.insert(iter, nueva);
			this->contador++;
			return true;
		}

	} else {
		this->consultar(nueva.getUrl())->setRelevancia(nueva.getRelevancia());
		this->consultar(nueva.getUrl())->setTitulo(nueva.getTitulo());
			}

			}
}

//Duda
Pagina* DicPaginas::consultar(string url)
{
	list<Pagina>::iterator iter = this->lista.begin();

	for( int it = 0; it < this->contador ; it++)
	{
		Pagina actual = *iter;
		if (actual.getUrl() == url)
		{
			return &(*iter);
		} 
		++iter;
	}
	return NULL;
}
