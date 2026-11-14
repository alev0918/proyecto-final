// ===================================================================
//  Proyecto final: Distancia de Hamming en secuencias de ADN
//  Código base
// ===================================================================

#include <iostream>   // cout y cin: escribir en pantalla y leer del teclado
#include <fstream>    // ifstream: leer archivos de texto
#include <string>     // string: cadenas de caracteres
#include <cctype>     // toupper: convertir una letra a mayúscula
#include <iomanip>    // setprecision: controlar los decimales al imprimir
using namespace std;

// -------------------------------------------------------------------
// Incluir aquí el código que ejecute lo solicitado en el enunciado
// (funciones de las Partes A y B).
// -------------------------------------------------------------------
// PARTE A: funciones básicas

// aMayusculas: Devuelve una copia de 's' con todas sus letras en mayúscula.
// 's' se recibe POR VALOR (sin &), así que ya es una copia: podemos 
// modificarla aquí sin dañar la secuencia original del main.
// Ejemplo: "atgc" -> "ATGC".  Los caracteres que no son letras
// (por ejemplo '3' o '-') quedan iguales, porque toupper no los cambia.

string aMayusculas(string s) {
    for (int i = 0; i < s.length(); i++) {
        s[i] = toupper(s[i]);   // convierte el carácter i a mayúscula
    }
    return s;
}

// esValida: Devuelve true si 's' solo contiene A, C, G o T. 
// Recibe la secuencia YA en mayúsculas, por eso solo se comparan las letras mayúsculas.
// Se pasa como 'const string &' para no copiarla y no modificarla.

bool esValida(const string &s) {
    for (int i = 0; i < s.length(); i++) {
        char c = s[i];
        // Si el carácter no es ninguno de los cuatro nucleótidos, la
        // secuencia ya es inválida y no hace falta revisar el resto.
        if (c != 'A' && c != 'C' && c != 'G' && c != 'T') {
            return false;
        }
    }
    // El ciclo terminó sin encontrar caracteres raros: es válida.
    return true;
}

// distanciaHamming: Cuenta en cuántas posiciones difieren 'a' y 'b'.
// Solo se llama cuando ambas tienen la misma longitud, así que 
// podemos usar a.length() como límite y acceder a b[i] sin salirnos.
// Ejemplo: "ATCGAT" vs "ATGGCT" -> difieren en las posiciones 2 y 4 -> distancia = 2.

int distanciaHamming(const string &a, const string &b) {
    int distancia = 0;               // contador de diferencias
    for (int i = 0; i < a.length(); i++) {
        if (a[i] != b[i]) {
            distancia++;             // en esta posición hay una mutación
        }
    }
    return distancia;
}

// porcentajeIdentidad: Devuelve (L - d) / L * 100, donde d es la distancia y L la longitud.
// Notita: si se hace (L - d) / L con enteros, C++ usa división entera
// (ejemplo: 4 / 6 = 0). Por eso convertimos a double
// antes de dividir: así 4.0 / 6 = 0.666... y el resultado da 66.7 %.

double porcentajeIdentidad(int d, int L) {
    return (double)(L - d) / L * 100.0;
}

// -------------------------------------------------------------------
// separarPareja
//
// Recibe una línea del archivo, por ejemplo "ATCGAT ATGGCT", y la
// divide en dos secuencias: seq1 = "ATCGAT" y seq2 = "ATGGCT".
//
// Parámetros:
//   linea  la línea leída del archivo. Se pasa como 'const string &'
//          para no copiarla y para garantizar que no se modifica.
//   seq1   aquí se guarda la primera secuencia.
//   seq2   aquí se guarda la segunda secuencia.
//          seq1 y seq2 se pasan por referencia ('string &') porque la
//          función debe "devolver" dos valores: los cambios que se les
//          hagan aquí dentro se ven en la variable original del main.
//
// Valor de retorno:
//   true   si la línea tiene exactamente un separador (un espacio o
//          una coma) con texto a ambos lados.
//   false  en cualquier otro caso; entonces seq1 y seq2 no se tocan.
// -------------------------------------------------------------------
bool separarPareja(const string &linea, string &seq1, string &seq2) {
    // 'pos' guardará la posición (el índice) del separador.
    // Empieza en -1, un valor imposible como índice, que aquí
    // significa "todavía no he encontrado ningún separador".
    int pos = -1;

    // Recorremos la línea carácter por carácter.
    // linea.length() es el número de caracteres de la línea, y
    // linea[i] es el carácter que está en la posición i (desde 0).
    for (int i = 0; i < linea.length(); i++) {
        if (linea[i] == ' ' || linea[i] == ',') {
            // Encontramos un separador. Si 'pos' ya no vale -1,
            // significa que este es el SEGUNDO separador de la línea,
            // por ejemplo en "AAA CCC GGG". Esa línea no es válida.
            if (pos != -1) {
                return false;
            }
            // Es el primer separador: recordamos dónde está.
            pos = i;
        }
    }

    // Al salir del ciclo hay tres situaciones inválidas:
    //   pos == -1                 no había separador ("ATCG")
    //   pos == 0                  el separador es el primer carácter
    //                             (" ATCG"): no hay primera secuencia
    //   pos == linea.length() - 1 el separador es el último carácter
    //                             ("ATCG,"): no hay segunda secuencia
    // La condición 'pos <= 0' cubre a la vez los casos -1 y 0.
    if (pos <= 0 || pos == linea.length() - 1) {
        return false;
    }

    // substr(inicio, cantidad) extrae 'cantidad' caracteres desde
    // 'inicio'. Con la línea "ATCGAT ATGGCT", pos vale 6:
    //   linea.substr(0, 6) -> "ATCGAT"  (posiciones 0 a 5)
    //   linea.substr(7)    -> "ATGGCT"  (desde la posición 7 hasta
    //                                    el final de la línea)
    seq1 = linea.substr(0, pos);
    seq2 = linea.substr(pos + 1);
    return true;
}


