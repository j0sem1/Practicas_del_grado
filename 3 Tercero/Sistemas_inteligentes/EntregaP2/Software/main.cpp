#include <string>
#include <iostream>
#include <fstream>
#include <cstring>
#include <stdlib.h>

using namespace std;


/**********************
***	TIPOS DEFINIDOS ***
***********************/

struct atributo {
    string nombre;
    string valor;
    int regla;
};

struct antecedente {
	string atributo;
	string operador;
	string valor;
};

struct consecuencia{
	string atributo; // Nombre del atributo al que le afectará la regla
	string valor;
};

struct regla{
	antecedente* antecedentes;
	consecuencia cons;
	bool marcada; // Será 'true' si la regla ya ha sido usada
	int n_antecedentes; // Número de antecedentes de la regla
	int nregla; // Usado por la regla ConjuntoConflicto
};


/*****************
***	CONSTANTES ***
******************/
/*
	Para almacenar los antecedentes de cada regla se usará un array. Como a
	priori no sabemos el número de antecedentes que tendrá la regla,
	se supone un máximo de 10 antecedentes en todos los casos.
*/
const int nAntecedentesMax = 10;


/*************************
***	VARIABLES GLOBALES ***
**************************/
// Número de reglas que tiene la Base de Conocimiento (BC)
int nReglas;
// Número de atributos que tiene el fichero de configuración
int nAtributos;
// Dominio leído del fichero de configuración
string dominio;
// Atributo objetivo del dominio actual
string objetivo;
/*
	Explicación de cómo se obtiene el valor del objetivo activando
	las reglas necesarias en el orden adecuado. Se usará para para
	imprimirlo en el fichero Salida1
*/
string resolucion;
/*
    Reglas que se han disparado para obtener el objetivo. SOLO contiene
    las que son necesarias para llegar a ese objetivo.
*/
string conclusionGlobal;

/*
	Arrays globales. Se declaran los punteros y más tarde
	se crearán los correspondientes arrays.
*/
int* prioridades;
atributo* BH;	// Base de Hechos
atributo* BHInicial; // Se usará para el archivo Salida1
regla* reglas; // Array con todas las reglas de la BC
/*
	Regla que simulará no tener ninguna regla escogida. Funcionará
	de manera similar a como lo haría un NULL. Utilizada en las
	funciones equiparar() y vacioCC().
*/
regla reglaVacia;


/*********************************************
***	DECLARACIONES ADELANTADAS DE FUNCIONES ***
**********************************************/
void leerConoc(char* nombreFicheroConoc);
void leerConf(char* nombreFicheroConf);
string getPrimeraPalabra(string texto);
void leerHechos(char* nombreFicheroHechos);
bool motorInferencia();
regla extraerCualquierRegla();
bool vacioCC(regla conjuntoConflicto);
bool objetivoCumplido();
bool comprobarAntecedentes(int idRegla);
regla equiparar();
void aplicarRegla(regla conjuntoConflicto);
void procesoSeguido(int atributo);
void salida1(char* nombreFicheroHechos);
void salida2(char* nombreFicheroHechos);


/***********
***	MAIN ***
************/

int main(int argc, char **argv){
	// Declaramos las variables
	char* nombreFicheroConf, *nombreFicheroConoc, *nombreFicheroHechos;

	// Inicializamos las variables leyendo los parámetros
	nombreFicheroConf = argv[1];
	nombreFicheroConoc = argv[2];
	nombreFicheroHechos = argv[3];

	// Inicializamos la regla vacía, que será utilizada por motorInferencia
	reglaVacia.antecedentes = NULL;

	// Inicializamos nReglas
	nReglas = 0;

	// Leer el fichero que contiene el Conocimiento
	leerConoc(nombreFicheroConoc);
    // Leer el fichero de configuración
    leerConf(nombreFicheroConf);
    // Leer el fichero que contiene los Hechos
    leerHechos(nombreFicheroHechos);

    if (motorInferencia())
    	cout << "¡Resuelto!" << endl;
    else
    	cout << "No se ha encontrado solución" << endl;

    salida1(nombreFicheroHechos); // Crea el fichero Salida1
    salida2(nombreFicheroHechos); // Crea el fichero Salida2

    // Liberamos toda la memoria dinámica que habíamos reservado
	delete[] reglas;
	//for (int i = 0; i < nReglas; i++)
		//delete[] reglas[i].antecedentes;
	delete[] BH;
	delete[] BHInicial;
	delete[] prioridades;

	return 0;
}


