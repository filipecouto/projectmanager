#ifndef FORMNOVACONTA_H
#define FORMNOVACONTA_H

#include "Formulario.h"
#include "Comunicacao.h"
#include "CampoString.h"
#include "CampoPalavraPasse.h"
#include "../BaseDados/BDados.h"

class NovaConta : public Formulario, public Comunicacao
{
	public:
		NovaConta(Comunicacao* contexto);
	private:
		string mNome, mApelido, mEmail, mNomeUtilizador, mPalavraPasse;
		void criarConta ();
		void cancelar ();
		void executarAccao (Accao* accao);
};

NovaConta::NovaConta(Comunicacao* contexto) : Formulario ("Criar Conta"), Comunicacao(contexto)
{
	juntarCampo (new CampoString ("Nome", 'N', &mNome));
	juntarCampo (new CampoString ("Apelido", 'A', &mApelido));
	juntarEspaco();
	juntarCampo (new CampoString ("E-mail", 'M', &mEmail));
	juntarEspaco();
	juntarCampo (new CampoString ("Nome de Utilizador", 'U', &mNomeUtilizador));
	juntarCampo (new CampoPalavraPasse ("Palavra-Passe", 'P', &mPalavraPasse));

	juntarAccao (new Accao ("Criar Conta", 'C'));
	juntarAccao (new Accao ("Cancelar", '0'));
}

void NovaConta::executarAccao (Accao* accao)
{
	switch (accao->getLetra())
	{
		case 'C':
			criarConta();
			return;
		case '0':
			fechar();
			return;
	}
}

void NovaConta::criarConta ()
{
	getCampo ('U')->setComentario ("");
	getCampo ('N')->setComentario ("");
	getCampo ('M')->setComentario ("");
	Utilizador novoUtilizador (mNomeUtilizador, mPalavraPasse, mNome, mApelido, mEmail);
	switch (getBaseDados()->criarConta (novoUtilizador))
	{
		case -2:
			getCampo ('U')->setComentario ("nome inválido");
			break;
		case -3:
			getCampo ('N')->setComentario ("nome inválido");
			break;
		case -4:
			getCampo ('M')->setComentario ("e-mail inválido");
			break;
		case 0:
			getCampo ('N')->setComentario ("ocorreu um erro");
			break;
		case 1:
			fechar();
			break;
	}
}

#endif // FORMNOVACONTA_H
