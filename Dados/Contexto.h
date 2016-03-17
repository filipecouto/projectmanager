/***************************************************
 * Gestor de Tarefas - Contexto.h                  *
 ***************************************************
 * Este ficheiro contém a declaração e             *
 * implementação da classe Contexto que armazena   *
 * informações relacionadas com um dado contexto   *
 ***************************************************
 * CONTEÚDO                                        *
 * Contexto                                        *
 *  string getDesignacao()                         *
 *  void setDesignacao(const string Designacao)    *
 *  string getDesignacao()                         *
 *  void setPrioridade(const int Prioridade)       *
 *  int getPrioridade();                           *
 ***************************************************/

#ifndef CONTEXTO_H
#define CONTEXTO_H

#include <string>
#include <locale>
#include <algorithm>

using namespace std;

class Contexto {
	public:
		Contexto();
		Contexto( string designacao, int prioridade );
		Contexto( int id, string designacao, int prioridade );
		virtual ~Contexto();
		
		int getId() const;
		void setId(const int id);
		string getDesignacao();
		void setDesignacao(const string);
		int getPrioridade();
		void setPrioridade(const int);
		
		//vector<Referencia*>* mReferencias;
	private:
		int mId;
		string mDesignacao;
		int mPrioridade;
// 		Utilizador *mUtilizador;
};

Contexto::Contexto() : mId(-1), mDesignacao("") , mPrioridade(-1) { }

Contexto::Contexto(string designacao, int prioridade) : mId(-1), mDesignacao(designacao) , mPrioridade(prioridade) {  }
Contexto::Contexto(int id, string designacao, int prioridade) : mId(id), mDesignacao(designacao) , mPrioridade(prioridade) {  }

Contexto::~Contexto() { }

/**
 * Devolve o id do contexto
 * @return o id do contexto
 */
int Contexto::getId() const {
	return mId;
}

/**
 * Define o id do contexto
 * @param id o novo id do contexto
 */
void Contexto::setId ( const int id ) {
	mId = id;
}

void Contexto::setDesignacao(const string Designacao) {
	mDesignacao = Designacao;
}

string Contexto::getDesignacao() {
	return mDesignacao;
}

void Contexto::setPrioridade(const int Prioridade) {
	mPrioridade = Prioridade;
}

int Contexto::getPrioridade() {
	return mPrioridade;
}

#endif