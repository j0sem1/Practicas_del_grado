#include <iostream>
#include <string.h>
using namespace std;

/*
Esta función se encarga de comprobar si las cadenas a y b forman entre ellas un caso especial que no puede ser resuelto en el combinar.
Este caso especial es explicado en la memoria del proyecto. Esta función devolverá un entero, que será 0 si no se trata de un caso especial,
y si lo es devolverá el número del caso correspondiente.
*/
int comprobar(int* res_1, int* res_2, string parte_1, string parte_2, string cadena_b, string cadena_a){
	// Primero calculamos y almacenamos el tamaño de cadena_b y de la parte_1, ya que los necesitaremos varias veces.
	int tam_cadena_b = cadena_b.length();
	int tam_parte_1 = parte_1.length();
	// Si el tamaño de cadena_b es 1 ya sabemos que no se trata de un caso especial, por lo tanto devolvemos 0
	if (tam_cadena_b == 1){
		return 0;
	}
	// Dividiremos el cauce de la función en dos tipos de casos: si cadena_b es par o impar
	if (tam_cadena_b % 2 == 0 ){	// Si la cadena_b es par
		// En primer lugar obtenemos el caracter elegido para estudiar el caso especial. Este caracter será la primera mitad de los caracteres de cadena_b
		string caracter = "";
		int i = 0;
		while (i < tam_cadena_b/2){
			caracter = caracter + cadena_b[i];
			i++;
		}
		// Ahora comprobamos que toda la cadena_b está formada por ese caracter
		int j = 0;
		while (i < tam_cadena_b){
			if (caracter[j] != cadena_b[i]) return 0;
			i++;
			j++;
		}
		// Almacenamos el tamaño del caracter, ya que lo vamos a usar a continuación
		int tam_caracter = caracter.length();
		// Si ni la parte izq. ni la parte dcha. tienen solución no se trata de un caso especial, por lo tanto devolveríamos 0.
		if ((res_1[1] == 0) && (res_2[1] == 0)){
			return 0;
		} else if ((res_1[1] != 0) && (res_2[1] != 0) && (res_1[4] == res_2[2]-1)){		// Si la parte izq. y dcha. tienen solución, y la última solución de la parte izq. engancha con la primera de la parte dcha.
			// Calculamos las posiciones de inicio y fin donde comenzaremos y terminaremos de comprobar si la cadena entre dichos limites coincide con el caracter
			int inicio = res_1[4] - tam_cadena_b * res_1[5] + 1 - tam_caracter - 1; //Hacemos -1 por el array
			int fin = res_1[4] - tam_cadena_b*res_1[5] - 1; //Hacemos -1 por el array
			i = 0;
			// Ahora comprobamos si las cadenas corresponden
			while ((inicio <= fin) && (cadena_a[inicio] == caracter[i])){
				inicio++;
				if (i == tam_caracter-1){
					i = 0;
				} else {
					i++;
				}
			}
			if (inicio > fin){	// Las cadenas son iguales, por lo tanto tenemos caso especial y devolvemos 3
				return 3;
			} else {			// Las cadenas no son iguales, no hay caso especial
				return 0;
			}
		} else if ((res_1[1] != 0) && (res_1[4] == tam_parte_1)){
			// Calculamos las posiciones de inicio y fin donde comenzaremos y terminaremos de comprobar si la cadena entre dichos limites coincide con el caracter
			int inicio = res_1[4] - tam_cadena_b*res_1[5] + 1 - tam_caracter - 1; //Hacemos -1 por el array
			int fin = res_1[4] - tam_cadena_b*res_1[5] - 1; //Hacemos -1 por el array
			i = 0;
			// Ahora comprobamos si las cadenas corresponden
			while ((inicio <= fin) && (cadena_a[inicio] == caracter[i])){
				inicio++;
				if (i == tam_caracter-1){
					i = 0;
				} else {
					i++;
				}
			}
			if (inicio > fin){	// Las cadenas son iguales, por lo tanto tenemos caso especial y devolvemos 3
				return 2;
			} else {			// Las cadenas no son iguales, no hay caso especial
				return 0;
			}
			
		} else if ((res_2[1] != 0) && (res_2[2] == tam_parte_1 + 1)) {
			// Calculamos las posiciones de inicio y fin donde comenzaremos y terminaremos de comprobar si la cadena entre dichos limites coincide con el caracter
			int inicio = res_2[2] - tam_caracter - 1; //Hacemos -1 por el array
			int fin = res_2[2] - 1 - 1; //Hacemos -1 por el array
			i = 0;
			// Ahora comprobamos si las cadenas corresponden
			while ((inicio <= fin) && (cadena_a[inicio] == caracter[i])){
				inicio++;
				if (i == tam_caracter-1){
					i = 0;
				} else {
					i++;
				}
			}
			if (inicio > fin){	// Las cadenas son iguales, por lo tanto tenemos caso especial y devolvemos 3
				return 1;
			} else {			// Las cadenas no son iguales, no hay caso especial
				return 0;
			}
		}
		return 0;	// Si no se han cumplido las condiciones de ningún if podemos asegurar que no es un caso especial
	} else {	// Si la cadena_b es impar
		// En este caso declaramos dos variables caracter, ya que en cadenas impares no es suficiente con un solo caracter.
		string caracter_1 = "";
		string caracter_2 = "";
		int i = 0;
		// Obtenemos caracter_1, que corresponder a un tercio de los caracteres de cadena_b
		while (i < tam_cadena_b/3){
			caracter_1 = caracter_1 + cadena_b[i];
			i++;
		}
		// caracter_2 contendrá el resto de caracteres de cadena_b
		while (i < tam_cadena_b){
			caracter_2 = caracter_2 + cadena_b[i];
			i++;
		}
		// Calculamos y almacenamos previamente el tamaño de cada caracter, ya que los utilizaremos mas adelante
		int tam_caracter_1 = caracter_1.length();
		int tam_caracter_2 = caracter_2.length();
		// Creamos un string auxiliar para comprobar si la cadena_b cumple la condición de caso especial
		string aux = caracter_1 + caracter_1;
		// Si la variable auxiliar no coincide con el caracter_2, entonces no es un caso especial
		if (aux != caracter_2){
			return 0;
		}
		// Si la parte izq. y la parte dcha. no tienen solución no se trata de un caso especial
		if ((res_1[1] == 0) && (res_2[1] == 0)){
			return 0;
		} else if ((res_1[1] != 0) && (res_2[1] != 0) && (res_1[4] == res_2[2]-1)){		// Si ambas partes tienen solución, y estas enganchan entre si, entramos a analizar por si se trata de un caso especial
			// En este for comprobaremos en una primera ejecución si se cumple el caso especial con el caracter_1 a la izq. Si no así se comprobará si está en la dcha. en una segunda ejecución.
			// Si esto tampoco funciona finalmente devolveremos cero.
			// Declaramos una variable auxiliar aux_4 que devolverá el caso correspondiente en cada ejecución del for
			int aux_4 = 4;
			for(int x = 1; x <= 2; x++){
				// Calculamos las posiciones de inicio y fin donde comenzaremos y terminaremos de comprobar si la cadena entre dichos limites coincide con el caracter
				int inicio = res_1[4] - tam_cadena_b*res_1[5] + 1 - tam_caracter_1 - 1; //Hacemos -1 por el array
				int fin = inicio+tam_caracter_1; //Hacemos -1 por el array
				i = 0;
				// Ahora comprobamos si las cadenas corresponden
				while ((inicio < fin) && (cadena_a[inicio] == caracter_1[i])){
					inicio++;
					if (i == tam_caracter_1-1){
						i = 0;
					} else {
						i++;
					}
				}
				// Si las cadenas corresponden tenemos que comprobar lo mismo pero en la parte dcha. y con caracter_2 (El caracter se invertirá en la segunda ejecución)
				if (inicio == fin){
					// Calculamos las posiciones de inicio y fin donde comenzaremos y terminaremos de comprobar si la cadena entre dichos limites coincide con el caracter
					inicio = res_2[2] + tam_cadena_b*res_2[3] - 1; //Hacemos -1 por el array
					fin = inicio + tam_caracter_2;
					i = 0;
					// Ahora comprobamos si las cadenas corresponden
					while ((inicio < fin) && (cadena_a[inicio] == caracter_2[i])){
						inicio++;
						if (i == tam_caracter_2-1){
							i = 0;
						} else {
							i++;
						}
					}
					if (inicio == fin){
						return aux_4;
					}
				}
				// Si hemos llegado hasta aquí no se ha podido comprobar que sea caso especial. Ahora lo intentaremos comprobando si caracter_1 está en la dcha. y caracter_2 en la izq.
				// Para ello, invertimos caracter_1 y caracter_2. También tendremos que invertir sus tamaños.
				string aux_2 = caracter_1;
				caracter_1 = caracter_2;
				caracter_2 = aux_2;
				int aux_3 = tam_caracter_1;
				tam_caracter_1 = tam_caracter_2;
				tam_caracter_2 = aux_3;
				aux_4 = 7;
			}
			// Si finalmente hemos llegado a este punto no se trata de un caso especial, por lo tanto devolvemos cero.
			return 0;
		} else if ((res_1[1] != 0) && (res_1[4] == tam_parte_1)){
			// En este for comprobaremos en una primera ejecución si se cumple el caso especial con el caracter_1 a la izq. Si no así se comprobará si está en la dcha. en una segunda ejecución.
			// Si esto tampoco funciona finalmente devolveremos cero.
			// Declaramos una variable auxiliar aux_4 que devolverá el caso correspondiente en cada ejecución del for
			int aux_5 = 5;
			for(int x = 1; x <= 2; x++){
				// Calculamos las posiciones de inicio y fin donde comenzaremos y terminaremos de comprobar si la cadena entre dichos limites coincide con el caracter
				int inicio = res_1[4] - tam_cadena_b*res_1[5] + 1 - tam_caracter_1 - 1; //Hacemos -1 por el array
				int fin = inicio+tam_caracter_1;
				i = 0;
				// Ahora comprobamos si las cadenas corresponden
				while ((inicio < fin) && (cadena_a[inicio] == caracter_1[i])){
					inicio++;
					if (i == tam_caracter_1-1){
						i = 0;
					} else {
						i++;
					}
				}
				// Si las cadenas corresponden tenemos que comprobar lo mismo pero en la parte dcha. y con caracter_2 (El caracter se invertirá en la segunda ejecución)
				if (inicio == fin){
					// Calculamos las posiciones de inicio y fin donde comenzaremos y terminaremos de comprobar si la cadena entre dichos limites coincide con el caracter
					inicio = res_1[4] + 1 - 1; //Hacemos -1 por el array
					fin = inicio + tam_caracter_2;
					i = 0;
					// Ahora comprobamos si las cadenas corresponden
					while ((inicio < fin) && (cadena_a[inicio] == caracter_2[i])){
						inicio++;
						if (i == tam_caracter_2-1){
							i = 0;
						} else {
							i++;
						}
					}
					if (inicio == fin){
						return aux_5;
					}
				}
				// Si hemos llegado hasta aquí no se ha podido comprobar que sea caso especial. Ahora lo intentaremos comprobando si caracter_1 está en la dcha. y caracter_2 en la izq.
				// Para ello, invertimos caracter_1 y caracter_2. También tendremos que invertir sus tamaños.
				string aux_2 = caracter_1;
				caracter_1 = caracter_2;
				caracter_2 = aux_2;
				int aux_3 = tam_caracter_1;
				tam_caracter_1 = tam_caracter_2;
				tam_caracter_2 = aux_3;
				aux_5 = 8;
			}
			// Si finalmente hemos llegado a este punto no se trata de un caso especial, por lo tanto devolvemos cero.
			return 0;	
		} else if ((res_2[1] != 0) && (res_2[2] == tam_parte_1 + 1)) {
			// En este for comprobaremos en una primera ejecución si se cumple el caso especial con el caracter_1 a la izq. Si no así se comprobará si está en la dcha. en una segunda ejecución.
			// Si esto tampoco funciona finalmente devolveremos cero.
			// Declaramos una variable auxiliar aux_4 que devolverá el caso correspondiente en cada ejecución del for
			int aux_6 = 6;
			for(int x = 1; x <= 2; x++){
				// Calculamos las posiciones de inicio y fin donde comenzaremos y terminaremos de comprobar si la cadena entre dichos limites coincide con el caracter
				int inicio = res_2[2] - tam_caracter_1 - 1; //Hacemos -1 por el array
				int fin = inicio + tam_caracter_1;
				i = 0;
				// Ahora comprobamos si las cadenas corresponden
				while ((inicio < fin) && (cadena_a[inicio] == caracter_1[i])){
					inicio++;
					if (i == tam_caracter_1-1){
						i = 0;
					} else {
						i++;
					}
				}
				// Si las cadenas corresponden tenemos que comprobar lo mismo pero en la parte dcha. y con caracter_2 (El caracter se invertirá en la segunda ejecución)
				if (inicio == fin){
					// Calculamos las posiciones de inicio y fin donde comenzaremos y terminaremos de comprobar si la cadena entre dichos limites coincide con el caracter
					inicio = res_2[2] + tam_cadena_b*res_2[3]  - 1; //Hacemos -1 por el array
					fin = inicio + tam_caracter_2;
					i = 0;
					// Ahora comprobamos si las cadenas corresponden
					while ((inicio < fin) && (cadena_a[inicio] == caracter_2[i])){
						inicio++;
						if (i == tam_caracter_2-1){
							i = 0;
						} else {
							i++;
						}
					}
					if (inicio == fin){
						return aux_6;;
					}
				}
				// Si hemos llegado hasta aquí no se ha podido comprobar que sea caso especial. Ahora lo intentaremos comprobando si caracter_1 está en la dcha. y caracter_2 en la izq.
				// Para ello, invertimos caracter_1 y caracter_2. También tendremos que invertir sus tamaños.
				string aux_2 = caracter_1;
				caracter_1 = caracter_2;
				caracter_2 = aux_2;
				int aux_3 = tam_caracter_1;
				tam_caracter_1 = tam_caracter_2;
				tam_caracter_2 = aux_3;
				aux_6 = 9;	
			}
			// Si finalmente hemos llegado a este punto no se trata de un caso especial, por lo tanto devolvemos cero.
			return 0;
		}
		return 0;
	}
}

