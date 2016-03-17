#ifndef FORMLOGIN_H
#define FORMLOGIN_H

#include <string>
#include <fstream>
#include "EcraPrincipal.h"
#include "Formulario.h"
#include "Comunicacao.h"
#include "NovaConta.h"
#include "../BaseDados/BDados.h"

using namespace std;

class Login : public Formulario, public Comunicacao {
    private:
    string utilizador, palavraPasse;
	bool mGuardado;
    public:
    Login ( Comunicacao* contexto ) : Formulario ( "Gestor de Tarefas - Iniciar Sessão" ), Comunicacao ( contexto ), mGuardado(false) {
        if ( getBaseDados()->getEstado() != BASEDADOS_ESTADO_LIGADO ) setTitulo ( "Gestor de Tarefas - Iniciar Sessão Offline" );

        juntarCampo ( new CampoString ( "Nome de Utilizador", 'U', &utilizador ) );
        juntarCampo ( new CampoPalavraPasse ( "Palavra-passe", 'P', &palavraPasse ) );
		
		ifstream definicoes(getPastaDados() + "/GestorTarefas");
		if(definicoes.is_open()) {
			definicoes >> utilizador >> palavraPasse;
			definicoes.close();
			mGuardado = true;
		}

        juntarAccao ( new Accao ( "Iniciar Sessão", 'I' ) );
        juntarAccao ( new Accao ( "Sessão Anónima", 'A' ) );
        juntarAccao ( new Accao ( "Nova Conta", 'N' ) );
        juntarAccao ( new Accao ( "Sair", '0' ) );
    }
protected:
    virtual void executarAccao ( Accao* accao );
    virtual void campoAlterado ( Campo* campo ) { mGuardado = false; }
};

void Login::executarAccao ( Accao* accao ) {
    switch ( accao->getLetra() ) {
    case 'I': {
        Utilizador* u = new Utilizador ( utilizador, palavraPasse ); // instanciar um Utilizador que representa aquele que vai iniciar sessão
		if(mGuardado) u->setPalavraPasseEncriptada(palavraPasse);

        switch ( getBaseDados()->iniciarSessao ( *u ) ) {
        case 1: {
			ofstream definicoes;
			definicoes.open (getPastaDados() + "/GestorTarefas", ofstream::out);
			definicoes << u->getNome() << ' ' << u->getPalavraPasseEncriptada();
			definicoes.close();
            setUtilizador ( u );
            EcraPrincipal ( this ).mostrar();
            fechar();
        }
        break;

        case 0:
            getCampo ( 'U' )->setComentario ( "nome de utilizador ou" );
            getCampo ( 'P' )->setComentario ( "palavra-passe errados" );
            break;

        default:
            getCampo ( 'U' )->setComentario ( "não foi possível" );
            getCampo ( 'P' )->setComentario ( "ligar ao servidor" );
            break;
        }

        return;
    }

    case 'N': {
        NovaConta n ( this );
        n.mostrar();
        return;
    }

    case 'A': {
        EcraPrincipal ep ( this );
        ep.mostrar();
        fechar();
        return;
    }

    case '0':
        fechar();
        return;
    }
}


#endif // FORMLOGIN_H
