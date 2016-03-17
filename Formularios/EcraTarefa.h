#ifndef FORMTAREFAS_H
#define FORMTAREFAS_H

#include <ctime>

#include "EcraLista.h"
#include "Comunicacao.h"
#include "EditorTarefa.h"
#include "CampoLista.h"
#include "EcraBase.h"
#include "../Dados/Projecto.h"
#include "../Dados/Delegacao.h"

class EcraTarefa : public EcraLista, public Comunicacao {
    public:
    EcraTarefa ( Comunicacao* comunicacao, Referencia* referencia );
    virtual void executarAccao ( Accao* accao );

    private:
    void concluirTarefa();
    void eliminarTarefa ( Tarefa* t );

    virtual float getLarguraColuna ( int coluna );
    virtual int getNumColunas();
    virtual int getNumLinhas();
    virtual string getNomeColuna ( int coluna );
    virtual void imprimirCelula ( int coluna, int linha );
    virtual void itemSelecionado ( int linha );

    inline void imprimirPrioridade ( int prioridade );

    virtual void imprimirCabecalho();

    Referencia *mReferencia;
};

EcraTarefa::EcraTarefa ( Comunicacao* comunicacao, Referencia* referencia ) : EcraLista ( ), mReferencia ( referencia ), Comunicacao ( comunicacao ) {
    setTitulo ( mReferencia->getDesignacao() + " (criado no dia " + mReferencia->getDataCriacao().listarstr() + ')' );

    getBaseDados()->completarDadosTarefa ( *getUtilizador(), mReferencia );

    setMargemTopo ( 8 );

    if ( typeid ( *mReferencia ) == typeid ( Projecto ) ) {
        juntarAccao ( new Accao ( "Editar", 'E' ) );
        juntarAccao ( new Accao ( "Delegar", 'D' ) );
        if ( ( ( Tarefa* ) mReferencia )->getProgresso() != 100 )
            juntarAccao ( new Accao ( "Concluir", 'C' ) );
        juntarAccao ( new Accao ( "Eliminar", 'X' ) );
        juntarAccaoOculta ( new Accao ( "Alterar progresso", 'P' ) );
        juntarAccaoOculta ( new Accao ( "Gerir tarefas", '.' ) );
    } else if ( typeid ( *mReferencia ) == typeid ( Tarefa ) ) {
        juntarAccao ( new Accao ( "Editar", 'E' ) );
        juntarAccao ( new Accao ( "Delegar", 'D' ) );
        if ( ( ( Tarefa* ) mReferencia )->getProgresso() != 100 )
            juntarAccao ( new Accao ( "Concluir", 'C' ) );
        juntarAccao ( new Accao ( "Eliminar", 'X' ) );
        juntarAccaoOculta ( new Accao ( "Alterar progresso", 'P' ) );
        juntarAccaoOculta ( new Accao ( "Adicionar tarefa", '+' ) );
        juntarAccaoOculta ( new Accao ( "Remover tarefa", '-' ) );
    } else {
        juntarAccao ( new Accao ( "Editar referencia", 'E' ) );
        juntarAccao ( new Accao ( "Eliminar referencia", 'X' ) );
    }
    juntarAccao ( new Accao ( "Converter", '*' ) );
    juntarAccao ( new Accao ( "Voltar", '0' ) );
}

