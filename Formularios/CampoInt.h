#ifndef CAMPOINT_H
#define CAMPOINT_H

#include <string>
#include <sstream>
#include <iostream>
#include "CampoString.h"
#include "EcraBase.h"

using namespace std;

class CampoInt : public CampoString {
	public:
		CampoInt(string titulo, char letra, int *valor);
		CampoInt(string titulo, char letra, int *valor, string comentario);

		int * getValor() const;

		void setValor(int *valor);
		void setValor(int valor);

		virtual void campoSeleccionado(EcraBase* ecra);
	
	private:
		int *mValor;
		string mString;
};

CampoInt::CampoInt(string titulo, char letra, int *valor) : CampoString(titulo, letra, &mString) , mValor(valor) 
{ stringstream s; s << *mValor; s >> mString; } 

CampoInt::CampoInt(string titulo, char letra, int *valor, string comentario) : CampoString(titulo, letra, &mString, comentario) , mValor(valor) 
{ stringstream s; s << *mValor; s >> mString; } 

int * CampoInt::getValor() const {
	return mValor;
}

void CampoInt::setValor(int *valor) {
	mValor = valor;
}

void CampoInt::setValor(int valor) {
	stringstream s; s << valor; s >> mString;
	*mValor = valor;
}

void CampoInt::campoSeleccionado(EcraBase* ecra) {
	CampoString::campoSeleccionado(ecra);
	stringstream s(mString);
	s >> *mValor;
	stringstream s2;
	s2 << *mValor;
	s2 >> mString;
}

#endif