/*
En este procedimiento se encuentra el algoritmo que encuentra la solución dada una cadena_a y una cadena_b. Además encuentra otros valores que serán necesitados en otras/os
funciones/parámetros de nuestro divide_y_venceras
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
Esta función devolverá un booleando indicando, a partir de la cadena_a y la cadena_b, si la cadena_a es suficientemente pequeña como para analizarla con
el algoritmo directo en lugar de con divide y vencerás, o si por el contrario tenemos que dividirla en dos
*/
bool pequenio(string cadena_a, string cadena_b){
	if (cadena_a.length() < 17){
		return true;
	} else {
		return false;
	}
}

/*
Esta función devuelve un entero indicando la mitad por la que se debe partir la cadena_a, para analizar cada parte por separado
*/
int dividir(string cadena){
	return cadena.length()/2;
}

/*
Este procedimiento será el encargado de combinar dos cadenas que se han resuelto de manera separada
*/
void combinar(int* res_1, int* res_2, string parte_1, string parte_2, string cadena_b, int* resultado, int m, string cadena_a){

	//cout << "Ahora tenemos que combinar las cadenas " << parte_1 << " y " << parte_2 << endl;

	// Declaramos todas las variables que irán conteniendo el resultado del procedimiento a lo largo de la ejecución de este
	int posicion = 0;
	int repeticiones = 0;
	int primera_aparicion = 0;
	int primera_aparicion_tam = 0;
	int ultima_aparicion = 0;
	int ultima_aparicion_tam = 0;

	// En primer lugar analizamos cadena_a y cadena_b para ver si se trata de un caso especial y tiene que ser resuelto de manera independiente
	int casoEspecial = comprobar(res_1, res_2, parte_1, parte_2, cadena_b, cadena_a);

	// Ahora, tal y como hemos realizado anteriormente en otras funciones, almacenamos los valores del tamaño de cadena_b
	int tam_cadena_b = cadena_b.length();

	//cout << "El caso especial es el numero " << casoEspecial << endl;

	if (casoEspecial > 0) {		// Si las cadenas forman un caso especial lo resolvemos independientemente del resto de casos

		// Esta resolución es ineficiente y no debería ser definitiva, pero se encuentra exenta de errores
		directo(cadena_a, cadena_b, resultado);

	} else if ((res_1[1] != 0) && (res_2[1] != 0) && (res_1[4] == res_2[2]-1)){		// Si la parte izq. y la dcha. tienen solución, y la ultima cadena valida
																					// de la parte izq. y la primera de la parte dcha. estan juntas
		// El numero de repeticiones de la cadena intermedia será la suma del tamaño de la última aparicion de 
		// la parte izquierda (parte 1) y el tamaño de la primera aparición de la parte derecha (parte 2)
		int rep_intermedio = res_1[5] + res_2[3];

		// primera_aparición será la primera aparición que tenemos en la parte izq.
		primera_aparicion = res_1[2];

		// ultima_aparicion será la de la parte dcha.
		ultima_aparicion = res_2[4];
		
		// Comprobamos cual de los tres resultados posibles es el mayor, y cuando lo sepamos, cambiamos repeticiones y posicion.
		if ((res_1[1] >= res_2[1]) && (res_1[1] >= rep_intermedio)){ 				// El nº de repeticiones la parte izq. es mayor o igual al de la parte dcha. y al de la cadena intermedia
			posicion = res_1[0];													// La posición y la repetición serán las de la parte izquierda
			repeticiones = res_1[1];
		} else if ((rep_intermedio > res_1[1]) && (rep_intermedio >= res_2[1])){	// El nº de repeticiones la cadena intermedia es mayor que el de la parte izq. y mayor o igual que el de la dcha.
			posicion = res_1[4] - tam_cadena_b*res_1[5] + 1;						// La posición será donde comienzan las cadenas de la ultima_aparicion, y las repeticiones serán el
			repeticiones = rep_intermedio;											// nº de repeticiones de la parte intermedia.
		} else if ((res_2[1] > res_1[1]) && (res_2[1] > rep_intermedio)){			// El nº de repeticiones la cadena dcha. es mayor que en la cadena izq. y la intermedia
			posicion = res_2[0];													// La posición y las repeticiones serán las de la cadena dcha.
			repeticiones = res_2[1];
		}

		// Ahora tenemos que calcular el tamaño de primera_aparicion (primera_aparicion_tam)
		if (res_1[4] - tam_cadena_b*res_1[5] + 1 == res_1[2]) {		// Si la ultima aparición de la parte izq. es también la primera aparición de esta parte
			primera_aparicion_tam = rep_intermedio;					// El tamaño de la primera_aparicion coincidirá el nº de repeticiones de la cadena intermedia
		} else {													// Si la parte izq. tienen distintas cadenas en primera_aparicion y ultima_aparicion
			primera_aparicion_tam = res_1[3];						// La primera_aparición_tam será igual al de la parte izq.
		}

		// Calculamos ahora el tamaño de ultima_aparicion (ultima_aparicion_tam)
		if (res_2[2] + tam_cadena_b*res_2[3] - 1 == res_2[4]) {		// Si las cadenas que forman la primera_aparición de la parte dcha. son las mismas que las que forman la
			ultima_aparicion_tam = rep_intermedio;					// ultima_aparicion de la parte dcha., ultima_aparicion_tam será el nº de repeticiones de la cadena intermedia
		} else {													// Si las cadenas que forman la primera_aparición de la parte dcha. NO son las mismas que las que forman la
			ultima_aparicion_tam = res_2[5];						// El tamaño de la ultima_aparicion será el tamaño de la ultima_aparicion de la parte dcha.
		}

		// Una vez obtenidos todos los resultados los almacenamos en el array resultado
		resultado[0] = posicion;
		resultado[1] = repeticiones;
		resultado[2] = primera_aparicion;
		resultado[3] = primera_aparicion_tam;
		resultado[4] = ultima_aparicion;
		resultado[5] = ultima_aparicion_tam;

	} else {	// Si no tenemos caso especial, y la ultima solucion de la parte izq. y la primera de la parte dcha. no enganchan (si ambas partes tienen solucion)

		// Una vez sabemos que nos encontramos en este caso, hemos de obtener la cadena intermedia para su estudio
		int inicio, final;						// Variables auxiliares para construir la cadena_a
		string cadena_int = "";					// Cadena intermedia que se analizara
		int tam_parte_1 = parte_1.length();		// Tamaño de la cadena izq.
		
		inicio = tam_parte_1 - (tam_cadena_b-1);	// Inicio comenzara en la posicion correspondiente a la longitud de cadena_b-1 contando desde el final
		final = tam_cadena_b - 2;					// Final será el tamaño de cadena_b - 1 (Se ajusta a -2 porque los arrays empiezan en 0)

		// Una vez sabemos inicio y final, formamos la cadena intermedia.
		int i = inicio;
		while(i < tam_parte_1){
			cadena_int = cadena_int + parte_1[i];
			i++;
		}
		i = 0;
		while(i <= final){
			cadena_int = cadena_int + parte_2[i];
			i++;
		}

		//cout << "Para combinarlas estudiaremos la cadena intermedia " << cadena_int << endl;

		// Ahora aplicamos el algoritmo directo sobre la cadena intermedia que hemos obtenido
		directo(cadena_int, cadena_b, resultado);

		/// Almacenamos en las variables en las que iremos calculando el resultado el resultado que hemos obtenido de la cadena intermedia
		posicion = resultado[0];
		repeticiones = resultado[1];
		primera_aparicion = resultado[2];
		primera_aparicion_tam = resultado[3];
		ultima_aparicion = resultado[4];
		ultima_aparicion_tam = resultado[5];

		// Creamos dos variables auxiliares en las que almacenaremos el tamaño de la cadena intermedia (ajustando con la parte izq. y la parte dcha.)
		// y la posición de inicio de esta cadena
		int tamano_1 = 0;
		int pos_1 = 0;

		if (posicion != 0){			// Si la cadena intermedia tiene resultado

			// Reajustamos las posiciones que obtuvimos como solución en la cadena intermedia para poder trabajar con ellas
			posicion = posicion + inicio;
			primera_aparicion = primera_aparicion + inicio;
			ultima_aparicion = ultima_aparicion + inicio;
			
			// Si parte 1 y parte 2 tienen solucion y la cadena de la izquierda y la de la derecha enganchan con la intermedia
			if ((res_1[1] != 0) && (res_2[1] != 0) && (res_1[4] == primera_aparicion - 1) && (res_2[2] == ultima_aparicion + 1)){
				tamano_1 = res_1[5] + repeticiones + res_2[3];			// El tamano sera el tamano de la cadena intermedia mas el tamano de la ultima aparicion de la parte 1 mas el tamano de la primera aparicion de la parte 2
				pos_1 = res_1[4] - tam_cadena_b*res_1[5] + 1;			// de la primera aparicion de la parte 2. pos_1 sera la posicion donde comienza el inicio de las cadenas de la ultima aparicion de la parte izq.
				primera_aparicion = res_1[2];							// La primera_aparición será la de la cadena de la izq.
				if (res_1[2] < pos_1){									// Si la primera_aparicion de la parte izq. se encuentra antes de la pos_1 que habíamos calculado antes
					primera_aparicion_tam = res_1[3];					// El tamaño de la primera_aparicion será el mismo que el de la primera_aparicion de la parte izq.
				} else {												// Si la primera_aparicion de la parte izq. es igual que la posición de pos_1
					primera_aparicion_tam = tamano_1;					// El tamaño de la primera aparición será el tamaño que habíamos calculado antes: tamano_1
				}
				ultima_aparicion = res_2[4];							// La última aparición será siempre la de la parte dcha.
				if (res_2[2] + res_2[3]*tam_cadena_b - 1 == res_2[4]){	// Si posición del último caracter de la primera_aparicion de la parte izq. coincide con su ultima_aparicion
					ultima_aparicion_tam = tamano_1;					// El tamaño de la ultima_aparicion será tamano_1
				} else {												// Si la ultima_aparicion es otra
					ultima_aparicion_tam = res_2[5];					// ultima_aparicion_tam será el tamaño de la ultima_aparicion de la parte dcha.
				}
			} else if ((res_2[1] != 0) && (res_2[2] == ultima_aparicion + 1)) {	// Si la parte derecha tiene resultado y la cadena intermedia engancha solo con la derecha
				tamano_1 = res_2[3] + ultima_aparicion_tam;						// La variable auxiliar tamano_1 valdrá el tamaño de la primera_aparicion de la dcha. y ultima_aparicion_tam
				pos_1 = primera_aparicion;										// La posición de esta cadena coincidirá con el de la primera_aparición de la cadena intermedia
				if (res_1[1] != 0){												// Si la parte izquierda tiene resultado
					primera_aparicion = res_1[2];								// La primera_aparicion y su tamaño serán las de la parte izq.
					primera_aparicion_tam = res_1[3];							
				} else {														// Si la parte izquierda no tiene resultado
					primera_aparicion_tam = tamano_1;							// Solo habrá que cambiar el tamaño de la primera aparición, que ahora será tamano_1
				}
				ultima_aparicion = res_2[4];									// La ultima_aparición será siempre la misma que la de la parte dcha.
				if (res_2[2] + res_2[3]*tam_cadena_b - 1 == res_2[4]){			// Si el último caracter de la primera_aparicion de la parte izq. es el mismo que el de ultima_aparicion
					ultima_aparicion_tam = tamano_1;							// El tamaño de ultima_aparicion será tamano_1
				} else {
					ultima_aparicion_tam = res_2[5];							// El tamaño de ultima_aparicion será el tamaño de la última aparición de la parte dcha.
				}
			} else if ((res_1[1] != 0) && (res_1[4] == primera_aparicion - 1)) {// Si la parte izquierda tiene resultado y la cadena intermedia engancha solo con la izquierda
				tamano_1 = res_1[5] + primera_aparicion_tam;					// tamano_1 valdrá la suma del tamaño de la ultima aparicion de la parte izquierda  y el de la primera aparicion
				pos_1 = res_1[4] - tam_cadena_b*res_1[5] + 1;					// de la cadena intermedia. La posición de esta cadena será la del comienzo de las cadenas de la ultima_aparicion de izq.
				if (res_2[1] != 0){												// Si la parte dcha. tiene resultado
					ultima_aparicion = res_2[4];								// La ultima_aparicion y su tamaño coincidirán con las de la dcha.
					ultima_aparicion_tam = res_2[5];
				} else {														// Si por el contrario no tiene resultado
					ultima_aparicion_tam = tamano_1;							// Mantenemos la ultima_aparicion y su tamaño será el de tamano_1
				}
				primera_aparicion = res_1[2];									// La primera_aparicion será siempre la de la parte izq.
				if (res_1[2] < pos_1){											// Si esta se encuentra antes que la pos_1 calculada con la cadena intermedia
					primera_aparicion_tam = res_1[3];							// El tamaño de la primera_aparicion será el mismo que el de la parte izq.
				} else {														// Si la primera_aparicion de la parte izq. se encuentra antes
					primera_aparicion_tam = tamano_1;							// El tamaño de la primera_aparicion será el calculado de la cadena intermedia (tamano_1)
				}
			} else {															// Si la cadena intermedia no engancha ni por la parte izq. no por la derecha
																				// No tenemos que calcular tamaños ni posiciones auxiliares
				if (res_1[1] != 0){												// Si la parte izquierda tiene resultado
					primera_aparicion = res_1[2];								// La primera_aparicion y su tamaño seran los de esta parte
					primera_aparicion_tam = res_1[3];
				}
				if (res_2[1] != 0){												// Si la parte derecha tiene resultado
					ultima_aparicion = res_2[4];								// La ultima_aparicion y su tamaño seran los de esta parte
					ultima_aparicion_tam = res_2[5];
				}
			}

		} else {									// Si la cadena intermedia no tiene resultado
													// No tenemos que calcular tamaños ni posiciones auxiliares
			if (res_1[1] == 0){						// Si la parte izquierda no tiene resultado
				primera_aparicion = res_2[2];		// La primera_aparicion y su tamaño serán los de la parte dcha.
				primera_aparicion_tam = res_2[3];
			} else if (res_1[1] != 0){				// Si la parte izquierda tiene resultado
				primera_aparicion = res_1[2];		// La primera_aparicion y su tamaño serán los de esta parte
				primera_aparicion_tam = res_1[3];
			}
			if (res_2[1] == 0){						// Si la parte derecha no tiene resultado
				ultima_aparicion = res_1[4];		// La ultima_aparicion y su tamaño serán los de la parte izq.
				ultima_aparicion_tam = res_1[5];
			} else if (res_2[1] != 0){				// Si la parte derecha tiene resultado
				ultima_aparicion = res_2[4];		// La ultima_aparicion y su tamaño serán los de esta parte
				ultima_aparicion_tam = res_2[5];
			}
		}
		
		// Tenemos que comprobar cual es el nº de repeticiones máximo que encontramos y elegirlo como resultado
		if ((res_1[1] >= tamano_1) && (res_1[1] >= repeticiones) && (res_1[1] >= res_2[1])){	// Si la cadena de la parte izq. es mayor o igual a las demas
			posicion = res_1[0];																// Nos quedaremos con su posición de inicio y su tamaño
			repeticiones = res_1[1];
		} else if ((tamano_1 > res_1[1]) && (tamano_1 > repeticiones) && (tamano_1 > res_2[1])){// Si la cadena intermedia es mayor a la izq. y mayor o igual a las dcha.
			posicion = pos_1;																	// Nos quedaremos con su posición de inicio y su tamaño
			repeticiones = tamano_1;
		} else if ((res_2[1] > res_1[1]) && (res_2[1] > tamano_1) && (res_2[1] > repeticiones)){// Si la cadena de la parte dcha. es mayor a las demas
			posicion = res_2[0];																// Nos quedaremos con su posición de inicio y su tamaño
			repeticiones = res_2[1];
		}

		// Para finalizar con el procedimiento almacenamos los resultados calculados en las variables en el array resultado
		resultado[0] = posicion;
		resultado[1] = repeticiones;
		resultado[2] = primera_aparicion;
		resultado[3] = primera_aparicion_tam;
		resultado[4] = ultima_aparicion;
		resultado[5] = ultima_aparicion_tam;

	}
}

