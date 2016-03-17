#ifndef Data_
#define Data_

#include <sstream>
#include <occi.h>

namespace oracle {
    namespace occi {
        class Date;
    }
}

using namespace oracle::occi;

class Data {
    private:
    int ano;			// qualquer ano
    unsigned int mes;			// 1-12
    unsigned int dia;			// 1-31 dependente do mês
    unsigned int hora;			//hora de 0 a 23
    unsigned int min;			// minutos de 0 a 59
    unsigned int seg;			//segundos de 0 a 59
    boolean mValido;

    static int diasPorMes[];
    int validaDia ( unsigned int d ) const; // Confirma o valor do dia baseado no mes e ano
    int validaMes ( unsigned int m ) const;
    int validaAno ( int a ) const;
    int validaHora ( unsigned int h ) const;
    int validaMinuto ( unsigned int m ) const;
    int validaSegundo ( unsigned int s ) const;
    bool anoBissexto ( int a ) const; // Testa se ano é bissexto

    public:

    Data();
    Data ( const Date data );
    Data ( string& data );
    Data ( int a, int m, int d );
    Data ( int a, int m , int d, int h, int mm, int s );
    Data ( const Data &d );
    ~Data() ;
	
	static int simplificarTempoDeSegundos( int& tempo );

    void setValido ( boolean );
    boolean getValido();
    void setAno ( int a );
    void setMes ( unsigned int m );
    void setDia ( unsigned int d );
    void setHora ( unsigned int h );
    void setMinuto ( unsigned int m );
    void setSegundo ( unsigned int s );
    void setData ( Data dt );
    int getAno() const ;
    unsigned int getMes() const;
    unsigned int getDia() const;
    unsigned int getHora() const;
    unsigned int getMinuto() const;
    unsigned int getSegundo() const;
    void listar() const ;
    string listarstr() const;
    static int obterSegAno ( int ano1,int ano2 );
    static int obterSegMes ( int mes1,int mes2 );
    static int obterSegDia ( int dia1, int dia2 );
    static int obterSegHoras ( int hora1, int hora2 );
    static int obterSegMin ( int min1, int min2 );
    static int obterSegSeg ( int seg1, int seg2 );

    string getFormatoOracle( );
    Date getFormatoOracle ( const Environment* envp );

	void somarXsegundos(int x);
	int substrair (Data d) const;
	bool operator< ( Data d2 ) const;
	bool operator> ( Data d2 ) const;
	bool operator== ( Data d2 ) const;
};


int Data::diasPorMes[]=	{0,31,28,31,30,31,30,31,31,30,31,30,31};

void Data::setValido ( boolean val ) {
    mValido = val;
}

boolean Data::getValido() {
    return mValido;
}

Data::Data() {
    setValido ( false );
    setAno ( 2000 );
    setMes ( 1 );
    setDia ( 1 );
    setHora ( 0 );
    setMinuto ( 0 );
    setSegundo ( 0 );
}

Data::Data ( const Data& d ) {
    setValido ( d.mValido );
    setAno ( d.ano );
    setMes ( d.mes );
    setDia ( d.dia );
    setHora ( d.hora );
    setMinuto ( d.min );
    setSegundo ( d.seg );
}

Data::Data ( const Date data ) {
    setValido ( true );
    data.getDate ( ano, mes, dia, hora, min, seg );
}

Data::Data ( string& data ) {
    if ( data.length() == 0 ) {
        setValido ( false );
    } else {
        stringstream ss ( data );
        string dia , mes , ano, hora, min, seg;

        int a, m, d, h, mm, s;
        char c, c1, c2, c3;

        ss >> d >> c >> m >> c >> a; // >> h >> c >> mm >> c >> s;
		if(ss.fail()) {
			setValido(false);
			return;
		}
        setValido ( true );

        setAno ( a < 100 ? 2000 + a : a );
        setMes ( m );
        setDia ( d );
        setHora ( 0 );
        setMinuto ( 0 );
        setSegundo ( 0 );
    }
}

