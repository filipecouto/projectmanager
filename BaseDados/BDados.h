#ifndef BDados_
#define BDados_

#include <iostream>
#include <string>
#include <sstream>
#include <iomanip>
#include <occi.h>
#include <list>
#include <vector>

#include "../Dados/Utilizador.h"
#include "../Dados/Contexto.h"
#include "../Dados/Tarefa.h"
#include "../Dados/Amizade.h"
#include "../Dados/Projecto.h"

#define BASEDADOS_ESTADO_NAOLIGADO 0
#define BASEDADOS_ESTADO_LIGADO 1
#define BASEDADOS_ESTADO_ERROLIGACAO -1

using namespace oracle::occi;
using namespace std;

class BDados {
    private:
    Environment* env;
    Connection* ligacao;
    Statement* instrucao;
    int mEstado;
    public:
    BDados ( string user, string passwd, string db );
    ~ BDados();
    int iniciarSessao ( Utilizador& u );
    int criarConta ( Utilizador& u );
    int criarContexto ( Contexto &c , Utilizador &u );
    int criarTarefa ( Referencia* t , Utilizador &u );
    int getContexto ( long, Contexto * );
    int editarContexto ( Contexto & );
    int editarTarefa ( Referencia * );
	int actualizarTarefasProjecto(Projecto* projecto);
    int eliminarTarefa ( Referencia * );
    int eliminarContexto ( Contexto&c , Utilizador &u );
    int definirDelegacao ( Utilizador& u,Tarefa * );
    bool completarDadosTarefa ( Utilizador& u, Referencia*& ref );
    vector<Contexto*> getContextos ( Utilizador& utilizador, string from = "", string where = "", string order = "" );
	vector<Tarefa*> getQuaisquerTarefas ( string from = "", string where = "", string order = "" );
    vector<Tarefa*> getTarefas ( Utilizador& utilizador, string from = "", string where = "", string order = "" );
    vector<Tarefa*> getTarefas ( Utilizador& utilizador, Contexto& contexto, string where = "", string order = "" );
    vector<Referencia*>getReferencias ( Utilizador& utilizador, string from = "", string where = "", string order = "" );
    vector< Amizade* > getAmigos ( Utilizador& utilizador );
    int criarAmizade ( Utilizador& amigo, Utilizador &u );
    int eliminarAmizade ( Utilizador& amigo, Utilizador &u );
    int despromoverProjecto ( Projecto* p, int tipo );
    int despromoverTarefa ( Tarefa* p, int tipo );
    int promoverReferencia ( Utilizador& u, Referencia*& r, int tipo );
    int promoverTarefa ( Utilizador& u, Tarefa*& r, int tipo );
    int sugestaoDuracao ( string , Utilizador & );
	vector<Tarefa*> getDelegacoesPorAceitar(Utilizador &u);
	int setDelegacoesPorAceitar(int, Utilizador&);

    int getEstado();
};

BDados::BDados ( string user, string passwd, string db ) : mEstado ( BASEDADOS_ESTADO_NAOLIGADO ) {
    try {
        cout << "A contactar servidor externo..." << endl;
        env = Environment::createEnvironment ( Environment::DEFAULT );
        ligacao = env->createConnection ( user, passwd, db );
        mEstado = BASEDADOS_ESTADO_LIGADO;
    } catch ( SQLException e ) {
        cout << "Não foi possível ligar ao servidor.";
        std::cout << e.what();
        mEstado = BASEDADOS_ESTADO_ERROLIGACAO;
    }
}

/**
 * Inicia sessão no servidor
 * @param u um utilizador contendo o nome de utilizador e palavra passe, dados adicionais (nome, email, etc.) serão preenchidos caso a sessão tenha sido iniciada com sucesso
 * @return 1 caso a sessão tenha sido iniciada com sucesso, 0 caso o nome de utilizador ou palavra passe estejam errados, -1 caso haja problemas de ligação
 */
int BDados::iniciarSessao ( Utilizador& u ) {
    if ( mEstado != BASEDADOS_ESTADO_LIGADO ) return -1;

    try {
        instrucao = ligacao->createStatement ( "Select count(*) from Utilizadores where nomeUtilizador='" + u.getNomeUtilizador() + "' and palavraPasse='" + u.getPalavraPasseEncriptada() + "'" );
        ResultSet* rset = instrucao->executeQuery();

        if ( rset->next() ) {
            if ( rset->getInt ( 1 ) == 1 ) {
                instrucao->closeResultSet ( rset );
                instrucao = ligacao->createStatement ( "Select id, nome, apelido, email from Utilizadores where nomeUtilizador='" + u.getNomeUtilizador() + "' and palavraPasse='" + u.getPalavraPasseEncriptada() + "'" );
                rset = instrucao->executeQuery();

                if ( rset->next() ) {
                    u.setId ( rset->getInt ( 1 ) );
                    u.setNome ( rset->getString ( 2 ) );
                    u.setApelido ( rset->getString ( 3 ) );
                    u.setEmail ( rset->getString ( 4 ) );
                }

                instrucao->closeResultSet ( rset );
                return 1;

            } else return 0;
        }

    } catch ( SQLException e ) {
        std::cout << e.what();
        return -1;
    }
}

