#ifndef FORMECRAPRINCIPAL_H
#define FORMECRAPRINCIPAL_H

#include "EcraBase.h"
#include "Comunicacao.h"
#include "EditorContexto.h"
#include "EditorTarefa.h"
#include "EcraContextos.h"
#include "EcraAmizade.h"
#include "EcraReferencias.h"
#include "EcraTarefas.h"

class EcraPrincipal : public EcraBase, public Comunicacao {
    public:
    EcraPrincipal ( Comunicacao* contexto );
    void mostrar();
};

EcraPrincipal::EcraPrincipal ( Comunicacao* contexto ) : Comunicacao ( contexto ) {

}

void EcraPrincipal::mostrar() {
    char c;
    Tamanho tamanhoConsola;
    string nomeUtilizador;
    string opcoes[] = {"Tarefas", "Contextos", "Referencias", "Amigos", "Definições", "Sair"};
    char letras[] = {'1', '2', '3', '4', '5', '0'};

    do {
        limparEcra();
        reporFormatacao();
        getTamanho ( tamanhoConsola );

        setPosicao ( 3, 1 );
        setNegrito();
        cout << "Gestor de Tarefas";

        reporFormatacao();
        if ( getUtilizador() )
            nomeUtilizador = getUtilizador()->getNome() + ' ' + getUtilizador()->getApelido();
        else
            nomeUtilizador = "Anónimo";
        setPosicao ( tamanhoConsola.largura - 2 - nomeUtilizador.length(), 1 );
        cout << nomeUtilizador;

        setPosicao ( 3, 3 );
        setNegrito();
        cout << "  MENU" << endl;;
		int desce = 1;
        for ( int i = 0; i < 6; i++ ) {
            setNegrito();
            cout << "   " << letras[i];
            reporFormatacao();
            cout << ' ' << opcoes[i] << endl;
			desce++;
		}
		setPosicao ( 3 , 3 + desce + 1);
		vector <Tarefa*> tarefas = getBaseDados()->getDelegacoesPorAceitar(*getUtilizador());
		if(tarefas.size() > 0) {
			cout << "Foram delegadas " << tarefas.size() << " tarefas consigo." << endl;
			setNegrito();
			setPosicao ( 3 , 3 + desce + 2);
			cout << "A";
			reporFormatacao();
			cout << " Aceitar\t";
			setNegrito();
			cout << "X";
			reporFormatacao();
			cout << " Recusar";
			setPosicao ( 3 , 3 + desce + 3);
		}
        cin >> c;

        switch ( c ) {
        case '1':
            //( new EcraTarefas ( this ) )->mostrar();
            EcraTarefas(this).mostrar();
            break;
        case '2':
            EcraContextos ( this ).mostrar();
            break;
        case '3':
            EcraReferencias ( this ).mostrar();
            break;
        case '4':
            EcraAmizade ( this ).mostrar();
            break;
		case 'A':
			getBaseDados()->setDelegacoesPorAceitar(1 , *getUtilizador());
			break;
		case 'X':
			getBaseDados()->setDelegacoesPorAceitar(0 , *getUtilizador());
			break;
        }
    } while ( c != '0' );
}

#endif
