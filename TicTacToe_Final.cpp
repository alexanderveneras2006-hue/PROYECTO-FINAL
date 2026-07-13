#include <iostream>
#include <vector>
#include <string>
#include <iomanip>
#include <limits>
#include <fstream> 
#include <stack>  
#include <ctime>   
#include <cstdlib> 

using namespace std;

/*Se define colores mediante la Tabla de codigo de color ANSI para una mejor 
presentacion y visualizacion del juego en terminales compatibles con estos codigos.
Esto permite resaltar diferentes elementos del juego como jugadores, mensajes y bordes.*/
#define VERDE   "\033[32m"
#define MAGENTA "\033[35m"
#define ROJO    "\033[31m"
#define CIAN    "\033[36m"
#define AMARILLO "\033[33m"
#define RESET   "\033[0m"

/*Variable global para almacenar el nombre del jugador durante la sesión.
Se define de manera global para facilitar su acceso en las funciones de registro
y mantener la consistencia del nombre a lo largo de toda la ejecución del programa.*/
string jugador_actual = "Invitado";

/*Estructura para guardar movimientos en el sistema undo. Esta estructura almacena 
la fila, columna y el jugador (X o O) que realizó el movimiento, permitiendo 
deshacer movimientos paso a paso manteniendo la información necesaria para restaurar
el estado anterior del tablero.*/
struct Movimiento {
    int fila;
    int columna;
    char jugador;
};

/*Estructura para la logica de la computadora. Se utiliza para representar 
posiciones en el tablero mediante coordenadas fila (r) y columna (c) de forma
compacta, facilitando el manejo de posiciones en los algoritmos de la IA.*/
struct Pos { int r, c; };

/*Definicion de puntero a funcion para manejar turnos (Humano vs Computadora).
Este tipo de función permitirá asignar dinámicamente la función de movimiento 
para cada jugador, dependiendo de si es humano o computadora, haciendo el código
más modular y extensible para futuros modos de juego.*/
typedef void (*FuncionMovimiento)(vector<vector<char>>*, int, char, int&, int&, stack<Movimiento>&);

/*Proceso para limpiar la pantalla, con el fin de obtener un juego visualmente mas limpio 
definimos la variable como void para que ejecute una unica accion mediante las directivas 
corresponcientes a un compilador windows (_WIN32) u otro dependiendo en donde se ejecute.
Esto asegura compatibilidad multiplataforma sin modificar la lógica del juego.*/
void Limpiar() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

/*Proceso para centrar texto, con el fin de obtener un juego visualmente atractivo
utilizamos funciones como "setfill" y "setw" que imprimiran el texto alineado y centrado 
dependiendo del tamaño del texto y el ancho utilizado. Esto mejora la presentación
de menús, títulos y mensajes en la interfaz del juego.*/
void centrar(const string &texto, int ancho) {
    int espacios = (ancho - (int)texto.length()) / 2;
    if (espacios < 0) espacios = 0;
    cout << setfill(' ') << setw(espacios + (int)texto.length()) << texto << endl;
}

/*Función para leer enteros asegurando que no rompa el cin. Esta función evita que 
el programa falle cuando el usuario ingresa un valor no numérico, limpiando el buffer 
de entrada y mostrando un mensaje de error hasta que se ingrese un número válido.
Proporciona una experiencia de usuario más robusta ante entradas incorrectas.*/
int LeerEntero(const string& mensaje) {
    int valor;
    while (true) {
        cout << mensaje;
        if (cin >> valor) {
            cin.ignore(numeric_limits<streamsize>::max(), '\n'); 
            return valor;
        } else {
            cout << ROJO << " Ingresa un numero valido!\n" <<  RESET;
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }
    }
}

/*Función auxiliar para el cálculo de puntaje base mediante punteros.
Se utiliza un puntero entero para almacenar el resultado del cálculo
de eficiencia basado en los movimientos realizados y el tamaño del tablero.
El puntaje aumenta cuando se gana en menos movimientos, premiando la eficiencia
del jugador. Se emplea memoria dinámica temporal para los cálculos intermedios.*/
void CalcularEficiencia(int* puntaje, int n, int movimientos) {
    int max_casillas = n * n;
    int* calculos_temp = new int[3];
    calculos_temp[0] = max_casillas - movimientos;
    calculos_temp[1] = 5;
    calculos_temp[2] = calculos_temp[0] * calculos_temp[1];

    if (calculos_temp[2] < 0) {
        calculos_temp[2] = 0;
    }

    *puntaje = *puntaje + calculos_temp[2];
    delete[] calculos_temp;
}

