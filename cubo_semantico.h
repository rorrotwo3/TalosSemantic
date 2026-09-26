#ifndef CUBO_SEMANTICO_H
#define CUBO_SEMANTICO_H

#include <string>
#include <unordered_map>
#include <QDebug>

// Definimos los operadores usando los numeros de token lexico
#define OP_SUMA 105
#define OP_RESTA 106
#define OP_MULT 107
#define OP_DIV 108
#define OP_POT 133
#define OP_MOD 128
#define OP_AND 117
#define OP_OR 118
#define OP_ASIG 109


// Definimos tipos para claridad
#define T_INT "int"
#define T_FLOAT "float"
#define T_CHAR "char"
#define T_STRING "string"
#define T_BOOL "bool"
#define T_ERROR "error_tipo"

// Relacionales
#define OP_IGUAL 110
#define OP_DIFERENTE 115
#define OP_MENOR 111
#define OP_MENOR_IGUAL 112
#define OP_MAYOR 113
#define OP_MAYOR_IGUAL 114

class CuboSemantico {
    private:
        // Un mapa estatico para guardar las reglas: Es [Operador][Tipo1][Tipo2] -> TipoResultado
        static std::unordered_map<int,
                                  std::unordered_map<std::string,
                                                     std::unordered_map<std::string, std::string>>> cubo;

        static bool inicializado;
        static void inicializarCubo();

    public:
        // La función principal que usa el analizador
        static std::string verificar(int op, const std::string& tipo1, const std::string& tipo2);
};

#endif // CUBO_SEMANTICO_H