/********************************
***	FUNCIÓN LEER CONOCIMIENTO ***
*********************************/

void leerConoc(char* nombreFicheroConoc){
	string lActual;

	// Abrimos el archivo de conocimiento
	ifstream f(nombreFicheroConoc);

	// Obviamos la primera línea y la almacenamos. Contiene el dominio
	getline(f, lActual);
	dominio = lActual;

	// Leemos la línea que contiene el número de atributos
	getline(f, lActual);

	// Convertimos el texto en un entero
	nReglas = atoi(lActual.c_str());

	reglas = new regla[nReglas];

	// Inicializar array reglas (para la función motorInferencia)
	consecuencia cAux;
	for (int i = 0; i < nReglas; i++){
		reglas[i].antecedentes = new antecedente[nAntecedentesMax];
		for (int j = 0; j < nAntecedentesMax; j++){
			reglas[i].antecedentes[j].valor = "";
		}
		reglas[i].cons = cAux;
		reglas[i].marcada = false;
	}

	// Leemos todas las reglas del fichero
	for (int i = 0; i < nReglas; i++){
		regla r; // Regla auxiliar
		int n_antecedentes;	// Número de antecedentes de la regla

		// Leemos la regla
		getline(f, lActual);

		// Almacenamos la regla de la línea que acabamos de leer
		for (unsigned int j = 4; j < sizeof(lActual); j++){
			if ((lActual[j] == 'S') && (lActual[j+1] == 'i')){
				int inicioPalabra, finPalabra;
				bool entonces; // Cuando se vaya a leer el consecuente de la regla será 'true'

				// Creamos el array para almacenar los antecedentes de la regla
				r.antecedentes = new antecedente[nAntecedentesMax];
				n_antecedentes = 0;

				entonces = false;
				// Situamos el puntero al inicio del primer antecedente
				finPalabra = inicioPalabra = j+3;

				// Leemos y almacenamos reglas hasta encontrar un 'Entonces'
				while (!entonces){
					antecedente a;

					// Leemos el atributo de la condición
					while (lActual[finPalabra] != ' ')
						finPalabra++;
					a.atributo = lActual.substr(inicioPalabra, finPalabra-inicioPalabra);
					finPalabra++;
					inicioPalabra = finPalabra;
					// Leemos el operador de la condición
					while (lActual[finPalabra] != ' ')
						finPalabra++;
					a.operador = lActual.substr(inicioPalabra, finPalabra-inicioPalabra);
					finPalabra++;
					inicioPalabra = finPalabra;
					// Leemos el valor de la condición
					while (lActual[finPalabra] != ' ')
						finPalabra++;
					a.valor = lActual.substr(inicioPalabra, finPalabra-inicioPalabra);
					finPalabra++;
					inicioPalabra = finPalabra;
					// Comprobamos si a continuación tenemos otra condición,
					// o si por el contrario encontramos un entonces
					while (lActual[finPalabra] != ' ')
						finPalabra++;

					// Almacenamos la condición en el array antecedentes
					r.antecedentes[n_antecedentes] = a;
					n_antecedentes++; // Aumentamos el número de antecedentes encontrados
					r.n_antecedentes = n_antecedentes;

					if (lActual.substr(inicioPalabra, finPalabra-inicioPalabra) == "Entonces")
						entonces = true;
					finPalabra++;
					inicioPalabra = finPalabra;
				}

				consecuencia cons;

				// Leemos el atributo de la consecuencia
				while (lActual[finPalabra] != ' ')
					finPalabra++;
				cons.atributo = lActual.substr(inicioPalabra, finPalabra-inicioPalabra);
				finPalabra++;
				inicioPalabra = finPalabra;
				// Leemos el operador igual, pero no lo almacenamos
				while (lActual[finPalabra] != ' ')
					finPalabra++;
				finPalabra++;
				inicioPalabra = finPalabra;
				// Leemos el valor de la consecuencia
				finPalabra = strlen(lActual.c_str());
				if (lActual[finPalabra-1] == '\r')
					cons.valor = lActual.substr(inicioPalabra, finPalabra-inicioPalabra-1);
				else
					cons.valor = lActual.substr(inicioPalabra, finPalabra-inicioPalabra);
				inicioPalabra = finPalabra;

				r.cons = cons;

			}
		}

		reglas[i] = r;

	}

	// Cerramos el fichero
	f.close();
}


