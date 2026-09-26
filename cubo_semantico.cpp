#include "cubo_semantico.h"

// Inicializacion de variables estaticas
std::unordered_map<int,
                   std::unordered_map<std::string,
                                      std::unordered_map<std::string, std::string>>> CuboSemantico::cubo;
bool CuboSemantico::inicializado = false;

void CuboSemantico::inicializarCubo() {
    // --- Logica para SUMA (+)
    cubo[OP_SUMA][T_INT][T_INT] = T_INT;
    cubo[OP_SUMA][T_INT][T_FLOAT] = T_FLOAT;
    cubo[OP_SUMA][T_FLOAT][T_INT] = T_FLOAT;
    cubo[OP_SUMA][T_FLOAT][T_FLOAT] = T_FLOAT;
    cubo[OP_SUMA][T_STRING][T_STRING] = T_STRING; // Concatenar
    cubo[OP_SUMA][T_STRING][T_INT] = T_STRING;
    cubo[OP_SUMA][T_INT][T_STRING] = T_STRING;
    //todas las demas sumas son T_ERROR

    // --- Logica para RESTA (-)
    cubo[OP_RESTA][T_INT][T_INT] = T_INT;
    cubo[OP_RESTA][T_INT][T_FLOAT] = T_FLOAT;
    cubo[OP_RESTA][T_FLOAT][T_INT] = T_FLOAT;
    cubo[OP_RESTA][T_FLOAT][T_FLOAT] = T_FLOAT;

    // --- Logica para MULTIPLICACION (*)
    cubo[OP_MULT][T_INT][T_INT] = T_INT;
    cubo[OP_MULT][T_INT][T_FLOAT] = T_FLOAT;
    cubo[OP_MULT][T_FLOAT][T_INT] = T_FLOAT;
    cubo[OP_MULT][T_FLOAT][T_FLOAT] = T_FLOAT;

    // --- Logica para DIVISION (/)
    cubo[OP_DIV][T_INT][T_INT] = T_FLOAT;     // int/int -> float (para evitar perdida)
    cubo[OP_DIV][T_INT][T_FLOAT] = T_FLOAT;
    cubo[OP_DIV][T_FLOAT][T_INT] = T_FLOAT;
    cubo[OP_DIV][T_FLOAT][T_FLOAT] = T_FLOAT;

    // --- Logica para ASIGNACION (=)
    cubo[OP_ASIG][T_INT][T_INT] = T_INT;
    cubo[OP_ASIG][T_FLOAT][T_FLOAT] = T_FLOAT;
    cubo[OP_ASIG][T_FLOAT][T_INT] = T_FLOAT; // Permite int -> float
    cubo[OP_ASIG][T_CHAR][T_CHAR] = T_CHAR;
    cubo[OP_ASIG][T_STRING][T_STRING] = T_STRING;
    cubo[OP_ASIG][T_BOOL][T_BOOL] = T_BOOL;
    // Asignar float a int es T_ERROR (requeriria casteo explicito)

    // --- Logica para AND (&&) y OR (||)
    cubo[OP_AND][T_BOOL][T_BOOL] = T_BOOL;
    cubo[OP_OR][T_BOOL][T_BOOL] = T_BOOL;


    // --- Logica para mod
    cubo[OP_MOD][T_INT][T_INT] = T_INT;

    // --- Logica para pot
    cubo[OP_POT][T_INT][T_INT] = T_FLOAT;
    cubo[OP_POT][T_INT][T_FLOAT] = T_FLOAT;
    cubo[OP_POT][T_FLOAT][T_INT] = T_FLOAT;
    cubo[OP_POT][T_FLOAT][T_FLOAT] = T_FLOAT;

    // --- Logica para relacionales

    // Operador == (110)
    cubo[OP_IGUAL][T_INT][T_INT] = T_BOOL;
    cubo[OP_IGUAL][T_FLOAT][T_FLOAT] = T_BOOL;
    cubo[OP_IGUAL][T_INT][T_FLOAT] = T_BOOL;
    cubo[OP_IGUAL][T_FLOAT][T_INT] = T_BOOL;
    cubo[OP_IGUAL][T_CHAR][T_CHAR] = T_BOOL;
    cubo[OP_IGUAL][T_STRING][T_STRING] = T_BOOL;
    cubo[OP_IGUAL][T_BOOL][T_BOOL] = T_BOOL;

    // Operador != (115)
    cubo[OP_DIFERENTE][T_INT][T_INT] = T_BOOL;
    cubo[OP_DIFERENTE][T_FLOAT][T_FLOAT] = T_BOOL;
    cubo[OP_DIFERENTE][T_INT][T_FLOAT] = T_BOOL;
    cubo[OP_DIFERENTE][T_FLOAT][T_INT] = T_BOOL;
    cubo[OP_DIFERENTE][T_CHAR][T_CHAR] = T_BOOL;
    cubo[OP_DIFERENTE][T_STRING][T_STRING] = T_BOOL;
    cubo[OP_DIFERENTE][T_BOOL][T_BOOL] = T_BOOL;

    // Operador < (111)
    cubo[OP_MENOR][T_INT][T_INT] = T_BOOL;
    cubo[OP_MENOR][T_FLOAT][T_FLOAT] = T_BOOL;
    cubo[OP_MENOR][T_INT][T_FLOAT] = T_BOOL;
    cubo[OP_MENOR][T_FLOAT][T_INT] = T_BOOL;
    cubo[OP_MENOR][T_CHAR][T_CHAR] = T_BOOL;
    cubo[OP_MENOR][T_STRING][T_STRING] = T_BOOL;
    cubo[OP_MENOR][T_BOOL][T_BOOL] = T_BOOL;

    // Operador <= (112)
    cubo[OP_MENOR_IGUAL][T_INT][T_INT] = T_BOOL;
    cubo[OP_MENOR_IGUAL][T_FLOAT][T_FLOAT] = T_BOOL;
    cubo[OP_MENOR_IGUAL][T_INT][T_FLOAT] = T_BOOL;
    cubo[OP_MENOR_IGUAL][T_FLOAT][T_INT] = T_BOOL;
    cubo[OP_MENOR_IGUAL][T_CHAR][T_CHAR] = T_BOOL;
    cubo[OP_MENOR_IGUAL][T_STRING][T_STRING] = T_BOOL;
    cubo[OP_MENOR_IGUAL][T_BOOL][T_BOOL] = T_BOOL;

    // Operador > (113)
    cubo[OP_MAYOR][T_INT][T_INT] = T_BOOL;
    cubo[OP_MAYOR][T_FLOAT][T_FLOAT] = T_BOOL;
    cubo[OP_MAYOR][T_INT][T_FLOAT] = T_BOOL;
    cubo[OP_MAYOR][T_FLOAT][T_INT] = T_BOOL;
    cubo[OP_MAYOR][T_CHAR][T_CHAR] = T_BOOL;
    cubo[OP_MAYOR][T_STRING][T_STRING] = T_BOOL;
    cubo[OP_MAYOR][T_BOOL][T_BOOL] = T_BOOL;

    //Operador >= (114)
    cubo[OP_MAYOR_IGUAL][T_INT][T_INT] = T_BOOL;
    cubo[OP_MAYOR_IGUAL][T_FLOAT][T_FLOAT] = T_BOOL;
    cubo[OP_MAYOR_IGUAL][T_INT][T_FLOAT] = T_BOOL;
    cubo[OP_MAYOR_IGUAL][T_FLOAT][T_INT] = T_BOOL;
    cubo[OP_MAYOR_IGUAL][T_CHAR][T_CHAR] = T_BOOL;
    cubo[OP_MAYOR_IGUAL][T_STRING][T_STRING] = T_BOOL;
    cubo[OP_MAYOR_IGUAL][T_BOOL][T_BOOL] = T_BOOL;

    inicializado = true;
}

