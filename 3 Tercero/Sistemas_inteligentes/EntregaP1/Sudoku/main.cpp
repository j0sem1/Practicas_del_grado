/* ------------------------- PROBLEMA DEL SUDOKU ----------------------- */
#include <ga/GASimpleGA.h> //  Algoritmo Genetico simple
#include <ga/GA1DArrayGenome.h> // Genoma --> array de enteros (dim. 2) alelos
#include <fstream>
#include <math.h>
#include <string>

using namespace std;

struct plantilla{
    int tam;
    int *fijo;
};

void leerSudoku(struct plantilla* S, char* nombreF){
    ifstream f(nombreF);		// Abre un fichero de entrada con el nombre 'nombreF'

    // Equivale a dividir 'f' entre 2 tantas veces como indique 'S->tam'
    f >> S->tam;			// Con esto obtenemos el tamaño del sudoku

    S->fijo = new int[S->tam*S->tam];

    for(int i = 0 ; i < S->tam*S->tam ; i++)
        f >> S->fijo[i];	// Equivale a dividir f entre 2 tantas veces como indique 'S->fijo[i]'

    f.close();	// Cerramos el fichero que habíamos abierto
}

void InicioSudoku(GAGenome& g){
    // Realizamos un casting para obtener genome
    GA1DArrayAlleleGenome<int>& genome = (GA1DArrayAlleleGenome<int>&) g;

    // Creamos una plantilla, y cogemos los datos de la plantilla que tiene el genoma
    struct plantilla* plantilla1;
    plantilla1 = (struct plantilla*) genome.userData();

    // Creamos un array de enteros auxiliar con el tamaño de una fila del Sudoku
    int aux[plantilla1->tam];

    // Recorremos el Sudoku por filas
    for(int f = 0; f < plantilla1->tam; f++) {

        // Inicializamos el array 'aux' a cero
        for(int j = 0; j < plantilla1->tam; j++) aux[j] = 0;

        // Rellenamos el array 'aux' con números del abecedario del Sudoku
        for(int j = 1; j <= plantilla1->tam; j++) {

            int v = GARandomInt(0, plantilla1->tam - 1);

            while (aux[v] != 0)
                v = (v + 1) % plantilla1->tam;

            aux[v] = j;
        }

        int i = 0;

        // Realizamos este while para cada fila del Sudoku
        while (i < plantilla1->tam) {

            // Realizamos este while para cada posición de esta fila. Mientras esta posición sera 0, avanzamos
            while ((plantilla1->fijo[(f * plantilla1->tam) + i] == 0) && (i < plantilla1->tam)) i++;

            if (i < plantilla1->tam) {

                bool encontrado = false;
                for(int j = 0; (j < plantilla1->tam) && (!encontrado); j++)
                    if (aux[j] == plantilla1->fijo[(f * plantilla1->tam) + i]) {
                        encontrado = true;
                        aux[j] = aux[i];
                    }

                aux[i] = plantilla1->fijo[(f * plantilla1->tam) + i];
            }

            i++;

        }

        // Asignamos a cada celda de la fila actual del Sudoku, el valor que tiene guardado en el array auxiliar
        for(int c = 0; c < plantilla1->tam; c++)
            genome.gene((f * plantilla1->tam) + c, aux[c]);
    }
}

int CruceSudoku(const GAGenome& p1, const GAGenome& p2, GAGenome* c1, GAGenome* c2){
    // Realizamos un casting para obtener m y p
    GA1DArrayAlleleGenome<int>& m = (GA1DArrayAlleleGenome<int>&) p1;
    GA1DArrayAlleleGenome<int>& p = (GA1DArrayAlleleGenome<int>&) p2;

    // Creamos una plantilla, y cogemos los datos de la plantilla que tiene el genoma
    struct plantilla* plantilla1 = (struct plantilla*) m.userData();
    int n = 0;

    int punto1 = GARandomInt(0, m.length());
    while ((punto1 % plantilla1->tam) != 0) punto1++;
    int punto2 = m.length() - punto1;

    if (c1){
            // Realizamos un casting para obtener h1
            GA1DArrayAlleleGenome<int>& h1 = (GA1DArrayAlleleGenome<int>&)* c1;

            h1.GA1DArrayGenome<int>::copy(m, 0, 0, punto1); // el metodo copy esta definido en la clase GA1DArrayGenome
            h1.GA1DArrayGenome<int>::copy(p, punto1, punto1, punto2);
            n++;
    }

    if (c2){
            // Realizamos un casting para obtener c2
            GA1DArrayAlleleGenome<int>& h2 = (GA1DArrayAlleleGenome<int>&)* c2;

            h2.GA1DArrayGenome<int>::copy(p, 0, 0, punto1);
            h2.GA1DArrayGenome<int>::copy(m, punto1, punto1, punto2);
            n++;
    }

    return n;

}