/*********************************
***	FUNCIÓN LEER CONFIGURACIÓN ***
**********************************/

void leerConf(char* nombreFicheroConf){
	string lActual, nombreAtributo;

	// Abrimos el archivo de configuración
	ifstream f(nombreFicheroConf);

	// Obviamos la primera línea, ya que no la queremos para nada
	getline(f, lActual);

	// Leemos la línea que contiene el número de atributos
	getline(f, lActual);

	// Convertimos el texto en un entero
	nAtributos = atoi(lActual.c_str());

	// Creamos los arrays de la BH y la BHInicial
	BH = new atributo[nAtributos];
	BHInicial = new atributo[nAtributos];

	/*
		Inicializamos los atributos de la BH, para poder luego
		consultar si un atributo tiene valor asignado o no.
		Usado en la función objetivoCumplido().
	*/
	for (int i = 0; i < nAtributos; i++){
		// Leer el atributo de la línea actual
		getline(f, lActual);

		nombreAtributo = getPrimeraPalabra(lActual);
		BH[i].nombre = nombreAtributo;
		BHInicial[i].nombre = nombreAtributo;
		BH[i].valor = "";
		BHInicial[i].valor = "";
	}

	// Obviamos la siguiente línea
	getline(f, lActual);

	// Leemos el objetivo
	getline(f, objetivo);

	// Obviamos la siguiente línea
	getline(f, lActual);

	// Creamos el array prioridades.
	prioridades = new int[nReglas];

	for (int i = 0; i < nReglas; i++){
		// Leer la prioridad de la regla actual
		getline(f, lActual);

		// Convertimos el texto en un entero
		prioridades[i] = atoi(lActual.c_str());
	}

	// Cerramos el fichero
	f.close();
}


/***************************************
***	FUNCIÓN AUXILIAR PARA leerConf() ***
****************************************/

string getPrimeraPalabra(string texto){
	int i;
	string palabra;

	i = 0;

	while (texto[i] != ' ')
		i++;

	palabra = texto.substr(0, i);
	return palabra;
}


/***********************************
***	FUNCIÓN PARA LEER LOS HECHOS ***
************************************/

