// Bonooo: Matriz de distancias filogeneticas
// Integrantes: Jacybeth Velasques, Thay Gomez
// Maria Aaestre y Andrea Carillo 

#include <iostream>
#include <fstream>
#include <string>

using namespace std;

int distanciaHamming(const string &a, const string &b) {
    int diferencias = 0;
    for (int i = 0; i < (int)a.length(); i++) {
        if (a[i] != b[i]) {
            diferencias++;
        }
    }
    return diferencias;
}

int main() {
    ifstream archivo("muestras.txt");
    if (!archivo.is_open()) {
        cout << "Error: no se pudo abrir muestras.txt" << endl;
        return 1;
    }

    string seq[10];
    int n = 0;
    string linea;

    while (n < 10 && getline(archivo, linea)) {
        if (!linea.empty() && linea[linea.length() - 1] == '\r') {
            linea.erase(linea.length() - 1);
        }
        if (!linea.empty()) {
            seq[n] = linea;
            n++;
        }
    }
    archivo.close();

    int D[10][10];
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            D[i][j] = distanciaHamming(seq[i], seq[j]);
        }
    }

    cout << "\t";
    for (int j = 0; j < n; j++) {
        cout << "S" << (j + 1) << "\t";
    }
    cout << endl;

    for (int i = 0; i < n; i++) {
        cout << "S" << (i + 1) << "\t";
        for (int j = 0; j < n; j++) {
            cout << D[i][j] << "\t";
        }
        cout << endl;
    }

    return 0;
}