vector< Contexto* > BDados::getContextos ( Utilizador& utilizador, string from, string where, string order ) {
    vector<Contexto*> contextos;
    if ( mEstado == BASEDADOS_ESTADO_LIGADO ) {
        try {
            stringstream ss;
            ss << utilizador.getId();
            if ( from.length() != 0 ) from = ", " + from;
            if ( where.length() != 0 ) where = " and " + where;
            if ( order.length() != 0 ) order = " " + order + ", ";
            instrucao = ligacao->createStatement ( "Select * from Contexto c" + from + " where utilizador='" + ss.str() + "'" + where + " order by" + order + " prioridade desc" );
            ResultSet* rset = instrucao->executeQuery();
            while ( rset->next() ) {
                contextos.push_back ( new Contexto ( rset->getInt ( 1 ), rset->getString ( 2 ), rset->getInt ( 3 ) ) );
            }
            instrucao->closeResultSet ( rset );
        } catch ( SQLException e ) {
            std::cout << e.what();
        }
    }
    return contextos;
}

std::vector< Referencia* > BDados::getReferencias ( Utilizador& utilizador, string from, string where, string order ) {
    vector<Referencia*> referencias;
    if ( mEstado == BASEDADOS_ESTADO_LIGADO ) {
        try {
            if ( from.length() != 0 ) from = ", " + from;
            if ( where.length() != 0 ) where = " and " + where;
            if ( order.length() != 0 ) order = " order by " + order;
            stringstream ss;
            ss << "Select id, designacao, dataCriacao, descricao from Referencia" << from << " where utilizador = " << utilizador.getId() << " and id not in (select idreferencia from Tarefa)" << where << order;
            instrucao = ligacao->createStatement ( ss.str() );
            ResultSet* rset = instrucao->executeQuery();
            while ( rset->next() ) {
                Referencia* ref = new Referencia ( rset->getInt ( 1 ), rset->getString ( 2 ), Data ( rset->getDate ( 3 ) ), vector<Contexto*>(), "" );
                ref->setDescricao ( rset->getString ( 4 ) );
                referencias.push_back ( ref );
            }
        } catch ( SQLException e ) {
            std::cout << e.what();
        }
    }
    return referencias;
}

std::vector< Tarefa* > BDados::getTarefas ( Utilizador& utilizador, string from, string where, string order ) {
	// TODO simplificar usando o getQuaisquerTarefas
    vector<Tarefa*> tarefas;
    if ( mEstado == BASEDADOS_ESTADO_LIGADO ) {
        try {
            if ( from.length() != 0 ) from = ", " + from;
            if ( where.length() != 0 ) where = " and " + where;
            if ( order.length() != 0 ) order = " order by " + order;
            stringstream ss;
            ss << "Select t.id, idReferencia, designacao, dataInicio, dataFim, progresso, duracaoPrevista, prioridade, delegacao, visibilidade, dataCriacao, descricao from Tarefa t, Referencia r" << from << " where r.id = t.idReferencia and utilizador=" << utilizador.getId() << where << order;
            //cout << ss.str() << endl;
            instrucao = ligacao->createStatement ( ss.str() );
            ResultSet* rset = instrucao->executeQuery();
            while ( rset->next() ) {
                Tarefa* t = new Tarefa ( rset->getInt ( 1 ), rset->getInt ( 2 ), rset->getString ( 3 ), ( rset->isNull ( 4 ) ? Data() : Data ( rset->getDate ( 4 ) ) ), ( rset->isNull ( 5 ) ? Data() : Data ( rset->getDate ( 5 ) ) ), rset->getInt ( 7 ), rset->getInt ( 6 ), rset->getInt ( 8 ), rset->getInt ( 10 ), ( rset->isNull ( 9 ) ? NULL : ( Delegacao* ) 1 ), NULL, vector<Contexto*>(), "", ( rset->isNull ( 11 ) ? Data() : Data ( rset->getDate ( 11 ) ) ) );
                t->setDescricao ( rset->getString ( 12 ) );
                tarefas.push_back ( t );
            }
            instrucao->closeResultSet ( rset );
        } catch ( SQLException e ) {
            std::cout << e.what();
        }
    }
    return tarefas;
}

