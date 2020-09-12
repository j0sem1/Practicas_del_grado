/*
	DicPaginas
		lista de paginas
*/

#ifndef _DIC_PAGINAS_
#define _DIC_PAGINAS_

#include <iostream>
#include <list>
#include "Pagina.h"

using namespace std;

class DicPaginas
{
	private:
		list<Pagina> lista;
		int contador;

	public:
		DicPaginas();
		bool insertar (Pagina nueva);
		Pagina* consultar(string url);
		int numElem(void)
			{
				return this->contador;
			}
};

#endif