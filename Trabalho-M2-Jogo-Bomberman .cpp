  /**
    Bomberman
    Trabalho M2
    Miguel Angel Morán
    Matheus Moritz Furlan
**/

#include <iostream>
#include <windows.h>
#include <conio.h>
#include <time.h>
#include <fstream>
#include <ctime>
#include <iomanip>

#define M_LINHAS 15
#define M_COLUNAS 15
#define SAVEGAME_FILE "savegame.txt"
#define MAPA1_FILE "mapa1.txt"
#define MAPA2_FILE "mapa2.txt"

using namespace std;

const int M = 3;

clock_t tempoInicioJogo; // Declare a variável global para controlar o tempo de jogo

bool verificaColisao(int m[M_LINHAS][M_COLUNAS], int x, int y) {
    if (m[x][y] == 0 || m[x][y] == 4) {
        return true;
    } else {
        return false;
    }
}

void bomba(int m[M_LINHAS][M_COLUNAS], int x, int y, bool* flagBomba) {
    if (!*flagBomba){//verifica se a bomba já existe, se não existe, coloca uma bomba
        *flagBomba = true; //altera a flag informando que agora existe uma bomba
        m[x][y] = 3; // coloca a bomba no mapa
    }

}

void movimentoInimigo(int m[M_LINHAS][M_COLUNAS], int& ex, int& ey) {
    int direcao = rand() % 4;  // escolhe uma direção aleatória
    int passo = rand() % 3 + 1; // escolhe um número aleatório de passos
    for (int i = 0; i < passo; i++) {
        if (direcao == 0 && verificaColisao(m, ex - 1, ey)) {
            ex--;
        } else if (direcao == 1 && verificaColisao(m, ex + 1, ey)) {
            ex++;
        } else if (direcao == 2 && verificaColisao(m, ex, ey - 1)) {
            ey--;
        } else if (direcao == 3 && verificaColisao(m, ex, ey + 1)) {
            ey++;
        } else {
            break;
        }
    }
}

int fimDeJogo() {
    cout << "\nFIM DE JOGO!";
    return 0;
}

///Imprime o jogo: mapa e personagem.
void imprimirMapa(int m[M_LINHAS][M_COLUNAS], int x, int y, int ex1, int ey1, int ex2, int ey2) {
    HANDLE out = GetStdHandle(STD_OUTPUT_HANDLE);
    for (int i = 0; i < M_LINHAS; i++) {
        for (int j = 0; j < M_COLUNAS; j++) {
            if (i == x && j == y) { SetConsoleTextAttribute(out, 14);
                cout << char(2); //personagem
            } else if (i == ex1 && j == ey1) { SetConsoleTextAttribute(out, 10);
                cout << char(153); // primeiro inimigo
            } else if (i == ex2 && j == ey2) { SetConsoleTextAttribute(out, 13);
                cout << char(154); // segundo inimigo
            } else {
                switch (m[i][j]) {
                    case 0: SetConsoleTextAttribute(out, 2); /// cor do caminho
                     cout << " "; break;  //caminho
                    case 1:  SetConsoleTextAttribute(out, 17); /// cor da parede solida
                    cout << char(219); break; //parede
                    case 2:  SetConsoleTextAttribute(out, 2); /// cor do caminho
                    cout << "#"; break; //parede quebrável
                    case 3:  SetConsoleTextAttribute(out, 8);/// cor da bomba
                    cout << char(207); break;//bomba
                    case 4:  SetConsoleTextAttribute(out, 12);/// cor quando explode a bomba
                    cout << "*"; break; //explosão da bomba
                }
            }
            SetConsoleTextAttribute(out, 15);
        }
        cout << "\n";
    } //fim for mapa
}

///----------///----------///----------///----------///-----------///-----------///-----------///

bool salvarJogo(int m[M_LINHAS][M_COLUNAS], int x, int y, int ex1, int ey1, int ex2, int ey2, bool flagBomba, int bx, int by, clock_t inicio, clock_t fim) {
    ofstream savegameFile(SAVEGAME_FILE, ios::out);
    if (savegameFile.is_open()) {
        savegameFile << x << " " << y << "\n";
        savegameFile << ex1 << " " << ey1 << "\n";
        savegameFile << ex2 << " " << ey2 << "\n";
        savegameFile << flagBomba << "\n";
        savegameFile << bx << " " << by << "\n";
        savegameFile << inicio << "\n";
        savegameFile << fim << "\n";

        for (int i = 0; i < M_LINHAS; i++) {
            for (int j = 0; j < M_COLUNAS; j++) {
                savegameFile << m[i][j] << " ";
            }
            savegameFile << "\n";
        }

        savegameFile.close();
        return true;
    }
    return false;
}

