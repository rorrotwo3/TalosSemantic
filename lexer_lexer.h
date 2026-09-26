#ifndef LEXER_LEXER_H
#define LEXER_LEXER_H

#include "token.h"
#include "lexer_automata.h"
#include <string>

class Lexer {
private:
    std::string inputAnalisis;
    size_t posicionActual;

public:
    Lexer();
    void reiniciarAnalisis(const std::string& texto);
    bool tieneMasTokens() const;
    Token obtenerSiguienteToken();
};

#endif // LEXER_LEXER_H