/*Función encargada de registrar los resultados de la partida en un archivo de texto.
Se emplean punteros para manejar la lógica de puntuación y se escribe la información
de manera secuencial para mantener un registro histórico. El archivo almacena datos
como jugador, tamaño de tablero, ganador, puntos obtenidos y un máximo teórico para
contextualizar los puntajes. Los datos se guardan en formato legible para futura consulta.*/
void RegistrarResultado(char ganador, int n, int movimientos) {
    ofstream archivo;
    archivo.open("historial.txt", ios::app);

    if (archivo.fail()) {
        return;
    }

    int puntos_base = 0;
    int* ptr_puntos = &puntos_base;

    if (ganador != ' ') {
        *ptr_puntos = 100;
        CalcularEficiencia(ptr_puntos, n, movimientos);
    } else {
        *ptr_puntos = 0;
    }

    int* max_calc = new int[2];
    max_calc[0] = 100 * 100;
    max_calc[1] = (n * n) * 50;
    
    int maximo_teorico = max_calc[0] + max_calc[1];
    
    delete[] max_calc;

    archivo << "Jugador: " << jugador_actual << "\n";
    archivo << "Tablero: " << n << "x" << n << "\n";
    archivo << "Ganador: " << ganador << "\n";
    archivo << "Puntos: " << *ptr_puntos << "\n";
    archivo << "Maximo_Posible_Temporada: " << maximo_teorico << "\n";
    archivo << "--------------------------------\n";

    archivo.close();
}

/*Función para visualizar el historial de partidas guardadas.
Para el procesamiento de las líneas del archivo se utiliza un sistema de
punteros dobles (char**) simulando una matriz dinámica de caracteres.
Esta implementación permite cargar hasta 100 líneas de historial, mostrando
la información de forma estructurada con colores y formato adecuado para
mejor legibilidad del registro de partidas anteriores.*/
void VerHistorial() {
    ifstream archivo;
    archivo.open("historial.txt");

    if (!archivo.is_open()) {
        cout << ROJO;
        centrar("No existe historial registrado aun.", 45);
        cout << RESET;
        return;
    }

    int capacidad_buffer = 100;
    char** buffer_lectura = new char*[capacidad_buffer];
    for (int i = 0; i < capacidad_buffer; i++) {
        buffer_lectura[i] = new char[100];
    }

    int contador_lineas = 0;
    string linea_temp;
    
    while (getline(archivo, linea_temp) && contador_lineas < capacidad_buffer) {
        for (int j = 0; j < linea_temp.length() && j < 99; j++) {
            buffer_lectura[contador_lineas][j] = linea_temp[j];
        }
        buffer_lectura[contador_lineas][linea_temp.length()] = '\0';
        contador_lineas++;
    }

    cout << MAGENTA << setfill('=') << setw(45) << "" << RESET << endl;
    cout << MAGENTA;
    centrar("HISTORIAL DE PARTIDAS", 45);
    cout << RESET;
    cout << MAGENTA << setfill('=') << setw(45) << "" << RESET << endl;

    for (int i = 0; i < contador_lineas; i++) {
        cout << CIAN << buffer_lectura[i] << RESET << endl;
    }
    
    for (int i = 0; i < capacidad_buffer; i++) {
        delete[] buffer_lectura[i];
    }
    delete[] buffer_lectura;

    archivo.close();
}

