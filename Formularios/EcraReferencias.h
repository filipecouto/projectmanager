#ifndef FORMREFERENCIAS_H
#define FORMREFERENCIAS_H

#include "EcraLista.h"
#include "Comunicacao.h"
#include "EditorContexto.h"
#include "EcraContexto.h"

class EcraReferencias : public EcraLista, public Comunicacao {
    public:
    EcraReferencias ( Comunicacao* contexto );
    virtual void executarAccao ( Accao* accao );

    private:
    virtual float getLarguraColuna ( int coluna );
    virtual int getNumColunas();
    virtual int getNumLinhas();
    virtual string getNomeColuna ( int coluna );
    virtual void imprimirCelula ( int coluna, int linha );
    virtual void itemSelecionado ( int linha );

    inline void imprimirPrioridade ( int prioridade );

    void carregarReferencias();
    vector<Referencia*> mReferencias;
};

EcraReferencias::EcraReferencias ( Comunicacao* contexto ) : EcraLista ( "Referencias" ), Comunicacao ( contexto ) {
    juntarAccao ( new Accao ( "Nova referencia", 'C' ) );
    juntarAccao ( new Accao ( "Voltar", '0' ) );
	
	carregarReferencias();
}

void EcraReferencias::carregarReferencias() {
	mReferencias.clear();
    mReferencias = getBaseDados()->getReferencias ( *getUtilizador());
	int len = mReferencias.size();
	for(int i = 0; i < len; i++) {
		stringstream s;
		s << mReferencias[i]->getIdReferencia();
		mReferencias[i]->setContextos(getBaseDados()->getContextos ( *getUtilizador(), " Referencia_Contexto t", "c.id = t.contexto and t.referencia = " + s.str(), "c.designacao" ));
	}
}

void EcraReferencias::itemSelecionado ( int linha ) {
    EcraTarefa ( this, mReferencias[linha] ).mostrar();
	carregarReferencias();
}

void EcraReferencias::imprimirCelula ( int coluna, int linha ) {
    switch ( coluna ) {
    case 0:
        cout << mReferencias[linha]->getDesignacao();
        return;
    case 1:
        int len = mReferencias[linha]->getContextos().size();
        for ( int i = 0; i < len; i++ )
            cout << mReferencias[linha]->getContextos() [i]->getDesignacao() << ' ';
        return;
    }
}

int EcraReferencias::getNumLinhas() {
    return mReferencias.size();
}

int EcraReferencias::getNumColunas() {
    return 2;
}

float EcraReferencias::getLarguraColuna ( int coluna ) {
    switch ( coluna ) {
    case 0:
        return 0.7f;
    case 1:
        return 0.3f;
    default:
        return 0;
    }
}

string EcraReferencias::getNomeColuna ( int coluna ) {
    switch ( coluna ) {
    case 0:
        return "Designação";
    case 1:
        return "Contextos";
    default:
        return 0;
    }
}

void EcraReferencias::executarAccao ( Accao* accao ) {
    switch ( accao->getLetra() ) {
    case 'C':
        EditorTarefa ( this, 2 ).mostrar();
		carregarReferencias();
        break;
    case '0':
        fechar();
        break;
    }
}

#endif