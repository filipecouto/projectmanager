#ifndef COMUNICACAO_H
#define COMUNICACAO_H

#include "../BaseDados/BDados.h"
#include "../Dados/Tarefa.h"
#include "../Dados/Referencia.h"

class Comunicacao {
    public:
    Comunicacao ( Comunicacao* contexto );
    static Comunicacao* criarContexto();
    string getPastaDados();

    void exportarCsv ( vector<Tarefa*> tarefas, string fileName );
    void exportarICal ( string ficheiro, Tarefa * tarefa );

    void setModoEdicao ( Referencia* alvo, int tipo );
    int getModoEdicao() const;
    Referencia* getAlvoEdicao() const;

    private:
    Comunicacao();
    BDados* mBaseDados;
    Utilizador* mUtilizador;

    int* mModoEdicao;
    Referencia** mAlvo;

    protected:
    BDados* getBaseDados();
    Utilizador* getUtilizador();
    void setUtilizador ( Utilizador* utilizador );
};

Comunicacao::Comunicacao ( Comunicacao* contexto ) {
    mBaseDados = contexto->mBaseDados;
    mUtilizador = contexto->mUtilizador;
	mModoEdicao = contexto->mModoEdicao;
	mAlvo = contexto->mAlvo;
}

Comunicacao::Comunicacao() : mBaseDados ( new BDados ( "\"B1-4\"", "qwerty", "gandalf.dei.isep.ipp.pt:1521/isepdb" ) ), mUtilizador ( NULL ), mModoEdicao(new int(0)), mAlvo(new Referencia*)
{ }

Referencia* Comunicacao::getAlvoEdicao() const {
    return *mAlvo;
}

int Comunicacao::getModoEdicao() const {
    return *mModoEdicao;
}

void Comunicacao::setModoEdicao ( Referencia* alvo, int tipo ) {
    *mAlvo = alvo;
    *mModoEdicao = tipo;
}

Comunicacao* Comunicacao::criarContexto() {
    return new Comunicacao();
}

BDados* Comunicacao::getBaseDados() {
    return mBaseDados;
}

Utilizador* Comunicacao::getUtilizador() {
    return mUtilizador;
}

void Comunicacao::setUtilizador ( Utilizador* utilizador ) {
    mUtilizador = utilizador;
}

void Comunicacao::exportarCsv ( vector<Tarefa*> tarefas, string fileName ) {
    ofstream myfile;
    myfile.open ( fileName );
    Tarefa* tempT;
    Contexto* tempC;
    vector<Contexto*> doisContextos;
    int* vectorDeContextos;
    vectorDeContextos = ( int* ) malloc ( 1* ( sizeof ( int ) ) );
    vectorDeContextos[0] = 0;
    while ( !tarefas.empty() ) {
        tempT = tarefas.front();
        myfile << tempT->getId() << ",";
        myfile << tempT->getDesignacao() << ",";
        myfile << tempT->getPrioridade() << ",";

        doisContextos = tempT->getContextos();
        while ( !doisContextos.empty() ) {
            tempC = doisContextos.front();
            if ( tempC->getId() > vectorDeContextos[0] ) {
                vectorDeContextos = ( int* ) malloc ( ( 1+tempC->getId() ) * ( sizeof ( int ) ) );
                for ( int i = ( 1 + vectorDeContextos[0] ); i < tempC->getId(); i++ ) {
                    vectorDeContextos[i] = 0;
                }
                vectorDeContextos[tempC->getId()] = 1;
                vectorDeContextos[0] = tempC->getId();
            }

        }
        for ( int i = 1; i < ( 1 + vectorDeContextos[0] ); i++ ) {
            myfile << vectorDeContextos[i];
        }
        myfile << "\n";
    }
    myfile.close();
}

void Comunicacao::exportarICal ( string ficheiro, Tarefa* tarefa ) {
    ofstream f ( ( ficheiro + ".ics" ), ios::out );
    f << "BEGIN:VCALENDAR\nVERSION:1.0\nBEGIN:VEVENT\nSTATUS:TENTATIVE\nDTSTART:" <<
    tarefa->getDataInicio().getAno() <<tarefa->getDataInicio().getMes() << tarefa->getDataInicio().getDia()
    << 'T'  << tarefa->getDataInicio().getHora() <<
    tarefa->getDataInicio().getMinuto() <<tarefa->getDataInicio().getSegundo() <<
    "Z\nDTEND:" << tarefa->getDataFim().getAno() <<tarefa->getDataFim().getMes() <<
    tarefa->getDataFim().getDia() <<'T' <<tarefa->getDataFim().getHora() <<
    tarefa->getDataFim().getMinuto() << tarefa->getDataFim().getSegundo() <<
    "Z\nSUMMARY:"+ tarefa->getDesignacao() << "\nDESCRIPTION:"<< tarefa->getDescricao()
    << "\nCLASS:PRIVATE\nEND:VEVENT\nEND:VCALENDAR";
    f.close();



}
string Comunicacao::getPastaDados() {
#ifdef __GNUC__
    return string ( getenv ( "HOME" ) ) + "/";
#elif defined(_WIN32)
    return string ( getenv ( "HOMEDRIVE" ) ) + '/' + getenv ( "HOMEDRIVE" ) + "/";
#endif
}

#endif