int main() {
    // ---------------------------------------------------------------
    // 1. Pedir el nombre del archivo y abrirlo
    // ---------------------------------------------------------------
    string nombreArchivo;
    cout << "Nombre del archivo: ";
    cin >> nombreArchivo;   // lee una palabra (el nombre no debe
                            // tener espacios, p. ej. secuencias.txt)
    cout << endl;

    // 'ifstream' (input file stream) es un flujo de entrada asociado
    // a un archivo. Se usa igual que 'cin', pero los datos vienen del
    // archivo en lugar del teclado. Al crearlo con el nombre, intenta
    // abrir el archivo. El archivo debe estar en la misma carpeta
    // desde donde se ejecuta el programa (o escribir la ruta completa).
    ifstream archivo(nombreArchivo);

    // is_open() devuelve false si el archivo no se pudo abrir: no
    // existe, el nombre está mal escrito o no hay permisos.
    // En ese caso no tiene sentido continuar: terminamos el programa.
    // 'return 1' le indica al sistema operativo que hubo un error
    // ('return 0', al final, indica que todo salió bien).
    if (!archivo.is_open()) {
        cout << "Error: no se pudo abrir el archivo." << endl;
        return 1;
    }

    // 'fixed' y 'setprecision(1)' hacen que todos los números con
    // decimales (double) que se impriman de aquí en adelante muestren
    // exactamente un decimal: 66.7 en lugar de 66.6667.
    cout << fixed << setprecision(1);

    // ---------------------------------------------------------------
    // 2. Leer el archivo línea por línea
    // ---------------------------------------------------------------
    string linea;        // aquí se guarda cada línea leída
    int numPareja = 0;   // cuántas parejas con formato correcto van

    // getline(archivo, linea) lee la línea completa siguiente del
    // archivo (hasta el salto de línea, sin incluirlo) y la guarda en
    // 'linea'. Devuelve un valor que se evalúa como verdadero si logró
    // leer algo, y como falso cuando ya no quedan líneas. Por eso el
    // ciclo 'while' se repite una vez por cada línea y se detiene
    // solo al llegar al final del archivo.
    //
    // Se usa getline y no 'archivo >> linea' porque >> se detiene en
    // el primer espacio, y aquí necesitamos la línea entera.
    while (getline(archivo, linea)) {

        // Los archivos creados en Windows terminan cada línea con dos
        // caracteres: '\r' y '\n'. getline quita el '\n' pero deja el
        // '\r' al final, invisible en pantalla, y haría que "ATGGCT\r"
        // tuviera un carácter inválido. Si el último carácter es '\r',
        // lo borramos con erase(posición).
        if (!linea.empty() && linea[linea.length() - 1] == '\r') {
            linea.erase(linea.length() - 1);
        }

        // Una línea vacía no tiene datos: 'continue' salta el resto
        // del cuerpo del ciclo y pasa directamente a leer la siguiente.
        if (linea.empty()) {
            continue;
        }

        // Intentamos dividir la línea en dos secuencias. Si el formato
        // es incorrecto, avisamos y pasamos a la siguiente línea sin
        // contarla como pareja.
        string seq1, seq2;
        if (!separarPareja(linea, seq1, seq2)) {
            cout << "Aviso: línea ignorada por formato incorrecto: "
                 << linea << endl;
            continue;
        }

        // Si llegamos aquí, la línea tiene formato correcto: es una
        // pareja nueva y le asignamos su número.
        numPareja++;

        // -----------------------------------------------------------
        // Incluir aquí el código que ejecute lo solicitado en el
        // enunciado para cada pareja.
        // -----------------------------------------------------------

        // --------------------------------------------
        // PARTE A: validación, distancia e identidad
        // --------------------------------------------
        
        // Primero se pasan ambas secuencias a mayúsculas (sección 3.2).
        // Así "atgc" se trata igual que "ATGC" y se imprime en mayúsculas
        seq1 = aMayusculas(seq1);
        seq2 = aMayusculas(seq2);

        cout << "Pareja " << numPareja << ": " << seq1 << " vs " << seq2 << " -> ";

        // Se revisan los errores EN ORDEN y solo se reporta el primero:
        // 1) caracteres inválidos, 2) longitudes diferentes
        if (!esValida(seq1) || !esValida(seq2)) {
            cout << "Error: caracteres inválidos" << endl;
            continue;   // pasamos a la siguiente línea
        }
        if (seq1.length() != seq2.length()) {
            cout << "Error: longitudes diferentes" << endl;
            continue;
        }

        // Si llegamos aquí la pareja es válida: calculamos resultados
        int L = seq1.length();
        int d = distanciaHamming(seq1, seq2);
        double identidad = porcentajeIdentidad(d, L);

        cout << "Distancia: " << d << " | Identidad: " << identidad << "%" << endl;

        // ---------------------------------------------------------------------
        // PARTE B: el conteo de nucleótidos y el contenido GC de seq1 y seq2
        // ---------------------------------------------------------------------

    }

    // Cerramos el archivo: ya no lo necesitamos. (Se cerraría solo al
    // terminar el programa, pero cerrarlo explícitamente es buena
    // costumbre.)
    archivo.close();
    return 0;
}
