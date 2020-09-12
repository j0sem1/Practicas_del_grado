#include <iostream>
#include <fstream>
using namespace std;

const int size = 1000;		// Tamano maximo de las cadenas
const int ntipos = 2;		// Número de distintos tipos de cadenas que formaremos
const int todos = 123 - 97;	// Todos los caracteres del alfabeto
const int pocos = 5;		// Cinco primeros caracteres del alfabeto
const int medio = 12;		// Doce primeros caracteres del alfabeto

/*
En este procedimiento se encuentra el algoritmo que encuentra la solución dada una cadena_a y una cadena_b.
*/
void directo(string cadena_a, string cadena_b, int* resultado){
	// La variable n almacenará el valor del tamaño de la cadena_a
	// La variable m almacenará el valor del tamaño de la cadena_b
	int n = cadena_a.length();
	int m = cadena_b.length();
	// Las variables i, j y k serán utilizadas para índices
	int j = 0; int i = 0; int k = 0;
	// posición almacenará la posición de inicio en la cadena_a de la cadena que sea el resultado, y primera eso mismo pero no con la cadena del resultado, sino con la
	// que se esté analizando en ese momento
	int posicion = 0; int primera = 0;
	// repeticiones almacenará el número de repeticiones de la cadena solución, y actual el número de repeticiones de la cadena que se esté analizando en ese momento
	int repeticiones = 0; int actual = 0;
	// ultima_aparicion guarda la posición del último carácter que forme parte de la última solución, y ultima_aparicion_tam las repeticiones de la última solución de cadena_a
	int ultima_aparicion = 0; int ultima_aparicion_tam = 0;
	// primera_aparicion guarda la posición del primer carácter que forme parte de la primera solución, y primera_aparicion_tam las repeticiones de la primera solución de cadena_a
	int primera_aparicion = 0; int primera_aparicion_tam = 0;
	// Ejecutamos una serie de instrucciones desde el primer carácter de la cadena_a hasta el último, pasando varias veces por algunos
	while(k < n){											// Mientras no lleguemos al último carácter de cadena_a (con el índice k)
		if ((j == m-1) && (cadena_a[i] == cadena_b[j])){	// Si hemos llegado al último carácter de cadena_b, y el caracter actual de cadena_a es igual al último de cadena_b
			if (primera == 0){	// Si es la primera vez que encontramos una solución en este ciclo almacenamos la posición
				primera = i+1;
			}
			actual++;		// Aumentamos el numero de veces que hemos encontrado esta cadena
			j = 0;			// Volvemos a inicializar j a 0 para buscar una nueva cadena como b despues de la ya encontrada
			i++;			// Incrementamos i para ir avanzando por cadena_a (en el ciclo actual, no en general)											
			if (posicion == 0){		// Si aún no se ha encontrado ninguna solución
				primera_aparicion = primera;	// Primera aparición será la posición de inicio de esta cadena
			}
			if (primera == primera_aparicion){	// Si la posición de inicio de esta cadena es igual a la primera_aparición de cadena_a
				primera_aparicion_tam++;		// Incrementamos el tamaño que tiene la primera_aparición
			}
			if (primera + (m*(actual-1)) > ultima_aparicion){			// Si la posicion de inicio de la ultima cadena actual es mayor que la ultima aparicion
				if (primera + (m*(actual-1)) - 1 == ultima_aparicion){	// Si la posicion anterior a la posición de inicio de la ultima cadena actual es igual a la última aparición
					ultima_aparicion_tam++;								// Aumentamos el tamaño de las apariciones de estas ultimas cadenas encontradas en cadena_a
				} else {												// Si la posicion anterior no coincide
					ultima_aparicion_tam = 1;							// El tamaño de estas ultimas apariciones será 1
				}
			}
			if (primera + (m*actual) > ultima_aparicion){	// Si la posición del último caracter de la cadena actual se encuentra después de la última aparición
				ultima_aparicion = primera + (m*actual)-1;	// La última aparición será la posición del último caracter de la cadena actual
			}
			if (actual > repeticiones){		//Si el numero de veces que hemos encontrado esta cadena supera al maximo
				repeticiones = actual;		//Sustituimos repeticiones por este maximo
				posicion = primera;			//Colocamos la posicion en el inicio de esta cadena
			}
		} else {	// Si no hemos llegado al último carácter de cadena_b, o si el caracter actual de cadena_a no es igual al último de cadena_b
			if (cadena_a[i] == cadena_b[j]){	// Si el caracter actual de la cadena_a coincide con el caracter actual de la cadena_b
				if (j == 0 && actual == 0){		// Si es el primer caracter de cadena_b y acabamos de empezar una serie
					primera = i+1;				// Primera se actualiza con la posición donde se encuentra el caracter que ha hecho coincidencia
				}
				j++;							// Incrementamos j para comparar en el siguiente ciclo el siguiente caracter de cadena_b
				i++;							// Incrementamos i para comparar en el siguiente ciclo el siguiente caracter de cadena_a
			} else {							// Si el caracter actual de la cadena_a no coincide con el caracter actual de la cadena_b
				k++;							// Incrementamos k para salir del ciclo actual y continuar con el bucle while y analizar otros ciclos											
				i =	k;							// Actualizamos la posición de la cadena_a con el valor de k
				j = 0;							// Volvemos al principio de la cadena_b
				actual = 0;						// Reseteamos el nº de repeticiones actuales
				primera = 0;					// Reseteamos la posición de la cadena actual
			}
		}
	}

	// A continuación actualizamos los valores de la variable resultado que se ha pasado como parámetro, que contendrá el resultado encontrado por el 
	// algoritmo directo entre cadena_a y cadena_b
	resultado[0] = posicion;
	resultado[1] = repeticiones;
	resultado[2] = primera_aparicion;
	resultado[3] = primera_aparicion_tam;
	resultado[4] = ultima_aparicion;
	resultado[5] = ultima_aparicion_tam;

	
	//cout << "Comienzo de la cadena: " << resultado[0] << endl;
	//cout << "Numero de veces que aparece: " << resultado[1] << endl;
	//cout << "Primera aparicion de una cadena valida " << resultado[2] << endl;
	//cout << "tamanio de Primera aparicion de una cadena valida " << resultado[3] << endl;
	//cout << "Ultima aparicion de una cadena valida " << resultado[4] << endl;
	//cout << "tamanio de Ultima aparicion de una cadena valida " << resultado[5] << endl;
	
}

