/*
 * main.cpp
 *
 *  Created on: 13 may. 2018
 *      Author: mrchemi
 */

#include <iostream>

using namespace std;

int M;				// Número de mecánicos
int A;				// Número de averías
int restantes;
bool C[1000][1000];	// Capacidad de un mecánico de reparar una avería [M, A]
int mpa[1000];		// Mecánicos por avería (Nº de mecánicos que pueden resolver una avería) [A]
int apm[1000];		// Averías por mecánico (Nº de averías que puede resolver un mecánico) [M]

bool solucion(int* S){
	int i = 0;
	while ((i < A) && (S[i] != 0)) i++;
	if (i == A) return true;
	return false;
}

int seleccionar(bool* R){
	int mejor, mecanico, i;
	mejor = 9999;
	i = 0;
	mecanico = 999;
	while (i < M){
		if ((R[i]) && (apm[i] < mejor)){
			mejor = apm[i];
			mecanico = i+1;
		}
		i++;
	}
	return mecanico;
}

bool factible(int* S, int mecanico){
	int i = 0;
	while (i < A){
		if (C[mecanico-1][i]){
			//if (S[i] == 0) return true;
			return true;
		}
		i++;
	}
	return false;
}

void insertar(int* S, int mecanico){
	int i, mejor, averia;
	mejor = 9999;
	i = 0;
	averia = 999;
	while (i < A){
		if ((mpa[i] > 0) && (mpa[i] < mejor) && (S[i] == 0) && (C[mecanico-1][i])){
			mejor = mpa[i];
			averia = i;
		}
		i++;
	}
	S[averia] = mecanico;
}

int objetivo(int* S){
	int i, n;
	i = 0;
	n = 0;
	while (i < A){
		if (S[i] != 0) n++;
		i++;
	}
	return n;
}

void voraz(bool* R, int* S){
	int mecanico;
	mecanico = 999;
	for (int i = 0; i < A; i++){
		S[i] = 0;
	}
	for (int i = 0; i < M; i++){
		R[i] = true;
	}
	while ((restantes != 0) && !(solucion(S))){
		mecanico = seleccionar(R);
		restantes--;
		R[mecanico-1] = false;
		if (factible(S, mecanico)){
			insertar(S, mecanico);
		}
	}
}

void almacenar(int j, string aux){
	int i = 0;
	int x = 0;
	while (i < A*2){
		if (aux[i] == '0'){
			C[j][x] = false;
		} else {
			C[j][x] = true;
		}
		x++;
		i = i + 2;
	}
}

void calcularmpa(){
	int n = 0;
	for (int a = 0; a < A; a++){
		for (int m = 0; m < M; m++){
			if (C[m][a]) n++;
		}
		mpa[a] = n;
		n = 0;
	}
}

void calcularapm(){
	int n = 0;
	for (int m = 0; m < M; m++){
		for (int a = 0; a < A; a++){
			if (C[m][a]) n++;
		}
		apm[m] = n;
		n = 0;
	}
}

void imprimir(int* S){
	for (int i = 0; i < A; i++){
		if (i != 0){
			cout << " ";
		}
		cout << S[i];
	}
	cout << endl;
}

int main(){
	int n;	// Número de casos de prueba
	int i = 0;
	string aux, comodin;
	cin >> n;
	cout << n << endl;
	while (i < n){
		cin >> M >> A;
		restantes = M;
		bool* R = new bool[1000];	// [M]
		int* S = new int[1000];		// [A]
		getline(cin, comodin);	// Necesario para que iostring no de problemas
		for (int j = 0; j < M; j++){
			getline(cin, aux);
			almacenar(j, aux);
		}
		calcularapm();
		calcularmpa();
		voraz(R, S);
		cout << objetivo(S) << endl;
		imprimir(S);
		i++;
	}
	return 0;
}
