# Makefile util para ambientes Linux de 64 bits
default:
	g++ -I/usr/include/oracle/11.2/client -L/usr/lib64/oracle/10.2.0.4/client/lib/ -locci -lclntsh -std=c++11 -o GestorTarefas main.cpp