void EcraTarefa::imprimirCabecalho() {
    int len = mReferencia->getContextos().size();
    reporFormatacao();
    if ( mReferencia->getDescricao().length() != 0 )
        cout << mReferencia->getDescricao() << endl << "   ";
    for ( int i = 0; i < len; i++ )
        cout << mReferencia->getContextos() [i]->getDesignacao() << ' '; // TODO atalho para o contexto
    cout << endl << endl;
    if ( typeid ( *mReferencia ) == typeid ( Tarefa ) || typeid ( *mReferencia ) == typeid ( Projecto ) ) {
        Tarefa *t = ( Tarefa* ) mReferencia;
        if ( t->getDelegacao() ) {
            setCor ( t->getDelegacao()->getAceite() ? 2 : 3 );
            Utilizador* u = t->getDelegacao()->getUtilizador();
            if ( u ) {
                cout << "   Delegado com " << u->getNome() << " " << u->getApelido();
                if ( !t->getDelegacao()->getAceite() ) cout << " (ainda não aceite)";
            } else
                cout << "   Tarefa delegada";
            cout << endl;
        }
        reporFormatacao();
        setNegrito();
        cout << "   Progresso ";
        reporFormatacao();
        cout << t->getProgresso() << "%\t\t";
        setNegrito();
        cout << "P ";
        reporFormatacao();
        cout << " alterar progresso" << endl;
        setNegrito();
        cout << "   Data ";
        reporFormatacao();
        if ( t->getDataInicio().getValido() ) {
            cout << t->getDataInicio().listarstr();
            if ( t->getDataFim().getValido() )
                cout << " até " << t->getDataFim().listarstr();
        } else if ( t->getProgresso() == 100 && t->getDataFim().getValido() ) {
            cout << "concluida em " << t->getDataFim().listarstr();
        } else cout << "não especificada";
        cout << endl;
        setNegrito();
        cout << "   Duracao ";
        if ( t->getProgresso() != 100 ) cout << "prevista ";
        reporFormatacao();
        int d = t->getDuracaoPrevista();
        switch ( Data::simplificarTempoDeSegundos ( d ) ) {
        case 1:
            cout << d << " segundos";
            break;
        case 2:
            cout << d << " minutos";
            break;
        case 3:
            cout << d << " horas";
            break;
        case 4:
            cout << d << " dias";
            break;
        }
        cout << endl;
        setNegrito();
        cout << "   Prioridade ";
        reporFormatacao();
        imprimirPrioridade ( t->getPrioridade() );
        cout << endl;
        //if ( t->getDependenteDe() )
        //    cout << "   A tarefa \"" << t->getDependenteDe()->getDesignacao() << "\" depende desta" << endl;
        // TODO apresentar de que tarefas esta depende (se existir)
        if ( typeid ( *mReferencia ) == typeid ( Projecto ) ) {
            setNegrito();
            cout << "   .";
            reporFormatacao();
            if ( getModoEdicao() == 1 && getAlvoEdicao() && ( ( Projecto* ) getAlvoEdicao() )->getIdProjecto() == ( ( Projecto* ) mReferencia )->getIdProjecto() ) cout << " Terminar adição/remoção de tarefas";
            else cout << " Adicionar/remover tarefas";
        } else if ( getModoEdicao() == 1 && typeid ( *mReferencia ) == typeid ( Tarefa ) ) {
            if ( ! ( ( Projecto* ) getAlvoEdicao() )->hasTarefa ( ( Tarefa* ) mReferencia ) ) {
                setNegrito();
                cout << "   +";
                reporFormatacao();
                cout << " Adicionar tarefa ao projecto";
            } else {
                setNegrito();
                cout << "   -";
                reporFormatacao();
                cout << " Remover tarefa do projecto";
            }
        }
    }
    if ( mReferencia->getDocumentoAssociado().length() != 0 ) {
        setNegrito();
        cout << "   Ficheiros associados: ";
        reporFormatacao();
        cout << mReferencia->getDocumentoAssociado(); // TODO permitir abrir o ficheiro e permitir separar por virgulas
    }
}

void EcraTarefa::itemSelecionado ( int linha ) {
    EcraTarefa ( this, ( ( Projecto* ) mReferencia )->getTarefas() [linha] ).mostrar();
    getBaseDados()->completarDadosTarefa ( *getUtilizador(), mReferencia );
}

