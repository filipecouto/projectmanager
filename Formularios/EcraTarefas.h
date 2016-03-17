#ifndef FORMLISTATAREFAS_H
#define FORMLISTATAREFAS_H

#include "EcraLista.h"
#include "Comunicacao.h"
#include "EditorTarefa.h"
#include "EcraTarefa.h"

class EcraTarefas : public EcraLista, public Comunicacao {
    public:
    EcraTarefas ( Comunicacao* contexto );
    virtual void executarAccao ( Accao* accao );
	
    private:
    virtual float getLarguraColuna ( int coluna );
    virtual int getNumColunas();
    virtual int getNumLinhas();
    virtual string getNomeColuna ( int coluna );
    virtual void imprimirCelula ( int coluna, int linha );
    virtual void itemSelecionado ( int linha );

    inline void imprimirPrioridade ( int prioridade );

    vector<Tarefa*> mTarefas;
};

EcraTarefas::EcraTarefas ( Comunicacao* contexto ) : EcraLista ( "Contextos" ), Comunicacao ( contexto ) {
	juntarAccao ( new Accao ( "Nova Tarefa", 'C' ) );
    juntarAccao ( new Accao ( "Voltar", '0' ) );
	mTarefas = getBaseDados()->getTarefas ( *getUtilizador(),string()," t.id not in (select id from projecto_tarefa)"  );
}

void EcraTarefas::itemSelecionado ( int linha ) {
    EcraTarefa ( this, mTarefas[linha] ).mostrar();
    mTarefas.clear();
    mTarefas = getBaseDados()->getTarefas ( *getUtilizador() );
}

void EcraTarefas::imprimirCelula ( int coluna, int linha ) {
    switch ( mTarefas[linha]->getPrioridade() ) {
    case 0:
        setCor ( 2 );
        break;	// se for o contexto prioritário, então a cor será vermelha
    case 99:
        setCor ( 1 );
        break;	// se for o contexto prioritário, então a cor será verde
    }
    switch ( coluna ) {
    case 0:
        cout << mTarefas[linha]->getDesignacao();
        return;
    case 1:
        imprimirPrioridade ( mTarefas[linha]->getPrioridade() );
        return;
    case 2:
        return;
    }
}

void EcraTarefas::imprimirPrioridade ( int prioridade ) {
    if ( prioridade == 0 ) {
        cout << "mínima";
        return;
    }
    if ( prioridade == 99 ) {
        cout << "máxima";
        return;
    }
    if ( prioridade >= 75 ) cout << "muito alta (";
    else if ( prioridade > 54 ) cout << "alta (";
    else if ( prioridade > 47 ) cout << "média (";
    else if ( prioridade >= 25 ) cout << "baixa (";
    else cout << "muito baixa (";
    cout << prioridade << ")";
}

int EcraTarefas::getNumLinhas() {
    return mTarefas.size();
}

int EcraTarefas::getNumColunas() {
    return 3;
}

float EcraTarefas::getLarguraColuna ( int coluna ) {
    switch ( coluna ) {
    case 0:
        return 0.6f;
    case 1:
        return 0.25f;
    case 2:
        return 0.15f;
    default:
        return 0;
    }
}

string EcraTarefas::getNomeColuna ( int coluna ) {
    switch ( coluna ) {
    case 0:
        return "Designação";
    case 1:
        return "Prioridade";
    case 2:
        return "Tarefas";
    default:
        return 0;
    }
}

void EcraTarefas::executarAccao ( Accao* accao ) {
    switch ( accao->getLetra() ) {
    case 'C':
        EditorTarefa ( this , 1 ).mostrar();
        mTarefas.clear();
        mTarefas = getBaseDados()->getTarefas ( *getUtilizador() );
        break;
    case '0':
        fechar();
        break;
    }
}

#endif