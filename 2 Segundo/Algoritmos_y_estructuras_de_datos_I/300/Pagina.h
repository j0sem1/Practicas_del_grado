#ifndef _PAGINA_
#define _PAGINA_

#include <iostream>

using namespace std;

class Pagina
{
	private:
		int relevancia;
		string url, titulo;

	public:
		Pagina(int relevancia, string url, string titulo);
		string getUrl();
		string getTitulo();
		int getRelevancia();
		void setUrl(string url);
		void setTitulo(string titulo);
		void setRelevancia(int relevancia);
		/*
		Criterio de comparación para las páginas:
		- Devuelve 'true' si la relevancia es mayor que la página del parámetro
			Si la revlevancia es igual compara las url:
			- Devuelve 'true' si la url es menor que la del parámetro
		*/
		bool comparar(Pagina* pagina);
};

#endif