/*
	Pagina
		Título
		url (ID)
		relevancia
*/

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
};

#endif