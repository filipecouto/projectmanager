#include <iostream>
#include <string>
#include <sstream>

#ifdef _WIN32
#include <Windows.h>
#include <conio.h>
#endif

#include <limits>
#include <occi.h>

#include "Formularios/Login.h"
#include "BaseDados/BDados.h"

using namespace std;

int main (int argc, char** argv)
{
	Login home(Comunicacao::criarContexto());
	home.mostrar();
	
	#ifdef __GNUC__
	std::cout << "\033[0m";
	#elif defined(__MINGW32__)
	#endif

	return 0;
}
