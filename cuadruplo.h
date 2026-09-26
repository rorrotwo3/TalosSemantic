#ifndef CUADRUPLO_H
#define CUADRUPLO_H

#include <string>
#include <iostream>

struct Cuadruplo {
    std::string op;   // Operador: +, -, *, /, =, SF, SI, GOTO, etc.
    std::string op1;  // Operando 1
    std::string op2;  // Operando 2 (puede estar vacío)
    std::string res;  // Resultado o Target del salto

    // Constructor para facilitar la creación
    Cuadruplo(std::string o, std::string o1, std::string o2, std::string r)
        : op(o), op1(o1), op2(o2), res(r) {}
};

#endif // CUADRUPLO_H
