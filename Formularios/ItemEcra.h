#ifndef ITEMECRA_H
#define ITEMECRA_H

#include <string>

class ItemEcra
{
	public:
		ItemEcra (std::string titulo, char letra);

		std::string getTitulo() const;
		char getLetra() const;

		void setTitulo (std::string titulo);
		void setLetra (char letra);
	private:
		std::string mTitulo;
		char mLetra;
};

ItemEcra::ItemEcra (std::string titulo, char letra) : mTitulo (titulo), mLetra (letra)
{ }

char ItemEcra::getLetra() const
{
	return mLetra;
}

std::string ItemEcra::getTitulo() const
{
	return mTitulo;
}

void ItemEcra::setLetra (char letra)
{
	mLetra = letra;
}

void ItemEcra::setTitulo (std::string titulo)
{
	mTitulo = titulo;
}

#endif // ITEMECRA_H