/*
A continuación encontramos el algoritmo Divide y Vencerás, que utilizará directa, o indirectamente, todas las funciones y parámetros
definidas/os anteriormente.
*/
void divide(string cadena_a, string cadena_b, int* resultado){

	//cout << "Vamos a aplicar el algoritmo sobre la cadena " << cadena_a << endl;

	// Comprobamos si la cadena_a es suficientemente pequeña para calcular su resultado con el algoritmo directo o si por divide_y_venceras
	if (pequenio(cadena_a, cadena_b)){			// Si la cadena es pequeña
		//cout << "Aplicamos el algoritmo directo" << endl;
		directo(cadena_a, cadena_b, resultado);	// Aplicamos algoritmo directo y almacenamos el resultado en el parámetro resultado del procedimiento
	} else {									// Si la cadena es demasiado grande
		string parte_1;							// Declaramos dos variables de tipo string para almacenar ambas partes en las que vamos a dividir cadena_a
		string parte_2;
		int m = dividir(cadena_a);				// Almacenamos en la variable m la posición intermedia de la cadena_a
		// Obtenemos a partir de m, la parte_1 y la parte_2 de la cadena_a
		int i = 0;
		while(i < m){
			parte_1 = parte_1 + cadena_a[i];
			i++;
		}
		while(i < cadena_a.length()){
			parte_2 = parte_2 + cadena_a[i];
			i++;
		}
		// Declaramos dos punteros que almacenarán dos arrays de enteros de tamaño 6. Estos arrays almacenarán
		// el resultado de cada parte al aplicar a cada cadena el algoritmo divide_y_venceras
		int* res_1 = new int[6];
		int* res_2 = new int[6];
		//cout << "Vamos a dividir la cadena en " << parte_1 << " y " << parte_2 << endl;
		// Aplicamos divide_y_venceras a cada cadena
		divide(parte_1, cadena_b, res_1);
		divide(parte_2, cadena_b, res_2);
		// Reajustamos las posiciones de la cadena dcha. para poder trabajar con ella en el combinar
		res_2[0] = res_2[0] + m;
		res_2[2] = res_2[2] + m;
		res_2[4] = res_2[4] + m;
		// Aplicamos el procedimiento combinar sobre las cadenas y los resultados de estas
		combinar(res_1, res_2, parte_1, parte_2, cadena_b, resultado, m, cadena_a);
		// Finalizamos el algoritmo eliminando la memoria dinámica reservada con los punteros res_1 y res_2
		delete[] res_1;
		delete[] res_2;
		// Apuntamos los punteros a NULL para evitar posibles resultados inesperados
		res_1 = NULL;
		res_2 = NULL;

		//cout << "El resultado de la cadena " << cadena_a << " es " << endl;
		//cout << "La cadena comienza en " << resultado[0] << endl;
		//cout << "Se repite " << resultado[1] << " veces" << endl;
		//cout << "La primera aparicion es " << resultado[2] << endl;
		//cout << "El numero de repeticiones de la primera aparicion es " << resultado[3] << endl;
		//cout << "La ultima aparicion es " << resultado[4] << endl;
		//cout << "El numero de repeticiones de la ultima aparicion es " << resultado[5] << endl;

	}
}

