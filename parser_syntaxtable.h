#ifndef PARSER_SYNTAXTABLE_H
#define PARSER_SYNTAXTABLE_H

#include "token.h"
#include <vector>
#include <unordered_map>
#include <string>

class SyntaxTable {
private:
    static std::vector<std::vector<int>> matrizPredictiva;
    static std::unordered_map<std::string, int> terminales;
    static std::unordered_map<std::string, int> noTerminales;
    static std::unordered_map<int, int> lexicoToSintactico;

public:
    static void inicializarMatrizPredictiva();
    static void inicializarLexicoToSintactico();
    static void inicializarTerminalesYNoTerminales();

    static int tokenToTerminal(const Token& token);
    static int obtenerProduccionIdx(int noTerminalNum, int terminal);

    static const std::unordered_map<int, std::string> inicializarErroresSintacticos();
};

#endif // PARSER_SYNTAXTABLE_H
