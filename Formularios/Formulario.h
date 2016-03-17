#ifndef FORMULARIO_H
#define FORMULARIO_H

#include <algorithm>
#include <string>
#include <iomanip>
#include "EcraBase.h"
#include "Campo.h"
#include "Accao.h"

using namespace std;

class Formulario : public EcraBase
{
	public:
		Formulario();
		Formulario (string titulo);

		virtual void mostrar();

		void setTitulo (string titulo);
		string getTitulo();

		void juntarCampo (Campo* campo);
		void juntarEspaco();
		Campo* getCampo (char letra);

		void juntarAccao (Accao* accao);
		Accao* getAccao (char letra);

		void fechar();
	private:
		vector<Campo*> mCampos;
		vector<Accao*> mAccoes;
		bool mVisivel;
		string mTitulo;
		
		bool testarSelecao( char letra, Campo*& campoSelecionado, int& posCampo, int nCampos, int nAccoes );
	protected:
		virtual void executarAccao (Accao* accao) {}
		virtual void campoAlterado (Campo* campo) {}
};

Formulario::Formulario() : mVisivel (true)
{
}

Formulario::Formulario (string titulo) : mVisivel (true), mTitulo (titulo)
{
}

void Formulario::mostrar()
{
	int len = mCampos.size();
	if (len == 0) return;

	int maior = mCampos[0]->getTitulo().size();
	for (int i = 1; i < len; i++)
		if (mCampos[i] && maior < mCampos[i]->getTitulo().size()) maior = mCampos[i]->getTitulo().length();
	maior += 2;
	int larguraColuna1;

	char c;
	string s;
	Tamanho tamanhoConsola;
	Campo* campoSelecionado = NULL;
	int posCampo;
	int nAccoes = mAccoes.size();
	do
	{
		limparEcra();
		reporFormatacao();
		getTamanho (tamanhoConsola);
		if (tamanhoConsola.largura > maior * 2) larguraColuna1 = (int) (maior * 1.3f);
		else larguraColuna1 = maior;
		setPosicao (3, 1);
		setNegrito();
		cout << mTitulo;
		setPosicao (0, 3);
		for (int i = 0; i < len; i++)
		{
			if (mCampos[i])
			{
				reporFormatacao();
				cout << setw (larguraColuna1) << mCampos[i]->getTitulo() << ": ";
				setNegrito();
				cout << mCampos[i]->getLetra();
				reporFormatacao();
				if (mCampos[i] == campoSelecionado)
				{
					cout << " ";
				}
				else
				{
					mCampos[i]->apresentarCampo(this);
					cout << " " << mCampos[i]->getComentario();
				}
			}
			cout << endl;
		}
		setPosicao (0, tamanhoConsola.altura);
		setCorFundo (7);
		setCor (0);
		for (int i = 0; i < nAccoes; i++)
		{
			printf ("%*s", -tamanhoConsola.largura / nAccoes, (string() + mAccoes[i]->getLetra() + " - " +  mAccoes[i]->getTitulo()).c_str());
		}
		if (campoSelecionado)
		{
			reporFormatacao();
			setPosicao (larguraColuna1 + 4, posCampo + 3);
			campoSelecionado->campoSeleccionado(this);
			campoAlterado(campoSelecionado);
			campoSelecionado = NULL;
			continue;
		}
		setPosicao (0, len + 3);
		reporFormatacao();
		setNegrito();
		cout << endl << setw (larguraColuna1) << "Escolha uma op" << (char)0xC3 << (char)0xA7 << (char)0xC3 << (char)0xA3 << "o ";
		reporFormatacao();
		cin >> c;
		if(!testarSelecao(c, campoSelecionado, posCampo, len, nAccoes) && c >= 'a' && c <= 'z') {
			c -= 32;
			testarSelecao(c, campoSelecionado, posCampo, len, nAccoes);
		}
	}
	while (mVisivel);
}

bool Formulario::testarSelecao ( char letra, Campo*& campoSelecionado, int& posCampo, int nCampos, int nAccoes ) {
	for (int i = 0; i < nCampos; i++)
	{
		if (mCampos[i] && mCampos[i]->getLetra() == letra)
		{
			campoSelecionado = mCampos[i];
			posCampo = i;
			return true;
		}
	}
	for (int i = 0; i < nAccoes; i++)
	{
		if (mAccoes[i]->getLetra() == letra)
		{
			executarAccao (mAccoes[i]);
			return true;
		}
	}
	return false;
}

string Formulario::getTitulo()
{
	return mTitulo;
}

void Formulario::setTitulo (string titulo)
{
	mTitulo = titulo;
}

Campo* Formulario::getCampo (char letra)
{
	int len = mCampos.size();
	for (int i = 0; i < len; i++) if (mCampos[i] && mCampos[i]->getLetra() == letra) return mCampos[i];
}

Accao* Formulario::getAccao (char letra)
{
	int len = mAccoes.size();
	for (int i = 0; i < len; i++) if (mAccoes[i]->getLetra() == letra) return mAccoes[i];
}

void Formulario::juntarCampo (Campo* campo)
{
	mCampos.push_back (campo);
}

void Formulario::juntarEspaco()
{
	if (mCampos.size() != 0)
		mCampos.push_back (NULL);
}

void Formulario::juntarAccao (Accao* accao)
{
	mAccoes.push_back (accao);
}

void Formulario::fechar()
{
	mVisivel = false;
}

#endif // FORMULARIO_H