/*Instrucciones del juego dependiendo el tablero seleccionado.
Estas instrucciones son dependientes de la función centrar, imprimiendo
dichas instrucciones acorde al orden del tablero en cuestión. Se adaptan
dinámicamente al tamaño del tablero (2x2, 3x3, o 4x4+) y al modo de juego
(PvP o PvC), proporcionando al jugador información precisa sobre las reglas,
controles y condiciones de victoria específicas para su partida actual.*/
void Instrucciones (int n, int ancho, bool modoComputadora) {
    cout << MAGENTA << setfill('=') << setw(ancho) << "" << RESET << endl;
    cout << MAGENTA;
    centrar("INSTRUCCIONES DE JUEGO", ancho);
    cout << RESET;
    cout << MAGENTA << setfill('=') << setw(ancho) << "" << RESET << endl;

    string titulo = "Tablero de " + to_string(n) + "x" + to_string(n);
    cout << ROJO;
    centrar(titulo, ancho);
    cout << RESET;

    if (modoComputadora) {
        cout << AMARILLO;
        centrar("MODO: Jugador vs Computadora", ancho);
        cout << RESET;
    } else {
        cout << AMARILLO;
        centrar("MODO: Jugador vs Jugador", ancho);
        cout << RESET;
    }

    cout << VERDE;
    centrar("1. Ingresa fila y columna para colocar tu marca.", ancho);
    centrar("2. Puedes escribir 'U' para deshacer (Undo).", ancho);

    if (n == 2) {
        centrar("El primer jugador que logre colocar dos simbolos seguidos", ancho);
        centrar("en linea (horizontal, vertical o diagonal) gana el juego.", ancho);
    } else if (n == 3) {
        centrar("El primer jugador que logre colocar tres simbolos seguidos", ancho);
        centrar("en linea (horizontal, vertical o diagonal) gana el juego.", ancho);
    } else {
        centrar("El primer jugador que logre colocar cuatro simbolos seguidos", ancho);
        centrar("en linea (horizontal, vertical o diagonal) gana el juego.", ancho);
    }

    centrar("Si todas las casillas estan llenas y nadie ha ganado,", ancho);
    centrar("el juego termina en empate.", ancho);
    cout << RESET;
}

/*Función para determinar ganadores por fila en tableros de orden n.
Se emplea la indexación del propio tablero para hacerlo, además del
uso de una variable auxiliar player, para no perder el conteo del
último turno. Esta función es dependiente del bucle for anidado, el
cual, usa un contador i para moverse a través de filas y otro j para
moverse a través de elementos de tal fila. Estos elementos se comparan
bajo relaciones de igualdad, tomando en consideración la comparación
previa para evitar errores, esto con ayuda de la variable player.
En cualquier caso de que la función encuentre valores que NO indican
victoria, se detendrá inmediatamente, empleando ayuda de estructuras
if else múltiples y break.*/
bool GanadorFila(vector<vector<char>>* tablero){
    bool filawinner=false;
    char player=' ';
    int tamanio = tablero->size();
    for(int i=0; i<tamanio; i++){
        filawinner=false;
        player=' ';
        for(int j=0; j<tamanio; j++){
            if((*tablero)[i][j]==' '){
                filawinner=false;
                break;
            } else if((*tablero)[i][0]==(*tablero)[i][j]){
                filawinner=true;
                player=(*tablero)[i][j];
            } else if((*tablero)[i][j]!=player){
                filawinner=false;
                break;
            }
        }
        if(filawinner){
            break;
        }
    }
    return filawinner;
}

/*Función para determinar ganadores por columna en tableros de orden n.
Se emplea la indexación del propio tablero para hacerlo, además del
uso de una variable auxiliar player, para no perder el conteo del 
último turno. Esta función es dependiente del bucle for anidado el
cual, usa un contador i para moverse a través de columnas y otro j para
moverse a través de elementos de tal columna, simulando el desplazamiento 
entre filas. Estos elementos se comparan bajo relaciones de igualdad, 
tomando en consideración la comparación previa para evitar errores, 
esto con ayuda de la variable player.
En cualquier caso de que la función encuentre valores que NO indican
victoria, se detendrá inmediatamente, empleando ayuda de estructuras
if else múltiples y break.*/
bool GanadorColumna(vector<vector<char>>* tablero){
    bool colwinner=false;
    char player=' ';
    int tam = tablero->size();
    for(int i=0; i<tam; i++){
        colwinner=false;
        player=' ';
        for(int j=0; j<tam; j++){
            if((*tablero)[j][i]==' '){
                colwinner=false;
                break;
            } else if((*tablero)[0][i]==(*tablero)[j][i]){
                colwinner=true;
                player=(*tablero)[j][i];
            } else if((*tablero)[j][i]!=player){
                colwinner=false;
                break;
            }
        }
        if(colwinner){
            break;
        }
    }
    return colwinner;
}

