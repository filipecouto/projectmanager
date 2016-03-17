#ifndef TAREFA_H
#define TAREFA_H

#include <iostream>
#include "Data.h"
#include "Delegacao.h"
#include "Referencia.h"
#include <time.h>
#include <sstream>

using namespace std;

class Tarefa: public Referencia {
    private:
    int mId;
    Data mDataInicio;
    Data mDataFim;
    int mDuracaoPrevista;
    short int mProgresso;
    int mPrioridade;
    Delegacao* mDelegacao;
    int mVisibilidade;
    Tarefa* mDependenteDe;
    public:
//     Tarefa();
    Tarefa ( string designacao, string inicio, string fim, int duracao, short int progresso, int prioridade, int visibilidade,
             Delegacao* d, Tarefa  * depende, vector<Contexto*> contextos, string documento );
    Tarefa ( int id, int idReferencia, string designacao, Data inicio, Data fim, int duracao, short int progresso, int prioridade, int visibilidade, Delegacao* d,
             Tarefa * depende, vector<Contexto*> contextos, string documento,
             Data criacao );
    Tarefa ( const Tarefa &t );
    ~Tarefa();
    void setId ( int id );
    void setDataInicio ( Data inicio );
    void setDataFim ( Data fim );
    void setDuracaoPrevista ( int duracao );
    void setDuracaoPrevista();
    void setDelegacao ( Delegacao* d );
    void setProgresso ( short int complecao );
    void setPrioridade ( int p );
    void setVisibilidade ( int p );
    void setDependenteDe ( Tarefa * depende );
    int getId();
    Data getDataInicio();
    Data getDataFim();
    int getDuracaoPrevista();
    short int getProgresso();
    int getPrioridade();
    Delegacao* getDelegacao();
    int getVisibilidade();
    Tarefa* getDependenteDe();
};

Tarefa::Tarefa ( string designacao, string inicio, string fim, int duracao, short int progresso, int prioridade, int visibilidade,
                 Delegacao* delegacao, Tarefa * depende, vector<Contexto*> contextos, string documento ) :Referencia ( designacao, contextos, documento ) {
    setId ( -1 );
    setDataInicio ( inicio );
    setDataFim ( fim );
    setProgresso ( progresso );
    setDuracaoPrevista( duracao );
    setPrioridade ( prioridade );
    setDelegacao ( delegacao );
    setVisibilidade ( visibilidade );
    setDependenteDe ( depende );
}

Tarefa::Tarefa ( int id, int idReferencia, string designacao, Data inicio, Data fim, int duracao, short int progresso, int prioridade, int visibilidade,
                 Delegacao* delegacao, Tarefa *  depende, vector<Contexto*> contextos,string documento,
                 Data criacao ) : mId ( id ), Referencia ( idReferencia, designacao, criacao, contextos, documento ) {
    setDataInicio ( inicio );
    setDataFim ( fim );
    setProgresso ( progresso );
    setDuracaoPrevista(duracao);
    setPrioridade ( prioridade );
    setDelegacao ( delegacao );
    setVisibilidade ( visibilidade );
    setDependenteDe ( depende );
    //cout << id << endl << idReferencia << endl << designacao << endl << inicio << endl << fim << endl << prioridade << endl << visibilidade << endl << delegacao << endl << depende << endl << contextos.size() << endl << documento << endl << criacao.listarstr() << endl;
}

Tarefa::Tarefa ( const Tarefa &t ) :Referencia ( t.getIdReferencia(), t.getDesignacao(), t.getDataCriacao(), t.getContextos(), t.getDocumentoAssociado() ) {
	setDescricao(t.getDescricao());
    setId ( t.mId );
    setDataInicio ( t.mDataInicio );
    setDataFim ( t.mDataFim );
    setProgresso ( t.mProgresso );
    setDuracaoPrevista ( t.mDuracaoPrevista );
    setPrioridade ( t.mPrioridade );
    setDelegacao ( t.mDelegacao );
    setVisibilidade ( t.mVisibilidade );
}
void Tarefa::setId ( int id ) {
    mId = id;
}

void Tarefa::setDataInicio ( Data inicio ) {
    mDataInicio=inicio;
}

void Tarefa:: setDataFim ( Data fim ) {
    mDataFim = fim;
}

void Tarefa::setProgresso ( short int complecao ) {
    mProgresso = complecao;
}

void Tarefa::setDuracaoPrevista ( int duracao ) {
    mDuracaoPrevista = duracao;
}

void Tarefa::setDuracaoPrevista() {
    int segundos = Data::obterSegAno ( mDataInicio.getAno(), mDataFim.getAno() ) +
                   Data::obterSegMes ( mDataInicio.getMes(), mDataFim.getMes() ) +
                   Data::obterSegDia ( mDataInicio.getDia(), mDataFim.getDia() ) +
                   Data::obterSegHoras ( mDataInicio.getHora(), mDataFim.getHora() ) +
                   Data::obterSegMin ( mDataInicio.getMinuto(), mDataFim.getMinuto() ) +
                   Data::obterSegSeg ( mDataInicio.getSegundo(), mDataFim.getSegundo() );
    mDuracaoPrevista = segundos;
}

void Tarefa::setDelegacao ( Delegacao* d ) {
    mDelegacao = d;
}

void Tarefa::setPrioridade ( int p ) {
    mPrioridade = p;
}

void Tarefa::setVisibilidade ( int p ) {
    mVisibilidade = p;
}

void Tarefa::setDependenteDe ( Tarefa * depende ) {
    mDependenteDe = depende;
}
int Tarefa::getId() {
    return mId;
}

Data Tarefa::getDataFim() {
    return mDataFim;
}

Data Tarefa::getDataInicio() {
    return mDataInicio;
}

Delegacao* Tarefa::getDelegacao() {
    return mDelegacao;
}

short int Tarefa::getProgresso() {
    return mProgresso;
}

int Tarefa::getDuracaoPrevista() {
    return mDuracaoPrevista;
}

int Tarefa::getPrioridade() {
    return mPrioridade;
}

int Tarefa::getVisibilidade() {
    return mVisibilidade;
}

Tarefa* Tarefa ::getDependenteDe() {
    return mDependenteDe;
}
Tarefa::~Tarefa() {

}
#endif
