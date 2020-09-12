#include <stdio.h>
#include <iostream>
using namespace std;

int main(void){

	string entrada;
	int i = 1;

	while(cin >> entrada){
		cout << i << ". " << entrada << endl;
		i++;
	}

	return 0;
}