void leerHechos(char* nombreFicheroHechos){
	string lActual, nombreAtributo, valor;
	// Número de atributos que coniene el archivo de la Base de Hechos
	int nA;

	// Abrimos el archivo
	ifstream f(nombreFicheroHechos);

	// Leemos la línea que contiene el número de atributos
	getline(f, lActual);

	// Convertimos el texto en un entero
	nA = atoi(lActual.c_str());

	for (int i = 0; i < nA; i++){
		// Posición en el array de la Base de Hechos
		// Posición en la línea que leemos en cada instante
		// Posición donde empieza la palabra que queremos almacenar
		int posArrayBH, posLinea, posLineaInicio;

		// Inicializamos las variables a la posición 0
		posArrayBH = posLinea = posLineaInicio = 0;

		// Leemos la línea
		getline(f, lActual);

		// Obtenemos la posición donde termina el nombre del atributo
		while (lActual[posLinea] != ' ')
			posLinea++;

		// Obtenemos el nombre
		nombreAtributo = lActual.substr(posLineaInicio, posLinea);

		// Buscamos en la Base de Hechos el atributo que tiene el nombre que acabamos
		// de encontrar
		while(strcmp(BH[posArrayBH].nombre.c_str(), nombreAtributo.c_str()) != 0)
			posArrayBH++;

		// Avanzamos el punto de inicio y fin donde empieza el valor del atributo
		posLinea += 3;
		posLineaInicio = posLinea;
		// El valor terminará donde termina la línea
		posLinea = strlen(lActual.c_str());

		// Obtenemos el valor y lo almacenamos en su atributo correspondiente
		BH[posArrayBH].valor = lActual.substr(posLineaInicio, posLinea-posLineaInicio);
		BHInicial[posArrayBH].valor = lActual.substr(posLineaInicio, posLinea-posLineaInicio);
		// Le ponemos un -1 para saber que el valor de ese atributo ha sido obtenido de la
		// Base de Hechos. Utilizado en salida2().
		BH[posArrayBH].regla = -1;
	}

	// Cerramos el fichero
	f.close();
}


/**************************
***	MOTOR DE INFERENCIA ***
***************************/

bool motorInferencia(){
	/*
		El Conjunto Conflicto solo almacenará una regla, ya que
		la función equiparar() se encargará de ver las reglas que
		pueden ser usadas, y filtrar para solo aplicar aquellas
		en las que sus antecedentes sean válidos, además de aplicar
		el valor de prioridad correspondiente para cada regla.
	*/
	regla conjuntoConflicto;

	conjuntoConflicto = extraerCualquierRegla();

	while (!(objetivoCumplido()) && !(vacioCC(conjuntoConflicto))){
        // ConjuntoConflicto solo contendrá una regla, escogida entre todo el
        // conjunto de reglas que pueden ser aplicadas
		conjuntoConflicto = equiparar();
		if (!(vacioCC(conjuntoConflicto))){
			aplicarRegla(conjuntoConflicto);
		}
	}

	if (objetivoCumplido())
		return true;
	else
		return false;
}


/***********************************************
***	FUNCIONES AUXILIARES MOTOR DE INFERENCIA ***
************************************************/

/*
	Extrae la primera regla de la Base de Conocimiento
*/
regla extraerCualquierRegla(){
	return reglas[0];
}

/*
	Devuelve 'true' si el 'conjuntoConflicto' está vacío
*/
bool vacioCC(regla conjuntoConflicto){
	if (conjuntoConflicto.antecedentes != NULL)
		return false;
	else
		return true;
}

/*
	Comprueba si el atributo objetivo de la Base de Hechos, tiene algún valor
*/
bool objetivoCumplido(){
	// Buscamos en la Base de Hechos en atributo objetivo
	for (int i = 0; i < nAtributos; i++){
		if (strcmp(BH[i].nombre.c_str(), objetivo.c_str()) == 0){
			// Comprobamos si el objetivo no está vacío
			if (strcmp(BH[i].valor.c_str(), "") != 0){
                conclusionGlobal = "";
                procesoSeguido(i);
                conclusionGlobal = conclusionGlobal + "El SBR ha concluido que '" + objetivo + "' es '" + BH[i].valor + "'";
                return true;
			}
			else
				return false;
		}
	}

	return false;
}

