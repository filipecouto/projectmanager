#ifndef FORMCONTEXTO_H
#define FORMCONTEXTO_H

#include "EcraLista.h"
#include "Comunicacao.h"
#include "EditorContexto.h"
#include "EditorTarefa.h"
#include "EcraTarefa.h"

class EcraContexto : public EcraLista, public Comunicacao {
    public:
    EcraContexto ( Comunicacao* comunicacao, Contexto* contexto );
    virtual void executarAccao ( Accao* accao );

    private:
    virtual float getLarguraColuna ( int coluna );
    virtual int getNumColunas();
    virtual int getNumLinhas();
    virtual string getNomeColuna ( int coluna );
    virtual void imprimirCelula ( int coluna, int linha );
    virtual void itemSelecionado ( int linha );

    inline void imprimirPrioridade ( int prioridade );

    Contexto* mContexto;
    vector<Tarefa*> mTarefas;

    bool mMostrarCompletos, mMostrarDelegadas;

    inline void carregarTarefas();
};

EcraContexto::EcraContexto ( Comunicacao* comunicacao, Contexto* contexto ) : EcraLista ( ), mContexto ( contexto ), mMostrarCompletos ( false ), mMostrarDelegadas( true ), Comunicacao ( comunicacao ) {
    setTitulo ( "Tarefas do Contexto \"" + mContexto->getDesignacao() + '\"' );

    juntarAccao ( new Accao ( "Nova tarefa", 'N' ) );
    juntarAccao ( new Accao ( "Editar", 'E' ) );
    juntarAccao ( new Accao ( "Exportar", '*' ) );
    juntarAccao ( new Accao ( "Eliminar", 'X' ) );
    juntarAccao ( new Accao ( "Filtros", 'F' ) );
    juntarAccao ( new Accao ( "Voltar", '0' ) );

    carregarTarefas();
}

void EcraContexto::carregarTarefas() {
    mTarefas.clear();
    mTarefas = getBaseDados()->getTarefas ( *getUtilizador(), *mContexto, ( mMostrarCompletos? (mMostrarDelegadas ? "" : "delegacao = null") : (mMostrarDelegadas ? "progresso != 100" : "delegacao = null and progresso != 100") ), "dataCriacao desc" );
}

void EcraContexto::itemSelecionado ( int linha ) {
    EcraTarefa ( this, mTarefas[linha] ).mostrar();
    carregarTarefas();
}

void EcraContexto::imprimirCelula ( int coluna, int linha ) {
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

void EcraContexto::imprimirPrioridade ( int prioridade ) {
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

int EcraContexto::getNumLinhas() {
    return mTarefas.size();
}

int EcraContexto::getNumColunas() {
    return 4;
}

float EcraContexto::getLarguraColuna ( int coluna ) {
    switch ( coluna ) {
    case 0:
        return 0.04f;
    case 1:
        return 0.55f;
    case 2:
        return 0.22f;
    case 3:
        return 0.19f;
    default:
        return 0;
    }
}

string EcraContexto::getNomeColuna ( int coluna ) {
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

void EcraContexto::executarAccao ( Accao* accao ) {
    switch ( accao->getLetra() ) {
    case 'N':
        EditorTarefa ( this , 1 ).mostrar();
        carregarTarefas();
        break;
    case 'E':
        EditorContexto ( this, mContexto ).mostrar();
        setTitulo ( "Tarefas do Contexto \"" + mContexto->getDesignacao() + '\"' );
        break;
    case 'F': {
        vector<string> opcoes;
        if ( mMostrarCompletos ) opcoes.push_back ( "Ocultar tarefas completas" );
        else opcoes.push_back ( "Mostrar tarefas completas" );
//         if ( mMostrarDelegadas ) opcoes.push_back ( "Ocultar tarefas delegadas" );
//         else opcoes.push_back ( "Mostrar tarefas delegadas" );
        CampoLista menu ( "", 0, &opcoes, 1 );
        menu.campoSeleccionado ( this );
        if ( menu.getSeleccao()->size() != 0 ) {
            switch ( ( *menu.getSeleccao() ) [0] ) {
            case 0:
                mMostrarCompletos = !mMostrarCompletos;
                carregarTarefas();
                break;
            case 1:
                mMostrarDelegadas = !mMostrarDelegadas;
                carregarTarefas();
                break;
            }
        }
    }
    break;
    case 'X': {
        vector<string> opcoes;
        opcoes.push_back ( "Eliminar contexto e tarefas associadas apenas a este" );
        opcoes.push_back ( "Cancelar" );
        CampoLista menu ( "", 0, &opcoes, 1 );
        menu.campoSeleccionado ( this );
        if ( menu.getSeleccao()->size() != 0 ) {
            getBaseDados()->eliminarContexto ( *mContexto , *getUtilizador() );
            fechar();
        }
    }
    break;
    case '*':
        exportarCsv ( mTarefas, "teste.csv" );
        break;
    case '0':
        fechar();
        break;
    }
}

#endif
