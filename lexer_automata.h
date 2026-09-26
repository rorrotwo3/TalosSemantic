#ifndef LEXER_AUTOMATA_H
#define LEXER_AUTOMATA_H

#include <vector>
#include <unordered_map>
#include <set>
#include <string>

class LexerAutomata {
public:
    static const int matriz[26][31];

public:
    static int relaciona(char c);
    static std::set<int> estadosDiferentes();
    static std::unordered_map<int, std::string> estadoAceptacion();
    static std::unordered_map<std::string, int> estadoAsignacion();
    static std::unordered_map<int, std::string> erroresLexicos();

    static int obtenerTransicion(int estado, int columna);
};

#endif // LEXER_AUTOMATA_H
