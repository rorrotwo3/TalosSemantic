#ifndef SEMANTIC_ANALYZER_H
#define SEMANTIC_ANALYZER_H

#include <iostream>
#include <string>
#include <unordered_map>
#include <stack>
#include <vector>
#include "cubo_semantico.h"
#include "lexer_lexer.h" // Para la estructura Token
#include "cuadruplo.h"

// Estructura para cada entrada en la Tabla de Simbolos
struct Simbolo {
    std::string tipo;
    int direccion;

};

#define FONDO_FALSO 999

class SemanticAnalyzer {
private:
    std::unordered_map<std::string, Simbolo> tablaSimbolos;
    std::stack<std::string> pilaLexemas;
    int cont_direcc; // Contador para asignar "direcciones" de memoria
    //pilas para expreciones
    std::stack<std::string> pilaTipos;
    std::stack<int> pilaOperadores;
    std::vector<std::string>* pListaErrores;


public:
    SemanticAnalyzer();

    // ACCIONES SEMANTICAS

    // Funcion para conectar la lista de errores
    void setReferenciaErrores(std::vector<std::string>* refLista);
    void reset();

    // Accion 1 (como la AccionId1): Revisa y apila el lexema del ID
    void accionDeclararId(const std::string& lexema);

    // Accion 2 (como la AccionId2): Desapila y registra en la tabla con un tipo
    void accionAsignarTipo(const std::string& tipo);

    // (Accion 1 -> 2003) y mi sugerencia (2002)
    void accionPushTipoConstante(int tokenEstado, const std::string& lexema);
    void accionVerificarIdYPushTipo(const std::string& lexema);

    // (Accion 2, 5, 6 -> 2004, 2007, 2008)
    void accionPushOperador(int opTokenEstado);

    // (Accion 3, 4 -> 2005, 2006)
    void accionResolverOpsPrecedencia(int precedencia);

    // (Accion 7, 8 -> 2009, 2010)
    void accionPushFondoFalso();
    void accionPopFondoFalso();

    // (Accion 9 -> 2011)
    void accionResolverAsignacion();

    //FUNCIONES AUXILIARES

    // Convierte un token de tipo (140, 141...) a un string ("int", "float"...)
    std::string getTipoFromToken(int tokenEstado);
    // Convierte token cte a string
    std::string getTipoConstanteFromToken(int tokenEstado);
    // Para depuración
    void imprimirTablaSimbolos();

    // Nuevas funciones para Cuadruplos
    void generarCuadruplo(std::string op, std::string op1, std::string op2, std::string res);
    void rellenar(int direccion, int valor); // Backpatching
    std::string nuevoTemporal();

    // Acciones para Estatutos (3000+)
    void accionIf_Inicio();      // 3000 (Despues de EXPR)
    void accionIf_Else();        // 3001 (Antes de ELSE)
    void accionIf_Fin();         // 3002 (Al final de ENDIF)

    void accionWhile_Inicio();   // 3003 (Antes de EXPR, guardar retorno)
    void accionWhile_FinExpr();  // 3004 (Despues de EXPR, generar SF)
    void accionWhile_Fin();      // 3005 (Al final, generar SI y rellenar)

    void accionDo_Inicio();      // 3006 (Guardar retorno)
    void accionDo_Fin();         // 3007 (Generar SF al inicio si condicion falla)

    //void accionFor_Inicio();     // 3008 (Asignacion inicial)
    void accionFor_Condicion();  // 3009 (Evaluar condicion y SF)
    void accionFor_Inc();        // 3010 (Incremento y SI) - Nota: Esto requiere manejo especial en sintaxis
    // Para el FOR simple del diagrama:
    //void accionFor_Fin();        // 3011

    // Acciones para el FOR
    void accionFor_Asignacion();   // Acción 3008
    void accionFor_Inicio();       // Acción 3009
    void accionFor_Comparacion();  // Acción 3010
    void accionFor_Fin();          // Acción 3011

    // Acciones para I/O (Lectura y Escritura)
    void accionRead();             // Acción 3012
    void accionWrite();            // Acción 3013

    // Getters para ver el resultado
    std::vector<Cuadruplo> getCuadruplos() const { return listaCuadruplos; }
    std::vector<Cuadruplo> getListaCuadruplos() const { return listaCuadruplos; }
    // --- NUEVO: Getters para la Interfaz ---
    std::vector<std::string> getHistorialOperandos() const { return historialOperandos; }
    std::vector<std::string> getHistorialOperadores() const { return historialOperadores; }
    std::vector<int> getHistorialSaltos() const { return historialSaltos; }

private:
    // Logica interna para usar el Cubo Semantico
    void resolverUnaOperacion();
    // Convierte token op a string
    std::string opTokenToString(int opTokenEstado) const;
    void debugImprimirPilaTipos(std::stack<std::string> pila) const;
    void debugImprimirPilaOperadores(std::stack<int> pila) const;

    //para cuadruplos
    std::stack<std::string> pilaOperandos; // Guarda "x", "5", "T1"
    std::vector<int> pilaSaltos;           // Pila de saltos (vector para acceso random)
    std::vector<Cuadruplo> listaCuadruplos; // El codigo generado
    int contadorTemporales;

    // --- Historiales
    // Guardan todo lo que entra, nunca se borran hasta el reset()
    std::vector<std::string> historialOperandos;
    std::vector<std::string> historialOperadores;
    std::vector<int> historialSaltos;

};

#endif // SEMANTIC_ANALYZER_H
