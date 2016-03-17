#ifndef CAMPOLISTA_H
#define CAMPOLISTA_H

#include <string>
#include "Campo.h"
#include "EcraBase.h"
#include <algorithm>
#include <iomanip>
#include <vector>
#include <iostream>

using namespace std;

class CampoLista : public Campo {
	public:
		CampoLista(string titulo, char letra, vector<string>* lista, int maximo);
		CampoLista(string titulo, char letra, vector<string>* lista, int maximo, string comentario);

		void setLista(vector<string>* lista);
		void setSeleccao(vector<int>* seleccao);
		void setMaximo(int maximo);

		virtual void apresentarCampo(EcraBase* ecra);
		virtual void campoSeleccionado(EcraBase* ecra);
		void processado();
		void processado_editado();
		void apagarElemento(int);

		vector<int>* getSeleccao();
		int getMaximo();
		bool checkMaximo();
	private:
		vector<int>* mSeleccao;
		vector<string>* mLista;
		bool *processados;
		int mMaximo;
};

CampoLista::CampoLista(string titulo, char letra, vector<string>* lista, int maximo) : Campo(titulo, letra) , mLista(lista) , mSeleccao(new vector<int>), mMaximo(maximo) { processado(); }

CampoLista::CampoLista(string titulo, char letra, vector<string>* lista, int maximo, string comentario) : Campo(titulo, letra, comentario) , mLista(lista) , mMaximo(maximo) { processado(); mSeleccao = new vector<int>; }

void CampoLista::setLista(vector<string>* lista) {
	mLista = lista;
}

void CampoLista::setSeleccao(vector<int>* seleccao) {
	mSeleccao = seleccao;
	if(seleccao->size() > 0) {
		processado_editado();
	}
}

void CampoLista::setMaximo(int maximo) {
	mMaximo = maximo;
}

vector<int>* CampoLista::getSeleccao() {
	return mSeleccao;
}

void CampoLista::processado() {
	processados = new bool[mLista->size()];
	for(int i = 0; i < mLista->size(); i++) {
		processados[i] = false;
	}
}

void CampoLista::processado_editado() {
	processados = new bool[mLista->size()];
	for(int i = 0; i < mLista->size(); i++) {
		for(int j = 0; j < mSeleccao->size() ; j++) {
			if(mSeleccao->at(j) == i) {
				processados[i] = true;
				break;
			} else {
				processados[i] = false;
			}
		}
	}
}

int CampoLista::getMaximo() {
	return mMaximo;
}

bool CampoLista::checkMaximo() {
	return mSeleccao->size() < mMaximo;
}

void CampoLista::apresentarCampo(EcraBase* ecra) {
	/*
		Percorrer a mLista e apresentar separado por ",".
		Se estiver vazio imprimir o simples "[_________________________]"
	*/
	if(mSeleccao->size() == 0) {
		cout << " [____________________]";
	} else {
		cout << " [";
		int size = mSeleccao->size();
		for(int i = 0 ; i < size; i++) {
			cout << mLista->at(mSeleccao->at(i));
			if((i+1) != size) {
				cout << ", ";
			}
		}
		cout << ".]";
	}
}

void CampoLista::apagarElemento(int elemento) {
	int size = mSeleccao->size();
	for(int i = 0; i < size; i++) {
		if(mSeleccao->at(i) == elemento) {
			mSeleccao->erase(mSeleccao->begin() + i);
			return;
		}
	}
}

void CampoLista::campoSeleccionado(EcraBase* ecra) {
	/*
		Imprimir para "baixo" como foi desenhado no documento os campos todos. Colocar um X nos que já estão seleccionados e guardar sempre na mSeleccao, 0 para concluir.
	*/

	// verificar se isto imprime bem
	int mOp;
	int i = 1;
	int size = mLista->size();
	Tamanho tamanhoConsola;
	int maior = mLista->at(0).length();
	for (int i = 1; i < size; i++)
		if (maior < mLista->at(i).length()) maior = mLista->at(i).length();
	maior += 7;
	do {
		ecra->getTamanho(tamanhoConsola);
		tamanhoConsola.largura /= 2;
		tamanhoConsola.altura /= 2;
		ecra->setPosicao(tamanhoConsola.largura - (maior / 2) , tamanhoConsola.altura - (size / 2));
		cout << " Escolha uma op" << (char)0xC3 << (char)0xA7 << (char)0xC3 << (char)0xA3 << "o:";
		for(i = 1 ; i <= size; i++) {
			ecra->setPosicao(tamanhoConsola.largura - (maior / 2), tamanhoConsola.altura - (size / 2 - i));
			cout << "| " << i << (processados[i-1] ? " X " : " - ")  << mLista->at(i-1);
			ecra->setPosicao(tamanhoConsola.largura + (maior / 2), tamanhoConsola.altura - (size / 2 - i));
			cout << "|";
		}
		ecra->setPosicao(tamanhoConsola.largura - (maior / 2), tamanhoConsola.altura - (size / 2 - i));
		cout << '|';
		for(int i = 1; i < maior; i++) cout << '_';
		cout << '|';
		ecra->setPosicao(tamanhoConsola.largura - (maior / 2) + 1, tamanhoConsola.altura - (size / 2 - i));
		cin >> mOp;
		if(mOp==0 || mOp > size) break;
		mOp--;
		if(mMaximo == 1 && mSeleccao->size() == 1) {
			processado();
			mSeleccao->pop_back();
		}
		if(!processados[mOp] && checkMaximo()) {
		  mSeleccao->push_back(mOp);
		  processados[mOp] = true;
		  if(mMaximo == 1) {
			  break;
		  }
		} else {	
			apagarElemento(mOp);
			processados[mOp] = false;
		}
	} while(true);
}

#endif // CAMPOLISTA_H
