#ifndef PARSER_PARSER_H
#define PARSER_PARSER_H

#include "token.h"
#include "parser_syntaxtable.h"
#include "parser_productions.h"
#include "semantic_analyzer.h"
#include <vector>
#include <stack>
#include <unordered_map>
#include <string>

class Parser {
private:
    std::stack<int> pilaSintactica;
    std::vector<Token> tokensAnalisis;
    size_t tokenActual;
    std::string codigoFuente;
    std::unordered_map<int, std::string> erroresSintactico;
    SemanticAnalyzer semantico;
    std::vector<std::string> listaErroresSemanticos;

    void ejecutarAccionSemantica(int numAccion);

    void inicializarParser();
    void pushProduccion(const Produccion& prod);
    int obtenerNumeroLinea(size_t posicion);

public:
    Parser();
    bool analizarSintaxis(const std::vector<Token>& tokens, const std::string& codigo);
    void mostrarErrorSintactico(const Token& token, int codigoError);
    std::vector<std::string> getErroresSemanticos() const;

    //para imprimir en interfaz
    std::vector<std::string> getHistorialOperandos() const;
    std::vector<std::string> getHistorialOperadores() const;
    std::vector<int> getHistorialSaltos() const;
    std::vector<Cuadruplo> getListaCuadruplos() const;
};

#endif // PARSER_PARSER_H