std::vector< Tarefa* > BDados::getTarefas ( Utilizador& utilizador, Contexto& contexto, string where, string order ) {
	// TODO simplificar usando o getQuaisquerTarefas
    vector<Tarefa*> tarefas;
    if ( mEstado == BASEDADOS_ESTADO_LIGADO ) {
        try {
            stringstream ss;
            ss << utilizador.getId();
            stringstream ssc;
            ssc << contexto.getId();
            if ( order.length() == 0 )
                instrucao = ligacao->createStatement ( "Select t.id, idReferencia, designacao, dataInicio, dataFim, progresso, duracaoPrevista, prioridade, delegacao, visibilidade, dataCriacao, descricao from Tarefa t, Referencia r, Referencia_Contexto c where r.id = t.idReferencia and utilizador='" + ss.str() + "' and c.referencia = r.id and c.contexto = " + ssc.str() + ( where.length() == 0 ? "" : " and " + where ) );
            else
                instrucao = ligacao->createStatement ( "Select t.id, idReferencia, designacao, dataInicio, dataFim, progresso, duracaoPrevista, prioridade, delegacao, visibilidade, dataCriacao, descricao from Tarefa t, Referencia r, Referencia_Contexto c where r.id = t.idReferencia and utilizador='" + ss.str() + "' and c.referencia = r.id and c.contexto = " + ssc.str() + ( where.length() == 0 ? "" : " and " + where ) + " order by " + order );
//             cout << "Select t.id, idReferencia, designacao, dataInicio, dataFim, progresso, duracaoPrevista, prioridade, delegacao, visibilidade, dataCriacao from Tarefa t, Referencia r, Referencia_Contexto c where r.id = t.idReferencia and utilizador='" + ss.str() + "' and c.referencia = r.id and c.contexto = " + ssc.str() + ( where.length() == 0 ? "" : " and " + where ) + " order by " + order;
            ResultSet* rset = instrucao->executeQuery();
            while ( rset->next() ) {
                Tarefa* t = new Tarefa ( rset->getInt ( 1 ), rset->getInt ( 2 ), rset->getString ( 3 ), ( rset->isNull ( 4 ) ? Data() : Data ( rset->getDate ( 4 ) ) ), ( rset->isNull ( 5 ) ? Data() : Data ( rset->getDate ( 5 ) ) ), rset->getInt ( 7 ), rset->getInt ( 6 ), rset->getInt ( 8 ), rset->getInt ( 10 ), ( rset->isNull ( 9 ) ? NULL : ( Delegacao* ) 1 ), NULL, vector<Contexto*>(), "", ( rset->isNull ( 11 ) ? Data() : Data ( rset->getDate ( 11 ) ) ) );
                t->setDescricao ( rset->getString ( 12 ) );
                tarefas.push_back ( t );
            }
            instrucao->closeResultSet ( rset );
        } catch ( SQLException e ) {
            std::cout << e.what();
        }
    }
    return tarefas;
}

std::vector< Tarefa* > BDados::getQuaisquerTarefas ( string from, string where, string order ) {
    vector<Tarefa*> tarefas;
    if ( mEstado == BASEDADOS_ESTADO_LIGADO ) {
        try {
            if ( from.length() != 0 ) from = ", " + from;
            if ( where.length() != 0 ) where = " and " + where;
            if ( order.length() != 0 ) order = " order by " + order;
            stringstream ss;
            ss << "Select t.id, idReferencia, designacao, dataInicio, dataFim, progresso, duracaoPrevista, prioridade, delegacao, visibilidade, dataCriacao, descricao from Tarefa t, Referencia r" << from << " where r.id = t.idReferencia " << where << order;
            //cout << ss.str() << endl;
            instrucao = ligacao->createStatement ( ss.str() );
            ResultSet* rset = instrucao->executeQuery();
            while ( rset->next() ) {
                Tarefa* t = new Tarefa ( rset->getInt ( 1 ), rset->getInt ( 2 ), rset->getString ( 3 ), ( rset->isNull ( 4 ) ? Data() : Data ( rset->getDate ( 4 ) ) ), ( rset->isNull ( 5 ) ? Data() : Data ( rset->getDate ( 5 ) ) ), rset->getInt ( 7 ), rset->getInt ( 6 ), rset->getInt ( 8 ), rset->getInt ( 10 ), ( rset->isNull ( 9 ) ? NULL : ( Delegacao* ) 1 ), NULL, vector<Contexto*>(), "", ( rset->isNull ( 11 ) ? Data() : Data ( rset->getDate ( 11 ) ) ) );
                t->setDescricao ( rset->getString ( 12 ) );
                tarefas.push_back ( t );
            }
            instrucao->closeResultSet ( rset );
        } catch ( SQLException e ) {
            std::cout << e.what();
        }
    }
    return tarefas;
}

