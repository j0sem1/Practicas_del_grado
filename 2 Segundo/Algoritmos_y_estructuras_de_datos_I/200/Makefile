a.out: Pagina.o TablaHash.o DicPaginas.o main.o
	g++ Pagina.o TablaHash.o DicPaginas.o main.o

Pagina.o: Pagina.cpp Pagina.h
	g++ -c Pagina.cpp

TablaHash.o: TablaHash.h Pagina.h TablaHash.cpp 
	g++ -c TablaHash.cpp

DicPaginas.o: Pagina.h TablaHash.h DicPaginas.h DicPaginas.cpp
	g++ -c DicPaginas.cpp

main.o: Pagina.h TablaHash.h DicPaginas.h main.cpp
	g++ -c main.cpp
