#include <stdio.h>
#include <iostream>
using namespace std;

string normalizar(string entrada){
	string salida;
	for(unsigned int i = 0; i < entrada.length(); i++){
		if ((entrada[i] >= 'A') && (entrada[i] <= 'Z')){
			salida = (salida + char(tolower(entrada[i])));
		}else
		if (entrada[i] == char(0xC3)){
			i++;
			if ((entrada[i] == char(0xA1)) || (entrada[i] == char(0x81))) salida = salida + 'a';
			else if ((entrada[i] == char(0xA9)) || (entrada[i] == char(0x89))) salida = salida + 'e';
			else if ((entrada[i] == char(0xAD)) || (entrada[i] == char(0x8D))) salida = salida + 'i';
			else if ((entrada[i] == char(0xB3)) || (entrada[i] == char(0x93))) salida = salida + 'o';
			else if ((entrada[i] == char(0xBA)) || (entrada[i] == char(0xBC)) || (entrada[i] == char(0x9A)) || (entrada[i] == char(0x9C))) salida = salida + 'u';
			else if ((entrada[i] == char(0xB1)) || (entrada[i] == char(0x91))) salida = salida + char(0xC3) + char(0xB1);
			else salida = salida + char(0xC3) + entrada[i];
		}else{
			salida = (salida + entrada[i]);
		}
	}	
	return salida;
}

int main(void){

	string entrada;
	int i = 1;

	while(cin >> entrada){
		cout << i << ". " << entrada << " -> " << normalizar(entrada) << endl;
		i++;
	}

	return 0;
}