void mostrarRelogio() {
    clock_t tempoAtual = clock();
    double tempoDecorrido = static_cast<double>(tempoAtual - tempoInicioJogo) / CLOCKS_PER_SEC;
    cout << "Tempo de Jogo: " << setprecision(2) << fixed << tempoDecorrido << " segundos" << endl;
}

bool carregarJogo(int m[M_LINHAS][M_COLUNAS], int& x, int& y, int& ex1, int& ey1, int& ex2, int& ey2, bool& flagBomba, int& bx, int& by, clock_t& inicio, clock_t& fim) {
    ifstream savegameFile(SAVEGAME_FILE, ios::in);
    if (savegameFile.is_open()) {
        savegameFile >> x >> y >> ex1 >> ey1 >> ex2 >> ey2;
        savegameFile >> flagBomba >> bx >> by >> inicio >> fim;
        for (int i = 0; i < M_LINHAS; i++) {
            for (int j = 0; j < M_COLUNAS; j++) {
                savegameFile >> m[i][j];
            }
        }
        savegameFile.close();
        return true;
    }
    return false;
}

void carregarMapa(int m[M_LINHAS][M_COLUNAS], const char* nomeArquivo) {
    ifstream mapaFile(nomeArquivo, ios::in);
    if (mapaFile.is_open()) {
        for (int i = 0; i < M_LINHAS; i++) {
            for (int j = 0; j < M_COLUNAS; j++) {
                mapaFile >> m[i][j];
            }
        }
        mapaFile.close();
    } else {
        cerr << "Erro ao carregar o mapa do arquivo: " << nomeArquivo << endl;
        exit(1);
    }
}

// Declare uma variável global para controlar o estado do jogo
bool emPausa = false;