/*Función para determinar ganadores por diagonal en tableros de orden n.
Se emplea la indexación del propio tablero para hacerlo, además del
uso de una variable auxiliar player, para no perder el conteo.
A diferencia de otras funciones, aquí se realizan 2 verificaciones por
separado. Ambas con for, por un lado, se analiza la diagonal principal, pasando
por todos los elementos de índice [i][i]. En caso de no encontrar ganadores,
se revisa la diagonal secundaria bajo los elementos [j][n-1-j]. Si la
diagonal secundaria no encuentra condición de victoria, se detiene, al 
igual que la diagonal principal, empleando if else múltiple y break.*/
bool GanadorDiagonal(vector<vector<char>>* tablero){
    bool diagwin=false;
    char player=' ';
    int size = tablero->size();
    for(int i=0; i<size; i++){
        if((*tablero)[i][i]==' '){
            diagwin=false;
            break;
        } else if((*tablero)[0][0]==(*tablero)[i][i]){
            diagwin=true;
            player=(*tablero)[i][i];
        } else if((*tablero)[i][i]!=player){
            diagwin=false;
            break;
        }
    }
    if(diagwin){
        return diagwin;
    } else{
        player=' ';
        int n = tablero->size();
        for(int j=0; j<n; j++){
            if((*tablero)[j][n-1-j]==' '){
                diagwin=false;
                break;
            } else if((*tablero)[0][n-1]==(*tablero)[j][n-1-j]){
                diagwin=true;
                player=(*tablero)[j][n-1-j];
            } else if((*tablero)[j][n-1-j]!=player){
                diagwin=false;
                break;
            }
        }
    return diagwin;}
}

/*Función para determinar ganadores de fila en tableros de orden
4 o mayor al encontrar 4 coincidencias. Esta función es similar a
la verificación del ganador de fila con ciertos matices. Primeramente
con for se verifican elementos que contengan caracter ' ', los cuales
son saltados para otra iteración con continue. En caso de encontrar
elementos distintos de la variable player, el contador se restablece y
se continúa comparando elementos posteriores respecto a ese elemento.
Si es que se encuentran 4 coincidencias consecutivas, se detiene el for
y se devuelve true.*/
bool GanadorRow4(vector<vector<char>>* tablero){
    int tamanio = tablero->size();
    if(tamanio<4){
        return false;
    }
    bool win4row=false;
    char player=' ';
    int num=0;
    for(int i=0; i<tamanio; i++){
        player=' ';
        num=0;
        win4row=false;
        for(int j=0; j<tamanio; j++){
            if((*tablero)[i][j]==' '){
                num=0;
                player=' ';
                win4row=false;
                continue;
            }
            if(player==' ' || (*tablero)[i][j]!=player){
                player=(*tablero)[i][j];
                num=1;
                win4row=false;
            } else if((*tablero)[i][j]==player){
                num++;
                if(num>=4){
                    win4row=true;
                    break;
                }
            }
        }
        if(win4row){
            break;
        }
    }
    return win4row;
}

/*Función para determinar ganadores de columna en tableros de orden
4 o mayor al encontrar 4 coincidencias. Esta función es similar a
la verificación del ganador de columna con ciertos matices. Primeramente
con for se verifican elementos que contengan caracter ' ', los cuales
son saltados para otra iteración con continue. En caso de encontrar
elementos distintos de la variable player, el contador se restablece y
se continúa comparando elementos posteriores respecto a ese elemento.
Si es que se encuentran 4 coincidencias consecutivas, se detiene el for
y se devuelve true.*/
bool GanadorCol4(vector<vector<char>>* tablero){
    int tam = tablero->size();
    if(tam<4){
        return false;
    }
    bool win4col=false;
    char player=' ';
    int num=0;
    for(int i=0; i<tam; i++){
        player=' ';
        num=0;
        win4col=false;
        for(int j=0; j<tam; j++){
            if((*tablero)[j][i]==' '){
                num=0;
                player=' ';
                win4col=false;
                continue;
            }
            if(player==' ' || (*tablero)[j][i]!=player){
                player=(*tablero)[j][i];
                num=1;
                win4col=false;
            } else if((*tablero)[j][i]==player){
                num++;
                if(num>=4){
                    win4col=true;
                    break;
                }
            }
        }
        if(win4col){
            break;
        }
    }
    return win4col;
}