std::string CuboSemantico::verificar(int op, const std::string& tipo1, const std::string& tipo2) {
    if (!inicializado) {
        inicializarCubo();
    }

    // Busca la operacion
    auto it_op = cubo.find(op);
    if (it_op == cubo.end()) {
        qDebug() << "Error Semántico: Variable no definida en Cubo:" << op;
        return T_ERROR;
    }

    // Busca el tipo1
    auto it_t1 = it_op->second.find(tipo1);
    if (it_t1 == it_op->second.end()) {
        qDebug() << "Error Semántico: Tipo1 no valido para op:" << op << "tipo:" << QString::fromStdString(tipo1);
        return T_ERROR;
    }

    // Busca el tipo2
    auto it_t2 = it_t1->second.find(tipo2);
    if (it_t2 == it_t1->second.end()) {
        qDebug() << "Error Semántico: Tipos incompatibles:"
                 << QString::fromStdString(tipo1) << op << QString::fromStdString(tipo2);
        return T_ERROR;
    }

    //duplicidad de variables
    /*auto it_t3 = it_t2->second.find(tipo3);
    if(it_t3 == it_t2->second.end()){
        qDebug() << "Error Semantico: Variable no definida: "
                 << QString::fromStdString(tipo1) << op << QString::fromStdString(tipo3);
        return T_ERROR;
    }*/

    /*
     * duplicidad d evaribales
     * error entre tipos
     * operacion entre tipos
     * varibale no definida
     */

    // Devuelve el tipo resultante
    return it_t2->second;
}
