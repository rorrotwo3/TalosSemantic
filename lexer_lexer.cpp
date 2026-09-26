#include "lexer_lexer.h"
#include <ctype.h>
using namespace std;

Lexer::Lexer() : posicionActual(0) {}

void Lexer::reiniciarAnalisis(const std::string& texto) {
    inputAnalisis = texto;
    if(!inputAnalisis.empty() && !isspace(inputAnalisis.back())) {
        inputAnalisis += ' ';  // Asegurar espacio final
    }
    posicionActual = 0;
}

bool Lexer::tieneMasTokens() const {
    return posicionActual < inputAnalisis.length();
}

Token Lexer::obtenerSiguienteToken() {
    auto reservadas = LexerAutomata::estadoAsignacion();
    auto aceptacion = LexerAutomata::estadoAceptacion();
    auto errores = LexerAutomata::erroresLexicos();
    auto diferentes = LexerAutomata::estadosDiferentes();

    // Saltar espacios en blanco
    while(posicionActual < inputAnalisis.length() &&
           (inputAnalisis[posicionActual] == ' ' ||
            inputAnalisis[posicionActual] == '\t' ||
            inputAnalisis[posicionActual] == '\n')) {
        posicionActual++;
    }

    if(posicionActual >= inputAnalisis.length()) {
        return Token{-1, "", "FIN", posicionActual};
    }

    char car = inputAnalisis[posicionActual];
    int col;

    // 1. Manejo especial para palabras reservadas e identificadores
    if(isalpha(car) || car == '_') {
        size_t inicio = posicionActual;
        while(posicionActual < inputAnalisis.length() &&
               (isalnum(inputAnalisis[posicionActual]) ||
                inputAnalisis[posicionActual] == '_')) {
            posicionActual++;
        }
        std::string lexema = inputAnalisis.substr(inicio, posicionActual - inicio);

        // Verificar si es palabra reservada
        auto it = reservadas.find(lexema);
        if(it != reservadas.end()) {
            return Token{it->second, lexema, "Palabra reservada", inicio};
        } else {
            return Token{101, lexema, aceptacion.at(101), inicio};
        }
    }

    // 2. Manejo especial para numeros
    if(isdigit(car) || (car == '.' && posicionActual + 1 < inputAnalisis.length() &&
                         isdigit(inputAnalisis[posicionActual+1]))) {
        size_t inicio_num = posicionActual;
        bool tienePunto = (car == '.');
        bool tieneExponente = false;
        bool errorNum = false;
        int codigoError = 500;

        while(posicionActual < inputAnalisis.length() && !errorNum) {
            car = inputAnalisis[posicionActual];
            if(isdigit(car)) {
                posicionActual++;
            }
            else if(car == '.' && !tienePunto && !tieneExponente) {
                tienePunto = true;
                posicionActual++;
                if(posicionActual >= inputAnalisis.length() || !isdigit(inputAnalisis[posicionActual])) {
                    errorNum = true;
                    codigoError = 500;
                }
            }
            else if((car == 'e' || car == 'E') && !tieneExponente) {
                tieneExponente = true;
                posicionActual++;
                if(posicionActual < inputAnalisis.length() &&
                    (inputAnalisis[posicionActual] == '+' || inputAnalisis[posicionActual] == '-')) {
                    posicionActual++;
                    if(posicionActual >= inputAnalisis.length() || !isdigit(inputAnalisis[posicionActual])) {
                        errorNum = true;
                        codigoError = 502;
                    }
                } else if(posicionActual >= inputAnalisis.length() || !isdigit(inputAnalisis[posicionActual])) {
                    errorNum = true;
                    codigoError = 501;
                }
            }
            else {
                break;
            }
        }

        if(errorNum) {
            return Token{codigoError,
                         inputAnalisis.substr(inicio_num, posicionActual - inicio_num),
                         errores.at(codigoError),
                         inicio_num};
        } else {
            std::string num = inputAnalisis.substr(inicio_num, posicionActual - inicio_num);
            if(tieneExponente) {
                return Token{104, num, aceptacion.at(104), inicio_num};
            } else if(tienePunto) {
                return Token{103, num, aceptacion.at(103), inicio_num};
            } else {
                return Token{102, num, aceptacion.at(102), inicio_num};
            }
        }
    }

    // Manejo especial para comentarios
    if (car == '/') {
        size_t inicio_comentario = posicionActual;
        posicionActual++;

        // Comentario de línea (//)
        if (posicionActual < inputAnalisis.length() && inputAnalisis[posicionActual] == '/') {
            posicionActual++;
            // Saltar hasta fin de línea
            while (posicionActual < inputAnalisis.length() && inputAnalisis[posicionActual] != '\n') {
                posicionActual++;
            }
            // Retornar el siguiente token (ignorar este comentario)
            return obtenerSiguienteToken();
        }
        // Comentario de bloque (/* */)
        else if (posicionActual < inputAnalisis.length() && inputAnalisis[posicionActual] == '*') {
            posicionActual++;
            bool cerrar = false;
            while (posicionActual < inputAnalisis.length() && !cerrar) {
                if (inputAnalisis[posicionActual] == '*' &&
                    posicionActual + 1 < inputAnalisis.length() &&
                    inputAnalisis[posicionActual+1] == '/') {
                    cerrar = true;
                    posicionActual += 2;
                } else {
                    posicionActual++;
                }
            }
            // Retornar el siguiente token (ignorar este comentario)
            return obtenerSiguienteToken();
        }
        // Si no es comentario, retroceder y continuar con el análisis normal
        else {
            posicionActual = inicio_comentario;
        }
    }

    // 2.5 Manejo especial para cadenas entre comillas
    if (car == '"') {
        size_t inicio_str = posicionActual;
        bool escape = false;
        posicionActual++; // Saltar la comilla inicial

        while (posicionActual < inputAnalisis.length()) {
            if (inputAnalisis[posicionActual] == '\\' && !escape) {
                escape = true;
                posicionActual++;
                continue;
            }

            if (inputAnalisis[posicionActual] == '"' && !escape) {
                // Comilla de cierre encontrada
                posicionActual++; // Incluir la comilla final
                std::string str = inputAnalisis.substr(inicio_str, posicionActual - inicio_str);

                // Verificar si hay un punto y coma inmediatamente después
                if (posicionActual < inputAnalisis.length() && inputAnalisis[posicionActual] == ';') {
                    size_t pos_puntocoma = posicionActual;
                    // Devolver primero el token de la cadena (incluyendo ambas comillas)
                    return Token{126, str, aceptacion.at(126), inicio_str};
                }

                return Token{126, str, aceptacion.at(126), inicio_str};
            }

            escape = false;
            posicionActual++;
        }

        // Si llegamos aquí, no se encontró la comilla de cierre
        return Token{505, inputAnalisis.substr(inicio_str), errores.at(505), inicio_str};
    }

    // 3. Analisis general con matriz de transicion
    int edo = 0;
    std::string lexema = "";
    size_t inicio = posicionActual;
    size_t ultima_aceptacion = posicionActual;
    int edo_aceptacion = -1;

    while(posicionActual < inputAnalisis.length()) {
        car = inputAnalisis[posicionActual];
        col = LexerAutomata::relaciona(car);

        if(col < 0 || col >= 31) {
            col = 27; // dif
        }

        if(edo < 0 || edo >= 26) {
            break;
        }

        int nuevoEdo = LexerAutomata::matriz[edo][col];

        // Caso especial para operador !
        if (car == '!') {
            // Verificar si el siguiente caracter es = (operador !=)
            if (posicionActual + 1 < inputAnalisis.length() && inputAnalisis[posicionActual+1] == '=') {
                if (!lexema.empty()) {
                    // Si ya tenemos un lexema (podría ser otro !), devolverlo primero
                    std::string singleLex = inputAnalisis.substr(inicio, posicionActual - inicio);
                    if (aceptacion.find(edo) != aceptacion.end()) {
                        return Token{edo, singleLex, aceptacion.at(edo), inicio};
                    }
                }
                // Devolver el operador !=
                std::string compLex = "!=";
                posicionActual += 2;
                return Token{115, compLex, aceptacion.at(115), inicio};
            }
            else {
                // Es un ! simple
                if (!lexema.empty()) {
                    std::string singleLex = inputAnalisis.substr(inicio, posicionActual - inicio);
                    if (aceptacion.find(edo) != aceptacion.end()) {
                        return Token{edo, singleLex, aceptacion.at(edo), inicio};
                    }
                }
                std::string singleBang = "!";
                posicionActual++;
                return Token{116, singleBang, aceptacion.at(116), inicio};
            }
        }

        if(nuevoEdo == 0) {
            break;
        }

        lexema += car;
        edo = nuevoEdo;
        posicionActual++;

        // Registrar ultimo estado de aceptacion
        if(aceptacion.find(edo) != aceptacion.end()) {
            ultima_aceptacion = posicionActual;
            edo_aceptacion = edo;
        }
    }

    // Procesar resultado del analisis
    if(edo_aceptacion != -1) {
        posicionActual = ultima_aceptacion;
        lexema = inputAnalisis.substr(inicio, posicionActual - inicio);
        return Token{edo_aceptacion, lexema, aceptacion.at(edo_aceptacion), inicio};
    }
    else if(!lexema.empty()) {
        if(errores.find(edo) != errores.end()) {
            return Token{edo, lexema, errores.at(edo), inicio};
        }
        else if(diferentes.find(edo) != diferentes.end()) {
            return Token{edo, lexema, "Token especial", inicio};
        }
        else {
            if(lexema != "\n" && lexema != "\t" && lexema != " ") {
                return Token{506, lexema, "Carácter no reconocido", inicio};
            }
        }
    }
    else {
        if(inputAnalisis[posicionActual] != ' ' &&
            inputAnalisis[posicionActual] != '\t' &&
            inputAnalisis[posicionActual] != '\n') {
            std::string charStr(1, inputAnalisis[posicionActual]);
            return Token{506, charStr, "Carácter no reconocido", posicionActual};
        }
        posicionActual++;
    }

    // Si llegamos aquí sin retornar, devolvemos un token de fin
    return Token{-1, "", "FIN", posicionActual};
}