/*Función para determinar ganadores de columna en tableros de orden
4 o mayor al encontrar 4 coincidencias. La lógica implementada en
esta función depende de la anidación de bucles for, separando la verificación
en diagonal principal y secundaria. Por un lado, la diagonal principal
considera el tamaño del tablero-4 como una variable de aumento dirigida 
a componer la variable de conteo (i y j) junto a otra variable en otro bucle
de nombre k. De esta manera se está iterando sobre cada diagonal existente en
el tablero. Similar a otras funciones, se reinicia el conteo al encontrar
espacios vacíos o un jugador diferente al del conteo, reiniciándolo. En caso 
de encontrar 4 coincidencias, se devuelve true para ganador.
De forma análoga, la diagonal secundaria emplea esta misma estructura, con
diferencias en las variables de control y conteo. Dado que se verifica de 
arriba a abajo, el control de las columnas empieza al menos desde 3 (para
un tablero 4). Asimismo, la variable interna de control j va bajando de valor
por la naturaleza de la verificación.*/
bool GanadorDiag4(vector<vector<char>>* tablero){
    int size = tablero->size();
    if(size<4){
        return false;
    }
    bool win4diag=false;
    char player=' ';
    int num=0;
    for(int row=0; row<=size-4; row++){
        for(int col=0; col<=size-4; col++){
            win4diag=false;
            player=' ';
            num=0;
            for(int k=0; k<4; k++){
                int i=row+k, j=col+k;
                if(i>=size||j>=size){
                    continue;
                }
                if((*tablero)[i][j]==' '){
                    win4diag=false;
                    player=' ';
                    num=0;
                    continue;
                }
                if(player==' '||(*tablero)[i][j]!=player){
                    player=(*tablero)[i][j];
                    num=1;
                    win4diag=false;
                } else if((*tablero)[i][j]==player){
                    num++;
                    if(num>=4){
                        win4diag=true;
                        return true;
                    }
                }
            }
        }
    }
    if(win4diag){
        return win4diag;
    } else{
        for(int row=0; row<=size-4; row++){
            for(int col=3; col<size; col++){
                win4diag=false;
                player=' ';
                num=0;
                for(int k=0; k<4; k++){
                    int i=row+k, j=col-k;
                    if(i>=size||j<0){
                        continue;
                    }
                    if((*tablero)[i][j]==' '){
                        win4diag=false;
                        player=' ';
                        num=0;
                        continue;
                    }
                    if(player==' '||(*tablero)[i][j]!=player){
                        player=(*tablero)[i][j];
                        num=1;
                        win4diag=false;
                    } else if((*tablero)[i][j]==player){
                        num++;
                        if(num>=4){
                            win4diag=true;
                            return true;
                        }
                    }
                }
            }
        }
    }
    return win4diag;
}

/*Función para determinar ganadores en tableros de orden n. Esta
función integra la verificación para ganar en fila, columna y 
diagonal en tableros de cualquier orden y de orden 4 o mayor. Para
saber a qué funciones llamar (4 o orden n) se emplea un if múltiple.
Acorde a esto, se llama a las 3 funciones dependiendo del orden del
tablero y, bajo el parámetro del tablero, se determina si en algún 
momento estas subfunciones devuelve true. Esta función centraliza la
lógica de detección de victoria, simplificando el código principal.*/
bool Ganador(vector<vector<char>>* tablero){
    bool win=false;
    int tam = tablero->size();
    if((tam<4) && (GanadorDiagonal(tablero)||GanadorColumna(tablero)||GanadorFila(tablero))){
        win=true;
    } else if(GanadorDiag4(tablero)||GanadorCol4(tablero)||GanadorRow4(tablero)){
        win=true;
    }
    return win;
}

