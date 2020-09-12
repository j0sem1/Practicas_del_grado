#include <list>
#include <iostream>
#include "Pagina.h"
#include "DicPaginas.h"

using namespace std;

string normalizar(string entrada){
	string salida;
	for(unsigned int i = 0; i < entrada.length(); i++){
		if ((entrada[i] >= 'A') && (entrada[i] <= 'Z')){
			salida = (salida + char(tolower(entrada[i])));
		} else if (entrada[i] == char(0xC3)){
			i++;
			if ((entrada[i] == char(0xA1)) || (entrada[i] == char(0x81))) salida = salida + 'a';
			else if ((entrada[i] == char(0xA9)) || (entrada[i] == char(0x89))) salida = salida + 'e';
			else if ((entrada[i] == char(0xAD)) || (entrada[i] == char(0x8D))) salida = salida + 'i';
			else if ((entrada[i] == char(0xB3)) || (entrada[i] == char(0x93))) salida = salida + 'o';
			else if ((entrada[i] == char(0xBA)) || (entrada[i] == char(0xBC)) || (entrada[i] == char(0x9A)) || (entrada[i] == char(0x9C))) salida = salida + 'u';
			else if ((entrada[i] == char(0xB1)) || (entrada[i] == char(0x91))) salida = salida + char(0xC3) + char(0xB1);
			else salida = salida + char(0xC3) + entrada[i];
		} else {
			salida = (salida + entrada[i]);
		}
	}	
	return salida;
}

void insertar(DicPaginas &dic){

	string comodin, url, titulo, entrada_palabra;
	int relevancia = 0;
	int n_palabras = 0;

	cin >> relevancia;

	getline(cin, comodin);

	getline(cin, url);

	getline(cin, titulo);

	Pagina p(relevancia, url, titulo);

	Pagina* ref = dic.insertar(p);

	while ((cin >> entrada_palabra)&&(normalizar(entrada_palabra) != "findepagina")){
		n_palabras++;
		dic.insertar(normalizar(entrada_palabra), ref);
	}

	cout << dic.numPaginas() << ". " << url << ", " << titulo << ", Rel. " << relevancia << endl;
	cout << n_palabras << " palabras" << endl;
}


void busquedaPalabra(DicPaginas &dic){
	string palabra;
	int resultados = 0;

	cin >> palabra;
	palabra = normalizar(palabra);

	cout << "b"  << " " << palabra << endl;

	list<Pagina*> palabras = dic.buscar(palabra);
	list<Pagina*>::iterator iter = palabras.begin();

	for (int i = 0; i < palabras.size(); i++){
		Pagina* actual = *iter;
		cout << (i + 1) << ". " << actual->getUrl() << ", " << actual->getTitulo() << ", Rel. " << actual->getRelevancia() << endl;
		iter++;
	}

	cout << "Total: " << palabras.size() << " resultados" << endl;
}

void busquedaUrl(DicPaginas &dic){
	string url;
	int resultados = 0;

	cin >> url;
	cout << "u" << " " << url << endl;
	
	Pagina* pagina = dic.consultar(url);
	if(pagina == NULL){
		resultados = 0;
	} else {
		cout << "1. " << url << ", " << pagina->getTitulo() << ", Rel. " << pagina->getRelevancia() << endl;
		resultados = 1;
	}


	cout << "Total: " << resultados << " resultados" << endl;
}

void busquedaAnd(DicPaginas &dic){
	string a;
	int resultados = 0;

	getline(cin, a);
	a = normalizar(a);

	cout << "a"  << " " << a << endl;
	cout << "Total: " << resultados << " resultados" << endl;
}

void busquedaOr(DicPaginas &dic){
	string o;
	int resultados = 0;

	getline(cin, o);
	o = normalizar(o);

	cout << "o"  << " " << o << endl;
	cout << "Total: " << resultados << " resultados" << endl;
}

void busquedaPrefijo(DicPaginas &dic){
	string prefijo;
	int resultados = 0;

	cin >> prefijo;
	prefijo = normalizar(prefijo);

	cout << "p" << " " << prefijo << endl;
	cout << "Total: " << resultados << " resultados" << endl;
}


void interprete(char comando, DicPaginas &dic){
	if (comando == 'i') insertar(dic);
	else if (comando == 'u') busquedaUrl(dic);
	else if (comando == 'b') busquedaPalabra(dic);
	else if (comando == 'a') busquedaAnd(dic);
	else if (comando == 'o') busquedaOr(dic);
	else if (comando == 'p') busquedaPrefijo(dic);
}

int main(void){
	DicPaginas dic;
	char comando;
	while ((cin >> comando)&&(comando != 's')){
		interprete(comando, dic);
	}
	if (comando == 's') cout << "Saliendo..." << endl;
	return 0;
}