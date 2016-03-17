#ifndef EDITORTAREFA_H
#define EDITORTAREFA_H

#include "Formulario.h"
#include "../Dados/Tarefa.h"
#include "../Dados/Contexto.h"
#include "../Dados/Projecto.h"
#include "../Dados/Referencia.h"
#include "CampoString.h"
#include "CampoInt.h"
#include "CampoPalavraPasse.h"
#include "../BaseDados/BDados.h"
#include <time.h>
#include "Comunicacao.h"
#include "CampoLista.h"
#include <iostream>
using namespace std;

class EditorTarefa : public Formulario, public Comunicacao {
    public:
    EditorTarefa ( Comunicacao* contexto , int tipo );
    EditorTarefa ( Comunicacao* contexto, Referencia * referencia );
    private:
    long mId;
    long mIdProjecto;
    long mIdReferencia;
    int mTipo;
    string mTitulo;
    string mDesignacao;
    int mDuracaoPrevista;
    vector<Contexto*> mContextos;
    vector<Contexto*> mContextosSeleccionados;
    vector<string> mNomeContextos;
    Data mDataInicio;
    Data mDataFim;
    Data mDataCriacao;
    string mDataFimString;
    string mDataInicioString;
    string mDocumento;
    string mDescricao;
    Referencia *mReferencia;
    Tarefa * mTarefa;
    Projecto * mProjecto;
    int mPrioridade;
    int mVisibilidade;
    int mProgresso;
    void criarTarefa ();
    void editarTarefa ();
    void cancelar ();
    void executarAccao ( Accao* accao );
    virtual void campoAlterado ( Campo* campo );
    void carregarNomes();
};

EditorTarefa::EditorTarefa ( Comunicacao* contexto , int tipo ) : Formulario ( "" ),  Comunicacao ( contexto ) {
    /*
    	 1 - Tarefa
    	 2 - Referencia
    	 3 - Projecto
    */
    mPrioridade = 50;
    mDuracaoPrevista = 0;
    switch ( tipo ) {
    case 1:
        mTitulo = "Criar Tarefa";
        mTipo = 1;
        break;
    case 2:
        mTitulo = "Criar Referencia";
        mTipo = 2;
        break;
    case 3:
        mTitulo = "Criar Projecto";
        mTipo = 3;
        tipo = 1;
        break;
    }
    setTitulo ( mTitulo );
    vector<string> * a = new vector<string>;
    vector<string> * tempo = new vector<string>;
    vector<int>*standardv = new vector<int>;
    vector<int>*standardf= new vector<int>;
    juntarCampo ( new CampoString ( "Designação" , 'D' , &mDesignacao ) );
    juntarCampo ( new CampoString ((tipo == 2 ? "Descrição" : "Notas"), 'Q' , &mDescricao ) );
    mContextos = getBaseDados()->getContextos ( *getUtilizador() );
    carregarNomes();
    juntarCampo ( new CampoLista ( "Contextos" , 'C' , &mNomeContextos, 2 ) );
    switch ( tipo ) {
    case 1:

        juntarCampo ( new CampoString ( "Data de início" , 'I' , &mDataInicioString ) );

        juntarEspaco();
        juntarCampo ( new CampoInt ( "Duracao prevista", 'T' , &mDuracaoPrevista ) );

        tempo->push_back ( "Segundos" );
        tempo->push_back ( "Minutos" );
        tempo->push_back ( "Horas" );
        tempo->push_back ( "Dias" );

        juntarCampo ( new CampoLista ( "Formato de Tempo" , 'F' , tempo , 1 ) );

        standardf->push_back ( 2 );
        ( ( CampoLista* ) getCampo ( 'F' ) )->setSeleccao ( standardf );
        juntarEspaco();

        juntarCampo ( new CampoInt ( "Prioridade" , 'P' , &mPrioridade ) );

        a->push_back ( "Publico" );
        a->push_back ( "Semi-Publico" );
        a->push_back ( "Privado" );
        juntarCampo ( new CampoLista ( "Visibilidade" , 'V' , a , 1 ) );

        standardv->push_back ( 2 );
        ( ( CampoLista* ) getCampo ( 'V' ) )->setSeleccao ( standardv );
        juntarEspaco();

        juntarAccao ( new Accao ( mTitulo  , '1' ) );
        juntarAccao ( new Accao ( "Cancelar", '0' ) );
        break;
    case 2:
        juntarCampo ( new CampoString ( "Documento Associado" , 'A' , &mDocumento ) );
        juntarAccao ( new Accao ( mTitulo  , '1' ) );
        juntarAccao ( new Accao ( "Cancelar", '0' ) );
        break;
    }
}