/*Función para mostrar el tablero en pantalla. Partiendo del tablero
como parámetro, la función imprime secuencialmente con el uso de for
los elementos del tablero. Simultáneamente, se imprime celdas para
cada elemento y separador. Finalmente, se considera no imprimir
separador en la última iteración, empleando un if de tipo simple.
La presentación utiliza el formato de celdas con bordes verticales '|'
y separadores horizontales '---' para una visualización clara del estado
del juego en cada turno.*/ 
void ImprimirTablero(vector<vector<char>>* tablero){
    int tamanio = tablero->size();
    for(int i=0; i<tamanio; i++){
        string separator="";
        for(int j=0; j<tamanio;j++){
            separator+="---";
            cout<<'|'<<(*tablero)[i][j]<<'|';
        }
        if(i!=(tamanio-1)){
            cout<<"\n";
            cout<<separator<<"\n"; 
        }
    }
}

/*Función para colocar el símbolo del jugador en una casilla del tablero.
Esta función toma la fila y columna (empezando en 1) y el jugador (X o O) 
y lo asigna en el tablero, siempre y cuando las coordenadas estén dentro 
del rango del tablero. La conversión de índices basados en 1 a índices basados
en 0 se realiza internamente para mantener la coherencia con las estructuras
de datos del programa.*/
void AsignarCasilla(vector<vector<char>>* tablero, int row, int col, char jugador){
    int size = tablero->size();
    if(row<=size && col<=size && row>=1 && col>=1){
        (*tablero)[row-1][col-1]=jugador;
    }
}

/*Función auxiliar para verificar si una jugada lleva a victoria inmediata (IA).
Esta función simula colocar el símbolo del jugador en la posición dada y 
llama a la función Ganador para verificar si con ese movimiento se gana la partida.
Luego deshace el movimiento (backtracking) y retorna el resultado. Es fundamental
para la lógica de la computadora, permitiéndole evaluar jugadas sin modificar
permanentemente el estado del tablero.*/
bool PuedeGanar(vector<vector<char>>* tablero, int row, int col, char jugador) {
    if ((*tablero)[row][col] != ' ') return false;
    (*tablero)[row][col] = jugador;
    bool gana = Ganador(tablero);
    (*tablero)[row][col] = ' ';
    return gana;
}

/*Funcion para el movimiento del Humano con soporte para UNDO. 
Esta función maneja la entrada del jugador humano, permitiéndole ingresar 
las coordenadas de su movimiento o la letra 'U' para deshacer el último movimiento.
Se valida que la casilla esté vacía y dentro del rango, y en caso de UNDO, 
se desapila el último movimiento y se libera la casilla correspondiente.
La función utiliza un buffer de caracteres para leer la entrada, permitiendo
detectar tanto números como el comando de undo de manera flexible.*/
void MovimientoHumano(vector<vector<char>>* tablero, int n, char jugador, int& row, int& col, stack<Movimiento>& undoStack) {
    char inputStr[10];
    string colorJugador = (jugador == 'X') ? MAGENTA : CIAN;
    bool movimientoValido = false;

    while (!movimientoValido) {
        cout << colorJugador << "\nJugador " << jugador << RESET << "\n";
        cout << VERDE << "Ingresa fila (o 'U' para Undo): " << RESET;
        cin >> inputStr;

        if (inputStr[0] == 'U' || inputStr[0] == 'u') {
            if (!undoStack.empty()) {
                Movimiento ultimo = undoStack.top();
                undoStack.pop();
                (*tablero)[ultimo.fila][ultimo.columna] = ' ';
                cout << AMARILLO << "UNDO realizado! Casilla liberada.\n" << RESET;
                ImprimirTablero(tablero);
                row = -1; 
                return;
            } else {
                cout << ROJO << "No hay movimientos para deshacer!\n" << RESET;
                continue;
            }
        }

        row = atoi(inputStr);
        cin >> col;

        if (row > n || col > n || row <= 0 || col <= 0) {
            cout << ROJO << "Ingresaste un valor fuera de rango!\n" << RESET;
        } else if ((*tablero)[row-1][col-1] != ' ') {
            cout << ROJO << "Este espacio esta ocupado\n" << RESET;
        } else {
            movimientoValido = true;
        }
    }
}