/*
	Función auxiliar de equiparar que comprueba los antecedentes
	de la regla, para saber si son válidos y la regla se puede ejecutar
*/
bool comprobarAntecedentes(int idRegla){
	// Recorremos los antecedentes de la regla
	for (int i = 0; i < reglas[idRegla].n_antecedentes; i++){
		/*
			Si el operador del antecedente actual es '=', entonces debemos
			comprobar si el atributo de nuestra BH que se corresponde con
			el del antecedente, tiene el valor que se pide.
		*/
		if (strcmp(reglas[idRegla].antecedentes[i].operador.c_str(), "=") == 0){
			// Buscamos el atributo en la Base de Hechos
			for (int j = 0; j < nAtributos; j++){
				if (strcmp(reglas[idRegla].antecedentes[i].atributo.c_str(), BH[j].nombre.c_str()) == 0){
					// Si el valor que se pide en la regla no coincide con el que tiene el atributo de la BH
					if (strcmp(reglas[idRegla].antecedentes[i].valor.c_str(), BH[j].valor.c_str()) != 0){
						return false;
					}
				}
			}
		} else if (strcmp(reglas[idRegla].antecedentes[i].operador.c_str(), ">") == 0){
			// Buscamos el atributo en la Base de Hechos
			for (int j = 0; j < nAtributos; j++){
				if (strcmp(reglas[idRegla].antecedentes[i].atributo.c_str(), BH[j].nombre.c_str()) == 0){
					/*
						Si el valor del atributo es mayor o igual que el de la regla, no se cumple el antecedente
						Además comprobamos que el valor del atributo no está vacío, para que atoi no nos devuelva un 0
					*/
					if ((strcmp(BH[j].valor.c_str(), "") == 0) || (atoi(BH[j].valor.c_str()) <= atoi(reglas[idRegla].antecedentes[i].valor.c_str())))
						return false;
				}
			}
		} else if (strcmp(reglas[idRegla].antecedentes[i].operador.c_str(), ">=") == 0){
			// Buscamos el atributo en la Base de Hechos
			for (int j = 0; j < nAtributos; j++){
				if (strcmp(reglas[idRegla].antecedentes[i].atributo.c_str(), BH[j].nombre.c_str()) == 0){
					/*
						Si el valor del atributo es mayor o igual que el de la regla, no se cumple el antecedente
						Además comprobamos que el valor del atributo no está vacío, para que atoi no nos devuelva un 0
					*/
					if ((strcmp(BH[j].valor.c_str(), "") == 0) || (atoi(BH[j].valor.c_str()) < atoi(reglas[idRegla].antecedentes[i].valor.c_str())))
						return false;
				}
			}
		} else if (strcmp(reglas[idRegla].antecedentes[i].operador.c_str(), "<") == 0){
			// Buscamos el atributo en la Base de Hechos
			for (int j = 0; j < nAtributos; j++){
				if (strcmp(reglas[idRegla].antecedentes[i].atributo.c_str(), BH[j].nombre.c_str()) == 0){
					/*
						Si el valor del atributo es mayor o igual que el de la regla, no se cumple el antecedente
						Además comprobamos que el valor del atributo no está vacío, para que atoi no nos devuelva un 0
					*/
					if ((strcmp(BH[j].valor.c_str(), "") == 0) || (atoi(BH[j].valor.c_str()) >= atoi(reglas[idRegla].antecedentes[i].valor.c_str())))
						return false;
				}
			}
		} else if (strcmp(reglas[idRegla].antecedentes[i].operador.c_str(), "<=") == 0){
			// Buscamos el atributo en la Base de Hechos
			for (int j = 0; j < nAtributos; j++){
				if (strcmp(reglas[idRegla].antecedentes[i].atributo.c_str(), BH[j].nombre.c_str()) == 0){
					/*
						Si el valor del atributo es mayor o igual que el de la regla, no se cumple el antecedente
						Además comprobamos que el valor del atributo no está vacío, para que atoi no nos devuelva un 0
					*/
					if ((strcmp(BH[j].valor.c_str(), "") == 0) || (atoi(BH[j].valor.c_str()) > atoi(reglas[idRegla].antecedentes[i].valor.c_str())))
						return false;
				}
			}
		}
	}
	return true;
}