EditorTarefa::EditorTarefa ( Comunicacao* contexto, Referencia *referencia ) : Formulario ( "" ), Comunicacao ( contexto ) {
    int tipo;
    mReferencia = referencia;
    getBaseDados()->completarDadosTarefa ( *getUtilizador() , mReferencia );
    if ( typeid ( Tarefa ) == typeid ( *referencia ) ) {
        mTarefa = ( Tarefa* ) referencia;
        tipo = 1;
        if ( typeid ( Projecto ) == typeid ( *mTarefa ) ) {
            tipo = 3;
            mIdProjecto = ( ( Projecto* ) mTarefa )->getIdProjecto();
        }
    } else {
        tipo = 2;
    }
    mTipo = tipo;
    switch ( tipo ) {
    case 1:
        mTitulo = "Editar Tarefa";
        break;
    case 2:
        mTitulo = "Editar Referencia";
        break;
    case 3:
        mTitulo = "Editar Projecto";
        break;
    }
    setTitulo ( mTitulo );
    vector<string>* tempo = new vector<string>;
    if ( tipo == 1 || tipo == 3 ) {
        mDescricao = mTarefa->getDescricao();
        mDataCriacao = mTarefa->getDataCriacao();
        mDesignacao = mTarefa->getDesignacao();
        mDataInicio = mTarefa->getDataInicio();
        mDataFim = mTarefa->getDataFim();
        mPrioridade = mTarefa->getPrioridade();
        mVisibilidade = mTarefa->getVisibilidade();
        mId = mTarefa->getId();
        mProgresso = mTarefa->getProgresso();
        mIdReferencia = mTarefa->getIdReferencia();
        juntarCampo ( new CampoString ( "Designação" , 'D' , &mDesignacao ) );
        juntarCampo ( new CampoString ( "Notas" , 'Q', &mDescricao ) );
        mContextos = getBaseDados()->getContextos ( *getUtilizador() );
        mContextosSeleccionados = mTarefa->getContextos();
        carregarNomes();
        juntarCampo ( new CampoLista ( "Contextos" , 'C' , &mNomeContextos, 2 ) );
        vector<int> * campos = new vector<int>;
        for ( int i = 0 ; i < mContextos.size() ; i++ ) {
            for ( int j = 0 ; j < mContextosSeleccionados.size() ; j++ ) {
                if ( mContextos.at ( i )->getId() == mContextosSeleccionados.at ( j )->getId() ) {
                    campos->push_back ( i );
                }
            }
        }
        ( ( CampoLista* ) getCampo ( 'C' ) )->setSeleccao ( campos );
        mDataInicioString = mTarefa->getDataInicio().listarstr();
        juntarCampo ( new CampoString ( "Data de início" , 'I' , &mDataInicioString ) );
        mDataFimString = mTarefa->getDataFim().listarstr();
        juntarCampo ( new CampoString ( "Data de Fim" , 'J' , &mDataFimString ) );
        getCampo ( 'J' )->setComentario ( "opcional" );
        juntarEspaco();

        mDuracaoPrevista = mTarefa->getDuracaoPrevista();

        tempo->push_back ( "Segundos" );
        tempo->push_back ( "Minutos" );
        tempo->push_back ( "Horas" );
        tempo->push_back ( "Dias" );

        int f = Data::simplificarTempoDeSegundos ( mDuracaoPrevista ) - 1;

        juntarCampo ( new CampoInt ( "Duracao Prevista" , 'T' , &mDuracaoPrevista ) );
        juntarCampo ( new CampoLista ( "Formato de Tempo" , 'F' , tempo, 1 ) );

        vector<int> * formatos = new vector<int>;
        formatos->push_back ( f );

        ( ( CampoLista* ) getCampo ( 'F' ) )->setSeleccao ( formatos );

        juntarEspaco();

        juntarCampo ( new CampoInt ( "Prioridade" , 'P' , &mPrioridade ) );
        vector<string> * a = new vector<string>;
        a->push_back ( "Publico" );
        a->push_back ( "Semi-Publico" );
        a->push_back ( "Privado" );
        juntarCampo ( new CampoLista ( "Visibilidade" , 'V' , a , 1 ) );

        vector<int> * visib = new vector<int>;
        visib->push_back ( mVisibilidade );
        ( ( CampoLista* ) getCampo ( 'V' ) )->setSeleccao ( visib );

        juntarEspaco();

        juntarAccao ( new Accao ( mTitulo , 'E' ) );
        juntarAccao ( new Accao ( "Cancelar", '0' ) );

    } else if ( tipo == 2 ) {
        mDescricao = referencia->getDescricao();
        mDocumento = referencia->getDocumentoAssociado();
        mContextosSeleccionados = referencia->getContextos();
        mDesignacao = referencia->getDesignacao();
        mId = referencia->getIdReferencia();
        mDataCriacao = referencia->getDataCriacao();

        juntarCampo ( new CampoString ( "Designacao" , 'D' , &mDesignacao ) );
        juntarCampo ( new CampoString ( "Descrição" , 'Q', &mDescricao ) );
        mContextos = getBaseDados()->getContextos ( *getUtilizador() );
        carregarNomes();
        juntarCampo ( new CampoLista ( "Contextos", 'C' , &mNomeContextos, 2 ) );
        vector<int> * campos = new vector<int>;
        for ( int i = 0 ; i < mContextos.size() ; i++ ) {
            for ( int j = 0 ; j < mContextosSeleccionados.size() ; j++ ) {
                if ( mContextos.at ( i )->getId() == mContextosSeleccionados.at ( j )->getId() ) {
                    campos->push_back ( i );
                }
            }
        }
        ( ( CampoLista* ) getCampo ( 'C' ) )->setSeleccao ( campos );
        juntarCampo ( new CampoString ( "Documento Associado" , 'A' , &mDocumento ) );
        juntarAccao ( new Accao ( mTitulo  , '1' ) );
        juntarAccao ( new Accao ( "Cancelar", '0' ) );
    }
}

