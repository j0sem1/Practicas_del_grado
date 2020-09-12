//============================================================================
// Name        : backtracking.cpp
// Author      : Josemi y Fran
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>
using namespace std;

int pesototal;
int nivel;
int nalumnos;
int voa;
int pact;
int alact;
int limitep;
int limitea;
int soa[1000];
int peso[1000];

void generar(int* S){
	S[nivel] = S[nivel] + 1;
	pact = pact + S[nivel]*peso[nivel];
	alact = alact + S[nivel];
}

bool solucion(int* S){
	return ((pact <= limitep) && (alact <= limitea));
}

bool sobrepasa(int* S){
	int i = nivel+1;
	if (alact != limitea){
		while (i <= nalumnos){
			if ((pact + peso[i]) <= limitep) return false;
			i++;
		}
	}
	return true;
}

bool criterio(int* S){
	return (solucion(S) && (nivel < nalumnos) && !(sobrepasa(S)));
}

bool mashermanos(int* S){
	return (S[nivel] < 1);
}

void retroceder(int* S){
	pact = pact - peso[nivel]*S[nivel];
	alact = alact - S[nivel];
	S[nivel] = -1;
	nivel--;
}

int valor(int* S){
	int valor = 0;
	for(int j = 1; j <= nalumnos; j++){
		if (S[j] == 1){
			valor = valor + peso[j];
		}
	}
	return valor;
}

void copiar(int* S){
	for(int i = 1; i <= nalumnos; i++){
		soa[i] = S[i];
	}
}

void backtracking(int* S){
	nivel = 1;
	for (int i = 1; i <= nalumnos; i++){
		S[i] = -1;
	}
	for (int i = 1; i <= nalumnos; i++){
		soa[i] = -1;
	}
	voa = -999;
	pact = 0;
	alact = 0;
	while(nivel > 0){
		generar(S);
		if (solucion(S) && (pact > voa)){
			voa = pact;
			copiar(S);
		}
		if (criterio(S)) nivel++;
		else while(!(mashermanos(S)) && (nivel > 0)) retroceder(S);
	}
}

int main() {
	int n;	// Número de casos de prueba
	int i = 0;
	int monton1 = 0;
	string aux, comodin;
	cin >> n;
	while (i < n){
		getline(cin, comodin);
		pesototal = 0;
		nalumnos = 0;
		cin >> nalumnos;
		int* S = new int[1000];
		for (int j = 1; j <= nalumnos; j++){
			cin >> peso[j];
		}
		for (int j = 1; j <= nalumnos; j++){
			pesototal = pesototal + peso[j];
		}
		limitep = pesototal / 2;
		limitea = nalumnos - nalumnos / 2;
		backtracking(S);
		monton1 = voa;
		cout << monton1 << " " << pesototal-monton1 << endl;
		i++;
	}
	return 0;
}
