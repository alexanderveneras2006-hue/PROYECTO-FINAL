#include <iostream>
#include <string>

using namespace std;

// --- VARIABLES GLOBALES COMPARTIDAS Y DE LA VARIANTE ---
char tablero[3][3];
int hX[3], hO[3];
int cantX, cantO;
char marcadorActual;
int jugadorActual;

// --- FUNCIONES COMUNES ---

// Reinicia el tablero a su estado original (1-9)
void inicializarTablero() {
    char inicio = '1';
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            tablero[i][j] = inicio++;
        }
    }
    // Reiniciar contadores de la variante
    cantX = 0;
    cantO = 0;
    for(int i=0; i<3; i++) { hX[i] = 0; hO[i] = 0; }
}

void dibujarTablero(string titulo) {
    cout << "\n--- " << titulo << " ---" << endl;
    cout << " Jugador 1: X  |  Jugador 2: O " << endl << endl;
    for (int i = 0; i < 3; i++) {
        cout << "  " << tablero[i][0] << "  |  " << tablero[i][1] << "  |  " << tablero[i][2] << endl;
        if (i < 2) cout << "_____|_____|_____" << endl;
    }
    cout << endl;
}

int verificarGanador() {
    for (int i = 0; i < 3; i++) {
        if (tablero[i][0] == tablero[i][1] && tablero[i][1] == tablero[i][2]) return 1;
        if (tablero[0][i] == tablero[1][i] && tablero[1][i] == tablero[2][i]) return 1;
    }
    if (tablero[0][0] == tablero[1][1] && tablero[1][1] == tablero[2][2]) return 1;
    if (tablero[0][2] == tablero[1][1] && tablero[1][1] == tablero[2][0]) return 1;
    return 0;
}

// --- MODALIDAD 1: JUEGO NORMAL ---
void juegoNormal() {
    inicializarTablero();
    jugadorActual = 1;
    marcadorActual = 'X';
    int ganador = 0;

    for (int turno = 0; turno < 9 && ganador == 0; turno++) {
        dibujarTablero("MODO NORMAL");
        int casilla;
        cout << "Jugador " << jugadorActual << " (" << marcadorActual << "), elige casilla: ";
        cin >> casilla;

        int f = (casilla - 1) / 3;
        int c = (casilla - 1) % 3;

        if (casilla < 1 || casilla > 9 || tablero[f][c] == 'X' || tablero[f][c] == 'O') {
            cout << "Movimiento invalido. Intenta de nuevo." << endl;
            turno--; 
            continue;
        }

        tablero[f][c] = marcadorActual;
        if (verificarGanador()) {
            ganador = jugadorActual;
        } else {
            jugadorActual = (jugadorActual == 1) ? 2 : 1;
            marcadorActual = (marcadorActual == 'X') ? 'O' : 'X';
        }
    }

    dibujarTablero("FIN DEL JUEGO");
    if (ganador != 0) cout << "¡Felicidades! Ganó el Jugador " << ganador << endl;
    else cout << "¡Es un empate!" << endl;
}

// --- MODALIDAD 2: JUEGO VARIANTE (3 FICHAS MAX) ---
void juegoVariante() {
    inicializarTablero();
    jugadorActual = 1;
    marcadorActual = 'X';
    bool juegoTerminado = false;

    while (!juegoTerminado) {
        dibujarTablero("MODO VARIANTE (MAX 3 FICHAS)");
        int eleccion;
        cout << "Jugador " << jugadorActual << " (" << marcadorActual << "), elige casilla: ";
        cin >> eleccion;

        int f = (eleccion - 1) / 3;
        int c = (eleccion - 1) % 3;

        if (eleccion < 1 || eleccion > 9 || tablero[f][c] == 'X' || tablero[f][c] == 'O') {
            cout << "Movimiento invalido. Intenta de nuevo." << endl;
            continue;
        }

        if (jugadorActual == 1) {
            if (cantX == 3) {
                int vieja = hX[0];
                tablero[(vieja-1)/3][(vieja-1)%3] = (char)(vieja + '0');
                hX[0] = hX[1]; hX[1] = hX[2]; hX[2] = eleccion;
            } else {
                hX[cantX++] = eleccion;
            }
        } else {
            if (cantO == 3) {
                int vieja = hO[0];
                tablero[(vieja-1)/3][(vieja-1)%3] = (char)(vieja + '0');
                hO[0] = hO[1]; hO[1] = hO[2]; hO[2] = eleccion;
            } else {
                hO[cantO++] = eleccion;
            }
        }

        tablero[f][c] = marcadorActual;

        if (verificarGanador()) {
            dibujarTablero("FIN DEL JUEGO");
            cout << "¡Felicidades! Ganó el Jugador " << jugadorActual << endl;
            juegoTerminado = true;
        } else {
            jugadorActual = (jugadorActual == 1) ? 2 : 1;
            marcadorActual = (marcadorActual == 'X') ? 'O' : 'X';
        }
    }
}

// --- MENÚ PRINCIPAL ---
int main() {
    int opcion;
    do {
        cout << "======= MENU 3 EN RAYA =======" << endl;
        cout << "1. Juego Clásico" << endl;
        cout << "2. Juego Variante (Máximo 3 fichas)" << endl;
        cout << "3. Salir" << endl;
        cout << "Seleccione una opcion: ";
        cin >> opcion;

        switch(opcion) {
            case 1: juegoNormal(); break;
            case 2: juegoVariante(); break;
            case 3: cout << "Saliendo del programa..." << endl; break;
            default: cout << "Opcion no valida." << endl;
        }
        cout << "\nPresiona una tecla para continuar...";
        cin.ignore();
        cin.get();
    } while (opcion != 3);

    return 0;
}