void EditorTarefa::campoAlterado ( Campo* campo ) {
    if ( campo->getLetra() == 'D' && mTipo != 2 ) {
        int valor = getBaseDados()->sugestaoDuracao ( mDesignacao, *getUtilizador() );
        vector<int> *formato = new vector<int>;
        formato->push_back ( Data::simplificarTempoDeSegundos ( valor ) - 1 );
        ( ( CampoLista* ) getCampo ( 'F' ) )->setSeleccao ( formato );
        if ( valor != 0 ) {
            ( ( CampoInt* ) getCampo ( 'T' ) )->setValor ( valor );
        }
    }
}

void EditorTarefa::carregarNomes() {
    for ( int i = 0 ; i < mContextos.size(); i++ ) {
        mNomeContextos.push_back ( mContextos.at ( i )->getDesignacao() );
    }
}

void EditorTarefa::executarAccao ( Accao* accao ) {
    switch ( accao->getLetra() ) {
    case '1':
        criarTarefa();
        return;
    case 'E':
        editarTarefa();
        return;
    case '0':
        fechar();
        return;
    }
}

/* Para criar a tarefa:
		1 - Criar o objecto de Tarefa.
		2 - Enviar os dados do objecto para a base de dados e o ID que for atribuido a essa tarefa na base de dados vai ficar o ID no programa.
*/
void EditorTarefa::criarTarefa () {
    if ( mTipo == 1 || mTipo == 3 ) {
        getCampo ( 'D' )->setComentario ( "" );
        getCampo ( 'I' )->setComentario ( "" );
        getCampo ( 'F' )->setComentario ( "" );
        getCampo ( 'C' )->setComentario ( "" );
        getCampo ( 'P' )->setComentario ( "" );
        getCampo ( 'V' )->setComentario ( "" );
        vector<int> * a = ( ( CampoLista* ) getCampo ( 'C' ) )->getSeleccao();
        mContextosSeleccionados.clear();
        for ( int i = 0 ; i < a->size(); i++ ) {
            mContextosSeleccionados.push_back ( mContextos.at ( a->at ( i ) ) );
        }
        mVisibilidade = ( ( CampoLista* ) getCampo ( 'V' ) )->getSeleccao()->at ( 0 );

        int formato = ( ( CampoLista* ) getCampo ( 'F' ) )->getSeleccao()->at ( 0 );
        switch ( formato ) {
        case 0:
            break; //btw tb deixei lá segundos apenas para evitar um espertinho que tente fazer asneiras
        case 1:
            mDuracaoPrevista = mDuracaoPrevista*60;
            break;
        case 2:
            mDuracaoPrevista = mDuracaoPrevista*3600;
            break;
        case 3:
            mDuracaoPrevista = mDuracaoPrevista*86400;
            break;
        }
        if ( mTipo == 1 ) {
            Tarefa *novaTarefa = new Tarefa ( mDesignacao, mDataInicioString, mDataFimString, mDuracaoPrevista, 0, mPrioridade, mVisibilidade, NULL,NULL, mContextosSeleccionados, "" );
            novaTarefa->setDescricao ( mDescricao );
            switch ( getBaseDados()->criarTarefa ( novaTarefa, *getUtilizador() ) ) {
            case -2:
                getCampo ( 'D' )->setComentario ( "Designação invalida" );
                break;
            case -3:
                getCampo ( 'I' )->setComentario ( "Data de ínicio invalida" );
                break;
            case -4:
                getCampo ( 'F' )->setComentario ( "Data de fim invalida" );
                break;
            case -5:
                getCampo ( 'C' )->setComentario ( "Contextos invalidos" );
                break;
            case -6:
                getCampo ( 'P' )->setComentario ( "Prioridade invalida" );
                break;
            case -7:
                getCampo ( 'V' )->setComentario ( "Visibilidade invalida" );
                break;
            case 1:
                fechar();
                break;
            }
        } else {
            Projecto *novoProjecto = new Projecto ( mDesignacao, mDataInicioString, mDataFimString, mDuracaoPrevista, 0, mPrioridade, mVisibilidade, NULL,NULL, mContextosSeleccionados, "" );
            novoProjecto->setDescricao ( mDescricao );
            switch ( getBaseDados()->criarTarefa ( novoProjecto, *getUtilizador() ) ) {
            case -2:
                getCampo ( 'D' )->setComentario ( "Designação invalida" );
                break;
            case -3:
                getCampo ( 'I' )->setComentario ( "Data de ínicio invalida" );
                break;
            case -4:
                getCampo ( 'F' )->setComentario ( "Data de fim invalida" );
                break;
            case -5:
                getCampo ( 'C' )->setComentario ( "Contextos invalidos" );
                break;
            case -6:
                getCampo ( 'P' )->setComentario ( "Prioridade invalida" );
                break;
            case -7:
                getCampo ( 'V' )->setComentario ( "Visibilidade invalida" );
                break;
            case 1:
                fechar();
                break;
            }
        }

    } else if ( mTipo == 2 ) {
        /*
        	Para a referencia:
        	  1 - Designacao
        	  2 - Contextos
        	  3 - Documento
        */

        vector<int> * a = ( ( CampoLista* ) getCampo ( 'C' ) )->getSeleccao();
        mContextosSeleccionados.clear();
        for ( int i = 0 ; i < a->size(); i++ ) {
            mContextosSeleccionados.push_back ( mContextos.at ( a->at ( i ) ) );
        }

        Referencia* novaReferencia = new Referencia ( mDesignacao, mContextosSeleccionados, mDocumento );
        novaReferencia->setDescricao ( mDescricao );
        switch ( getBaseDados()->criarTarefa ( novaReferencia, *getUtilizador() ) ) {
        case -2:
            getCampo ( 'D' )->setComentario ( "Designacao invalida" );
            break;
        case -3:
            getCampo ( 'A' )->setComentario ( "Documento invalido" );
            break;
        case 1:
            fechar();
            break;
        }


    }
}