bool checkColumna(int col[], int * check, int tam){
    bool repe = false;

    // Inicializa el array 'check' a 0
    for(int i = 0; i < tam; i++) check[i] = 0;

    // Suma 1 a cada posición del array 'check' conforme
    // avanzamos en la columna actual del Sudoku
    for(int i = 0; i < tam; i++)
        check[col[i] - 1]++;

    // Si la columna actual del Sudoku tiene alguna
    // casilla que no está vacía devolveremos TRUE
    for(int i = 0; i < tam; i++)
        if (check[i] > 1) repe = true;

     return repe;
}


int MutacionSudoku(GAGenome& g, float pmut){
    // Realizamos un casting para obtener genome
    GA1DArrayAlleleGenome<int>& genome = (GA1DArrayAlleleGenome<int>&) g;

    // Creamos una plantilla, y cogemos los datos de la plantilla que tiene el genoma
    struct plantilla* plantilla1;
    plantilla1 = (struct plantilla*) genome.userData();

    int nmut, aux, fil;
    nmut = fil = 0;
    bool fila;

    int caux[plantilla1->tam];
    int* checkC = new int[plantilla1->tam];

    if (pmut <= 0.0) return 0;

    for(int f = 0; f < plantilla1->tam; f++)

        for(int c = 0; c < plantilla1->tam; c++)

            if (plantilla1->fijo[(f * plantilla1->tam) + c] == 0){

                if (GAFlipCoin(pmut) ){

                    if (GAFlipCoin(0.5)) fila = true;
                    else fila = false;

                    if (!fila){

                        for(int j = 0; j < plantilla1->tam; j++) caux[j] = genome.gene((j * plantilla1->tam) + c);

                        if (checkColumna(caux, checkC, plantilla1->tam)){
                            int v1 = GARandomInt(0, plantilla1->tam - 1);
                            while (checkC[v1] <= 1) v1 = (v1 + 1) % plantilla1->tam;
                            v1++;
                            int v2 = GARandomInt(0, plantilla1->tam - 1);
                            while (checkC[v2] != 0) v2 = (v2 + 1) % plantilla1->tam;
                            v2++;

                            bool encontrado = false;
                            for(int j = 0; j < plantilla1->tam && !encontrado; j++)
                                if ((plantilla1->fijo[j * (plantilla1->tam) + c] == 0)&&(genome.gene(j * (plantilla1->tam) + c) == v1)){
                                    encontrado = true;
                                    genome.gene((j * plantilla1->tam) + c, v2);
                                    fil = j;
                                 }

                            int col = (c + 1) % plantilla1->tam;
                            while(genome.gene((fil * plantilla1->tam) + col) != v2) col = (col + 1) % plantilla1->tam;
                            if (plantilla1->fijo[(fil * plantilla1->tam) + col] == 0) {
                                nmut++;
                                genome.gene((fil * plantilla1->tam) + col, v1);
                            } else {
                                genome.gene((fil * plantilla1->tam) + c, v1);
                            }

                        }

                    } else {
                        int v1 = (c + 1) % plantilla1->tam;
                        while ((plantilla1->fijo[(f * plantilla1->tam) + v1] != 0)) v1 = (v1 + 1) % plantilla1->tam;
                        aux = genome.gene((f * plantilla1->tam) + c);
                        genome.gene((f * plantilla1->tam) + c, genome.gene((f * plantilla1->tam) + v1));
                        genome.gene((f * plantilla1->tam) + v1, aux);
                        nmut++;
                    }
                }
            }

    return nmut;
}

// Funcion objetivo/fitness
float Objetivo(GAGenome& g) {

    // Realizamos un casting para obtener genome
    GA1DArrayAlleleGenome<int>& genome = (GA1DArrayAlleleGenome<int>&) g;

    float fitness = 0;
    int tam;

    struct plantilla* plantilla1;
    plantilla1 = (struct plantilla*) genome.userData();

    // Tamaño de un lado del Sudoku
    tam = plantilla1->tam;

    int aux[tam];

    // Jaques en cada fila
    for (int fila = 0; fila < tam; fila++){
        // Inicializar array en cada fila
        memset(aux, 0, sizeof(aux));
        // Para cada posición de esa fila
        for (int posicion = 0; posicion < tam; posicion++){
            // Si nunca ha aparecido el número en esta fila, se le coloca un 1 en 'aux'
            if (aux[genome.gene((fila*tam) + posicion) - 1] == 0){
                aux[genome.gene((fila*tam) + posicion) - 1] = 1;
            } else {    // Si ya había aparecido, incrementamos el fitness
                fitness++;
            }
        }
    }

    // Jaques en cada columna siguiente el mismo procedimiento que en las filas
    for (int columna = 0; columna < tam; columna++){
        // Inicializar array
        memset(aux, 0, sizeof(aux));
        for (int posicion = 0; posicion < tam; posicion++){
            if (aux[genome.gene((posicion*tam) + columna) - 1] == 0){
                aux[genome.gene((posicion*tam) + columna) - 1] = 1;
            } else {
                fitness++;
            }
        }
    }

    // Jaques en cada cuadrícula
    /* La longitud de las filas de cada caja será la raíz cuadrada
       del tamaño de las filas del Sudoku */
    int tamCaja = sqrt(tam);
    int posAbs = 0;
    for (int nCaja = 0; nCaja < tam; nCaja++){
        // Inicializar array
        memset(aux, 0, sizeof(aux));
        /* Si no estamos en la primera casilla del Sudoku, y la caja que
           acabamos de recorrer no está en el borde derecho del Sudoku,
           habrá que retroceder la posición absoluta, para colocarla al
           principio de la siguiente caja */
        if ((posAbs != 0) && (posAbs % tam != 0))
            posAbs = posAbs - tam*(tamCaja - 1);
        // Recorremos todas las celdas de la caja actual
        for (int posicion = 0; posicion < tam; posicion++){
            /* Si no estamos en la primera posición de la caja, y hemos
               llegado al final de una fila de la caja, avanzamos la posición
               absoluta hasta el principio de la siguiente fila */
            if ((posicion != 0) && (posicion % tamCaja == 0))
                posAbs = posAbs + (tam - tamCaja);
            if (aux[genome.gene(posAbs) - 1] == 0){
                aux[genome.gene(posAbs) - 1] = 1;
            } else {
                fitness++;
            }
            posAbs++;
        }
    }

    return fitness;
}