// Confirma o valor do mes e chama o metodo validaDia
// para confirmar o valor do dia.
Data::Data ( int a, int m, int d ) {
    setValido ( true );
    setAno ( a );
    setMes ( m );
    setDia ( d );
}

Data::Data ( int a, int m , int d, int h, int mm, int s ) {
    setValido ( true );
    setAno ( a );
    setMes ( m );
    setDia ( d );
    setHora ( h );
    setMinuto ( mm );
    setSegundo ( s );
}

Data::~Data() {
}

int Data::simplificarTempoDeSegundos ( int& tempo ) {
	if(tempo == 0) return 1;
	if(!(tempo % 60)) {
		tempo /= 60;
		if(!(tempo % 60)) {
			tempo /= 60;
			if(!(tempo % 24)) {
				tempo /= 24;
				return 4;
			}
			return 3;
		}
		return 2;
	}
	return 1;
}

string Data::getFormatoOracle() {
    if ( !mValido ) return "null";
    stringstream s;
	s << "to_date('{" << dia << '-' << mes << '-' << ano << ' ' << hora << ':' << min << ':' << seg << "}', '{DD-MM-YYYY HH24:MI:SS}')";
    return s.str();
}

Date Data::getFormatoOracle ( const Environment* envp ) {
    return Date ( envp, ano, mes, dia, hora, min, seg );
}

// Outros métodos
void Data::setAno ( int a ) {
    ano = validaAno ( a ) ;
}

void  Data::setMes ( unsigned int m ) {
    mes = validaMes ( m ); // valida o mes
}

void  Data::setDia ( unsigned int d ) {
    dia = validaDia ( d );	// valida o dia
}

void Data::setHora ( unsigned int h ) {
    hora = validaHora ( h );
}

void Data::setMinuto ( unsigned int m ) {
    min = validaMinuto ( m );
}

void Data:: setSegundo ( unsigned int s ) {
    seg = validaSegundo ( s );
}

void Data::setData ( Data dt ) {
    setAno ( dt.ano ) ;
    setMes ( dt.mes );
    setDia ( dt.dia );
}

int Data::getAno() const {
    return ano;
}

unsigned int Data::getMes() const {
    return mes;
}

unsigned int Data::getDia() const {
    return dia;
}

unsigned int Data::getHora() const {
    return hora;
}

unsigned int Data::getMinuto() const {
    return min;
}

unsigned int Data::getSegundo() const {
    return seg;
}

void Data::listar() const {
    cout << dia << '/' << mes << '/' << ano << ' ' << hora << ':' << ( min > 9 ? min : 0 << min ) << ':' << ( seg > 9 ? seg : 0 << seg );
}

string Data::listarstr() const {
    if ( mValido ) {
        stringstream s;
        s << dia << '/' << mes << '/' << ano << ' ' << hora << ':' << ( min > 9 ? min : 0 << min ) << ':' << ( seg > 9 ? seg : 0 << seg );
        return s.str();
    } else {
        return "";
    }

}

// Confirma o valor do dia baseado no mes e ano.
int Data::validaDia ( unsigned int d ) const {
    if ( d > 0 && d <= diasPorMes[ mes ] )
        return d;

    if ( mes == 2 && d == 29 && anoBissexto ( ano ) ) // se Fevereiro: Verifica se ano bissexto
        return d;
    return 1;  // Deixa o objecto num estado consistente
}

int Data::validaMes ( unsigned int m ) const {
    if ( m > 0 && m <= 12 )		// valida o mes
        return m;
    else
    {
        // cout<<"\nMês inválido -> mes=1";
        return  1;
    }
}

int Data::validaAno ( int a ) const {
    if ( a < 0 ) {
        // cout<<"\nAno negativo - inválido -> ano=0";
        return 0;
    } else
        return a;
}

int Data::validaHora ( unsigned int h ) const {
    if ( h >= 0 && h <= 23 )		// valida o mes
        return h;
    else
    {
        // cout<<"\nHora inválida -> hora=0";
        return  1;
    }
}