/* Para editar o contexto:
		1 - Receber o id e ir buscar o contexto à base de dados.
		2 - Apresentar o contexto nos campos.
		3 - Depois de modificar alterar o Contexto do id recebido.
*/

void EditorTarefa::editarTarefa () {
    if ( mTipo == 1 || mTipo == 3 ) {
        getCampo ( 'D' )->setComentario ( "" );
        getCampo ( 'I' )->setComentario ( "" );
        getCampo ( 'F' )->setComentario ( "" );
        getCampo ( 'C' )->setComentario ( "" );
        getCampo ( 'P' )->setComentario ( "" );
        getCampo ( 'V' )->setComentario ( "" );
        ( ( CampoLista* ) getCampo ( 'V' ) )->getSeleccao()->push_back ( mTarefa->getVisibilidade() );
        vector<int> * a = ( ( CampoLista* ) getCampo ( 'C' ) )->getSeleccao();
        mContextosSeleccionados.clear();
        for ( int i = 0 ; i < a->size(); i++ ) {
            mContextosSeleccionados.push_back ( mContextos.at ( a->at ( i ) ) );
        }
        mDataInicio = Data ( mDataInicioString );
        mDataFim = Data ( mDataFimString );
        int formato = ( ( CampoLista* ) getCampo ( 'F' ) )->getSeleccao()->at ( 0 );
        switch ( formato ) {
        case 0:
            break;
        case 1:
            mDuracaoPrevista = mDuracaoPrevista*60;
            break;
        case 2:
            mDuracaoPrevista = mDuracaoPrevista*3600;
            break;
        case 3:
            mDuracaoPrevista = mDuracaoPrevista*86400;
            break;
        }
        mVisibilidade = ( ( CampoLista* ) getCampo ( 'V' ) ) ->getSeleccao()->at ( 0 );
        if ( mTipo == 1 ) {
            Referencia * novaTarefa = new Tarefa ( mId,mIdReferencia,mDesignacao,mDataInicio,mDataFim,mPrioridade,mProgresso,mPrioridade,mVisibilidade,NULL,NULL,mContextosSeleccionados,mDocumento,mDataCriacao ); //mId , mDesignacao, mDataInicio, mDataFim, mPrioridade, mVisibilidade, NULL, NULL, mContextosSeleccionados);
            ( ( Tarefa* ) novaTarefa )->setDuracaoPrevista ( mDuracaoPrevista );
            ( ( Tarefa* ) novaTarefa )->setDescricao ( mDescricao );
            switch ( getBaseDados()->editarTarefa ( novaTarefa ) ) {
            case 1:
                mTarefa->setDataFim ( mDataFim );
                mTarefa->setDataInicio ( mDataInicio );
                mTarefa->setDesignacao ( mDesignacao );
                mTarefa->setContextos ( mContextos );
                mTarefa->setPrioridade ( mPrioridade );
                mTarefa->setVisibilidade ( mVisibilidade );
                mTarefa->setDuracaoPrevista ( mDuracaoPrevista );
                mTarefa->setDescricao ( mDescricao );
                fechar();
                break;
            case -2:
                getCampo ( 'D' )->setComentario ( "Designação invalida" );
                break;
            case -3:
                getCampo ( 'I' )->setComentario ( "Data de ínicio invalida" );
                break;
            case -4:
                getCampo ( 'F' )->setComentario ( "Data de fim invalida" );
                break;
            case -5:
                getCampo ( 'C' )->setComentario ( "Contextos invalidos" );
                break;
            case -6:
                getCampo ( 'P' )->setComentario ( "Prioridade invalida" );
                break;
            case -7:
                getCampo ( 'V' )->setComentario ( "Visibilidade invalida" );
                break;
            }
        } else {
            Referencia * novoProjecto = new Projecto ( mIdProjecto,mId,mIdReferencia,mDesignacao,mDataInicio,mDataFim,mPrioridade,mProgresso,mPrioridade,mVisibilidade,NULL,NULL,mContextosSeleccionados,mDocumento,mDataCriacao ); //mId ,mDesignacao, mDataInicio, mDataFim, mPrioridade, mVisibilidade, NULL, NULL, mContextosSeleccionados);
            ( ( Projecto* ) novoProjecto )->setDescricao ( mDescricao );
            ( ( Projecto* ) novoProjecto )->setDuracaoPrevista ( mDuracaoPrevista );
            switch ( getBaseDados()->editarTarefa ( novoProjecto ) ) {
            case 1:
                mTarefa->setDataFim ( mDataFim );
                mTarefa->setDataInicio ( mDataInicio );
                mTarefa->setDesignacao ( mDesignacao );
                mTarefa->setContextos ( mContextos );
                mTarefa->setPrioridade ( mPrioridade );
                mTarefa->setVisibilidade ( mVisibilidade );
                mTarefa->setDuracaoPrevista ( mDuracaoPrevista );
                mTarefa->setDescricao ( mDescricao );
                fechar();
                break;
            case -2:
                getCampo ( 'D' )->setComentario ( "Designação invalida" );
                break;
            case -3:
                getCampo ( 'I' )->setComentario ( "Data de ínicio invalida" );
                break;
            case -4:
                getCampo ( 'F' )->setComentario ( "Data de fim invalida" );
                break;
            case -5:
                getCampo ( 'C' )->setComentario ( "Contextos invalidos" );
                break;
            case -6:
                getCampo ( 'P' )->setComentario ( "Prioridade invalida" );
                break;
            case -7:
                getCampo ( 'V' )->setComentario ( "Visibilidade invalida" );
                break;
            }
        }
    } else {

        vector<int> * a = ( ( CampoLista* ) getCampo ( 'C' ) )->getSeleccao();
        mContextosSeleccionados.clear();
        for ( int i = 0 ; i < a->size(); i++ ) {
            mContextosSeleccionados.push_back ( mContextos.at ( a->at ( i ) ) );
        }

        Referencia * novaReferencia = new Referencia ( mId,mDesignacao,mDataCriacao,mContextosSeleccionados,mDocumento ); //Referencia novaReferencia(mId, mDesignacao, mContextos,mDocumento);
        novaReferencia->setDescricao ( mDescricao );
        switch ( getBaseDados()->editarTarefa ( novaReferencia ) ) {
        case 1:
            mReferencia->setDesignacao ( mDesignacao );
            mReferencia->setContextos ( mContextos );
            mReferencia->setDocumentoAssociado ( mDocumento );
            mReferencia->setDescricao ( mDescricao );
            fechar();
            break;
        case -2:
            getCampo ( 'D' )->setComentario ( "Designacao invalida" );
            break;
        case -3:
            getCampo ( 'A' )->setComentario ( "Documento invalido" );
        }
    }

}
#endif // FORMEDITORTAREFA_H
