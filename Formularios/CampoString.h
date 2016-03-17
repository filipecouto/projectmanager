#ifndef CAMPOSTRING_H
#define CAMPOSTRING_H

#include <string>
#include "Campo.h"

using namespace std;

class CampoString : public Campo {
	public:
		CampoString(string titulo, char letra, string* valor);
		CampoString(string titulo, char letra, string* valor, string comentario);

		string* getValor() const;

		void setValor(string* valor);

		virtual void apresentarCampo(EcraBase* ecra);
		virtual void campoSeleccionado(EcraBase* ecra);
	private:
		string* mValor;
};

CampoString::CampoString(string titulo, char letra, string* valor) : Campo(titulo, letra), mValor(valor)
{ }

CampoString::CampoString(string titulo, char letra, string* valor, string comentario) : Campo(titulo, letra, comentario), mValor(valor)
{ }

string* CampoString::getValor() const {
	return mValor;
}

void CampoString::setValor(string* valor) {
	mValor = valor;
}

void CampoString::apresentarCampo(EcraBase* ecra) {
	if (*mValor == "") cout << " [____________________]";

	else {
		cout << " [";
		//ecra->setSublinhado();
		cout << *mValor;
		ecra->reporFormatacao();
		cout << ']';
	}
}

void CampoString::campoSeleccionado(EcraBase* ecra) {
	cout << ' ';
	//ecra->setSublinhado();
	char c;
	cin >> c;
	getline(cin, *mValor);
	*mValor = c + *mValor;
	//cin >> *mValor;
}

#endif // CAMPOSTRING_H