int BDados::criarTarefa ( Referencia* referencia, Utilizador& u ) {
    if ( mEstado != BASEDADOS_ESTADO_LIGADO ) return -1;

    try {
        stringstream s;
        if ( typeid ( Projecto ) == typeid ( *referencia ) ) {
            Projecto *t = ( Projecto* ) referencia;
            if ( t->getContextos().size() == 1 ) {
                s << "declare resultado number;\nbegin\n\tresultado := criarProjecto(" << u.getId() << " , '" << t->getDescricao() << "' , '" << t->getDesignacao() << "' , " << t->getDataInicio().getFormatoOracle() << " , " << t->getDataFim().getFormatoOracle () << " , " << t->getDuracaoPrevista() << " , " << t->getPrioridade() << " , null , " << t->getVisibilidade() << ", " << t->getContextos() [0]->getId() << ", -1);\nend;";
            } else if ( t->getContextos().size() == 2 ) {
                s << "declare resultado number;\nbegin\n\tresultado := criarProjecto(" << u.getId() << " , '" << t->getDescricao() << "' , '" << t->getDesignacao() << "' , " << t->getDataInicio().getFormatoOracle () << " , " << t->getDataFim().getFormatoOracle () << " , " << t->getDuracaoPrevista() << " , " << t->getPrioridade() << " , null , " << t->getVisibilidade() << ", " << t->getContextos() [0]->getId() << ", " << t->getContextos() [1]->getId() << ");\nend;";
            }
        } else if ( typeid ( Tarefa ) == typeid ( *referencia ) ) {
            Tarefa *t = ( Tarefa* ) referencia;
            if ( t->getContextos().size() == 1 ) {
                s << "declare resultado number;\nbegin\n\tresultado := criarTarefa(" << u.getId() << " , '" << t->getDescricao() << "' , '" << t->getDesignacao() << "' , " << t->getDataInicio().getFormatoOracle() << " , " << t->getDataFim().getFormatoOracle () << " , " << t->getDuracaoPrevista() << " , " << t->getPrioridade() << " , null , " << t->getVisibilidade() << ", " << t->getContextos() [0]->getId() << ", -1);\nend;";
            } else if ( t->getContextos().size() == 2 ) {
                s << "declare resultado number;\nbegin\n\tresultado := criarTarefa(" << u.getId() << " , '" << t->getDescricao() << "' , '" << t->getDesignacao() << "' , " << t->getDataInicio().getFormatoOracle () << " , " << t->getDataFim().getFormatoOracle () << " , " << t->getDuracaoPrevista() << " , " << t->getPrioridade() << " , null , " << t->getVisibilidade() << ", " << t->getContextos() [0]->getId() << ", " << t->getContextos() [1]->getId() << ");\nend;";
            }
        } else {
            //cout << t.getContextos().size() << endl;
            if ( referencia->getContextos().size() == 1 ) {
                s << "declare resultado number;\nbegin\n\tresultado := criarReferencia(" << u.getId() << " , '" << referencia->getDesignacao() << "' , " << referencia->getContextos() [0] ->getId() << " , " << "-1 , '" << referencia->getDocumentoAssociado() << "' , '" << referencia->getDescricao() << "');\nend;";
            } else if ( referencia->getContextos().size() == 2 ) {
                s << "declare resultado number;\nbegin\n\tresultado := criarReferencia(" << u.getId() << " , '" << referencia->getDesignacao() << "' , " << referencia->getContextos() [0]->getId() << " , " << referencia->getContextos() [1] ->getId() << " , '" << referencia->getDocumentoAssociado() << "' , '" << referencia->getDescricao() << "');\nend;";
            }
        }
        //cout << s.str() << endl;
        instrucao = ligacao->createStatement ( s.str() );
        instrucao->executeUpdate();
        ligacao->commit();
        return 1;
    } catch ( SQLException e ) {
        std::cout << e.what();
        return 0;
    }
}

int BDados::criarConta ( Utilizador& u ) {
    if ( mEstado != BASEDADOS_ESTADO_LIGADO ) return -1;

    try {
        if ( u.getNomeUtilizador().length() == 0 ) return -2;

        if ( u.getNome().length() == 0 ) return -3;

        if ( u.getEmail().length() == 0 ) return -4;

        instrucao = ligacao->createStatement ( "declare resultado number;\nbegin\n\tresultado := criarUtilizador('" + u.getNomeUtilizador() + "', '" + u.getPalavraPasseEncriptada() + "', '" + u.getNome() + "', '" + u.getApelido() + "', '" + u.getEmail() + "');\nend;" );
        instrucao->executeQuery();
        ligacao->commit();
        return 1;
    } catch ( SQLException e ) {
        std::cout << e.what();
        return 0;
    }
}

int BDados::criarContexto ( Contexto &c , Utilizador &u ) {
    if ( mEstado != BASEDADOS_ESTADO_LIGADO ) return -1;

    if ( c.getDesignacao().length() == 0 ) return -2;

    if ( c.getPrioridade() == 0 || c.getPrioridade() == 100 ) return -3;

    try {
        stringstream s;
        string statement;
        s << c.getDesignacao();
        statement = "Insert into Contexto (designacao, prioridade, utilizador) values ('";
        statement += s.str();
        statement += "' , ";
        s.str ( std::string() );
        s << c.getPrioridade();
        statement += s.str();
        statement += " , ";
        s.str ( std::string() );
        s << u.getId();
        statement += s.str();
        statement += ")";
        instrucao = ligacao->createStatement ( statement );
        instrucao->executeUpdate();
        ligacao->commit();
    } catch ( SQLException e ) {
        std::cout << e.what();
        return 0;
    }
}