/*
	Devolver una regla que tenga un conjunto de antecedentes verificadas,
	y no haya sido ejecutada (marcada) anteriormente. Si no existe esa regla,
	se devolverá una regla vacía. Si hay más de una regla que cumple las
	condiciones, se elegirá aquella que tenga mayor preferencia.
*/
regla equiparar(){
	// Enteros que serán utilizado en ambos for
	// Se declaran aquí para poder usarlos fuera de los for
	int i, j;
	int mejorRegla, nReglasValidas, nReglasMarcadas, nReglasNoValidas, nReglasNoVerificadas;
	string reglasValidas, reglasMarcadas, reglasNoValidas, reglasNoVerificadas;
	bool prioridad;

	mejorRegla = -1;
	nReglasValidas = nReglasMarcadas = nReglasNoValidas = nReglasNoVerificadas = 0;
	prioridad = false;

	for (i = 0; i < nReglas; i++){
		// Si la regla no está marcada
		if (!(reglas[i].marcada)){
			// Comprobar si sus antedecentes están verificados
			for (j = 0; j < reglas[i].n_antecedentes; j++){
				// Si un antecedente de la regla no es válido, no nos interesa esta regla
				if (strcmp(reglas[i].antecedentes[j].valor.c_str(), "") == 0){
                    nReglasNoValidas++;
                    /*
                        Almacenamos el número de la regla en un char
                        para poder imprimirlo al fichero 'Salida1'.
                    */
                    char aux[10];
                    sprintf(aux, "%d", i+1);
                    // Si es la primera regla válida
                    if (nReglasNoValidas == 1)
                        reglasNoValidas = aux;
                    else
                        reglasNoValidas = reglasNoValidas + ", " + aux;
					j = reglas[i].n_antecedentes+1;
				}
			}
			// Si los antecedentes son válidos y se verifican, la regla puede ser usada
			if ((j == reglas[i].n_antecedentes) && (comprobarAntecedentes(i))){
                nReglasValidas++;
				/*
                    Almacenamos el número de la regla en un char
                    para poder imprimirlo al fichero 'Salida1'.
                */
                char aux[10];
                sprintf(aux, "%d", i+1);
				if (mejorRegla == -1)
                    reglasValidas = aux;
                else
                    reglasValidas = reglasValidas + ", " + aux;

				/*
					Si la regla actual tiene mayor prioridad que la mejor que
					teníamos anteriormente (y sus antecedentes son válidos) la escogemos.
					Si su prioridad es igual no debemos cogerla, ya que tendrá un
					identificador mayor.
				*/
				if (mejorRegla == -1)
                    mejorRegla = i;
                else if (prioridades[mejorRegla] < prioridades[i]){
                    prioridad = true;
                    mejorRegla = i;
                }
			} else if (j == reglas[i].n_antecedentes){
                nReglasNoVerificadas++;
                /*
                    Almacenamos el número de la regla en un char
                    para poder imprimirlo al fichero 'Salida1'.
                */
                char aux[10];
                sprintf(aux, "%d", i+1);
                if (nReglasNoVerificadas == 1)
                    reglasNoVerificadas = aux;
                else
                    reglasNoVerificadas = reglasNoVerificadas + ", " + aux;
			}
		} else {
            nReglasMarcadas++;
            /*
                Almacenamos el número de la regla en un char
                para poder imprimirlo al fichero 'Salida1'.
            */
            char aux[10];
            sprintf(aux, "%d", i+1);
            if (nReglasMarcadas == 1)
                reglasMarcadas = aux;
            else
                reglasMarcadas = reglasMarcadas + ", " + aux;
		}
	}

	resolucion = resolucion + "\n";
    // Imprimimos las reglas descartadas y por que se han descartado
    // Reglas Marcadas
	if (nReglasMarcadas == 1)
        resolucion = resolucion + "Se descarta la regla " + reglasMarcadas + " por estar marcada.\n";
	else if (nReglasMarcadas > 1)
        resolucion = resolucion + "Se descartan las reglas " + reglasMarcadas + " por estar marcadas.\n";
    // Reglas no válidas
    if (nReglasNoValidas == 1)
        resolucion = resolucion + "Se descarta la regla " + reglasNoValidas + " por no tener antecedentes válidos.\n";
	else if (nReglasNoValidas > 1)
        resolucion = resolucion + "Se descartan las reglas " + reglasNoValidas + " por no tener antecedentes válidos.\n";
    // Reglas no verificadas
    if (nReglasNoVerificadas == 1)
        resolucion = resolucion + "Se descarta la regla " + reglasNoVerificadas + " por no poder verificar sus antecedentes.\n";
	else if (nReglasNoVerificadas > 1)
        resolucion = resolucion + "Se descartan las reglas " + reglasNoVerificadas + " por no poder verificar sus antecedentes.\n";

	if (mejorRegla != -1){

        if (nReglasValidas == 1)
            resolucion = resolucion + "El ConjuntoConflicto contiene la regla: " + reglasValidas + ".\n";
        else
            resolucion = resolucion + "El ConjuntoConflicto contiene las reglas: " + reglasValidas + ".\n";

		reglas[mejorRegla].marcada = true;

		/*
			Almacenamos el número de la regla escogida en un char
			para poder imprimirlo al fichero 'Salida1'.
		*/
		char aux[10];
		sprintf(aux, "%d", mejorRegla+1);
		/*
			Añadimos el número de la regla escogida al string 'resolucion',
			que será usado para escribir en el fichero 'Salida1'
		*/

		if (prioridad)
            resolucion = resolucion + "Se aplica la regla " + aux + " porque tiene mayor prioridad que el resto.\n";
        else if (nReglasValidas == 1)
            resolucion = resolucion + "Se aplica la regla " + aux + " porque es la única regla que se puede disparar.\n";
        else
            resolucion = resolucion + "Se aplica la regla " + aux + " porque su identificador es menor que el del resto.\n";

        reglas[mejorRegla].nregla = mejorRegla;
		return reglas[mejorRegla];
	} else {
	    resolucion = resolucion + "El ConjuntoConflicto está vacío. No se puede aplicar ninguna regla.\n";
	    return reglaVacia;
	}
}

