#ifndef DELEGACAO_H
#define DELEGACAO_H

#include "Tarefa.h"
#include "Utilizador.h"

#include <iostream>
class Delegacao {
    private:
    bool mAceite;
    Utilizador* mUtilizador;

    public:
    Delegacao ( bool aceite, Utilizador * utilizador );
    Delegacao ( const Delegacao &d );
    Delegacao ( );
    void setAceite ( bool aceite );
    void setUtilizador ( Utilizador* utilizador );
    bool getAceite();
    Utilizador * getUtilizador();
    ~Delegacao();
};

Delegacao::Delegacao ( bool aceite, Utilizador* utilizador ) {
    setAceite ( aceite );
    setUtilizador ( utilizador );
}

Delegacao::Delegacao ( const Delegacao & d ) {
    setAceite ( d.mAceite );
    setUtilizador ( d.mUtilizador );
}

Delegacao::Delegacao() : mAceite(true), mUtilizador(0) {

}

Delegacao::~Delegacao() {
}

void Delegacao::setAceite ( bool aceite ) {
    mAceite = aceite;
}

void Delegacao::setUtilizador ( Utilizador* utilizador ) {
    mUtilizador = utilizador;
}

bool Delegacao::getAceite() {
    return mAceite;
}

Utilizador* Delegacao::getUtilizador() {
    return mUtilizador;
}

#endif