int BDados::editarTarefa ( Referencia *t ) {
    if ( mEstado != BASEDADOS_ESTADO_LIGADO ) return -1;
    try {
        stringstream s;
        s << "Update Referencia set descricao = '" << t->getDescricao() << "' , designacao = '" << t->getDesignacao() << "' where id = " << t->getIdReferencia();
        ligacao->createStatement ( s.str() )->executeUpdate();
        s.str ( "" );
        s << "delete from Referencia_Contexto where referencia = " << t->getIdReferencia();
        ligacao->createStatement ( s.str() )->executeUpdate();
        int len = t->getContextos().size();
        for ( int i = 0; i < len; i++ ) {
            s.str ( "" );
            s << "insert into Referencia_Contexto (Referencia, Contexto) values (" << t->getIdReferencia() << ", " << t->getContextos() [i]->getId() << ")";
            ligacao->createStatement ( s.str() )->executeUpdate();
        }
        s.str ( "" );
        s << "delete from Referencia_Ficheiro where referencia = " << t->getIdReferencia();
        ligacao->createStatement ( s.str() )->executeUpdate();
        if ( t->getDocumentoAssociado().length() != 0 ) {
            s.str ( "" );
            s << "insert into referencia_ficheiro (Referencia, Ficheiro) values (" << t->getIdReferencia() << ", '" << t->getDocumentoAssociado() << "')";
            ligacao->createStatement ( s.str() )->executeUpdate();
        }
        if ( typeid ( Tarefa ) == typeid ( *t ) || typeid ( Projecto ) == typeid ( *t ) ) {
            // tarefa
            Tarefa * a = ( Tarefa* ) t;
            s.str ( "" );
            s << "Update Tarefa set datainicio = " << a->getDataInicio().getFormatoOracle() << " , datafim = " << a->getDataFim().getFormatoOracle() << " , duracaoprevista = " << a->getDuracaoPrevista() << " , prioridade = " << a->getPrioridade() << " , visibilidade = " << a->getVisibilidade() << " , progresso = " << a->getProgresso() <<  " where id = " << a->getId();
            ligacao->createStatement ( s.str() )->executeUpdate();
            ligacao->commit();
            return 1;
        }
        ligacao->commit();
        return 1;
    } catch ( SQLException e ) {
        std::cout << e.what();
        ligacao->rollback();
        return 0;
    }
}

int BDados::actualizarTarefasProjecto ( Projecto* projecto ) {
    if ( mEstado != BASEDADOS_ESTADO_LIGADO ) return -1;
    try {
		stringstream s;
		s << "delete from Projecto_Tarefa where idProjecto = " << projecto->getIdProjecto();
        ligacao->createStatement ( s.str() )->executeUpdate();
		int len = projecto->getTarefas().size();
		for(int i = 0; i < len; i++) {
			s.str(string());
			s << "insert into Projecto_Tarefa (id, idProjecto) values (" << projecto->getTarefas()[i]->getId() << ", " << projecto->getIdProjecto() << ")";
			ligacao->createStatement ( s.str() )->executeUpdate();
		}
        ligacao->commit();
        return 1;
    } catch ( SQLException e ) {
        std::cout << e.what();
        ligacao->rollback();
        return 0;
    }
}

/**
 * Preenche alguns atributos da referencia/tarefa/projecto que não são carregados através do getTarefas()
 */
bool BDados::completarDadosTarefa ( Utilizador& u, Referencia*& ref ) {
    if ( mEstado == BASEDADOS_ESTADO_LIGADO ) {
        try {
            stringstream s;
            s << ref->getIdReferencia();
            ref->setContextos ( getContextos ( u, " Referencia_Contexto t", "c.id = t.contexto and t.referencia = " + s.str(), "c.designacao" ) );
            if ( typeid ( Tarefa ) == typeid ( *ref ) || typeid ( Projecto ) == typeid ( *ref ) ) {
                stringstream ss;
                ss << "Select delegacao, dependeDe from Tarefa t where t.id=" << ( ( Tarefa* ) ref )->getId();
                instrucao = ligacao->createStatement ( ss.str() );
                ResultSet* rset = instrucao->executeQuery();
                // TODO poderá haver objectos que não são apagados da memória
                if ( rset->next() ) {
                    // carregar delegacao
                    if ( rset->isNull ( 1 ) ) ( ( Tarefa* ) ref )->setDelegacao ( NULL ); // não há delegação
                    else {
                        // há delegação
                        s.str ( string() );
                        s << "select utilizador, aceite from delegacao where id=" << rset->getInt ( 1 );
                        instrucao = ligacao->createStatement ( s.str() );
                        ResultSet* rset2 = instrucao->executeQuery();
                        if ( rset2->next() ) {
                            if ( !rset2->isNull ( 1 ) ) { // nao esta delegado com alguem fora do sistema
                                Utilizador* u = NULL;
                                int idU = rset2->getInt ( 1 );
                                s.str ( string() );
                                s << "select nomeUtilizador, nome, apelido, email from utilizadores where id=" << idU;
                                instrucao = ligacao->createStatement ( s.str() );
                                ResultSet* rset3 = instrucao->executeQuery();
                                if ( rset3->next() ) { // carregar info do utilizador a quem esta tarefa foi delegada
                                    u = new Utilizador ( idU, rset3->getString ( 1 ), rset3->getString ( 2 ), rset3->getString ( 3 ), rset3->getString ( 4 ) );
                                }
                                instrucao->closeResultSet ( rset3 );
                                ( ( Tarefa* ) ref )->setDelegacao ( new Delegacao ( rset2->getInt ( 2 ) == 1, u ) );
                            } else {
                                ( ( Tarefa* ) ref )->setDelegacao ( new Delegacao() ); // esta delegado com alguem fora do sistema
                            }
                        }
                        instrucao->closeResultSet ( rset2 );
                        rset->getInt ( 1 );
                    }
                    // carrega tarefa que depende desta
                    ( ( Tarefa* ) ref )->setDependenteDe ( NULL );
                    if ( !rset->isNull ( 2 ) ) {
                        s.str ( string() );
                        s << "t.id = ";
                        s << rset->getInt ( 2 );
                        vector<Tarefa*> tarefa = getTarefas ( u, string(), s.str() );
                        if ( tarefa.size() != 0 )
                            ( ( Tarefa* ) ref )->setDependenteDe ( tarefa [0] );
                    }
                }
                instrucao->closeResultSet ( rset );
                s.str(string());
                s <<"Select id from Projecto where idTarefa=" << ((Tarefa*)ref)->getId();
                instrucao = ligacao->createStatement ( s.str() );
                rset = instrucao->executeQuery();
                if ( rset->next() ) { // verificar se é projecto
                    Projecto* p = new Projecto ( rset->getInt ( 1 ), *((Tarefa*)ref) );
                    ref = p; // carregar tarefas do projecto
					stringstream idP;
					idP << ((Projecto*)ref)->getIdProjecto();
					((Projecto*)ref)->setTarefas(getTarefas(u, "", "t.id in (select id from projecto_tarefa where idProjecto = " + idP.str() + ")"));
                }
                instrucao->closeResultSet ( rset );
            }
            return true;
        } catch ( SQLException e ) {
            std::cout << e.what();
            return false;
        }
    }
    return false;
}