int Data::validaMinuto ( unsigned int m ) const {
    if ( m >= 0 && m <= 59 )		// valida o mes
        return m;
    else
    {
        // cout<<"\nMinuto inválido -> minuto=0";
        return  1;
    }
}
int Data::validaSegundo ( unsigned int s ) const {
    if ( s >= 0 && s <= 59 )		// valida o mes
        return s;
    else
    {
        // cout<<"\nSegundo inválido -> segundo=0";
        return  1;
    }
}
// Testa se ano é bissexto
bool Data::anoBissexto ( int a ) const {
    return ( a % 400 == 0 || a % 4 == 0 && a % 100 != 0 ) ;
}

int Data::obterSegAno ( int ano1,int ano2 ) {
    int anos = ano2-ano1;
    return anos * 31556926;
}
int Data:: obterSegMes ( int mes1,int mes2 ) {
    int meses =0;
    if ( mes1<mes2 ) {
        meses = mes2-mes1;
    } else {
        meses = mes2 - mes1;
    }
    return meses * ( 2629743.83 );
}

int Data::obterSegDia ( int dia1, int dia2 ) {
    int dias =0;
    if ( dia1<dia2 ) {
        dias = dia2-dia1;
    } else {
        dias =  dia1 - dia2;
    }
    return dias * 86400;
}

int Data:: obterSegHoras ( int hora1, int hora2 ) {
    int horas =0;
    if ( hora1<hora2 ) {
        horas = hora2 - hora1;
    } else {
        horas = hora1 - hora2;
    }
    return horas * 3600;
}

int Data::obterSegMin ( int min1, int min2 ) {
    int mins =0;
    if ( min1<min2 ) {
        mins = min2 - min1;
    } else {
        mins = min1 - min2;
    }
    return mins * 60;
}

int Data::obterSegSeg ( int seg1, int seg2 ) {
    int segs =0;
    if ( seg1<seg2 ) {
        segs = seg2 - seg1;
    } else {
        segs = seg1 - seg2;
    }
    return segs;
}

void Data::somarXsegundos ( int x ) {
	seg = seg + x;
	while ( seg > 59 ) {
		if ( seg > 59 ) {
			seg = seg - 60;
			min = min + 1;
		}
		if ( min > 59 ) {
			min = min - 60;
			hora = hora + 1;
		}
		if ( hora > 23 ) {
			hora = hora - 24;
			dia = dia + 1;
		}
		if ( dia > diasPorMes[mes] ) {
			dia = dia - diasPorMes[mes];
			mes = mes + 1;
		}
		if ( mes > 12) {
			mes = mes - 12;
			ano = ano + 1;
		}
	}
}

int Data::substrair ( Data d ) const {
	int res = (ano - d.ano) * 31536000;
	res += (mes - d.mes) * 2592000;
	res += (dia - d.dia) * 86400;
	res += (hora - d.hora) * 3600;
	res += (min - d.min) * 60;
	res += seg - d.seg;
	return res;
}

bool Data::operator< ( Data d2 )  const {
	if ( ano < d2.ano ) {
		return true;
	} else {
		return false;
	}
	if ( mes < d2.mes ) {
		return true;
	} else {
		return false;
	}
	if ( dia < d2.dia ) {
		return true;
	} else {
		return false;
	}
	if ( hora < d2.hora ) {
		return true;
	} else {
		return false;
	}
	if ( min < d2.min ) {
		return true;
	} else {
		return false;
	}
	if ( seg < d2.seg ) {
		return true;
	}

	return false;
}

bool Data::operator> ( Data d2 )  const {
	if ( ano > d2.ano ) {
		return true;
	} else {
		return false;
	}
	if ( mes > d2.mes ) {
		return true;
	} else {
		return false;
	}
	if ( dia > d2.dia ) {
		return true;
	} else {
		return false;
	}
	if ( hora > d2.hora ) {
		return true;
	} else {
		return false;
	}
	if ( min > d2.min ) {
		return true;
	} else {
		return false;
	}
	if ( seg > d2.seg ) {
		return true;
	}

	return false;
}

bool Data::operator== ( Data d2 )  const {
	if ( ano == d2.ano && mes == d2.mes && dia == d2.dia && hora == d2.hora && min == d2.min && seg == d2.seg) {
		return true;
	}
	return false;
}

#endif