void EcraTarefa::imprimirCelula ( int coluna, int linha ) {
    Tarefa* t = ( ( Projecto* ) mReferencia )->getTarefas() [linha];
    switch ( coluna ) {
    case 0:
        if ( t->getProgresso() == 100 )
            cout << ( char ) 226 << ( char ) 156 << ( char ) 147 << ' ';
        if ( t->getDelegacao() )
            cout << ( char ) 0xE2 << ( char ) 0x9E << ( char ) 0x9F;
        break;
    case 1:
        cout << t->getDesignacao();
        break;
    case 2: {
        Data di = t->getDataInicio();
        Data df = t->getDataFim();
        if ( df.getValido() )
            cout << di.getDia() << '/' << di.getMes() << " ~ " << df.getDia() << '/' << df.getMes();
        else if ( di.getValido() ) {
            int te = t->getDuracaoPrevista();
            switch ( Data::simplificarTempoDeSegundos ( te ) ) {
            case 1:
                cout << di.getDia() << '/' << di.getMes() << " (" << te << " seg)";
                break;
            case 2:
                cout << di.getDia() << '/' << di.getMes() << " (" << te << " min)";
                break;
            case 3:
                cout << di.getDia() << '/' << di.getMes() << " (" << te << " h)";
                break;
            case 4:
                cout << di.getDia() << '/' << di.getMes() << " (" << te << " dia(s))";
                break;
            }
        } else {
            int te = t->getDuracaoPrevista();
            switch ( Data::simplificarTempoDeSegundos ( te ) ) {
            case 1:
                cout << te << " segundos";
                break;
            case 2:
                cout << te << " minutos";
                break;
            case 3:
                cout << te << " horas";
                break;
            case 4:
                cout << te << " dias";
                break;
            }
        }
        break;
    }
    case 3:
        imprimirPrioridade ( t->getPrioridade() );
        break;
    }
}