/*
	Se aplica la regla que se encuentre en el conjuntoConflicto
*/
void aplicarRegla(regla conjuntoConflicto){

	// Explicamos qué valor se le ha aplicado a un determinado atributo al aplicar la regla escogida
	resolucion = resolucion + "Se ha asignado el valor '" + conjuntoConflicto.cons.valor + "' al atributo '" + conjuntoConflicto.cons.atributo + "'.\n";

	for (int i = 0; i < nAtributos; i++){
		if (strcmp(conjuntoConflicto.cons.atributo.c_str(), BH[i].nombre.c_str()) == 0){
			// Actualizamos el atributo que modifica la regla actual
			BH[i].valor = conjuntoConflicto.cons.valor;
			BH[i].regla = conjuntoConflicto.nregla;
			i = nAtributos;
		}
	}
}

void procesoSeguido(int atributo){
    bool primero;
    int anterior;
    string conclusion;

    primero = true;
    anterior = -1;

    // Si el atributo ha sido deducido
    if (BH[atributo].regla != -1){
        // Por cada antecedente/atributo de la regla
        for (int i = 0; i < nAntecedentesMax; i++){
            // Necesitamos saber a que posicion en BH corresponde ese atributo
            for (int j = 0; j < nAtributos; j++){
                // Si el atributo corresponde con la posición 'j' de la BH
                if((j != anterior) && (strcmp(reglas[BH[atributo].regla].antecedentes[i].atributo.c_str(), BH[j].nombre.c_str()) == 0)){
                    // Otenemos el número que tiene cada atributo en la Base de Hechos (j)
                    // Lo usamos para llamar recursivamente a procesoSeguido y explicar como se ha obtenido dicho atributo
                    procesoSeguido(j);
                    if (primero){
                        if (BH[j].regla == -1)
                            conclusion = conclusion + "Como el atributo '" + BH[j].nombre + "' tiene el valor '" + BH[j].valor + "' introducido por la Base de Hechos";
                        else
                            conclusion = conclusion + "Como el atributo '" + BH[j].nombre + "' tiene el valor '" + BH[j].valor + "'";
                        primero = false;
                    } else
                        if (BH[j].regla == -1)
                            conclusion = conclusion + ", y el atributo '" + BH[j].nombre + "' tiene el valor '" + BH[j].valor + "' introducido por la Base de Hechos";
                        else
                            conclusion = conclusion + ", y el atributo '" + BH[j].nombre + "' tiene el valor '" + BH[j].valor + "'";
                    anterior = j;
                }
            }
        }
        conclusion = conclusion + ", el SBR concluye que '" + BH[atributo].nombre + "' tiene el valor '" + BH[atributo].valor + "'\n";
    }
    // Almacenamos todas las conclusiones en el string global
    conclusionGlobal = conclusionGlobal + conclusion;
}


