a.out: Pagina.o DicPaginas.o main.o
	g++ Pagina.o DicPaginas.o main.o

Pagina.o: Pagina.cpp Pagina.h
	g++ -c Pagina.cpp

DicPaginas.o: Pagina.h DicPaginas.h DicPaginas.cpp
	g++ -c DicPaginas.cpp

main.o: Pagina.h DicPaginas.h main.cpp
	g++ -c main.cpp