/*
Función que obtiene un caracter aleatorio a partir de un tamaño máximo de caracteres y de un número de caracteres que
se quiere utilizar como máximo del alfabeto del problema. Estos valores se pasan como parámetros
*/
string aleatorio(int size, int nCaracteres){
	string cadena;
	// Realizamos un bucle con tantas ejecuciones como el tamaño de la cadena deseada
	for(int i = 0; i < size; i++){
		// El carácter que se obtenga en cada paso será un carácter aleatorio dentro del alfabeto del problema
		char nuevo_caracter = 97 + rand() % nCaracteres;
		// Se almacena en la cadena resultado junto a los encontrados anteriormente
		cadena = cadena + nuevo_caracter;
	}
	// Finalmente se devuelve la cadena aleatoria
	return cadena;
}

/*
Función que obtiene una subcadena de una cadena pasada como parámetro a la función
*/
string subcadena(string cadena_a){
	string cadena_b;
	int tam_cadena_a = cadena_a.length();
	// Tamaño de la subcadena
	int tamanoTotal = rand() % tam_cadena_a;
	// Posición dentro de cadena_a desde donde se comenzará a obtener la subcadena
	int posicionInicio = rand() % tam_cadena_a;
	int posicionActual = posicionInicio;
	// La variable tamanoActual almacenará en cada momento el tamaño de la subcadena
	int tamanoActual = 0;
	// Mientras que la posición actual de la cadena_a no sobrepasa el tamaño de esta y el tamaño de la subcadena no sobrepasa su máximo
	// se va construyendo la subcadena
	while((posicionActual < tam_cadena_a) && (tamanoActual <= tamanoTotal)){
		cadena_b = cadena_b + cadena_a[posicionActual];
		posicionActual++;
		tamanoActual++;
	}
	// Finalmente se devuelve la subcadena
	return cadena_b;
}

/*
Método main del programa que se encagará de poner éste en ejecución
*/
int main(void){
	// Se inicializa srand para poder ser usado
	srand(time(NULL));
	string cadena_a;
	int* resultado = new int[6];
	for (int i = 0; i < 6; i++){
		resultado[i] = 0;
	}
	// Declaramos dos variables de tipo ofstream, una para el archivo .in y otra para el .out
	ofstream ficheroIn;
	ofstream ficheroOut;
	// Se abren ambos ficheros (o se crean)
	ficheroIn.open("generacion.in");
	ficheroOut.open("generacion.out");
	// Ejecutamos un bucle tantas veces como tipos de cadena_a y cadena_b queramos crear
	for (int j = 1; j <= ntipos; j++){
		// Ejecutamos un bucle, con una ejecución por cada tamaño de cadena_a, desde 1 hasta size
		for (int n = 1; n <= 10; n++){
			string cadena_b;
			// Se obtiene la cadena_a de manera aleatoria
			cadena_a = aleatorio(size, todos);
			// Dependiendo de en que ejecución del primer bucle for estemos calculamos cadena_b de manera aleatoria o como subcadena de cadena_a
			if (j == 1){		// cadena_b aleatoria
				// El tamaño de cada cadena_b siempre será aleatorio dentro del tamaño de cadena_a
				cadena_b = aleatorio(rand() % size + 1, todos);
			} else if (j == 2){	// cadena_b subcadena
				cadena_b = subcadena(cadena_a);
			}
			// Aplicamos el algoritmo directo sobre las cadenas obtenidas para obtener un resultado preciso
			directo(cadena_a, cadena_b, resultado);
			// Almacenamos ambas cadenas en el fichero .in
			ficheroIn << cadena_a << endl;
			ficheroIn << cadena_b << endl;
			// Almacenamos los resultados en el fichero .out
			ficheroOut << resultado[0] << " " << resultado[1] << endl;
		}
	}
	delete[] resultado;
	resultado = NULL;
	// Cerramos los ficheros .in y .out
	ficheroIn.close();
	ficheroOut.close();
	return 0;
}