int BDados::eliminarTarefa ( Referencia* ref ) {
    if ( mEstado != BASEDADOS_ESTADO_LIGADO ) return -1;
    try {
        stringstream s;
        if ( typeid ( Projecto ) == typeid ( *ref ) ) {
            s << "delete from projecto_tarefa where idProjecto = " << ( ( Projecto* ) ref )->getIdProjecto();
            ligacao->createStatement ( s.str() )->executeUpdate();
            s.str ( "" );
            s << "delete from projecto where id = " << ( ( Projecto* ) ref )->getIdProjecto();
            ligacao->createStatement ( s.str() )->executeUpdate();
        }
        if ( typeid ( Projecto ) == typeid ( *ref ) || typeid ( Tarefa ) == typeid ( *ref ) ) {
            s.str ( "" );
            s << "delete from delegacao where tarefa = " << ( ( Tarefa* ) ref )->getId();
            ligacao->createStatement ( s.str() )->executeUpdate();
            s.str ( "" );
            s << "delete from tarefa where id = " << ( ( Tarefa* ) ref )->getId();
            ligacao->createStatement ( s.str() )->executeUpdate();
        }
        s.str ( "" );
        s << "delete from referencia_ficheiro where referencia = " << ( ( Projecto* ) ref )->getIdReferencia();
        ligacao->createStatement ( s.str() )->executeUpdate();
        s.str ( "" );
        s << "delete from referencia_contexto where referencia = " << ( ( Projecto* ) ref )->getIdReferencia();
        ligacao->createStatement ( s.str() )->executeUpdate();
        s.str ( "" );
        s << "delete from referencia where id = " << ( ( Projecto* ) ref )->getIdReferencia();
        ligacao->createStatement ( s.str() )->executeUpdate();
        ligacao->commit();
    } catch ( SQLException e ) {
        std::cout << e.what();
        return false;
    }
}

int BDados::definirDelegacao ( Utilizador& u, Tarefa* t ) {
    if ( mEstado != BASEDADOS_ESTADO_LIGADO ) return -1;
    try {
        stringstream s;
        if ( t->getDelegacao() ) {
            if ( t->getDelegacao()->getUtilizador() ) s << "declare resultado number;\nbegin\n\tresultado := criarDelegacao(" << t->getDelegacao()->getUtilizador()->getId() << ", " << t->getId() << ", 0);\nend;";
            else s << "declare resultado number;\nbegin\n\tresultado := criarDelegacao(null, " << t->getId() << ", 0);\nend;";
        } else s << "declare resultado number;\nbegin\n\tresultado := criarDelegacao(null, " << t->getId() << ", -1);\nend;";
        ligacao->createStatement ( s.str() )->executeUpdate();
        ligacao->commit();
        return 1;
    } catch ( SQLException e ) {
        std::cout << e.what();
        return 0;
    }
}

int BDados::editarContexto ( Contexto &c ) {
    if ( mEstado != BASEDADOS_ESTADO_LIGADO ) return -1;

    if ( c.getDesignacao().length() == 0 ) return -2;

    if ( c.getPrioridade() == 0 || c.getPrioridade() == 100 ) return -3;

    try {
        stringstream s;
        string statement;
        s << c.getDesignacao();
        statement = "Update Contexto set Designacao = '";
        statement += s.str();
        statement += "' , Prioridade = ";
        s.str ( std::string() );
        s << c.getPrioridade();
        statement += s.str();
        statement += " where Id = ";
        s.str ( std::string() );
        s << c.getId();
        statement += s.str();
        instrucao = ligacao->createStatement ( statement );
        instrucao->executeUpdate();
        ligacao->commit();
    } catch ( SQLException e ) {
        std::cout << e.what();
        return 0;
    }
}

