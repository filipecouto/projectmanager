#ifndef ACCAO_H
#define ACCAO_H

#include <string>
#include "ItemEcra.h"

class Accao : public ItemEcra
{
	public:
		Accao (std::string titulo, char letra);
};

Accao::Accao (std::string titulo, char letra) : ItemEcra (titulo, letra)
{ }

#endif // ACCAO_H