// Funcion de terminación
GABoolean Termina(GAGeneticAlgorithm & ga){
    if ((ga.statistics().minEver() == 0) || (ga.statistics().generation() == ga.nGenerations())) return gaTrue;
    else return gaFalse;
}

int main(int argc, char **argv) {

    plantilla* p = new plantilla;
    char* nombreF = argv[1];
    leerSudoku(p, nombreF);

    // Creamos un fichero con el nombre 'Archivo_aux.txt'
    ofstream f_aux("Archivo_aux.txt");

    cout << "Sudoku de tamaño " << p->tam << "x" << p->tam << "\n\n";

    // Declaramos variables para los parametros del GA y las inicializamos
    int popsize = atoi(argv[2]);
    int ngen = atoi(argv[3]);
    float pcross = atof(argv[4]);
    float pmut = atof(argv[5]);
    int select = atoi(argv[6]);

    cout << "Parametros:    - Tamano poblacion: " << popsize << endl;
    cout << "               - Numero de generaciones: " << ngen << endl;
    cout << "               - Probabilidad cruce: " << pcross << endl;
    cout << "               - Probabilidad mutacion: " << pmut << endl << endl;

    // Conjunto enumerado de alelos --> valores posibles de cada gen del genoma
    GAAlleleSet<int> alelos;
    // Aquí creamos el alfabeto del problema
    for (int i = 1; i <= p->tam; i++)
        alelos.add(i);

    // Creamos el genoma y definimos operadores de inicio, cruce y mutación
    GA1DArrayAlleleGenome<int> genome(p->tam*p->tam, alelos, Objetivo, p);
    genome.initializer(InicioSudoku);
    genome.crossover(CruceSudoku);
    genome.mutator(MutacionSudoku);

    // Inicializamos el Sudoku
    InicioSudoku(genome);

    // Creamos el algoritmo genetico
    GASimpleGA ga(genome);

    // Inicializamos selección en función de la elección del usuario
    if (select == 0){
        GARouletteWheelSelector selector;
        ga.selector(selector);
    } else {
        GATournamentSelector selector;
        ga.selector(selector);
    }

    // Inicializamos - minimizar funcion objetivo, tamaño poblacion, nº generaciones,
    // pr. cruce y pr. mutacion, y le indicamos que evolucione.
    ga.minimaxi(-1);
    ga.populationSize(popsize);
    ga.nGenerations(ngen);
    ga.pCrossover(pcross);
    ga.pMutation(pmut);
    ga.terminator(Termina);
    ga.evolve(1);

    // Imprimimos el mejor individuo que encuentra el GA y su valor fitness
    // En el fichero auxiliar creado antes, guardamos el Sudoku solución
    f_aux << ga.statistics().bestIndividual() << endl;
    // Cerramos el fichero auxiliar
    f_aux.close();
    // Ahora abrimos ese fichero en modo lectura y almacenamos lo que contiene en un string,
    // así podemos tener el sudoku solución en string
    ifstream f_aux2("Archivo_aux.txt");
    string res;
    getline(f_aux2, res);

    // Introducimos los saltos de línea donde es necesario para que el Sudoku sea legible
    for (int i = p->tam*2-1; i < (p->tam*p->tam)*2; i += p->tam*2){
        res[i] = '\n';
    }

    cout << "El GA encuentra la solucion\n\n" << res << endl;

    cout << "con valor fitness " << ga.statistics().minEver() << endl;

    free(p);
    f_aux2.close();     // Cerramos el fichero que habíamos abierto
}
