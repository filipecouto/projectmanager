#ifndef FORMCONTEXTOS_H
#define FORMCONTEXTOS_H

#include "EcraLista.h"
#include "Comunicacao.h"
#include "EditorContexto.h"
#include "EcraContexto.h"

class EcraContextos : public EcraLista, public Comunicacao {
    public:
    EcraContextos ( Comunicacao* contexto );
    virtual void executarAccao ( Accao* accao );
	
    private:
    virtual float getLarguraColuna ( int coluna );
    virtual int getNumColunas();
    virtual int getNumLinhas();
    virtual string getNomeColuna ( int coluna );
    virtual void imprimirCelula ( int coluna, int linha );
    virtual void itemSelecionado ( int linha );

    inline void imprimirPrioridade ( int prioridade );

    vector<Contexto*> mContextos;
};

EcraContextos::EcraContextos ( Comunicacao* contexto ) : EcraLista ( "Contextos" ), Comunicacao ( contexto ) {
    juntarAccao ( new Accao ( "Novo contexto", 'C' ) );
    juntarAccao ( new Accao ( "Voltar", '0' ) );

    mContextos = getBaseDados()->getContextos ( *getUtilizador() );
}

void EcraContextos::itemSelecionado ( int linha ) {
    EcraContexto ( this, mContextos[linha] ).mostrar();
    mContextos.clear();
    mContextos = getBaseDados()->getContextos ( *getUtilizador() );
}

void EcraContextos::imprimirCelula ( int coluna, int linha ) {
    switch ( mContextos[linha]->getPrioridade() ) {
    case 0:
        setCor ( 2 );
        break;	// se for o contexto prioritário, então a cor será vermelha
    case 99:
        setCor ( 1 );
        break;	// se for o contexto prioritário, então a cor será verde
    }
    switch ( coluna ) {
    case 0:
        cout << mContextos[linha]->getDesignacao();
        return;
    case 1:
        imprimirPrioridade ( mContextos[linha]->getPrioridade() );
        return;
    case 2:
        return;
    }
}

void EcraContextos::imprimirPrioridade ( int prioridade ) {
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

int EcraContextos::getNumLinhas() {
    return mContextos.size();
}

int EcraContextos::getNumColunas() {
    return 3;
}

float EcraContextos::getLarguraColuna ( int coluna ) {
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

string EcraContextos::getNomeColuna ( int coluna ) {
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

void EcraContextos::executarAccao ( Accao* accao ) {
    switch ( accao->getLetra() ) {
    case 'C':
        EditorContexto ( this ).mostrar();
        mContextos.clear();
        mContextos = getBaseDados()->getContextos ( *getUtilizador() );
        break;
    case '0':
        fechar();
        break;
    }
}

#endif


