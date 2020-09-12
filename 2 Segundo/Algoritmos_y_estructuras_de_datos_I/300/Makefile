a.out: Pagina.o TablaHash.o Nodo.o Arbol.o DicPaginas.o main.o
	g++ Pagina.o TablaHash.o Nodo.o Arbol.o DicPaginas.o main.o

Pagina.o: Pagina.h Pagina.cpp 
	g++ -c Pagina.cpp

TablaHash.o: Pagina.h TablaHash.h TablaHash.cpp 
	g++ -c TablaHash.cpp

Nodo.o: Pagina.h Nodo.h Nodo.cpp
	g++ -c Nodo.cpp

Arbol.o: Pagina.h Nodo.h Arbol.h Arbol.cpp
	g++ -c Arbol.cpp

DicPaginas.o: Pagina.h TablaHash.h Nodo.h Arbol.h DicPaginas.h DicPaginas.cpp
	g++ -c DicPaginas.cpp

main.o: Pagina.h TablaHash.h Nodo.h Arbol.h DicPaginas.h main.cpp
	g++ -c main.cpp
