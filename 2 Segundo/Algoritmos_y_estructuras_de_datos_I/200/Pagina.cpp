#include <iostream>
#include <sstream>
#include "Pagina.h"

using namespace std;

Pagina::Pagina(int relevancia, string url, string titulo)
{
	this->relevancia = relevancia;
	this->url = url;
	this->titulo = titulo;
}

string Pagina::getUrl()
{
	return this->url;
}

string Pagina::getTitulo()
{
	return this->titulo;
}

int Pagina::getRelevancia()
{
	return this->relevancia;
}

void Pagina::setUrl(string url)
{
	this->url = url;
}

void Pagina::setTitulo(string titulo)
{
	this->titulo = titulo;
}

void Pagina::setRelevancia(int relevancia)
{
	this->relevancia = relevancia;
}