/*
Método main del programa que se encagará de poner éste en ejecución
*/
int main(void){
	// Declaramos las variables donde se almacenarán las cadenas que se introducirán por la entrada
	string cadena_a, cadena_b;
	// Declaramos el puntero resultado y reservamos memoria para un array de 6 enteros donde se almacenará el resultado
	// resultado apuntará a esta estructura dinámica
	int* resultado = new int[6];
	//cout << "Introduce la cadena a" << endl;
	// Comenzamos un bucle para introducir todas las cadenas que se desee. Se puede finalizar el programa introduciendo el
	// caracter ñ en lugar de una cadena_a (se elije este caracter porque no forma parte del alfabeto)
	// En la condición del bucle se leerán cadena_a y cadena_b
	while ((cin >> cadena_a)&&(cadena_a != "ñ")&&(cin >> cadena_b)){
		// Reseteamos todos los valores de resultado
		for (int i = 0; i < 6; i++){
			resultado[i] = 0;
		}
		// Aplicamos el algoritmo divide_y_venceras sobre cadena_a y cadena_b
		divide(cadena_a, cadena_b, resultado);
		// Imprimimos el resultado
		cout << resultado[0] << " " << resultado[1] << endl;
	}
	// Borramos la memoria dinámica reservada con el puntero resultado, y luego dejamos resultado a NULL
	delete[] resultado;
	resultado = NULL;
	// Por último finalizamos el programa
	return 0;
}

/*
MUY IMPORTANTE: Siempre se debe finalizar el programa con el carácter ñ para eliminar corectamente la memoria dinámica reservada.
*/