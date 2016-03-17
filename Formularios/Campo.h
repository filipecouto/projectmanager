#ifndef CAMPO_H
#define CAMPO_H

#include <string>
#include "ItemEcra.h"

using namespace std;

class Campo : public ItemEcra
{
	public:
		Campo (string titulo, char letra);
		Campo (string titulo, char letra, string comentario);

		string getComentario() const;
		int getEstado() const;

		void setComentario (string comentario);
		void setEstado (int estado);
		
		virtual void apresentarCampo(EcraBase* ecra) { }
		virtual void campoSeleccionado(EcraBase* ecra) { }
	private:
		string mComentario;
		int mEstado;
};

Campo::Campo (string titulo, char letra) : ItemEcra (titulo, letra)
{ }

Campo::Campo (string titulo, char letra, string comentario) : ItemEcra (titulo, letra), mComentario(comentario), mEstado (0)
{

}

string Campo::getComentario() const
{
	return mComentario;
}

int Campo::getEstado() const
{
	return mEstado;
}

void Campo::setComentario (string comentario)
{
	mComentario = comentario;
}

void Campo::setEstado (int estado)
{
	mEstado = estado;
}

#endif // CAMPO_H
