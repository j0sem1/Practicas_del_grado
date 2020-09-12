/*
	DicPaginas
		lista de paginas
*/

#ifndef _DIC_PAGINAS_
#define _DIC_PAGINAS_

#include <iostream>
#include "Pagina.h"
#include "TablaHash.h"

using namespace std;

class DicPaginas
{
	private:
		TablaHash tabla;

	public:
		//DicPaginas();						// Es necesario?
		bool insertar(Pagina nueva);
		Pagina* consultar(string url){
			return tabla.buscar(url);
		}
		int numElem(void)
			{
				return tabla.numElem();
			}
};

#endif