/**************************************
***	IMPRIMIR LA SALIDA A UN FICHERO ***
***************************************/

void salida1(char* nombreFicheroHechos){
	fstream archivo;  // objeto de la clase ofstream
	string nombreArchivo, nombreFicheroH;

	// Convertirmos el char* que contiene el nombre del fichero en un string
	nombreFicheroH = nombreFicheroHechos;

	/*
		Definimos que nombre tendrá el archivo de salida, en función del nombre
		del archivo que contenía la Base de Hechos.
	*/
	nombreArchivo = "Salida1-" + nombreFicheroH.substr(3, strlen(nombreFicheroHechos)-3);

	// Abrimos el archivo con permisos para sobreescribirlo
    archivo.open(nombreArchivo.c_str(), ios::out);

	archivo << "Dominio: " << dominio << endl;
	archivo << "Objetivo: " << objetivo << endl;
	archivo << "Base de Hechos inicial:" << endl;
	for (int i = 0; i < nAtributos; i++){
		archivo << "\t" << BHInicial[i].nombre << " = " << BHInicial[i].valor << endl;
	}
	archivo << endl << "RAZONAMIENTO SEGUIDO POR EL SBR" << endl << resolucion;

	archivo.close();
}

void salida2(char* nombreFicheroHechos){
	fstream archivo;  // objeto de la clase ofstream
	string nombreArchivo, nombreFicheroH;

	// Convertirmos el char* que contiene el nombre del fichero en un string
	nombreFicheroH = nombreFicheroHechos;

	/*
		Definimos que nombre tendrá el archivo de salida, en función del nombre
		del archivo que contenía la Base de Hechos.
	*/
	nombreArchivo = "Salida2-" + nombreFicheroH.substr(3, strlen(nombreFicheroHechos)-3);

	// Abrimos el archivo con permisos para sobreescribirlo
    archivo.open(nombreArchivo.c_str(), ios::out);

	archivo << "Dominio: " << dominio << endl;
	for (int i = 0; i < nAtributos; i++){
		if (strcmp(objetivo.c_str(), BH[i].nombre.c_str()) == 0){
			// Actualizamos el atributo que modifica la regla actual
			archivo << "Objetivo: " << objetivo << " = " << BH[i].valor << endl;
			i = nAtributos;
		}
	}
	archivo << "Base de Hechos final:" << endl;
	for (int i = 0; i < nAtributos; i++){
		archivo << "\t" << BH[i].nombre << " = " << BH[i].valor << endl;
	}

	archivo << endl << "CAMINO HACIA EL OBJETIVO" << endl << conclusionGlobal << endl;

	archivo.close();
}
