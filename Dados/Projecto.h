#ifndef PROJECTO_H
#define PROJECTO_H


#include "Tarefa.h"

class Projecto : public Tarefa {
    private:
    vector<Tarefa*> mTarefas;
    int mId;
    public:
    Projecto ( string designacao, string inicio, string fim, int duracao, short int progresso, int prioridade, int visibilidade,
               Delegacao* d, Tarefa  * depende, vector<Contexto*> contextos, string documento );
    Projecto ( int id, int idTarefa, int idReferencia, string designacao, Data inicio, Data fim, int duracao, short int progresso, int prioridade, int visibilidade, Delegacao* d,
               Tarefa * depende, vector<Contexto*> contextos, string documento,
               Data criacao );
    Projecto ( int id, const Tarefa & );
    Projecto ( const Projecto & );
    ~Projecto();
    void addTarefa ( Tarefa * );
    void removeTarefa ( Tarefa * );
    vector<Tarefa*> getTarefas() const;
    bool hasTarefa ( Tarefa* tarefa ) const;
    void setTarefas ( vector<Tarefa*> );
    int getIdProjecto() const;
};

Projecto::Projecto ( string designacao, string inicio, string fim, int duracao, short int progresso, int prioridade, int visibilidade,
                     Delegacao* delegacao, Tarefa * depende, vector<Contexto*> contextos, string documento ) : Tarefa ( designacao, inicio, fim, duracao, progresso, prioridade, visibilidade, delegacao, depende, contextos, documento ) { }

Projecto::Projecto ( int id, int idTarefa, int idReferencia, string designacao, Data inicio, Data fim, int duracao, short int progresso, int prioridade, int visibilidade,
                     Delegacao* delegacao, Tarefa *  depende, vector<Contexto*> contextos, string documento,
                     Data criacao ) : Tarefa ( idTarefa, idReferencia, designacao, inicio, fim, duracao, progresso, prioridade, visibilidade, delegacao, depende, contextos, documento, criacao ) { }

Projecto::Projecto ( int id, const Tarefa& p ) : Tarefa ( p ), mId ( id ) {
}

Projecto::Projecto ( const Projecto & p ) : Tarefa ( p ) {
    mTarefas = p.mTarefas;
}

Projecto::~Projecto() { }

void Projecto::addTarefa ( Tarefa * tarefa ) {
    mTarefas.push_back ( tarefa );
}

void Projecto::removeTarefa ( Tarefa *tarefa ) {
    for ( int i = 0; i < mTarefas.size(); i++ ) {
        if ( mTarefas.at ( i ) == tarefa ) {
            mTarefas.erase ( mTarefas.begin() + i );
            return;
        }
    }
}

bool Projecto::hasTarefa ( Tarefa* tarefa ) const {
    int len = mTarefas.size();
    for ( int i = 0; i < len; i++ ) {
        if ( mTarefas[i]->getId() == tarefa->getId() ) return true;
    }
    return false;
}

vector<Tarefa*> Projecto::getTarefas() const {
    return mTarefas;
}

void Projecto::setTarefas ( vector<Tarefa*> tarefas ) {
    mTarefas = tarefas;
}

int Projecto::getIdProjecto() const {
    return mId;
}

#endif // PROJECTO_H