/*Función para el movimiento de la Computadora con estrategia básica.
La computadora primero intenta ganar colocando su símbolo en alguna casilla,
luego intenta bloquear al oponente si este está a punto de ganar, y finalmente 
elige una casilla aleatoria de entre las disponibles. Utiliza un arreglo dinámico
para almacenar las posiciones libres y luego selecciona una al azar.
Esta implementación proporciona una IA funcional aunque básica, con tres niveles
de prioridad que simulan un pensamiento estratégico mínimo.*/
void MovimientoComputadora(vector<vector<char>>* tablero, int n, char jugador, int& row, int& col, stack<Movimiento>& undoStack) {
    cout << AMARILLO << "\n Computadora está pensando...\n" << RESET;
    char oponente = (jugador == 'X') ? 'O' : 'X';

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            if (PuedeGanar(tablero, i, j, jugador)) {
                row = i + 1;
                col = j + 1;
                return;
            }
        }
    }

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            if (PuedeGanar(tablero, i, j, oponente)) {
                row = i + 1;
                col = j + 1;
                return;
            }
        }
    }

    Pos* libres = new Pos[n * n];
    int count = 0;

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            if ((*tablero)[i][j] == ' ') {
                libres[count].r = i;
                libres[count].c = j;
                count++;
            }
        }
    }

    if (count > 0) {
        int idx = rand() % count;
        row = libres[idx].r + 1;
        col = libres[idx].c + 1;
    } else {
        row = 1; col = 1;
    }
    
    delete[] libres;
}

// FUNCIÓN DEL JUEGO UNIFICADA
/*Función principal del juego que controla el flujo de la partida.
Inicializa el tablero, alterna los turnos entre los jugadores (humanos o computadora),
maneja el sistema de undo, verifica si hay un ganador o empate, y al finalizar
una partida permite volver al menú principal o repetir la partida.
Esta función integra todas las componentes del juego, coordinando la lógica,
la interfaz de usuario, y la persistencia de datos en un flujo coherente y completo.*/
void Juego(int n, bool modoComputadora){
    bool repetir = true;
    const int ancho = 45;
    while (repetir) {
        Limpiar();
        Instrucciones(n, ancho, modoComputadora);
        cout << ROJO;
        centrar("Presiona ENTER para jugar...", ancho);
        cout << RESET;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cin.get();

        vector<vector<char>> example(n, vector<char>(n, ' '));
        vector<vector<char>>* ptrTablero = &example;
        int row, col, contador=0;
        
        stack<Movimiento> undoStack;

        FuncionMovimiento jugadorX_Func = MovimientoHumano;
        FuncionMovimiento jugadorO_Func;

        if (modoComputadora) {
            jugadorO_Func = MovimientoComputadora;
        } else {
            jugadorO_Func = MovimientoHumano;
        }
        
        ImprimirTablero(ptrTablero);

        while(!(contador>=n*n) && !Ganador(ptrTablero)){
            contador+=1;
            
            jugadorX_Func(ptrTablero, n, 'X', row, col, undoStack);
            
            if (row == -1) {
                contador -= 2;
                if (contador < 0) contador = 0;
                continue;
            }

            AsignarCasilla(ptrTablero, row, col, 'X');
            Movimiento movX = {row-1, col-1, 'X'};
            undoStack.push(movX);
            
            ImprimirTablero(ptrTablero);
            
            if(Ganador(ptrTablero)){
                cout<<VERDE<<"\nHa ganado el jugador X 🏆\n"<<RESET;
                RegistrarResultado('X', n, contador);
                break;
            }
            if(contador>=n*n){
                cout<<CIAN<<"\nHa sido un empate! 🤝 \n"<<RESET;
                RegistrarResultado(' ', n, contador);
                break;
            }

            contador+=1;

            jugadorO_Func(ptrTablero, n, 'O', row, col, undoStack);

             if (row == -1) {
                contador -= 2;
                if (contador < 0) contador = 0;
                continue;
            }

            AsignarCasilla(ptrTablero, row, col, 'O');
            Movimiento movO = {row-1, col-1, 'O'};
            undoStack.push(movO);

            ImprimirTablero(ptrTablero);
            
            if(Ganador(ptrTablero)){
                if (modoComputadora)
                    cout<<VERDE<<"\nHa ganado la Computadora 🏆\n"<<RESET;
                else
                    cout<<VERDE<<"\nHa ganado el jugador O 🏆\n"<<RESET;
                
                RegistrarResultado('O', n, contador);
                break;
            }
            
            if(contador>=n*n){
                cout<<CIAN<<"\nHa sido un empate!🤝 \n"<<RESET;
                RegistrarResultado(' ', n, contador);
                break;
            }
        }
        
        VerHistorial();
        cout << "\nPresiona ENTER para continuar...";
        cin.ignore();
        cin.get();

        cout << "\n" << MAGENTA << setfill('=') << setw(ancho) << "" << RESET << endl;
        cout << MAGENTA;
        centrar("🔥 FIN DEL JUEGO 🔥", ancho);
        cout << RESET;
        cout << MAGENTA << setfill('=') << setw(ancho) << "" << RESET << endl;

        cout << VERDE;
        centrar("1. Volver al menu principal", ancho);
        centrar("2. Reiniciar partida", ancho);
        cout << RESET;
        
        int op2;
        do {
            cout << "\nElige una opcion: ";
            cin >> op2;
        } while(op2 != 1 && op2 != 2);
        
        if(op2 == 1)
            repetir = false;
        else
            repetir = true;
    }
}

