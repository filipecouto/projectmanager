#ifndef ECRALISTA_H
#define ECRALISTA_H

#include <algorithm>
#include <iomanip>
#include "EcraBase.h"
#include "Accao.h"

using namespace std;

class EcraLista : public EcraBase {
    public:
    EcraLista ( );
    EcraLista ( string titulo );

    virtual void mostrar();

    void setTitulo ( string titulo );
    string getTitulo();

    void setMargemTopo ( int margemTopo );
    int getMargemTopo();

    void juntarAccao ( Accao* accao );
    void juntarAccaoOculta ( Accao* accao );
    Accao* getAccao ( char letra );

    void fechar();

    private:
    vector<Accao*> mAccoes;
    vector<Accao*> mAccoesOcultas;
    bool mVisivel;
    string mTitulo;
    int mMargemTopo;

    bool testarSelecao ( string texto, int& posCampo );

    protected:
    virtual void executarAccao ( Accao* accao ) {}

    virtual int getNumColunas() {
        return 0;
    }
    virtual int getNumLinhas() {
        return 0;
    }
    virtual float getLarguraColuna ( int coluna ) {
        return 0;
    }
    virtual string getNomeColuna ( int coluna ) {
        return "";
    }

    virtual void imprimirCelula ( int coluna, int linha ) {}
    virtual void itemSelecionado ( int linha ) {}

    virtual void imprimirCabecalho () {}
};

EcraLista::EcraLista ( ) : mVisivel ( true ), mMargemTopo ( 0 ) {
}

EcraLista::EcraLista ( string titulo ) : mVisivel ( true ), mTitulo ( titulo ), mMargemTopo ( 0 ) {
}

void EcraLista::mostrar() {
    int i, x, y;
    bool b;
    string c;
    string s;
    Tamanho tamanhoConsola;
    int posCampo;
    int nAccoes;
    do {
		nAccoes = mAccoes.size();
        limparEcra();
        reporFormatacao();
        getTamanho ( tamanhoConsola );
        setPosicao ( 3, 1 );
        setNegrito();
        cout << mTitulo;
        int nc = getNumColunas();
        int nl = getNumLinhas();
        int* larguras = new int[nc];
        for ( i = 0; i < nc; i++ )
            larguras[i] = ( int ) ( getLarguraColuna ( i ) * ( tamanhoConsola.largura-3 ) );
        if ( mMargemTopo != 0 ) { setPosicao(3, 2); imprimirCabecalho(); }
        setNegrito();
        x = 3;
        y = 3 + mMargemTopo;
        for ( i = 0; i < nc; i++ ) {
            setPosicao ( x, y );
            cout << getNomeColuna ( i );
            x += larguras[i];
        }
        reporFormatacao();
        for ( i = 0; i < nl; i++ ) {
            y++;
            x = 3;
            setPosicao ( 0, y );
            reporFormatacao();
            setNegrito();
            cout << setw ( 2 ) << i+1;
            reporFormatacao();
            for ( int t = 0; t < nc; t++ ) {
                setPosicao ( x, y );
                imprimirCelula ( t, i );
                x += larguras[t];
            }
        }
        free ( larguras );
        reporFormatacao();
        setPosicao ( 0, tamanhoConsola.altura );
        setCorFundo ( 7 );
        setCor ( 0 );
        for ( int i = 0; i < nAccoes; i++ ) {
            printf ( "%*s", -tamanhoConsola.largura / nAccoes, ( string() + mAccoes[i]->getLetra() + " - " +  mAccoes[i]->getTitulo() ).c_str() );
        }
        reporFormatacao();
        setNegrito();
        setPosicao ( 0, tamanhoConsola.altura - 2 );
        cout << "Escolha uma opção ou item da lista: ";
        reporFormatacao();
        cin >> c;
        if ( !testarSelecao ( c, posCampo ) && c[0] >= 'a' && c[0] <= 'z' ) {
            c[0] -= 32;
            testarSelecao ( c, posCampo );
        }
    } while ( mVisivel );
}

bool EcraLista::testarSelecao ( string texto, int& posCampo ) {
    char l = texto[0];
    if ( l >= '0' && l <= '9' ) {
        stringstream s ( texto );
        int i;
        s >> i;
        i--;
        if ( i >= 0 && i < getNumLinhas() ) {
            itemSelecionado ( i );
            return true;
        }
    }
    int len = mAccoes.size();
    for ( int i = 0; i < len; i++ ) {
        if ( mAccoes[i]->getLetra() == l ) {
            executarAccao ( mAccoes[i] );
            return true;
        }
    }
    len = mAccoesOcultas.size();
    for ( int i = 0; i < len; i++ ) {
        if ( mAccoesOcultas[i]->getLetra() == l ) {
            executarAccao ( mAccoesOcultas[i] );
            return true;
        }
    }
    return false;
}

int EcraLista::getMargemTopo() {
    return mMargemTopo;
}

void EcraLista::setMargemTopo ( int margemTopo ) {
    mMargemTopo = margemTopo;
}

string EcraLista::getTitulo() {
    return mTitulo;
}

void EcraLista::setTitulo ( string titulo ) {
    mTitulo = titulo;
}

Accao* EcraLista::getAccao ( char letra ) {
    int len = mAccoes.size();
    for ( int i = 0; i < len; i++ ) if ( mAccoes[i]->getLetra() == letra ) return mAccoes[i];
    len = mAccoesOcultas.size();
    for ( int i = 0; i < len; i++ ) if ( mAccoesOcultas[i]->getLetra() == letra ) return mAccoesOcultas[i];
}

void EcraLista::juntarAccao ( Accao* accao ) {
    mAccoes.push_back ( accao );
}

void EcraLista::juntarAccaoOculta ( Accao* accao ) {
    mAccoesOcultas.push_back ( accao );
}

void EcraLista::fechar() {
    mVisivel = false;
}

#endif