void EcraTarefa::imprimirPrioridade ( int prioridade ) {
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

int EcraTarefa::getNumLinhas() {
    return ( typeid ( *mReferencia ) == typeid ( Projecto ) ? ( ( Projecto* ) mReferencia )->getTarefas().size() : 0 );
}

int EcraTarefa::getNumColunas() {
    return ( typeid ( *mReferencia ) == typeid ( Projecto ) ? 4 : 0 );
}

float EcraTarefa::getLarguraColuna ( int coluna ) {
    switch ( coluna ) {
    case 0:
        return 0.05f;
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

string EcraTarefa::getNomeColuna ( int coluna ) {
    switch ( coluna ) {
    case 0:
        return "";
    case 1:
        return "Designacao";
    case 2:
        return "Data";
    case 3:
        return "Prioridade";
    default:
        return 0;
    }
}

void EcraTarefa::concluirTarefa() {
    vector<string> opcoes;
    opcoes.push_back ( "A tarefa foi concluida agora" );
    Data possivelFim = ( ( Tarefa* ) mReferencia )->getDataInicio();
    if ( possivelFim.getValido() ) {
        possivelFim.somarXsegundos ( ( ( Tarefa* ) mReferencia )->getDuracaoPrevista() );
        opcoes.push_back ( "A tarefa foi concluida em " + possivelFim.listarstr() );
    }
    opcoes.push_back ( "Noutra altura" );
    opcoes.push_back ( "Não sei" );
    CampoLista menu ( "", 0, &opcoes, 1 );
    menu.campoSeleccionado ( this );
    if ( menu.getSeleccao()->size() != 0 ) {
        int s = ( *menu.getSeleccao() ) [0];
        if ( s == 0 ) {
            time_t t = time ( 0 );
            struct tm * now = localtime ( & t );
            ( ( Tarefa* ) mReferencia )->setDataFim ( Data ( now->tm_year + 1900, now->tm_mon + 1, now->tm_mday, now->tm_hour, now->tm_min, now->tm_sec ) );
        } else if ( possivelFim.getValido() && s == 1 ) {
            ( ( Tarefa* ) mReferencia )->setDataFim ( possivelFim );
        } else if ( ( possivelFim.getValido() && s == 2 ) || s == 1 ) {
            string data;
            Tamanho t;
            do {
                reporFormatacao();
                getTamanho ( t );
                setPosicao ( 0, t.altura - 2 );
                for ( int i = 0; i < t.largura - 1; i++ ) cout << ' ';
                setPosicao ( 0, t.altura - 2 );
                setNegrito();
                cout << "Data de finalização da tarefa:";
                reporFormatacao();
                CampoString campo ( "", 0, &data );
                campo.campoSeleccionado ( this );
            } while ( Data ( data ) < ( ( Tarefa* ) mReferencia )->getDataInicio() );
            ( ( Tarefa* ) mReferencia )->setDataFim ( Data ( data ) );
        } else {
            ( ( Tarefa* ) mReferencia )->setDataFim ( Data () );
        }
        ( ( Tarefa* ) mReferencia )->setDuracaoPrevista(( ( Tarefa* ) mReferencia )->getDataFim().substrair(( ( Tarefa* ) mReferencia )->getDataFim()));
        ( ( Tarefa* ) mReferencia )->setProgresso ( 100 );
        getBaseDados()->editarTarefa ( mReferencia );
    }
}

void EcraTarefa::eliminarTarefa ( Tarefa* t ) {
    Referencia* r = t;
    getBaseDados()->completarDadosTarefa ( *getUtilizador(), r );
    if ( t->getDependenteDe() ) eliminarTarefa ( t->getDependenteDe() ); // TODO por alguma razao não está a apagar os outros...
    getBaseDados()->eliminarTarefa ( mReferencia );
}

void EcraTarefa::executarAccao ( Accao* accao ) {
    switch ( accao->getLetra() ) {
    case 'E':
        EditorTarefa ( this, mReferencia ).mostrar();
        setTitulo ( mReferencia->getDesignacao() + " (criado no dia " + mReferencia->getDataCriacao().listarstr() + ')' );
        getBaseDados()->completarDadosTarefa ( *getUtilizador(), mReferencia );
        break;
    case 'X':
        if ( typeid ( Referencia ) == typeid ( *mReferencia ) ) {
            getBaseDados()->eliminarTarefa ( mReferencia );
            fechar();
        } else {
            vector<string> opcoes;
            if ( ( ( Tarefa* ) mReferencia )->getDependenteDe() ) opcoes.push_back ( "Eliminar incluindo as tarefas que dependem desta" );
            else opcoes.push_back ( "Eliminar" );
            opcoes.push_back ( "Cancelar" );
            CampoLista menu ( "", 0, &opcoes, 1 );
            menu.campoSeleccionado ( this );
            if ( menu.getSeleccao()->size() != 0 ) {
                if ( ( *menu.getSeleccao() ) [0] == 0 ) {
                    eliminarTarefa ( ( Tarefa* ) mReferencia );
                    fechar();
                }
            }
        }
        break;
    case 'D': {
        vector<string> opcoes;
        vector<Amizade*> amigos = getBaseDados()->getAmigos ( *getUtilizador() );
        int len = amigos.size();
        for ( int i = 0; i < len; i++ )
            opcoes.push_back ( amigos[i]->getAmigo()->getNome() + " " + amigos[i]->getAmigo()->getApelido() );
        opcoes.push_back ( "Outra pessoa" );
        if ( ( ( Tarefa* ) mReferencia )->getDelegacao() ) opcoes.push_back ( "Remover delegação" );
        CampoLista menu ( "", 0, &opcoes, 1 );
        menu.campoSeleccionado ( this );
        if ( menu.getSeleccao()->size() != 0 ) {
            int opcao = ( *menu.getSeleccao() ) [0];
            if ( opcao < len ) ( ( Tarefa* ) mReferencia )->setDelegacao ( new Delegacao ( false, amigos[opcao]->getAmigo() ) );
            else if ( opcao == len ) ( ( Tarefa* ) mReferencia )->setDelegacao ( new Delegacao ( ) );
            else ( ( Tarefa* ) mReferencia )->setDelegacao ( NULL );
            getBaseDados()->definirDelegacao ( *getUtilizador(), ( ( Tarefa* ) mReferencia ) );
        }
    }
    break;
    case 'C':
        concluirTarefa();
        break;
    case 'P': {
        setPosicao ( 12, 4 );
        cout << "     ";
        setPosicao ( 12, 4 );
        int p;
        CampoInt progresso ( "", 0, &p );
        progresso.campoSeleccionado ( this );
        if ( p != ( ( Tarefa* ) mReferencia )->getProgresso() ) {
            if ( p == 100 ) concluirTarefa();
            else {
                ( ( Tarefa* ) mReferencia )->setProgresso ( p );
                getBaseDados()->editarTarefa ( mReferencia );
            }
        }
    }
    break;
    case '*': {
        vector<string> opcoes;
        if ( typeid ( *mReferencia ) != typeid ( Referencia ) )
            opcoes.push_back ( "Para referencia" );
        if ( typeid ( *mReferencia ) != typeid ( Tarefa ) )
            opcoes.push_back ( "Para tarefa" );
        if ( typeid ( *mReferencia ) != typeid ( Projecto ) )
            opcoes.push_back ( "Para projecto" );
        if ( typeid ( *mReferencia ) == typeid ( Tarefa ) )
            opcoes.push_back ( "Deferir (exportar em iCal)" );
        CampoLista menu ( "", 0, &opcoes, 1 );
        menu.campoSeleccionado ( this );
        if ( menu.getSeleccao()->size() != 0 ) {
            int opcao = ( *menu.getSeleccao() ) [0];
            switch ( opcao ) {
            case 0: {
                if ( typeid ( *mReferencia ) != typeid ( Referencia ) ) { // tarefas e projectos serão convertidos para referencias
                    if ( typeid ( *mReferencia ) == typeid ( Projecto ) ) getBaseDados()->despromoverProjecto ( ( Projecto* ) mReferencia, 0 );
                    else if ( typeid ( *mReferencia ) == typeid ( Tarefa ) ) getBaseDados()->despromoverTarefa ( ( Tarefa* ) mReferencia, 0 );
                    Referencia r ( *mReferencia );
                    EcraTarefa ( this, &r ).mostrar();
                } else {
                    getBaseDados()->promoverReferencia ( *getUtilizador(), mReferencia, 1 );
                    EcraTarefa ( this, mReferencia ).mostrar();
                }
                fechar();
            }
            break;
            case 1:
                if ( typeid ( *mReferencia ) == typeid ( Projecto ) ) { // projectos serão convertidos para tarefas
                    Projecto* p = ( Projecto* ) mReferencia;
                    getBaseDados()->despromoverProjecto ( p, 1 );
                    EcraTarefa ( this, p ).mostrar();
                } else {
                    if ( typeid ( *mReferencia ) == typeid ( Tarefa ) ) {
                        Tarefa* t = ( Tarefa* ) mReferencia;
                        getBaseDados()->promoverTarefa ( *getUtilizador (), t, 1 );
                        EcraTarefa ( this, t ).mostrar();
                    }
                }
                EcraTarefa ( this, mReferencia ).mostrar();
                fechar();
                break;
            case 2:
                exportarICal ( "teste", ( Tarefa* ) mReferencia );
                break;
            }
        }
    }
    break;
    case '+':
        if ( getModoEdicao() == 1 && getAlvoEdicao() ) {
            if ( ! ( ( Projecto* ) getAlvoEdicao() )->hasTarefa ( ( Tarefa* ) mReferencia ) ) {
                ( ( Projecto* ) getAlvoEdicao() )->addTarefa ( ( Tarefa* ) mReferencia );
                getBaseDados()->actualizarTarefasProjecto ( ( Projecto* ) getAlvoEdicao() );
            }
        }
        break;
    case '-':
        if ( getModoEdicao() == 1 && getAlvoEdicao() ) {
            if ( ( ( Projecto* ) getAlvoEdicao() )->hasTarefa ( ( Tarefa* ) mReferencia ) ) {
                ( ( Projecto* ) getAlvoEdicao() )->removeTarefa ( ( Tarefa* ) mReferencia );
                getBaseDados()->actualizarTarefasProjecto ( ( Projecto* ) getAlvoEdicao() );
            }
        }
        break;
    case '.':
        if ( getModoEdicao() == 1 && getAlvoEdicao() && ( ( Projecto* ) getAlvoEdicao() )->getIdProjecto() == ( ( Projecto* ) mReferencia )->getIdProjecto() ) setModoEdicao ( NULL, 0 );
        else setModoEdicao ( mReferencia, 1 );
        break;
    case '0':
        fechar();
        break;
    }
}

#endif