/*Función principal del programa. Inicializa la semilla para números aleatorios,
solicita el nombre del jugador, y presenta un menú principal con opciones para
jugar en modo PvP o PvC, ver el historial, o salir del juego. Utiliza un bucle
do-while para mantener el menú hasta que el usuario elija salir.
Esta función sirve como punto de entrada y coordinador general de la aplicación,
gestionando la interacción inicial con el usuario y la navegación entre las
diferentes funcionalidades del sistema.*/
int main(){
    srand((unsigned)time(0));
    int op;
    int num;
    const int ancho = 45;
    const char sig = '=';

    cout << MAGENTA << "Antes de iniciar, escribe tu nombre: " << RESET;
    getline(cin, jugador_actual);

    do {
        Limpiar();
        cout<< MAGENTA << setfill(sig) << setw(ancho) << "" << endl;
        centrar("✖ 🟣  TIC TAC TOE 🟣 ✖", ancho);
        cout << MAGENTA << setfill(sig) << setw(ancho) << "" << endl;

        cout << VERDE;
        centrar("MENU 🕹", ancho);
        centrar("1. Jugar PvP (Humano vs Humano)", ancho);
        centrar("2. Jugar PvC (Humano vs Computadora)", ancho);
        centrar("3. Ver Historial", ancho);
        centrar("4. Salir ", ancho);
        cout << RESET; 
        
        cout << setfill(' ') << setw((ancho - 20)/2) << "" << "👉  Elige una opcion: ";
        cin >> op;
        cout << endl;

        switch (op) {
            case 1:
                do {
                    cout<< VERDE;
                    cout << setfill(' ') << setw((ancho - 25)/2) 
                         << "" << " Ingresa un numero (2-9): " << RESET;
                    cin >> num;  

                    if (num>=2 && num<10){
                        Juego(num, false);
                        break;
                    } else {
                        cout << ROJO;
                        centrar("Ingresa un numero entre 2 y 9 😓", ancho);
                        cout << RESET;
                    }
                    
                } while (true);
                break;
            
            case 2:
                 do {
                    cout<< VERDE;
                    cout << setfill(' ') << setw((ancho - 25)/2) 
                         << "" << " Ingresa un numero (2-9): " << RESET;
                    cin >> num;  

                    if (num>=2 && num<10){
                        Juego(num, true);
                        break;
                    } else {
                        cout << ROJO;
                        centrar("Ingresa un numero entre 2 y 9 😓", ancho);
                        cout << RESET;
                    }
                    
                } while (true);
                break;

            case 3:
                Limpiar();
                VerHistorial();
                cout << "\nPresiona ENTER para volver...";
                cin.ignore();
                cin.get();
                break;

            case 4: 
                cout<< CIAN;
                centrar("Bye Bye... regresa pronto 😭", ancho);
                cout<< RESET;
                break;

            default:
                cout<< ROJO;
                centrar("¡La opcion no es valida!", ancho);
                cout<< RESET;
        }

    } while (op != 4);
    
    return 0;
}