/***************************************************
 * Gestor de Tarefas - Ecra.h                      *
 ***************************************************
 * Este ficheiro contém a declaração e             *
 * implementação da classe Utilizador que armazena *
 * informações relacionadas com um dado utilizador *
 ***************************************************
 * CONTEÚDO                                        *
 * Tamanho                                         *
 * Ecra                                            *
 *  bool getTamanho (Tamanho& tamanho)             *
 *  bool setNegrito()                              *
 *  bool setSublinhado()                           *
 *  bool setCor(int cor)                           *
 *  bool setCorFundo(int cor)                      *
 *  bool setPiscar()                               *
 *  bool setPosicao(int coluna, int linha)         *
 *  bool reporFormatacao()                         *
 ***************************************************/
#ifndef ECRA_H
#define ECRA_H

#ifdef __GNUC__
#include <sys/ioctl.h>
#include <stdio.h>
#include <unistd.h>
#endif

using namespace std; 
typedef struct Tamanho Tamanho;

struct Tamanho {
	int largura, altura;
};

#ifdef _WIN32
		HANDLE hconsole = GetStdHandle(STD_OUTPUT_HANDLE);;
		int actual_cor = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE;
		int actual_fundo = 0;
#endif
class EcraBase {
	public:
		EcraBase() {
		
		}
		
		/**
		 * Devolve o tamanho actual da consola
		 * @param tamanho um objecto do tipo Tamanho onde as informações ficarão guardadas
		 * @return true se foi possível obter o tamanho, senão devolve false
		 */
		bool getTamanho(Tamanho& tamanho) {
#ifdef __GNUC__
#ifdef TIOCGSIZE
			struct ttysize ts;
			ioctl(STDIN_FILENO, TIOCGSIZE, &ts);
			tamanho.largura = ts.ts_cols;
			tamanho.altura = ts.ts_lines;
			return true;
#elif defined(TIOCGWINSZ)
			struct winsize ts;
			ioctl(STDIN_FILENO, TIOCGWINSZ, &ts);
			tamanho.largura = ts.ws_col;
			tamanho.altura = ts.ws_row;
			return true;
#endif
#elif defined(_WIN32)
			/*RECT a;
			HWND window = GetConsoleWindow();
			GetWindowRect(window , &a);
			tamanho.largura = a.right;
			cout << "Left: " << a.left << "\t";
			cout << "Right: " << a.right << "\t";
			cout << "Bottom: " << a.bottom << "\t";
			cout << "Top: " << a.top << "\t";
			tamanho.altura = a.bottom;
*/
			CONSOLE_SCREEN_BUFFER_INFO csbi;
            int columns, rows;

            GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
            columns = csbi.srWindow.Right - csbi.srWindow.Left;
            rows = csbi.srWindow.Bottom - csbi.srWindow.Top;
			/*cout << "Colunas: " << columns;
			cout << "  Linhas: " << rows << endl;*/
			tamanho.largura = columns;
			tamanho.altura = rows;
			return true;
#endif
			return false;
		}

		/**
		 * Todo o texto impresso depois da chamada a este método será em negrito e com uma côr mais intensa
		 * @return true se foi possível mudar a formatação, senão devolve false
		 */
		bool setNegrito() {
#ifdef __GNUC__
			std::cout << "\033[1m";
			return true;
#elif defined(_WIN32)
			
			SetConsoleTextAttribute(hconsole, actual_cor | FOREGROUND_INTENSITY);
			return true;
#endif
			return false;
		}
			
		/**
		 * Todo o texto impresso depois da chamada a este método será sublinhado, apenas compatível com o Linux
		 * @return true se foi possível mudar a formatação, senão devolve false
		 */
		bool setSublinhado() {
#ifdef __GNUC__
			std::cout << "\033[4m";
			return true;
#elif defined(_WIN32)
#endif
			return false;
		}

		/**
		 * Todo o texto impresso depois da chamada a este método será escrito com a cor especificada
		 * @param cor a nova cor, entre 0 a 7
		 * @return true se foi possível mudar a formatação, senão devolve false
		 */
		bool setCor(int cor) {
#ifdef __GNUC__
			std::cout << "\033[" << (30 + cor) << 'm';
			return true;
#elif defined(_WIN32)
			SetConsoleTextAttribute(hconsole, cor | actual_fundo);
			actual_cor = cor;
			return true;
#endif
			return false;
		}

		/**	
		 * Todo o texto impresso depois da chamada a este método será escrito com a cor de fundo especificada
		 * @param cor a nova cor, entre 0 a 7
		 * @return true se foi possível mudar a formatação, senão devolve false
		 */
		bool setCorFundo(int cor) {
#ifdef __GNUC__
			std::cout << "\033[" << (40 + cor) << 'm';
			return true;
#elif defined(_WIN32)
			switch(cor) {
				case 7: cor = BACKGROUND_BLUE | BACKGROUND_GREEN | BACKGROUND_RED;
						break;
			}
			SetConsoleTextAttribute(hconsole, actual_cor | cor);
			actual_fundo = cor;
			return true;
#endif
			return false;
		}

		bool setPiscar() {
#ifdef __GNUC__
			std::cout << "\033[5m";
			return true;
#elif defined(_WIN32)
#endif
			return false;
		}

		bool limparEcra() {
#ifdef __GNUC__
			std::cout << "\033[2J";
			return true;
#elif defined(_WIN32)
			system("cls");
			return true;
#endif
			return false;
		}

		bool setPosicao(int coluna, int linha) {
#ifdef __GNUC__
			std::cout << "\033[" << linha + 1 << ';' << coluna + 1 << 'H';
			return true;
#elif defined(_WIN32)
			COORD posicao = { coluna , linha };
			SetConsoleCursorPosition(hconsole,posicao);
			//cout << " Linha << " << linha << " Coluna " << coluna << endl;
			
			
			return true;
#endif
			return false;

		}

		bool reporFormatacao() {
#ifdef __GNUC__
			std::cout << "\033[0m";
			return true;
#elif defined(_WIN32)
			SetConsoleTextAttribute(hconsole, FOREGROUND_GREEN | FOREGROUND_RED | FOREGROUND_BLUE);
			actual_cor = FOREGROUND_GREEN | FOREGROUND_RED | FOREGROUND_BLUE;
			actual_fundo = 0;
			return true;
#endif
			return false;
		}

		virtual ~EcraBase() {}
	private:
};

#endif // ECRA_H
