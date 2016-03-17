#ifndef FORMAMIZADEH_
#define FORMAMIZADEH_

#include "../Formularios/EcraLista.h"
#include "../Dados/Utilizador.h"
#include "../Formularios/Comunicacao.h"
#include "EditorAmizade.h"
#include "../Dados/Amizade.h"
#include "../BaseDados/BDados.h"
#include "../Formularios/EcraListaAmigos.h"

#include <iostream>

class EcraAmizade : public EcraLista, public Comunicacao {
    public:
    EcraAmizade ( Comunicacao* contexto );
    virtual void executarAccao ( Accao* accao );
    private:
    virtual float getLarguraColuna ( int coluna );
    virtual int getNumColunas();
    virtual int getNumLinhas();
    virtual string getNomeColuna ( int coluna );
    virtual void imprimirCelula ( int coluna, int linha );
    virtual void itemSelecionado ( int linha );
    vector<Amizade*> mAmigos;
};

EcraAmizade::EcraAmizade ( Comunicacao* contexto ) : EcraLista ( "Lista dos Amigos" ), Comunicacao ( contexto ) {
    juntarAccao ( new Accao ( "Adicionar amigo", 'V' ) );
    juntarAccao ( new Accao ( "Voltar", '0' ) );

    mAmigos = getBaseDados()->getAmigos ( *getUtilizador() );
}

void EcraAmizade::imprimirCelula ( int coluna, int linha ) {

    switch ( coluna ) {
    case 0:
        cout << mAmigos[linha]->getAmigo()->getNomeUtilizador();
        return;
    case 1:
        cout << mAmigos[linha]->getAmigo()->getNome() <<" "<< mAmigos[linha]->getAmigo()->getApelido();
        return;
    case 3:
        return;
    }
}

void EcraAmizade::itemSelecionado ( int linha ) {
    EcraListaAmigos ( this, mAmigos[linha] ).mostrar();
    mAmigos.clear();
    mAmigos = getBaseDados()->getAmigos ( *getUtilizador() );
}


int EcraAmizade::getNumLinhas() {
    return mAmigos.size();
}

int EcraAmizade::getNumColunas() {
    return 2;
}

float EcraAmizade::getLarguraColuna ( int coluna ) {
    switch ( coluna ) {
    case 0:
        return 0.6f;
    case 1:
        return 0.6f;
    case 2:
        return 0.8f;
    default:
        return 0;
    }
}

string EcraAmizade::getNomeColuna ( int coluna ) {
    switch ( coluna ) {
    case 0:
        return "Nome do Utilizador";
    case 1:
        return "Nome";
    default:
        return 0;
    }
}

void EcraAmizade::executarAccao ( Accao* accao ) {
    switch ( accao->getLetra() ) {
    case 'V':
        EditorAmizade ( this ).mostrar();
        mAmigos.clear();
        mAmigos = getBaseDados()->getAmigos ( *getUtilizador() );
        break;
    case '0':
        fechar();
        break;
    }
}

#endif //FORMAMIZADEH_
