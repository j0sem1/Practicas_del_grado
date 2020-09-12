/* ------------------------- PROBLEMA DEL SUDOKU ----------------------- */
#include <ga/GASimpleGA.h> //  Algoritmo Genetico simple
#include <ga/GA1DArrayGenome.h> // Genoma --> array de enteros (dim. 2) alelos
#include <fstream>
#include <math.h>
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

    // Recorremos el Sudoku por filas para ir inicializándolo
    for(int f = 0; f < plantilla1->tam; f++) {

        // Inicializamos el array auxiliar a cero
        for(int j = 0; j < plantilla1->tam; j++) aux[j] = 0;

        // En este bucle asignaremos un número aleatorio (del abecedario del Sudoku) a cada
        // posición del array 'aux' previamente creado
        for(int j = 1; j <= plantilla1->tam; j++) {
            // Creamos un número aleatorio que esté en el rango de 0 al tamaño del sudoku - 1
            // (abecedario del Sudoku)
            int v = GARandomInt(0, plantilla1->tam - 1);
            // Mientras que la posición actual tenga algún número distinto de cero avanzamos
            while (aux[v] != 0)
                v = (v + 1) % plantilla1->tam;
            // Accedemos aleatoriamente a una posición obtenida de manera aleatoria en el array,
            // y le asignamos el valor actual del bucle
            aux[v] = j;
        }

        int i = 0;

        /*
           Recorremos la fila actual del Sudoku desde la columna 'i' hasta encontrar una celda del Sudoku que
           no tenga el valor cero o hasta llegar al final de la fila. Si encontramos una celda del array
           auxiliar que tenga el mismo valor que la celda que tenía el valor distinto de cero en esta fila
           del Sudoku, entonces sustituimos el valor de esta cela del array auxiliar por el valor de la
           celda actual del array auxiliar (valor que hemos calculado de manera aleatoria) [Y ESTO PARA QUE].
           Finalmente a la posición actual de array auxilar le damos el valor del Sudoku que era distinto de cero.
        */
        while (i < plantilla1->tam) {

            // Aumentamos 'i' hasta que llegue al tamaño del Sudoku o hasta que la casilla de la
            // posición del array 'fijo' obtenida con el valor de 'i' sumado a la multiplicación de
            // 'f' por el tamaño, sea igual a cero. Es decir, aumentamos 'i' hasta que llegamos al límite
            // del Sudoku o encontramos una celda del array que no sea cero
            while ((plantilla1->fijo[(f * plantilla1->tam) + i] == 0) && (i < plantilla1->tam)) i++;

            // Si hemos encontrado una celda que no es cero buscamos una celda del array 'aux' que sea igual al
            // valor de la celda actual del array. Una vez encontrado, cambiamos el valor de la celda de
            // 'aux' por el de la celda de 'aux' de la posición 'i'.
            if (i < plantilla1->tam) {

                bool encontrado = false;
                for(int j = 0; (j < plantilla1->tam) && (!encontrado); j++)
                    if (aux[j] == plantilla1->fijo[(f * plantilla1->tam) + i]) {
                        encontrado = true;
                        aux[j] = aux[i];
                    }
                // Encontremos o no un valor de dos celdas que sea igual, asignamos el valor de la casilla actual
                // del Sudoku a la posición 'i' del array aux
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
     bool repe=false;

     for(int i=0;i<tam;i++) check[i]=0;

     for(int i=0;i<tam;i++)
             check[col[i]-1]++;
     for(int i=0;i<tam;i++) if (check[i]>1) repe=true;

     return repe;
}


int MutacionSudoku(GAGenome& g,float pmut){
    // Realizamos un casting para obtener genome
    GA1DArrayAlleleGenome<int>& genome = (GA1DArrayAlleleGenome<int>&) g;

    struct plantilla* plantilla1;
    plantilla1 = (struct plantilla*) genome.userData();
    int nmut = 0;
    int aux;
    int fil = 0;
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

                      for(int j = 0; j < plantilla1->tam; j++) caux[j]=genome.gene((j * plantilla1->tam) + c);
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
                         }
                         else {
                              genome.gene((fil * plantilla1->tam) + c, v1);
                         }

                      }

                }
                else{
                   int v1 = (c + 1) % plantilla1->tam;
                   while ((plantilla1->fijo[(f * plantilla1->tam) + v1] !=0 )) v1 = (v1 + 1) % plantilla1->tam;
                   aux = genome.gene((f * plantilla1->tam) + c);
                   genome.gene((f * plantilla1->tam) + c, genome.gene((f * plantilla1->tam) + v1));
                   genome.gene((f * plantilla1->tam) + v1, aux);
                   nmut++;
                }
           }
          }

    return nmut;
}

// Funcion objetivo.
float Objetivo(GAGenome& g) {
    // Realizamos un casting para obtener genome
    GA1DArrayAlleleGenome<int>& genome = (GA1DArrayAlleleGenome<int>&) g;
    float fitness = 0;
    int tam;

    struct plantilla* plantilla1;
    plantilla1 = (struct plantilla*) genome.userData();
    tam = plantilla1->tam;

    // Si en una fila/columna/cuadrado ha aparecido ya el valor 'x',
    // en la posición 'x' de este array aparecerá un 1, si no, un 0.
    int aux[tam];
    // Inicializar array
    memset(aux, 0, sizeof(aux));

    // Jaques en cada fila
    for (int fila = 0; fila < tam; fila++){
        // Inicializar array
        memset(aux, 0, sizeof(aux));
        for (int posicion = 0; posicion < tam; posicion++){
            if (aux[genome.gene((fila*tam) + posicion) - 1] == 0){
                aux[genome.gene((fila*tam) + posicion) - 1] = 1;
            } else {
                fitness++;
            }
        }
    }

    // Jaques en cada columna
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
    int tamCaja = sqrt(tam);
    int posAbs = 0;
    for (int nCaja = 0; nCaja < tam; nCaja++){
        // Inicializar array
        memset(aux, 0, sizeof(aux));
        if ((posAbs != 0) && (posAbs % tam != 0))
            posAbs = posAbs - tam*(tamCaja - 1);
        for (int posicion = 0; posicion < tam; posicion++){
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
    if ( (ga.statistics().minEver()==0) ||
        (ga.statistics().generation()==ga.nGenerations()) ) return gaTrue;
    else return gaFalse;
}

int main(int argc, char **argv) {

    fstream archivo;  // objeto de la clase ofstream
    archivo.open("Salida.txt", ios::app);

    plantilla* p = new plantilla;
    char* nombreF = argv[1];
    leerSudoku(p, nombreF);

    // Declaramos variables para los parametros del GA y las inicializamos
    int popsize = atoi(argv[2]);
    int ngen = atoi(argv[3]);
    float pcross = atof(argv[4]);
    float pmut = atof(argv[5]);
    int select = atoi(argv[6]);

    if (strcmp(argv[1], "Caso-A1.txt") == 0){
        if (select == 0){
            archivo << "GARouletteWheel ";
        } else {
            archivo << "GATournament ";
        }
        archivo << popsize << " ";
        archivo << pcross << " ";
        archivo << pmut << " ";
    }

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
    archivo << ga.statistics().minEver() << " ";
    if (strcmp(argv[1], "Caso-A5.txt") == 0){
        archivo << endl;
    }

    free(p);
    archivo.close();	// Cerramos el fichero que habíamos abierto
}