void iniciarNovoJogo() {
    int escolhaMapa;
    cout << "Escolha o mapa (1 para Mapa 1, 2 para Mapa 2): ";
    cin >> escolhaMapa;

    int m[M_LINHAS][M_COLUNAS];
    if (escolhaMapa == 1) {
        carregarMapa(m, MAPA1_FILE);
    } else if (escolhaMapa == 2) {
        carregarMapa(m, MAPA2_FILE);
    } else {
        cerr << "Escolha de mapa inválida. O jogo será encerrado." << endl;
        return;
    }

    system("cls");

    int x = 13, y = 1; //Posi��o inicial do personagem no console
    char tecla; //Variavel para tecla precionada
    int ex1 = 1, ey1 = 1; // posição do primeiro inimigo
    int ex2 = 13, ey2 = 13; // posição do segundo inimigo
    int contadorMovimentoInimigo = 0; // contador para controlar o movimento dos inimigos
    bool flagBomba = false; //Variável para verificar se já existe bomba
    int bx, by; //localização da bomba
    clock_t inicio, fim; //declaração das entidades de contagem de tempo para armazenar inicio e fim do período da bomba


    tempoInicioJogo = clock(); ///para iniciar a contagem do tempo:

     ///----------///-----------////-----------////----------///---------////----------////---------////---------

    while (true) {
        if (!emPausa) {
            SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), {0, 0});
            imprimirMapa(m, x, y, ex1, ey1, ex2, ey2);
            mostrarRelogio();

            contadorMovimentoInimigo++;
            if (contadorMovimentoInimigo == 30) { // move os inimigos a cada 10 iterações do loop
                movimentoInimigo(m, ex1, ey1);
                movimentoInimigo(m, ex2, ey2);
                contadorMovimentoInimigo = 0;
            }

///-----------///----------///---------///----------///------------///-----------///-----------///----------///


            if (m[x][y] == 4) { // condição de fim : explodiu com a bomba
                fimDeJogo();
                return;
            }
            if ((x == ex1 && y == ey1) || (x == ex2 and y == ey2)) {
                fimDeJogo();
                return;
            }
            if (m[ex1][ey1] == 4) { // o primeiro inimigo foi atingido pela explosão da bomba
                ex1 = -1;
            }
            if (m[ex2][ey2] == 4) { // o segundo inimigo foi atingido pela explosão da bomba
                ex2 = -1;
            }
            if (ex1 == -1 and ex2 == -1) { // verifica se o jogador matou todos os inimigos
                cout << "\nVOCÊ VENCEU!";
                return;
            }
///-----------///----------///---------///----------///------------///-----------///-----------///----------///
///executa os movimentos

            if (_kbhit()) {
                tecla = getch();
                switch (tecla) {
                     /// Switch para pausar o jogo, salvar o jogo, carregar o jogo, menu....
                    case 27: // Tecla 'Esc' para pausar o jogo
                        emPausa = true;
                        break;
                    case 83: case 'z': // Tecla 'Z' para salvar o jogo
                        if (salvarJogo(m, x, y, ex1, ey1, ex2, ey2, flagBomba, bx, by, inicio, fim)) {
                            cout << "\nJogo salvo com sucesso!" << endl;
                        } else {
                            cout << "\nErro ao salvar o jogo." << endl;
                        }
                        break;
                    case 'c': // Código para carregar o jogo
                        if (carregarJogo(m, x, y, ex1, ey1, ex2, ey2, flagBomba, bx, by, inicio, fim)) {
                            cout << "\nJogo carregado com sucesso!" << endl;
                        } else {
                            cout << "\nNenhum jogo salvo encontrado." << endl;
                        }
                        break;
                    case 'm': // Código para o menu
                        cout << "\nVoltando ao Menu Principal..." << endl;
                        emPausa = true;
                        return; // Encerra o jogo e retorna ao menu principal
                        break;

///-----------///----------///---------///----------///------------///-----------///-----------///----------///

                    case 72: case 'w': ///cima
                        if (verificaColisao(m, x - 1, y)) {
                            x--;
                        }
                        break;
                    case 80: case 's': ///baixo
                        if (verificaColisao(m, x + 1, y)) {
                            x++;
                        }
                        break;
                    case 75: case 'a': ///esquerda
                        if (verificaColisao(m, x, y - 1)) {
                            y--;
                        }
                        break;
                    case 77: case 'd': ///direita
                        if (verificaColisao(m, x, y + 1)) {
                            y++;
                        }
                        break;
                    case 32:
                        if (!flagBomba) {
                            inicio = clock();
                            bx = x;
                            by = y;
                        }
                        bomba(m, x, y, &flagBomba);
                        break;
                }
            }

///-----------///----------///---------///----------///------------///-----------///-----------///----------///


            if (flagBomba) {
                fim = clock();
                if ((fim - inicio) / CLOCKS_PER_SEC == 3) {
                    m[bx][by] = 4;
                    if (m[bx + 1][by] == 0 || m[bx + 1][by] == 2) m[bx + 1][by] = 4;
                    if (m[bx - 1][by] == 0 || m[bx - 1][by] == 2) m[bx - 1][by] = 4;
                    if (m[bx][by + 1] == 0 || m[bx][by + 1] == 2) m[bx][by + 1] = 4;
                    if (m[bx][by - 1] == 0 || m[bx][by - 1] == 2) m[bx][by - 1] = 4;
                }

                if ((fim - inicio) / CLOCKS_PER_SEC == 4) {
                    m[bx][by] = 0;
                    if (m[bx + 1][by] != 1) m[bx + 1][by] = 0;
                    if (m[bx - 1][by] != 1) m[bx - 1][by] = 0;
                    if (m[bx][by + 1] != 1) m[bx][by + 1] = 0;
                    if (m[bx][by - 1] != 1) m[bx][by - 1] = 0;
                    flagBomba = false;
                }
            }
            //fim do laço do jogo
        } else {
            // O jogo está pausado, você pode exibir uma mensagem na tela e esperar pela tecla de continuação.
            cout << "Jogo Pausado. Pressione 'P' para continuar...";
            while (true) {
                if (_kbhit()) {
                    tecla = getch();
                    if (tecla == 'p' || tecla == 'P') {
                        system("cls");
                        emPausa = false;
                        break;
                    }
                }
            }
        }
    }
}

///-----------///----------///---------///----------///------------///-----------///-----------///----------///


int main() {
    HANDLE out = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO cursorInfo;
    GetConsoleCursorInfo(out, &cursorInfo);
    cursorInfo.bVisible = false;
    SetConsoleCursorInfo(out, &cursorInfo);

    short int CX = 0, CY = 0;
    COORD coord;
    coord.X = CX;
    coord.Y = CY;

    int m[M_LINHAS][M_COLUNAS];
    int x, y, ex1, ey1, ex2, ey2;
    bool flagBomba;
    int bx, by;
    clock_t inicio, fim;

    char escolha;

    while (true) {
        system("cls");
        cout << "Bomberman" << endl;
        cout << "1. Novo Jogo" << endl;
        cout << "2. Carregar Jogo" << endl;
        cout << "3. Sair" << endl;

        if (emPausa) {
            cout << "4. Continuar" << endl; // Adicione a opção de continuar apenas quando o jogo estiver em pausa.
        }

        escolha = _getch();

        switch (escolha) {
            case '1':
                iniciarNovoJogo();
                break;
            case '2':
                if (carregarJogo(m, x, y, ex1, ey1, ex2, ey2, flagBomba, bx, by, inicio, fim)) {
                    iniciarNovoJogo();
                } else {
                    cout << "\nNenhum jogo salvo encontrado." << endl;
                }
                break;
            case '3':
                return 0;
                break;
            default:
                break;
            case '4':
                if (emPausa) {
                    emPausa = false;
                }
                break;
        }
    }
    return 0;
}
//fim main
