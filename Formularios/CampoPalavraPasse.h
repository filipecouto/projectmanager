#ifndef CAMPOPALAVRAPASSE_H
#define CAMPOPALAVRAPASSE_H

#include "CampoString.h"

class CampoPalavraPasse : public CampoString {
	public:
		CampoPalavraPasse(string titulo, char letra, string* valor);
		CampoPalavraPasse(string titulo, char letra, string* valor, string comentario);

		virtual void apresentarCampo(EcraBase* ecra);
		virtual void campoSeleccionado(EcraBase* ecra);
};

CampoPalavraPasse::CampoPalavraPasse(string titulo, char letra, string* valor): CampoString(titulo, letra, valor) { }

CampoPalavraPasse::CampoPalavraPasse(string titulo, char letra, string* valor, string comentario): CampoString(titulo, letra, valor, comentario) { }

void CampoPalavraPasse::apresentarCampo(EcraBase* ecra) {
	if (*getValor() == "") CampoString::apresentarCampo(ecra);

	else {
		cout << " [";
		//ecra->setSublinhado();
		int len = getValor()->length();

		for (int i = 0; i < len; i++) cout << '*';

		ecra->reporFormatacao();
		cout << ']';
	}
}

void CampoPalavraPasse::campoSeleccionado(EcraBase* ecra) {
	cout << ' ';
#ifdef __GNUC__
	char* password = getpass("");
	getValor()->assign(password);
#elif defined(_WIN32)
	string p;
	char c = 0;
	while (c != 13) {
		c = getch();
		if(c == 13) break;
		if(c != '\b') {
			p+=c;
			cout <<	"*";
		} else if(p.size() > 0){
			cout << '\b';
			cout << ' ';
			cout << '\b';
			p.resize(p.size()-1);
		}
	}

	getValor()->assign(p);
#endif
}


#endif // CAMPOPALAVRAPASSE_H
