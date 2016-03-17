#ifndef EDITORCONTEXTO_H
#define EDITORCONTEXTO_H

#include "Formulario.h"
#include "Comunicacao.h"
#include "CampoString.h"
#include "CampoInt.h"
#include "CampoLista.h"
#include "CampoPalavraPasse.h"
#include "../BaseDados/BDados.h"

class EditorContexto : public Formulario, public Comunicacao
{
	public:
		EditorContexto(Comunicacao* contexto);
		EditorContexto(Comunicacao* comunicacao, Contexto* contexto);
	private:
		long mId;
		string mDesignacao;
		int mPrioridade;	
		Utilizador *mUtilizador;
		Contexto * mContexto;
		long mIdUtilizador;
		void criarContexto ();
		void editarContexto ();
		void cancelar ();
		void executarAccao (Accao* accao);
};

EditorContexto::EditorContexto(Comunicacao* contexto) : Formulario ("Criar Contexto"), Comunicacao(contexto)
{
	mUtilizador = getUtilizador();
	mDesignacao="";
	mPrioridade=50;
	juntarCampo (new CampoString ("Designação" , 'D' , &mDesignacao));
	juntarCampo (new CampoInt ("Prioridade" , 'P' , &mPrioridade));
	juntarEspaco();

	juntarAccao(new Accao ("Criar Contexto", 'C'));
	juntarAccao(new Accao ("Cancelar", '0'));
}

EditorContexto::EditorContexto( Comunicacao* comunicacao, Contexto* contexto ) : Formulario ("Editar Contexto"), Comunicacao(comunicacao)
{
	mUtilizador = getUtilizador();
	mContexto = contexto;
	mId = contexto->getId();
	mDesignacao = contexto->getDesignacao();
	mPrioridade = contexto->getPrioridade();
	juntarCampo (new CampoString ("Designação" , 'D' , &mDesignacao));
	juntarCampo (new CampoInt ("Prioridade" , 'P' , &mPrioridade));
	juntarEspaco();

	juntarAccao(new Accao ("Editar Contexto", 'E'));
	juntarAccao(new Accao ("Cancelar", '0'));
}

void EditorContexto::executarAccao (Accao* accao)
{
	switch (accao->getLetra())
	{
		case 'C':
			criarContexto();
			return;
		case 'E':
			editarContexto();
			return;
		case '0':
			fechar();
			return;
	}
}

/** Para criar o contexto:
		1 - Criar o objecto de Contexto obviamente sem ID e com utilizador já.
		2 - Enviar os dados do objecto para a base de dados e o ID que for atribuido a esse contexto na base de dados vai ficar o ID no programa.
*/
void EditorContexto::criarContexto ()
{
	getCampo ('D')->setComentario ("");
	getCampo ('P')->setComentario ("");
	Contexto novoContexto (mDesignacao, mPrioridade);
	switch (getBaseDados()->criarContexto(novoContexto, *getUtilizador()))
	{
		case -2:
			getCampo ('D')->setComentario ("Designacao invalida");
			break;
		case -3:
			getCampo ('P')->setComentario ("Prioridade invalida");
			break;
	}
	fechar();
}

/** Para editar o contexto:
		1 - Receber o contexto.
		2 - Apresentar o contexto nos campos.
		3 - Depois de modificar alterar o Contexto do id recebido.
*/

void EditorContexto::editarContexto () 
{
	getCampo ('D')->setComentario ("");
	getCampo ('P')->setComentario ("");
	Contexto a(mId, mDesignacao,mPrioridade);
	switch (getBaseDados()->editarContexto(a)) {
		case 1:
			mContexto->setDesignacao(mDesignacao);
			mContexto->setPrioridade(mPrioridade);
			break;
		case -2:	
			getCampo('D')->setComentario("Designacao invalida");
			break;
		case -3:
			getCampo('P')->setComentario("Prioridade invalida");
			break;
	}
	fechar();
}
#endif // FORMEDITORCONTEXTO_H
