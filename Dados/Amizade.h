#ifndef AMIZADEH_
#define AMIZADEH_

#include "Utilizador.h"
#include <iostream>

class Amizade
{
private:
	Utilizador* mAmigo;
	int mEstado;
public:
	Amizade(Utilizador*amigo, int estado);
	void setAmigo(Utilizador* amigo);
	void setEstado(int estado);
	Utilizador* getAmigo();
	int getEstado();
};

Amizade::Amizade(Utilizador* amigo, int estado)
{
	setAmigo(amigo);
	setEstado(estado);
}

void Amizade::setAmigo(Utilizador*amigo)
{
	mAmigo = amigo;
}

void Amizade::setEstado(int estado)
{
	mEstado = estado;
}

Utilizador* Amizade::getAmigo()
{
	return mAmigo;
}

int Amizade:: getEstado()
{
	return mEstado;
}
#endif