int BDados::eliminarContexto ( Contexto &c, Utilizador &u ) {
    if ( mEstado != BASEDADOS_ESTADO_LIGADO ) return -1;
    try {
        vector<Tarefa*> mTarefas = getTarefas ( u,c );
		Referencia* f;
        for ( int i = 0; i< mTarefas.size() ; i++ ) {
			f = mTarefas[i];
            completarDadosTarefa ( u,f );
            if ( f->getContextos().size() == 1 ) {
                eliminarTarefa ( f );
            }
        }
        stringstream s;
        s << "delete from referencia_contexto where contexto = " << c.getId();
        ligacao->createStatement ( s.str() )->executeUpdate();
        s.str ( "" );
        s << "delete from contexto where id = " << c.getId();
        ligacao->createStatement ( s.str() )->executeUpdate();
        ligacao->commit();
		return 1;
    } catch ( SQLException e ) {
        std::cout << e.what() << endl;
		return 0;
    }
}

/**
 * Converte o projecto p numa tarefa ou referência
 * @param tipo 0 =  referencia, 1 = tarefa
 */
int BDados::despromoverProjecto ( Projecto* p, int tipo ) {
    if ( mEstado != BASEDADOS_ESTADO_LIGADO ) return -1;
    try {
        stringstream s;
        s << "delete from projecto where id = " << p->getIdProjecto();
        ligacao->createStatement ( s.str() )->executeUpdate();
        if ( tipo == 0 ) {
            s.str ( string() );
            s << "delete from tarefa where id = " << p->getId();
            ligacao->createStatement ( s.str() )->executeUpdate();
        }
        ligacao->createStatement ( s.str() )->executeUpdate();
        ligacao->commit();
    } catch ( SQLException e ) {
        std::cout << e.what();
        return false;
    }
}

/**
 * Converte a a tarefa t numa referência
 * @param tipo 0 =  referencia
 */
int BDados::despromoverTarefa ( Tarefa* t, int tipo ) {
    if ( mEstado != BASEDADOS_ESTADO_LIGADO ) return -1;
    try {
        stringstream s;
        s << "delete from tarefa where id = " << t->getId();
        ligacao->createStatement ( s.str() )->executeUpdate();
        ligacao->commit();
    } catch ( SQLException e ) {
        std::cout << e.what();
        return false;
    }
}

/**
 * Converte a referência r numa tarefa ou projecto
 * @param tipo 1 = tarefa, 2 = projecto
 */
int BDados::promoverReferencia ( Utilizador& u, Referencia*& r, int tipo ) {
    if ( mEstado != BASEDADOS_ESTADO_LIGADO ) return -1;
    try {
        stringstream s;
        s << "insert into tarefa (idReferencia, duracaoPrevista, prioridade) values (" << r->getIdReferencia() << ", " << sugestaoDuracao ( r->getDesignacao(), u ) << ", 50)";
        ligacao->createStatement ( s.str() )->executeUpdate();
        s.str ( string() );
        s << r->getIdReferencia();
        vector<Tarefa*> ts = getTarefas ( u, string(), "idReferencia = " + s.str() );
        if ( ts.size() != 0 ) {
            r = ts[0];
            ligacao->commit();
        }
    } catch ( SQLException e ) {
        std::cout << e.what();
        return false;
    }
}

/**
 * Converte a tarefa r num projecto
 * @param tipo 2 = projecto
 */
int BDados::promoverTarefa ( Utilizador& u, Tarefa*& r, int tipo ) {
    if ( mEstado != BASEDADOS_ESTADO_LIGADO ) return -1;
    try {
        stringstream s;
        s << "insert into projecto (idTarefa) values (" << r->getId() << ")";
        ligacao->createStatement ( s.str() )->executeUpdate();
        s.str ( string() );
        s <<"Select id from Projecto where idTarefa=" << r->getId();
        instrucao = ligacao->createStatement ( s.str() );
        ResultSet* rset = instrucao->executeQuery();
        if ( rset->next() ) {
            Projecto* p = new Projecto ( rset->getInt ( 1 ), *r );
            r = p;
            ligacao->commit();
        }
    } catch ( SQLException e ) {
        std::cout << e.what();
        return false;
    }
}

int BDados::criarAmizade ( Utilizador & amigo, Utilizador & u ) {
    try {
        if ( mEstado != BASEDADOS_ESTADO_LIGADO ) return -1;
        if ( amigo.getNomeUtilizador().length() == 0 ) return -2;

        stringstream ss;
        ss <<"Select * from Utilizadores where nomeUtilizador='" << amigo.getNomeUtilizador() << '\'';
        cout << ss.str();
        instrucao = ligacao->createStatement ( ss.str() );
        ResultSet* rset = instrucao->executeQuery();
        if ( rset->next() ) { // pedir à BD a primeira linha, caso o utilizador não exista nada acontecerá
            ss.str ( "" ); //supondo que isto limpa
            ss << "Insert into Amizade (idUtilizador,idAmigo, estado) values (" << u.getId() << ", " << rset->getInt ( 1 ) << ", " << 1 << ')';
            cout << ss.str();
            instrucao = ligacao->createStatement ( ss.str() );
            instrucao->executeUpdate();
            ligacao->commit();
        } else return -4;
        return 1;
    } catch ( SQLException e ) {
        std::cout << e.what();
        return 0;
    }
}

