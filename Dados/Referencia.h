/*********************************************************
 * Gestor de Tarefas - Referencia.h                      *
 *********************************************************
 * CONTEÚDO                                              *
 * Referencia                                            *
 *  string getDesignacao()                               *
 *  void setDesignacao(string designacao)                *
 *  Data getDataCriacao()                                *
 *  void setDataCriacao(Data dataCriacao)                *
 *  vector<Contexto*> getContextos()                     *
 *  string getDocumentoAssociado()                       *
 *  void setDocumentoAssociado(string documentoAssociado)*
 *********************************************************/
#ifndef REFERENCIA_H
#define REFERENCIA_H

#include <string>
#include "Data.h"
#include "Contexto.h"

using namespace std;

class Referencia {
    public:
    Referencia ( string designacao, vector<Contexto*> contextos, string documentoAssociado );
    Referencia ( int id, string designacao, Data dataCriacao, vector<Contexto*> contextos, string documentoAssociado );
    virtual ~Referencia();

    int getIdReferencia() const;
    void setIdReferencia ( int id );
	string getDescricao() const;
	void setDescricao(string descricao);
    string getDesignacao() const;
    void setDesignacao ( string designacao );
    Data getDataCriacao() const;
    void setDataCriacao ( Data dataCriacao );
    void setContextos ( vector<Contexto*> contextos );
    vector<Contexto*> getContextos() const;
    string getDocumentoAssociado() const;
    void setDocumentoAssociado ( string documentoAssociado );
    private:
    int mId;
	string mDescricao;
    string mDesignacao;
    Data mDataCriacao;
    vector<Contexto*> mContextos;
    string mDocumentoAssociado;
};

/**
 * Construtor da referência recebendo os seus parâmetros principais
 */
Referencia::Referencia ( string designacao, std::vector< Contexto* > contextos, string documentoAssociado ) : mId ( -1 ), mDesignacao ( designacao ), mContextos(contextos), mDocumentoAssociado ( documentoAssociado ) {

}

/**
 * Construtor da referência recebendo os seus parâmetros principais e o id
 */
Referencia::Referencia ( int id, string designacao, Data dataCriacao, std::vector< Contexto* > contextos, string documentoAssociado ) : mId ( id ), mDesignacao ( designacao ), mDataCriacao ( dataCriacao ), mContextos(contextos), mDocumentoAssociado ( documentoAssociado ) {

}

/**
 * Devolve o id da referência
 * @return o id
 */
int Referencia::getIdReferencia() const {
    return mId;
}

/**
 * Define o id da referência
 * @param id o novo id
 */
void Referencia::setIdReferencia ( int id ) {
    mId = id;
}

/**
 * Devolve a descrição da referência
 * @ return a descrição
 */
string Referencia::getDescricao() const {
	return mDescricao;
}

/**
 * Define a descrição da referência
 * @param descrição a nova descrição
 */

void Referencia::setDescricao(string descricao) {
	mDescricao = descricao;
}

/**
 * Devolve a designação da referência
 * @return a designação
 */
string Referencia::getDesignacao() const {
    return mDesignacao;
}

/**
 * Define a designação da referência
 * @param designacao a nova designação
 */
void Referencia::setDesignacao ( string designacao ) {
    mDesignacao = designacao;
}

/**
 * Devolve a data de criação da referência
 * @return a data de criação
 */
Data Referencia::getDataCriacao() const {
    return mDataCriacao;
}

/**
 * Define a data de criação da referência
 * @param datacriacao a nova data de criação
 */
void Referencia::setDataCriacao ( Data dataCriacao ) {
    mDataCriacao = dataCriacao;
}

/**
 * Define os contextos da referência
 * @param contextos os contextos da referencia
 */

void Referencia::setContextos ( vector<Contexto*> contextos ) {
    mContextos = contextos;
}

/**
 * Devolve os contextos da referência
 * @return os contextos
 */
vector<Contexto*> Referencia::getContextos() const {
    return mContextos;
}

/**
 * Devolve o endereço do documento associado à referência
 * @return o endereço do documento associado
 */
string Referencia::getDocumentoAssociado() const {
    return mDocumentoAssociado;
}

/**
 * Define o endereço do documento associado à referência
 * @param documentoassociado o novo endereço do documento associado
 */
void Referencia::setDocumentoAssociado ( string documentoAssociado ) {
    mDocumentoAssociado = documentoAssociado;
}

Referencia::~Referencia() {

}

#endif
