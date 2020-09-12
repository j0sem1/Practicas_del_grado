#include <iostream>
#include "Pagina.h"

using namespace std;

Pagina::Pagina(int relevancia, string url, string titulo){
	this->relevancia = relevancia;
	this->url = url;
	this->titulo = titulo;
}

string Pagina::getUrl(){
	return url;
}

string Pagina::getTitulo(){
	return titulo;
}

int Pagina::getRelevancia(){
	return relevancia;
}

void Pagina::setUrl(string url){
	this->url = url;
}

void Pagina::setTitulo(string titulo){
	this->titulo = titulo;
}

void Pagina::setRelevancia(int relevancia){
	this->relevancia = relevancia;
}

bool Pagina::comparar(Pagina* pagina){
	if (relevancia > pagina->getRelevancia()){
		return true;
	} else if (relevancia < pagina->getRelevancia()){
		return false;
	} else {	// Comparar urls
		if (url < pagina->getUrl()){
			return true;
		} else {
			return false;
		}
	}
}