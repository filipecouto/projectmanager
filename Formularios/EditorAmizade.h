#ifndef EDITORAMIZADEH_
#define EDITORAMIZADEH_

#include "Formulario.h"
#include "../Dados/Tarefa.h"
#include "CampoString.h"
#include "CampoInt.h"
#include "CampoPalavraPasse.h"
#include "../BaseDados/BDados.h"
#include "Comunicacao.h"
#include <iostream>

using namespace std;


class EditorAmizade: public Formulario, public Comunicacao
{
public:
	EditorAmizade(Comunicacao* contexto);
private:
	long mId;
	//string mEmail;
	string mNomeAmigo;
	vector<Amizade*> mAmizades;
	vector<string>* mNomeUtilizador;
	void criarAmizade ();
	void cancelar ();
	void executarAccao (Accao* accao);
	void carregarNomes();
};

EditorAmizade::EditorAmizade(Comunicacao* contexto) : Formulario ("Adicionar Novo Amigo"), Comunicacao(contexto)
{
	juntarCampo (new CampoString ("Nome do Utilizador" , 'N' , &mNomeAmigo));
	//juntarCampo (new CampoString ("E-mail" , 'E' , &mEmail));
	juntarEspaco();

	juntarAccao(new Accao ("Adicionar", 'A'));
	juntarAccao(new Accao ("Cancelar", '0'));
}

void EditorAmizade::executarAccao (Accao* accao)
{
	switch (accao->getLetra())
	{
	case 'A':
		criarAmizade();
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
void EditorAmizade::criarAmizade ()
{
	getCampo ('N')->setComentario ("");
	//getCampo ('E')->setComentario ("");
	Utilizador *amigo = new Utilizador(mNomeAmigo, "","","", ""); // mEmail);
	switch (getBaseDados()->criarAmizade(*amigo, *getUtilizador()))
	{
		case -2:
			getCampo ('N')->setComentario ("Nome do utilizador invalido");
			break;
// 		case -3:
// 			getCampo ('E')->setComentario ("Email invalido");
// 			break;
	}
	fechar();
}

void EditorAmizade::carregarNomes() {
	for(int i = 0 ; i < mAmizades.size(); i++) {
		mNomeUtilizador->push_back(mAmizades.at(i)->getAmigo()->getNomeUtilizador());
	}
}
#endif // FORMEDITORCONTEXTO_H