/***************************************************
 * Gestor de Tarefas - Utilizador.h                *
 ***************************************************
 * Este ficheiro contém a declaração e             *
 * implementação da classe Utilizador que armazena *
 * informações relacionadas com um dado utilizador *
 ***************************************************
 * CONTEÚDO                                        *
 * Utilizador                                      *
 *  string getNomeUtilizador()                     *
 *  void setNomeUtilizador(string nomeUtilizador)  *
 *  string getPalavraPasseEncriptada()             *
 *  void setPalavraPasse(string palavraPasse)      *
 *  string getNome()                               *
 *  void setNome(string nome)                      *
 *  string getApelido()                            *
 *  void setApelido(string apelido)                *
 *  string getEmail()                              *
 *  void setEmail(string email)                    *
 ***************************************************/
#ifndef UTILIZADOR_H
#define UTILIZADOR_H

#include <string>
#include <locale>

using namespace std;

class Utilizador {
	public:
		Utilizador(string, string);
		Utilizador( string, string, string, string, string);
		Utilizador( int id, string nomeUtilizador, string nome, string apelido, string email );
		virtual ~Utilizador();

		int getId() const;
		void setId(const int id);
		string getNomeUtilizador() const;
		void setNomeUtilizador(const string nomeUtilizador);
		string getPalavraPasseEncriptada() const;
		void setPalavraPasse(const string palavraPasse);
		void setPalavraPasseEncriptada(const string palavraPasse);
		string getNome() const;
		void setNome(const string nome);
		string getApelido() const;
		void setApelido(const string apelido);
		string getEmail() const;
		void setEmail(const string email);
	private:
		int mId;
		string mNomeUtilizador;
		string mPalavraPasse;
		string mNome;
		string mApelido;
		string mEmail;
};

/**
 * Construtor útil para o início de sessão
 * @param nomeUtilizador o nome do utilizador
 * @param palavraPasse a palavra passe do utilizador
 */
Utilizador::Utilizador(string nomeUtilizador, string palavraPasse) : mNomeUtilizador(nomeUtilizador) {
	setPalavraPasse(palavraPasse);
}

/**
 * Construtor útil para a criação de contas
 * @param nomeUtilizador o nome do utilizador
 * @param palavraPasse a palavra passe do utilizador
 * @param nome o nome do dono da conta
 * @param apelido o apelido do dono da conta
 * @param email o e-mail do dono da conta
 */
Utilizador::Utilizador(string nomeUtilizador, string palavraPasse, string nome, string apelido, string email) : mNomeUtilizador(nomeUtilizador), mNome(nome), mApelido(apelido), mEmail(email) {
	setPalavraPasse(palavraPasse);
}

/**
 * Construtor útil para a criação de contas
 * @param id o id do utilizador
 * @param nomeUtilizador o nome do utilizador
 * @param nome o nome do dono da conta
 * @param apelido o apelido do dono da conta
 * @param email o e-mail do dono da conta
 */
Utilizador::Utilizador (int id, string nomeUtilizador,  string nome, string apelido, string email) : mId(id), mNomeUtilizador(nomeUtilizador), mNome(nome), mApelido(apelido), mEmail(email) {
}

/**
 * Destrutor da classe
 */
Utilizador::~Utilizador() { }

/**
 * Devolve o id do utilizador
 * @return o id do utilizador
 */
int Utilizador::getId() const {
	return mId;
}

/**
 * Define o id do utilizador
 * @param id o novo id do utilizador
 */
void Utilizador::setId ( const int id ) {
	mId = id;
}

/**
 * Devolve o nome do utilizador do utilizador
 * @return o nome do utilizador
 */
string Utilizador::getNomeUtilizador() const {
	return mNomeUtilizador;
}

/**
 * Define o nome do utilizador do utilizador
 * @param nomeUtilizador o novo nome do utilizador
 */
void Utilizador::setNomeUtilizador(const string nomeUtilizador) {
	mNomeUtilizador = nomeUtilizador;
}
/**
 * Devolve a palavra passe do utilizador encriptada
 * @return a palavra passe encriptada
 */
string Utilizador::getPalavraPasseEncriptada() const {
	return mPalavraPasse;
}

/**
 * Define a palavra passe do utilizador de forma encriptada
 * @param palavraPasse a nova palavra passe já encriptada
 */
void Utilizador::setPalavraPasseEncriptada ( const string palavraPasse ) {
	mPalavraPasse = palavraPasse;
}

/**
 * Define a palavra passe do utilizador, a mesma ficará guardada de forma encriptada
 * @param palavraPasse a nova palavra passe
 */
void Utilizador::setPalavraPasse(const string palavraPasse) {
	mPalavraPasse.clear();
	mPalavraPasse += (palavraPasse[0]/2);
	int l = palavraPasse.length();
	int i;
	for(i = 1; i < l; i++)
	  mPalavraPasse += (mPalavraPasse[i-1]/2 + palavraPasse[i]/2);
	for(i = 0; i < l; i++)
	  if(mPalavraPasse[i] == '\'' || mPalavraPasse[i] == '"') mPalavraPasse[i] = '_';
}

/**
 * Devolve o nome do utilizador
 * @return o nome
 */
string Utilizador::getNome() const {
	return mNome;
}

/**
 * Define o nome do utilizador
 * @param nome o novo nome
 */
void Utilizador::setNome(const string nome) {
	mNome = nome;
}

/**
 * Devolve o apelido do utilizador
 * @return o apelido
 */
string Utilizador::getApelido() const {
	return mApelido;
}

/**
 * Define o apelido do utilizador
 * @param apelido o novo apelido
 */
void Utilizador::setApelido(const string apelido) {
	mApelido = apelido;
}

/**
 * Devolve o e-mail do utilizador
 * @return o e-mail
 */
string Utilizador::getEmail() const {
	return mEmail;
}

/**
 * Define o e-mail do utilizador
 * @param email o novo e-mail
 */
void Utilizador::setEmail(const string email) {
	mEmail = email;
}

#endif
