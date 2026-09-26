#ifndef TOKEN_H
#define TOKEN_H

#include <string>

struct Token {
    int estado;
    std::string lexema;
    std::string tipo;
    size_t posicion;

    bool esTipo(const std::string& tipoComparar) const {
        return tipo == tipoComparar;
    }
};

#endif // TOKEN_H
