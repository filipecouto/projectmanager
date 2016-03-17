#ifndef FORMLISTAAMIGOS_H
#define FORMLISTAAMIGOS_H

#include "../Formularios/EcraLista.h"
#include "../Dados/Utilizador.h"
#include "../Formularios/Comunicacao.h"
#include "EditorAmizade.h"
#include "../Dados/Amizade.h"
#include "../BaseDados/BDados.h"
#include "../Formularios/EcraTarefa.h"
#include <iostream>

class EcraListaAmigos : public EcraLista, public Comunicacao {
    public:
    EcraListaAmigos ( Comunicacao* comunicacao, Amizade * amigo );
    virtual void executarAccao ( Accao* accao );

    private:
    virtual float getLarguraColuna ( int coluna );
    virtual int getNumColunas();
    virtual int getNumLinhas();
    virtual string getNomeColuna ( int coluna );
    virtual void imprimirCelula ( int coluna, int linha );
    virtual void itemSelecionado ( int linha );

    inline void imprimirPrioridade ( int prioridade );

    Amizade *mAmigo;
    vector<Tarefa*> mTarefas;

    bool mMostrarCompletos;

    inline void carregarTarefas();
};

EcraListaAmigos::EcraListaAmigos ( Comunicacao* comunicacao, Amizade * amigo ) : EcraLista ( ), mAmigo ( amigo ), mMostrarCompletos ( false ), Comunicacao ( comunicacao ) {
    setTitulo ( "Tarefas do Utilizador \"" + mAmigo->getAmigo()->getNomeUtilizador() + '\"' );
    mAmigo = amigo;
    juntarAccao ( new Accao ( "Adicionar amigo", 'V' ) );
    juntarAccao ( new Accao ( "Remover amigo", 'X' ) );
    juntarAccao ( new Accao ( "Voltar", '0' ) );

    carregarTarefas();
}

void EcraListaAmigos::carregarTarefas() {
    mTarefas.clear();
	// TODO isto carrega todas as tarefas publicas e semi-publicas mesmo se a outra pessoa nao e amiga
    mTarefas = getBaseDados()->getTarefas ( *mAmigo->getAmigo(), string(), string() + ( mMostrarCompletos? "" : "progresso != 100 and " ) + "visibilidade != 2", "dataCriacao" );
}

void EcraListaAmigos::itemSelecionado ( int linha ) {
    EcraTarefa ( this, mTarefas[linha] ).mostrar();
    carregarTarefas();
}

void EcraListaAmigos::imprimirCelula ( int coluna, int linha ) {
    switch ( coluna ) {
    case 0:
        if ( mTarefas[linha]->getProgresso() == 100 )
            cout << ( char ) 226 << ( char ) 156 << ( char ) 147 << ' ';
        if ( mTarefas[linha]->getDelegacao() )
            cout << ( char ) 0xE2 << ( char ) 0x9E << ( char ) 0x9F;
        break;
    case 1:
        cout << mTarefas[linha]->getDesignacao();
        break;
    case 2: {
        Data di = mTarefas[linha]->getDataInicio();
        Data df = mTarefas[linha]->getDataFim();
        if ( df.getValido() )
            cout << di.getDia() << '/' << di.getMes() << " ~ " << df.getDia() << '/' << df.getMes();
        else if ( di.getValido() ) {
            int t = mTarefas[linha]->getDuracaoPrevista();
            switch ( Data::simplificarTempoDeSegundos ( t ) ) {
            case 1:
                cout << di.getDia() << '/' << di.getMes() << " (" << t << " seg)";
                break;
            case 2:
                cout << di.getDia() << '/' << di.getMes() << " (" << t << " min)";
                break;
            case 3:
                cout << di.getDia() << '/' << di.getMes() << " (" << t << " h)";
                break;
            case 4:
                cout << di.getDia() << '/' << di.getMes() << " (" << t << " dia(s))";
                break;
            }
        } else {
            int t = mTarefas[linha]->getDuracaoPrevista();
            switch ( Data::simplificarTempoDeSegundos ( t ) ) {
            case 1:
                cout << t << " segundos";
                break;
            case 2:
                cout << t << " minutos";
                break;
            case 3:
                cout << t << " horas";
                break;
            case 4:
                cout << t << " dias";
                break;
            }
        }
        break;
    }
    case 3:
        imprimirPrioridade ( mTarefas[linha]->getPrioridade() );
        break;
    }
}

void EcraListaAmigos::imprimirPrioridade ( int prioridade ) {
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

int EcraListaAmigos::getNumLinhas() {
    return mTarefas.size();
}

int EcraListaAmigos::getNumColunas() {
    return 4;
}

float EcraListaAmigos::getLarguraColuna ( int coluna ) {
    switch ( coluna ) {
    case 0:
        return 0.04f;
    case 1:
        return 0.55f;
    case 2:
        return 0.25f;
    case 3:
        return 0.15f;
    default:
        return 0;
    }
}

string EcraListaAmigos::getNomeColuna ( int coluna ) {
    switch ( coluna ) {
    case 0:
        return "";
    case 1:
        return "Designacao";
    case 2:
        return "Data/Duracao";
    case 3:
        return "Prioridade";
    default:
        return 0;
    }
}

void EcraListaAmigos::executarAccao ( Accao* accao ) {
    switch ( accao->getLetra() ) {
    case 'V':
        EditorAmizade ( this ).mostrar();
        break;
    case 'X':
        if(getBaseDados()->eliminarAmizade ( *(mAmigo->getAmigo()), *getUtilizador() ) == 1) fechar();
        break;
    case '0':
        fechar();
        break;
    }
}
#endif


