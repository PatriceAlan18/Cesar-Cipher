#include <iostream>
#include <string>
#include <regex>
#include <fstream>  
#include <sstream>

using namespace std;

string leerArchivo(string nombre) {
    ifstream fich(nombre + ".txt");
    if (!fich.is_open())
    {
        cout << "Error al abrir " + nombre +".txt \n";
        exit(EXIT_FAILURE);
    }
    string archivo = "";
    string linea = "";
    while (getline(fich, linea)) archivo += linea;
    fich.close();
    return archivo;
}

void escribirArchivo(string nombre, string texto) {
    ofstream fich(nombre+".txt");
    if (!fich)
    {
        cout << "Error al abrir"+nombre+".txt \n";
        exit(EXIT_FAILURE);
    }

    fich << texto << endl;
}

char encriptar(char c, int clave) {
    int index = 0;
    while (clave >= 26) clave = clave - 26;
    if (clave == 26) return c;
    if (c < 'Z' +1 && c > 'A' - 1) {//Mayuscula
        if (c + clave > 'Z') c = c + clave - 26;
        else c = c + clave;
    }
    else  if (c < 'z' + 1 && c > 'a' - 1) {//Minuscula
        if (c + clave > 'z') c = c + clave - 26;
        else c = c + clave;
    }
    return c;
}

char desencriptar(char c, int clave) {
    int index = 0;
    while (clave >= 26) clave = clave - 26;
    if (clave == 26) return c;
    if (c < 'Z' + 1 && c > 'A' - 1) {//Mayuscula
        if (c - clave < 'A') c = c - clave + 26;
        else c = c - clave;
        return c;
    }
    if (c < 'z' + 1 && c > 'a' - 1) {//Minuscula
        if (c - clave < 'a') c = c - clave + 26;
        else c = c - clave;
    }
    return c;
}

string obtenerClaves() {
    string aux;
    regex patron("^([0-9]*(\\s?))*$");
    do {
        cout << "Escribe la clave de encriptacion (Separadas con un espacio si son varias):";
        getline(cin, aux);
    } while (!regex_match(aux, patron));
    
    return aux;
}

bool masDeUna(string claves) {
    bool una = false;
    bool espacioAntes = false;
    for (int i = 0; i < claves.length(); i++) {
        
        if (claves[i] > ('0' -1) && claves[i] < ('9' + 1)) {
            if (una && espacioAntes)return true;
            una = true;
        }
        if (claves[i] == 32) espacioAntes = true;
        else espacioAntes = false;
    }
    return false;
}

string encriptadoCesar(string archivo, string claves) {
    string final = "";
    string aux;
    stringstream separador(claves);
    bool variasClaves = masDeUna(claves);
    aux = claves;
    for (int i = 0; i < archivo.length(); i++) {
        if (variasClaves) {
            if (!getline(separador, aux, ' ')) {
                separador.clear();
                separador.seekg(0);
                getline(separador, aux, ' ');
            }
        }
        final = final + encriptar(archivo[i], stoi(aux));
    }
    return final;
}

string desencriptadoCesar(string archivo, string claves) {
    string final = "";
    string aux;
    stringstream separador(claves);
    bool variasClaves = masDeUna(claves);
    aux = claves;

    for (int i = 0; i < archivo.length(); i++) {
        if (variasClaves) {
            if (!getline(separador, aux, ' ')) {
                separador.clear();
                separador.seekg(0);
                getline(separador, aux, ' ');
            }
        }
        final = final + desencriptar(archivo[i], stoi(aux));
    }
    return final;
}


string decimalAHexadecimal(int decNum) {
    int decimalNumber = decNum;
    string hexNumber;
    while (decimalNumber > 0) {
        int remainder = decimalNumber % 16;
        char hexDigit;

        if (remainder < 10)
            hexDigit = remainder + '0';
        else
            hexDigit = remainder - 10 + 'A';

        hexNumber = hexDigit + hexNumber;
        decimalNumber /= 16;
    }

    return hexNumber;
}

int hexadecimalADecimal(string hexadecimal) {
    int decimal = 0;
    int power = 0;

    for (int i = hexadecimal.size() - 1; i >= 0; --i) {
        char digit = hexadecimal[i];
        int value = 0;

        if (isdigit(digit)) {
            value = digit - '0';
        }
        else {
            value = toupper(digit) - 'A' + 10;
        }

        decimal += value * pow(16, power);
        ++power;
    }

    return decimal;
}

string convertirAHexa(string texto) {
    string resultado = "";
    for (char c : texto) resultado = resultado + decimalAHexadecimal(c) + " ";
    return resultado;
}

string desconvertirDeHexa(string texto) {
    string resultado = "", aux;
    stringstream separador(texto);
    char salida;
    while (getline(separador, aux, ' ')) {
        salida = hexadecimalADecimal(aux);
        resultado = resultado + char(salida);
    }
    return resultado;
}


void procesoEncriptar() {
    string nombreArchivo, resultados;
    cout << "Nombre del archivo a leer " << endl;
    getline(cin, nombreArchivo);
    resultados = leerArchivo(nombreArchivo);
    string claves = obtenerClaves();
    resultados = encriptadoCesar(resultados, claves);
    cout << "Nombre del archivo a escribir lo encriptado" << endl;
    getline(cin, nombreArchivo);
    escribirArchivo(nombreArchivo, resultados);
    cout << "Se ha encriptado correctamente" << endl;
    resultados = convertirAHexa(resultados);
    cout << "Nombre del archivo a escribir lo encriptado en hexadecimal" << endl;
    getline(cin, nombreArchivo);
    escribirArchivo(nombreArchivo, resultados);
    cout << "Se ha generado el archivo hexadecimal correctamente" << endl;
}

void procesoDesencriptar() {
    string nombreArchivo, resultados;
    cout << "Nombre del archivo a desencriptar" << endl;
    getline(cin, nombreArchivo);
    resultados = leerArchivo(nombreArchivo);
    resultados = desconvertirDeHexa(resultados);
    string claves = obtenerClaves();
    resultados = desencriptadoCesar(resultados, claves);
    cout << "Nombre del archivo a escribir lo desencriptado" << endl;
    getline(cin, nombreArchivo);
    escribirArchivo(nombreArchivo, resultados);
    cout << "Se ha generado el archivo correctamente" << endl;
}

void menu() {
    int eleccion;

    do {
        cout << "Selecciona la opcion deseada" << endl;
        cout << "1. Encriptar" << endl;
        cout << "2. Desenciprtar" << endl;
        cout << "3. Salir" << endl;
        cin >> eleccion;
        cin.ignore();
        switch(eleccion)
        {

        case 1:
            procesoEncriptar();
            break;

        case 2: 
            procesoDesencriptar();
            break;
        case 3: break;
        default:
            cout << "Escribe una opcion valida" << endl;
        }
        cout << "---------------------------------------------------------------------------" << endl;

    } while (eleccion != 3);
}

int main()
{
    menu();
    
    return 0;
}