int BDados::sugestaoDuracao ( string designacao, Utilizador &u ) {
    int devolve = 0;
    try {
        stringstream s;
        s << "Select avg(t.duracaoprevista) from Tarefa t, Referencia r where r.designacao = '" << designacao << "' and t.idreferencia = r.id and r.utilizador = " << u.getId();
        instrucao = ligacao->createStatement ( s.str() );
        ResultSet * rset = instrucao->executeQuery();
        if ( rset->next() && !rset->isNull ( 1 ) ) {
            devolve = rset->getInt ( 1 );
            int razao, temp;
            if ( devolve % 60 != 0 && devolve > 60 ) {
                razao = devolve / 60;
                temp = 60 * ( razao+1 );
                razao = temp - devolve;
                devolve += razao;
            }
        }
        instrucao->closeResultSet ( rset );
    } catch ( SQLException e ) {
        std::cout << e.what();
    }
    return devolve;
}

int BDados::eliminarAmizade ( Utilizador & amigo, Utilizador & u ) {
    if ( mEstado != BASEDADOS_ESTADO_LIGADO ) return -1;
    try {
        stringstream ss;
        ss << "Delete from Amizade where idAmigo = " << amigo.getId() << " and idUtilizador = " << u.getId();
        cout << ss.str() << endl;
        instrucao = ligacao->createStatement ( ss.str() );
        instrucao->executeUpdate();
        ligacao->commit();
        return 1;
    } catch ( SQLException e ) {
        std::cout << e.what();
        return 0;
    }
}

vector< Amizade* > BDados::getAmigos ( Utilizador& utilizador ) {
    vector<Amizade*> amigos;
    if ( mEstado == BASEDADOS_ESTADO_LIGADO ) {
        try {
            stringstream ss;
            ss << "select a.estado, u.nomeUtilizador, u.nome, u.apelido, u.email, u.id from amizade a, utilizadores u where a.idamigo = u.id and a.idutilizador = " << utilizador.getId();
            instrucao = ligacao->createStatement ( ss.str() );
            ResultSet* rset = instrucao->executeQuery();
            //cout << ss.str() << endl;
            while ( rset->next() ) {
                amigos.push_back ( new Amizade ( new Utilizador ( rset->getInt ( 6 ), rset->getString ( 2 ), rset->getString ( 3 ), rset->getString ( 4 ), rset->getString ( 5 ) ), rset->getInt ( 1 ) ) );
            }
            instrucao->closeResultSet ( rset );
        } catch ( SQLException e ) {
            std::cout << e.what();
        }
    }
    return amigos;
}

int BDados::setDelegacoesPorAceitar(int v, Utilizador &u) {
	stringstream s,ss;
	vector<Tarefa*> tarefas;
	switch(v) {
		case 1:
			s << "update delegacao set aceite = 1 where utilizador = " << u.getId();
			instrucao = ligacao->createStatement(s.str());
			instrucao->executeUpdate();
			break;
		case 0:
			tarefas = getDelegacoesPorAceitar(u);
			for(int i = 0; i < tarefas.size(); i++) {
				s << "update tarefa set delegacao = null where id = " << tarefas[i]->getId();
				instrucao = ligacao->createStatement(s.str());
				instrucao->executeUpdate();
				s.str("");
				s << "delete from delegacao where tarefa = " << tarefas[i]->getId();
				instrucao = ligacao->createStatement(s.str());
				instrucao->executeUpdate();
			}
			break;
	}
	ligacao->commit();
	return 0;
}

vector<Tarefa*> BDados::getDelegacoesPorAceitar(Utilizador &u) {
	vector<Tarefa*> tarefas;
	if( mEstado == BASEDADOS_ESTADO_LIGADO ) {
		try {
			stringstream s;
			s << "select tarefa , aceite from delegacao where utilizador = " << u.getId() << " and aceite = 0";
			instrucao = ligacao->createStatement(s.str());
			ResultSet * rset = instrucao->executeQuery();
			
			while(rset->next()) {
				int tarefa = rset->getInt(1);
				int aceite = 0;
				stringstream ss;
				ss << "t.id = " << tarefa;
				Referencia * t = getQuaisquerTarefas(string(), ss.str())[0];
				completarDadosTarefa(u, t);
				tarefas.push_back((Tarefa*) t);
			}
			instrucao->closeResultSet(rset);
		} catch (SQLException e) {
			std::cout << e.what();
		}
	}
	return tarefas;
}

int BDados::getEstado() {
    return mEstado;
}

BDados::~BDados() {
    env->terminateConnection ( ligacao );
    Environment::terminateEnvironment ( env );
}

#endif



